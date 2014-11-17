/*
 * Scott Sines (ssines)
 * Elmer Landaverde (elmer)
 * 
 * This mm.c file contains our memory manager implementation implementing 
 * malloc, free, and realloc. Our memory implementation uses boundary tags for
 * headers and footers, fences to mark the boundaries of the heap, and an array
 * of linked lists as the segregated free list. All payloads are 8 byte aligned
 * and the minimum block size is 24 bytes in 32-bit and 48 bytes in 64-bit.  We
 * implemented the code such that the same code could be run on 32-bit or 64-bit
 * machines without any changes.
 *
 * # # # # # # # # # # # # # # BLOCK STRUCTURE # # # # # # # # # # # # # # # # #
 * Boundary Tags:
 * Each block, allocated or free, has two boundary tags, one for the header and
 * the other for the footer. The boundary tag stores the size of the block and
 * whether or not the block is free or allocated. Since the blocks are 8 byte
 * aligned, the size of each block is guaranteed to be a multiple of 8 (1000 in
 * binary), therefore, the low three bits of the size are guaranteed to be zero.
 * We leveraged this fact to store the size and allocated bit in the same number
 * using bit-wise operations (info = size | allocated). We defined helper
 * methods to extract the size and allocation bit from the info value. The
 * header boundary tag is stored in the block struct since it is the very first
 * thing in the struct, however, since we do not know the size of the payload,
 * the footer boundary tag is manually added to the block once we know the size
 * of the block.
 *
 * Free List Element:
 * In addition to the boundary tag, the block contains a list_elem, which
 * contains a pointer to a next and prev element to be stored in a segregated
 * list in our free list. The list element supports the "horizontal" list
 * functionality of our free list (described below).
 *
 * Sub List Element:
 * In addition to the boundary tag, the block contains a sub_list_elem, which
 * contains a pointer to the next and prev element in a sub list. The sub list
 * is used to store free blocks of the same size at the same position in the 
 * "horizontal" segregated list. The sub list element supports the "vertical"
 * list functionality of our free list (described below).
 *
 * Since the heap space of a free block is not being used by the user, we
 * decided to store the free list element and sub list element pointers in the
 * heap space so that we don't waste space outside of the heap. Because of this
 * design decision, we must enforce a minimum block size so that there is space
 * to store this information in a free. When the block is allocated for the
 * user, the user will write over these pointers with their own data.
 *
 * # # # # # # # # # # # # # # # # FREE LIST # # # # # # # # # # # # # # # # # #
 * We decided to implement a segregated free list which is an array of linked
 * lists using the list.c list implementation.  Our free list has 20
 * lists that each represent a range of block sizes.  The sizes start at 32 and
 * increase by powers of two. We use a log function to hash a block into its
 * respective list based on the block size. We implemented a list of lists for
 * each segregated list in that there is a "horizontal" list and a "vertical"
 * list. The "horizontal" list is sorted by block size and is the primary list
 * of the segregated list. Additionally, if there are more than one block of a
 * given size, we felt that it would be a waste of time to put them both in the
 * segregated list at the same level. If one block is not big enough, there is
 * no need to check if a different block of the same size is big enough.
 * Therefore, we impelemented a "vertical" component such that all blocks of the
 * same size would be in the same "vertical" list and only the first element in
 * the "vertical" list would be in the "horizontal" segregated list. The
 * list_elem is used to facilitate the "horizontal" lists and the sub_list_elem
 * is used to facilitate the "vertical" lists. Each segregated list is sorted
 * from least to greatest by block size to make finding a block simpler with our
 * implementation. When a free block is created via extending the heap or
 * freeing a previously allocated block, the new free block's list_elem is added
 * to the respective segregated list and is added to a sub list in that
 * segregated list if necessary. When a free block is needed, either by malloc
 * or realloc, the free list is searched starting at the beginning of the list
 * that corresponds to the needed size. If the search reaches the end of the
 * list without finding a block large enough to fill the request, the search
 * proceeds to the beginning of the next larger list. If all lists after
 * unsuccessfully exhausted, the heap is extended by the size of the request to
 * create a free block of the necessary size. If a free block is found in the
 * free list, we check if that free block can be split to both fill the request
 * and leave behind a block of at least the minimum block size. When a free
 * block is allocated we remove it from its segregated list and its sub list if
 * necessary.
 *
 * When a block is freed, we attempt to coalesce the freed block with the block
 * immediately to the left and right of the block. We use the footers in this
 * situation so that we access the block previous to any block in the heap. We
 * use fences, a boundary tag signifying size 0 and allocated, one on either end
 * of the heap to eliminate edge cases in coalescing.  The memory manager will
 * simply see these as allocated blocks.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <inttypes.h>

#include "mm.h"
#include "memlib.h"
#include "config.h"
#include "list.h"

/*############################################################################*/
/* Team information */
/*############################################################################*/
team_t team = {
    /* Team name */
    "Big Dog Team",
    /* First member's full name */
    "Elmer Landaverde",
    /* First member's SLO (@cs.vt.edu) email address */
    "elmer",
    /* Second member's full name (leave blank if none) */
    "Scott Sines",
    /* Second member's SLO (@cs.vt.edu) email address (leave blank if none) */
    "ssines"
};

/*############################################################################*/
/* Structure definitions */
/*############################################################################*/

/**
 * Each block has a boundary tag for a header and a footer. The boundary tag
 * is used to store the size of the block measured in bytes and to determine
 * if the block is allocated or not.
 *
 * The allocation bit is stored on the least significant bit of the 'info'
 * element inside of the boundary tag structure. The allocation bit is set
 * to 1 when the block is allocated and 0 when the block is free.
 *
 * The size of the block is measured in bytes. The size is also 8 byte aligned.
 * In addition to being 8 byte aligned the last bit is used for the allocation
 * status and should never be used for determining the size of the block.
 */
typedef struct _boundary_tag
{
    size_t info;  // Size of the block is in bytes. The last bit is used
                    // to indicate if the block is allocated or free. The size
                    // must always be 8 bytes aligned.
} boundary_tag;

/**
 * Each block has a sub list element which adds the "vertical" list
 * functionality. The "horizontal" list is the respective segregated list that
 * the free block is in. In addition to this, if there is already a free block
 * of the given size in the free list, we add to the block to the "vertical"
 * list which holds only blocks of the same size. This way all of the blocks of
 * the same size are at the same position in the "horizontal" list.
 */
struct sub_list_elem
{
    struct sub_list_elem * sub_prev; // Previous element in the sub list
    struct sub_list_elem * sub_next; // Next element in the sub list
};

/**
 * Every block in memory can be type-casted into a block structure. This
 * structure has a header, a list element structure, and a sub list element.
 * The header holds information regarding the block's size and allocation
 * status. The list element is used to position the block in one of the
 * segregated lists. The sub list element is used to hold blocks of the same
 * size so that they are at the same position in the segregated list.
 *
 * In addition to the header, every block also has a footer. This footer,
 * however, is not stored in the block structure. Instead, it is manually
 * positioned at the end of the block every time a new block is created.
 */
typedef struct _block {
    boundary_tag header;
    struct list_elem elem;
    struct sub_list_elem sub_elem;
} block;

/*############################################################################*/
/* Function headers! */
/*############################################################################*/

// Helper functions.
static size_t   roundup(size_t size);
static void     set_info(boundary_tag * tag, size_t size, bool alloc);
static size_t   get_boundary_size(boundary_tag * tag);
static bool     is_boundary_free(boundary_tag * tag);
static size_t   get_block_size(block * b);
static bool     is_block_free(block * b);
static block *  get_next_block(block * b);
static block *  get_prev_block(block * b);
static void     set_header_footer(block * b, size_t size, bool alloc);
static void     mark_block_used(block * b, size_t size);
static void     mark_block_free(block * b, size_t size);
static int      log_b2(size_t num);
static size_t   hash(size_t size);
static boundary_tag *   get_prev_block_footer(block * b);
static boundary_tag *   get_block_footer(block * b);

// Memory management functions.
static block *  coalesce(block * b);
static void     init_segregated_lists(void);
static void     insert_block_into_list(block * b);
static void     remove_block_from_list(block * b);
static block *  extend_heap(size_t bytes);
static block *  find_block(size_t bytes);
static block *  expand_block(block * b, size_t old_size, size_t new_size);
static void     split_block(block * new_block, 
    size_t avail_size, size_t required_size);

/*############################################################################*/
/* Global variables */
/*############################################################################*/

/**
 * Defines the size of a boundary tag structure. This is used to determine the
 * offset from a block payload to a block starting point. For the 32-bit
 * version, the boundary size is 4 bytes. For the 64-bit version, the boundary
 * size is 8 bytes.
 */
#define BOUNDARY_SIZE sizeof(boundary_tag)

/**
 * Defines the minimum size of an allocated block. The size is measured in
 * bytes as is everything else. For the 32-bit code the minimum block size is
 * 24 bytes. For the 64-bit version, the minimum block size is 48 bytes.
 * 
 */
#define MIN_BLOCK_SIZE_BYTES sizeof(block) + BOUNDARY_SIZE

/**
 * Total number of segregated lists.
 */
#define NUM_LIST 20

/**
 * Array of free lists that represent the segregated lists.
 */
struct list segregated_list[NUM_LIST];

/**
 * Macro to find the largest of two numbers.
 */
#define MAX(x, y) ((x) > (y)? (x) : (y))

/**
 * A constant boundary tag structure that will be used as the prologue footer
 * and epilogue header.
 */
const boundary_tag FENCE = {.info = 0 | 1};


/*############################################################################*/
/* Helper functions */
/*############################################################################*/

/**
 * Rounds the given size to the closest multiple of ALIGNMENT. By default
 * ALIGNMENT should be set to 8 (bytes).
 * @param  size The size to round up.
 * @return The rounded up size.
 */
static size_t roundup(size_t size)
{
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

/**
 * Sets the size and allocation bit to the given boundary tag structure.
 * @param tag The boundary tag to be set.
 * @param size The size of the block.
 * @param alloc The allocation bit. 1 = Allocated, 0 = Free.
 */
static void set_info(boundary_tag * tag, size_t size, bool alloc)
{
    assert((size & 1) == 0);
    // Set allocation bit.
    if (alloc) {
        size = size | 1;
    } else {
        size = size & (~1);
    }
    // Store info to tag.
    tag->info = size;
}

/**
 * Gets the size of the block.
 * @param  tag The boundary tag that has the size information.
 * @return The size of the block measured in bytes.
 */
static size_t get_boundary_size(boundary_tag * tag)
{
    return tag->info & ~1;
}

/**
 * Determines if the allocation bit of the boundary tag is set to false.
 * @param  tag The boundary tag.
 * @return True if the allocated bit is set to false.
 */
static bool is_boundary_free(boundary_tag * tag)
{
    return !(tag->info & 1);
}

/**
 * Gets the size of the block measured in bytes.
 * @param  b The block.
 * @return The size of the block measured in bytes.
 */
static size_t get_block_size(block * b)
{
    return get_boundary_size(&(b->header));
}

/**
 * Determines if the given block is free.
 * @param  b The block.
 * @return True if the block is free, false if it is allocated.
 */
static bool is_block_free(block * b)
{
    return is_boundary_free(&(b->header));
}

/**
 * Gets a pointer to the footer of the previous block.
 * @param  b The block.
 * @return The pointer to the footer of the previous block.
 */
static boundary_tag * get_prev_block_footer(block * b)
{
    // Cast the block pointer to a void pointer. Then subtract the number
    // of bytes that are occupied by the boundary tag.
    return (void *)b - BOUNDARY_SIZE;
}

/**
 * Gets a pointer to the next block.
 * @param  b The block.
 * @return A pointer to the next block.
 */
static block * get_next_block(block * b)
{
    // Get the size of the block.
    size_t size = get_block_size(b);
    // Make sure that the block is not of size zero.
    // This would mean that we have reached the end of the heap.
    if(size == 0) { return NULL; }
    // Get the starting address of the next block.
    return (void *)b + size;
}

/**
 * Gets a pointer to the previous block.
 * @param  b The block.
 * @return A pointer to the previous block.
 */
static block * get_prev_block(block * b)
{
    // Get the pointer to the previous' block footer.
    boundary_tag * footer_tag = get_prev_block_footer(b);
    // Get the size of the previous block.
    size_t prev_block_size = get_boundary_size(footer_tag);
    // Check the size of the previous block is not zero. This means that the
    // edge of the heap has being reached.
    if(prev_block_size == 0) { return NULL; }
    // Get the starting address of the previous block.
    return (void *)b - prev_block_size;
}

/**
 * Gets a pointer to the footer of the block.
 * @param  b The block.
 * @return A pointer to the footer of the block.
 */
static boundary_tag * get_block_footer(block * b)
{
    assert(get_block_size(b) != 0);
    // First cast the block pointer to a void pointer. This will make the step
    // size equal to 1 byte. Then increase the pointer by the number of bytes
    // in the block. Finally subtract the length of the footer in bytes to get
    // the starting pointer of the block footer.
    return ((void *)b + get_block_size(b)) - BOUNDARY_SIZE;
}

/**
 * Set the specified size and allocation bit to the header and footer of the
 * block.
 * @param b The block.
 * @param size The size of the block in bytes.
 * @param alloc The allocation bit of the block.
 */
static void set_header_footer(block * b, size_t size, bool alloc)
{
    // Set header the size and allocation status of the block
    set_info(&(b->header), size, alloc);
    // Set the footer size and allocation status of the block.
    set_info(get_block_footer(b), size, alloc);
}

/**
 * Labels the given block as allocated.
 * @param b The block.
 * @param size The size of the block.
 */
static void mark_block_used(block * b, size_t size)
{
    set_header_footer(b, size, true);
}

/**
 * Labels the given block as free.
 * @param b The block.
 * @param size The size of the block.
 */
static void mark_block_free(block * b, size_t size)
{
    set_header_footer(b, size, false);
}

/**
 * Gets the log base two of the given number.
 * @param  num The number.
 * @return The log base 2 of the number.
 */
static int log_b2(size_t num)
{
    assert(num > 0);
    int count = -1;
    while (num != 0) {
        num = num >> 1;
        count++;
    }
    return count;
}

/**
 * Determines the index of the segregated that a block of size 'size' should
 * go into.
 * @param  size The size of the block in bytes.
 * @return The index corresponding to the segregated list that the block should
 *          go into.
 */
static size_t hash(size_t size)
{
    int index = log_b2(size) - 5;
    if (index <= 0) {
        return 0;
    } else {
        if (index >= NUM_LIST) {
            return NUM_LIST - 1;
        } else {
            return index;
        }
    }
}

/*############################################################################*/
/* Heap management functions */
/*############################################################################*/

/**
 * Check that the heap is properly formatted. The following checks are done
 * during the execution of this function:
 * FIRST - Verify that all blocks in the any free list are marked as free.
 * SECOND - Mark all free block visited through the free lists by setting the
 *          second least significant bit to 1. All sizes are 8 byte aligned
 *          so it is possible to use the first three least significant bits.
 * THIRD - Go through the entire heap and make sure that all free blocks are in
 *          some free list by checking that their second least significant bit
 *          is set to 1.
 * FOURTH - Make sure that no two free blocks are next to each other. This will
 *          spot any block that was not coalesced for some reason.
 * FITFH - Check that the payload of the block is 8 byte aligned.
 * @return -1 if there is an error with the heap.
 */
int mm_check(void)
{
    // Check that all free blocks are indeed free.
    int i;
    for (i = 0; i < NUM_LIST; i++) {
        // Get list.
        struct list * l = &segregated_list[i];
        // Iterate through every block and make sure they are marked as free.
        struct list_elem * current_elem = list_begin(l);
        struct list_elem * end_elem = list_end(l);

        // Iterate through segregated list
        for (; current_elem != end_elem; current_elem = list_next(current_elem))
        {
            struct sub_list_elem * current_sub_elem = 
                &list_entry(current_elem, block, elem)->sub_elem;

            // Iterate through sub list
            do
            {
                block * b = (block *) ((void *) current_sub_elem - 
                    sizeof(struct list_elem) - BOUNDARY_SIZE);

                if(!is_block_free(b)) { return -1; }
                size_t size = get_block_size(b);
                if(size % 8 != 0) { return -1; }

                // Set the second least significant bit to 1. This will mark all
                // the visited free blocks and enable us to later check if all
                // free block are in the free list. (This is possible because
                // all addresses should be 8 byte aligned).
                size = size | (1 << 1);
                set_info((boundary_tag *)b, size, false);

                current_sub_elem = current_sub_elem->sub_next;
            } while(current_sub_elem != NULL);
        }
    }

    // Go through the heap one more time.
    block * b = (block *)(mem_heap_lo() + BOUNDARY_SIZE);
    while (get_block_size(b) != 0) {
        // If block is free assert that it was visited by the previous check.
        if (is_block_free(b)) {
            // Get block size.
            size_t size = get_block_size(b);
            // Assert that second least significant bit is set to true.
            if((size & (1 << 1)) != 2) { return -1; }
            // Reset check bit.
            *(size_t *)b = *(size_t *)b & ~(1 << 1);

            // Check that adjacent blocks are not free.
            if(is_block_free(get_next_block(b))) { return -1; }
            if(is_boundary_free(get_prev_block_footer(b))) { return -1; }
        }

        // Check that payload is aligned.
        if(((size_t)(void *)&b->elem % 8) != 0) { return -1; }
        b = get_next_block(b);
    }

    return 0;
}

/**
 * Checks the previous and next blocks, if either is marked as free they will
 * be merged with the current block. After the block has being merge with its
 * neighbors it will be added to the free list.
 * @param  b The block.
 * @return A pointer to the start of the new block if any merging occurred. A
 *          pointer to the same block if no merging occurred.
 */
static block * coalesce(block * b)
{
    bool prev_free = is_boundary_free(get_prev_block_footer(b));
    bool next_free = is_block_free(get_next_block(b));

    if (!prev_free && !next_free) { // Case 1: prev and next are allocated.
        insert_block_into_list(b);
        return b;
    } else if(!prev_free && next_free) { // Case 2: prev is allocated.
        // Get size of current and next block.
        size_t b_size = get_block_size(b);
        block * next_b = get_next_block(b);
        size_t b_n_size = get_block_size(next_b);
        // Mark current and next block as free.
        mark_block_free(b, b_size + b_n_size);

        // Remove old block from list.
        remove_block_from_list(next_b);
        // Add new expanded block to list.
        insert_block_into_list(b);
        return b;
    } else if (prev_free && !next_free) { // Case 3: next is allocated.
        // Get the size of the current and previous block.
        size_t b_size = get_block_size(b);
        block * prev_b = get_prev_block(b);
        size_t b_p_size = get_block_size(prev_b);
        // Mark current and previous block as free.
        mark_block_free(prev_b, b_size + b_p_size);

        // Remove previous block from free list.
        remove_block_from_list(prev_b);
        // Add current and previous block to list.
        insert_block_into_list(prev_b);
        return prev_b;
    } else { // Case 4: prev and next are free.
        // Get size of current, previous and next block.
        size_t b_size = get_block_size(b);
        block * prev_b = get_prev_block(b);
        block * next_b = get_next_block(b);
        size_t b_p_size = get_block_size(prev_b);
        size_t b_n_size = get_block_size(next_b);

        // Mark current, previous and next block as free.
        mark_block_free(prev_b, b_p_size + b_size + b_n_size);

        // Remove next and previous blocks from free list.
        remove_block_from_list(prev_b);
        remove_block_from_list(next_b);

        // Insert current, previous and next block to free list.
        insert_block_into_list(prev_b);

        return prev_b;
    }
}

/**
 * Expand the heap by the indicated number of bytes. The number of bytes will
 * be rounded to ALIGNMENT and will have a minimum size of MIN_BLOCK_SIZE_BYTES.
 * @param  bytes The number of bytes to extend the heap by.
 * @return A pointer to the newly allocated free block.
 */
static block * extend_heap(size_t bytes)
{
    // Round the number of requested bytes to 8 byte aligned and make
    // sure that the number of bytes is higher that the minimum allowed.
    bytes = MAX(roundup(bytes), MIN_BLOCK_SIZE_BYTES);

    // Expand the heap.
    void * b = mem_sbrk(bytes);
    // Check heap result.
    if (b == NULL) {
        return NULL;
    }

    // Set the start of the new block to the header of the old epilogue.
    b = b - BOUNDARY_SIZE;
    // Mark the new block as free.
    mark_block_free(b, bytes);
    // Set the last header of the epilogue of the new end of the end.
    *(boundary_tag *)get_next_block(b) = FENCE;

    // Coalesce adjacent free blocks.
    return coalesce(b);
}

/**
 * Inserts the given block into the appropriate segregated list. If a block
 * of the given size is already in the segregated list, add the block to the
 * sub list at that element.
 * @param b block to be added to the free list
 */
static void insert_block_into_list(block * b)
{
    // Get size of block.
    size_t b_size = get_block_size(b);
    // Determine index of list according to block size.
    size_t index = hash(b_size);

    // Get list that the element should be inserted into
    struct list * hash_list = &segregated_list[index];

    // Get beginning and end of the list
    struct list_elem * current_elem = list_begin(hash_list);
    struct list_elem * end_elem = list_end(hash_list);

    // Iterate through the list to insert in sorted order
    for(; current_elem != end_elem; current_elem = list_next(current_elem))
    {
        // Get block from the list_elem
        block * current_block = list_entry(current_elem, block, elem);

        // Get sub_list_elem from the block
        struct sub_list_elem * current_sub_elem = &current_block->sub_elem;

        // Get the size of the block
        size_t current_size = get_block_size(current_block);

        // If inserting block of the same size, add to the node's sub list
        if(current_size == b_size)
        {
            // NULL out the next and prev list_elems
            b->elem.next = NULL;
            b->elem.prev = NULL;

            // Add this sub_elem after the first sub_elem in the list
            b->sub_elem.sub_next = current_sub_elem->sub_next;
            b->sub_elem.sub_prev = current_sub_elem;

            if(current_sub_elem->sub_next != NULL)
            {
                current_sub_elem->sub_next->sub_prev = &b->sub_elem;
            }
            current_sub_elem->sub_next = &b->sub_elem;

            return;
        }
        // If found a larger element, break out of the for loop
        else if(current_size > b_size)
        {
            break;
        }
    }

    // Insert new element into the respective list either at end of list
    // or before the greater block
    list_insert(current_elem, &b->elem);
    b->sub_elem.sub_next = NULL;
    b->sub_elem.sub_prev = NULL;
}

/**
 * Removes a block from the free list. The block must be removed from its
 * appropriate segregated list. If the block is in a sub_list, it must be
 * removed from the sub_list as well.
 * @param b block to be removed from the free list
 */
static void remove_block_from_list(block * b)
{
    struct sub_list_elem * b_sub_elem = &b->sub_elem;
    
    // Case 1: sub_next and sub_prev are NULL meaning there is no sub_list so
    // just remove the list_elem from the list
    if(b_sub_elem->sub_prev == NULL && b_sub_elem->sub_next == NULL)
    {
        // Remove the list_elem from the segregated list
        list_remove(&b->elem);
    }
    // Case 2: sub_prev is NULL and sub_next is not null meaning this is the
    // first sub_elem of a sub_list. Remove this list_elem from the list and
    // insert the sub_next list_elem in its place
    else if(b_sub_elem->sub_prev == NULL && b_sub_elem->sub_next != NULL)
    {
        // NULL out pointer to this block
        b_sub_elem->sub_next->sub_prev = NULL;

        // Remove this block from the segregated list
        struct list_elem * n_hor_elem = list_remove(&b->elem);

        // Get the next sub_list_elem to this block
        struct sub_list_elem * n_ver_sub_elem = b->sub_elem.sub_next;

        // Get the block from the next sub_list_elem
        block * n_ver_block = (block*) ((void *) n_ver_sub_elem 
            - sizeof(struct list_elem) - BOUNDARY_SIZE);
        
        // Insert the next block into the segregated list in place of this block
        list_insert(n_hor_elem, &n_ver_block->elem);
    }
    // Case 3: sub_prev is not NULL and sub_next is NULL meaning this is a
    // sub_elem at the end of a sub_list. Remove is from the sub_list
    else if(b_sub_elem->sub_prev != NULL && b_sub_elem->sub_next == NULL)
    {
        // NULL out pointer to the last element in the sub_list
        b_sub_elem->sub_prev->sub_next = NULL;
    }
    // Case 4: sub_prev and sub_next are not NULL meaning this is a sub_elem in
    // a sub_list. Remove it from the sub_list
    else if(b_sub_elem->sub_prev != NULL && b_sub_elem->sub_next != NULL)
    {
        // Rearrange pointers to cut out this block from the sub_list
        b_sub_elem->sub_prev->sub_next = b_sub_elem->sub_next;
        b_sub_elem->sub_next->sub_prev = b_sub_elem->sub_prev;
    }
}

/**
 * Initializes all the segregated lists.
 */
static void init_segregated_lists(void)
{
    int i;
    // Initialize lists
    for (i = 0; i < NUM_LIST; i++) {
        list_init(&segregated_list[i]);
    }
}

/**
 * Searches through the segregated list to find a free block that can fit a
 * block of the requested size.
 * @param  bytes The size of the requested block.
 * @return A pointer to a free block of size equal to or greater than the
 *          requested size.
 */
static block * find_block(size_t bytes)
{
    // Get index by hashing
    size_t index = hash(bytes);

    // Iterate segregated lists until we find match.
    int i;
    for (i = index; i < NUM_LIST; i++) {
        struct list * current_list = &segregated_list[i];

        if (list_empty(current_list)) {
            continue;
        }
        struct list_elem * current_elem = list_begin(current_list);
        struct list_elem * end_elem = list_end(current_list);
        for (; current_elem != end_elem; current_elem = list_next(current_elem))
        {
            // Get current block.
            block * current_block = list_entry(current_elem, block, elem);
            size_t block_size = get_block_size(current_block);
            // Check if current block is big enough for request.
            if (block_size >= bytes) {
                return current_block;
            }
        }
    }

    // If no block was found allocate a new block and return pointer to the
    // newly created block.
    return extend_heap(bytes);
}

/**
 * Takes a free block, removes it from its segregated list, determines if the
 * block can split and does so if necessary. Finally it marks the free block as
 * used and inserts the remainder block to a free list.
 * @param free_block
 * @param requested_size
 */
static void trim_block(block * free_block, size_t requested_size)
{
    // Check if block is too big.
    size_t free_block_size = get_block_size(free_block);
    if (free_block_size - requested_size >= MIN_BLOCK_SIZE_BYTES) {
        // Remove free bock from list.
        remove_block_from_list(free_block);
        // Mark request size block as used.
        mark_block_used(free_block, requested_size);
        // Get pointer to next block (remainder from free block).
        block * next_block = get_next_block(free_block);
        // Mark remainder block free.
        mark_block_free(next_block, free_block_size - requested_size);
        // Add remainder block to segregated list.
        insert_block_into_list(next_block);
    } else {
        // Remove free block from list.
        remove_block_from_list(free_block);
        // Mark block as used.
        mark_block_used(free_block, free_block_size);
    }
}

/**
 * This function takes a block and splits off the end if it possible. This
 * function is similar to trim_block. The difference is that this function does
 * assume that the new block was previously free. If the block is split, the
 * remainder that was split will be added to one of the free lists.
 * @param new_block The starting position of the new block.
 * @param avail_size The total size available for the new block.
 * @param required_size The required size of the new block.
 */
static void split_block(block * new_block, 
    size_t avail_size, size_t required_size)
{
    // Determine if the expanded block is big enough to be split.
    if (avail_size - required_size >= MIN_BLOCK_SIZE_BYTES) {
        // Mark required size as allocated
        mark_block_used(new_block, required_size);
        // Get pointer to next block.
        block * remainder_block = get_next_block(new_block);
        // Mark remainder block as free.
        mark_block_free(remainder_block, avail_size - required_size);
        // Insert free block into list.
        insert_block_into_list(remainder_block);
    } else {
        // Mark the entire available block as used.
        mark_block_used(new_block, avail_size);
    }
}

/**
 * This functions takes one block tries to expand its size. It can do this in
 * one of three ways. (1) By combining the current and next block if the next
 * block is free. (2) By combining the current, previous and next block if the
 * previous and next block are free (If next block is free is still attempts to
 * join the current and previous block). (3) By requesting a new block of the
 * desired size with a call to malloc and then freeing the current block. In
 * either of the three cases the data that was previously allocated to the
 * current block is copied over to the new expanded block.
 * @param  b The current block to be expanded.
 * @param  old_size The size of the current block.
 * @param  new_size The requested size of the new block.
 * @return A pointer to a block of size 'new_size' containing all the data from
 *          the current block.
 */
static block * expand_block(block * b, size_t old_size, size_t new_size)
{
    // Get pointer to next block.
    block * next_block = get_next_block(b);
    bool next_free = is_block_free(next_block);

    size_t next_size = 0;

    // CASE 1: Check if next and current block are enough to hold requested
    // new size.
    if (next_free) {
        // Get the size of the next block.
        next_size = get_block_size(next_block);
        // Check if current and next block are enough for the new size.
        if (old_size + next_size >= new_size) {
            // Remove next free block from list.
            remove_block_from_list(next_block);
            // Determine the total available size of the current and next block.
            size_t avail_size = old_size + next_size;
            split_block(b, avail_size, new_size);
            return b;
        }
    }

    // CASE 2: Check if any combination between previous current and next block
    // is big enough to hold the requested new size.
    // Check if previous block is free.
    bool prev_free = is_boundary_free(get_prev_block_footer(b));
    if (prev_free) {
        // Get pointer to previous block.
        block * prev_block = get_prev_block(b);
        // Get size of previous block.
        size_t prev_size = get_block_size(prev_block);
        // Check if the previous, current and next block are enough for the
        // new size.
        if (prev_size + next_size + old_size >= new_size) {
            // Remove the prev block from its list
            remove_block_from_list(prev_block);
            // Only remove the next block from its list if it is free
            if(next_free)
            {
                remove_block_from_list(next_block);
            }

            // Get the total available size
            size_t avail_size = prev_size + next_size + old_size;

            // Copy data from current block into new block.
            // Get payload source and destination.
            void * payload_dest = (void *)prev_block + BOUNDARY_SIZE;
            void * payload_src = (void *)b + BOUNDARY_SIZE;
            // Get size of payload.
            size_t payload_size = old_size - 2 * BOUNDARY_SIZE;
            // Move payload contents.
            memmove(payload_dest, payload_src, payload_size);

            split_block(prev_block, avail_size, new_size);
            return prev_block;
        }
    }

    // No space in the adjacent blocks, so malloc new block and free current.
    void * new_payload = mm_malloc(new_size - 2 * BOUNDARY_SIZE);
    // Get size of original payload.
    size_t payload_size = old_size - 2 * BOUNDARY_SIZE;
    // Copy payload into new memory position.
    memcpy(new_payload, (void*)b + BOUNDARY_SIZE, payload_size);
    // Free old block.
    mm_free((void *)b + BOUNDARY_SIZE);

    return new_payload - BOUNDARY_SIZE;
}

/*############################################################################*/
/* Memory allocation functions */
/*############################################################################*/

/**
 * Initialize the heap by placing a header for the prologue and a footer for
 * the epilogue. It also initializes the segregated list array.
 * @return -1 if there was an error, 0 if the heap was initializes successfully.
 */
int mm_init(void)
{
    // Get the space for the initial prologue and epilogue.
    boundary_tag * initial = mem_sbrk(2 * BOUNDARY_SIZE);
    if (initial == NULL) {
        return -1;
    }
    // Set the epilogue and prologue to the FENCE.
    boundary_tag * prologe = &initial[0];
    boundary_tag * epiloge = &initial[1];
    *prologe = FENCE;
    *epiloge = FENCE;
    // Initialize segregated lists.
    init_segregated_lists();

    return 0;
}

/**
 * Allocates a new block of memory that is at least 'bytes' bytes long.
 * @param  size The size in bytes of requested block.
 * @return A Pointer to the start of the requested block.
 */
void *mm_malloc(size_t size)
{
    size = MAX(roundup(size + 2 * BOUNDARY_SIZE), MIN_BLOCK_SIZE_BYTES);
    // Find free block.
    block * free_block = find_block(size);
    if (free_block == NULL) {
        perror("Could not get a free block");
        return NULL;
    }

    // Trim block.
    trim_block(free_block, size);
    return (void *)free_block + BOUNDARY_SIZE;
}

/**
 * Takes a pointer of a block that was previously allocated with mm_malloc and
 * frees up the block of memory for re-use.
 * @param ptr The pointer to the block.
 */
void mm_free(void *ptr)
{
    block * b = ptr - BOUNDARY_SIZE;

    // Get block size.
    size_t b_size = get_block_size(b);
    // Mark block as free.
    mark_block_free(b, b_size);

    // Coalesce with neighbor blocks.
    coalesce(b);
}

/**
 * Takes a pointer to a block that was previously allocated with mm_malloc or
 * mm_realloc and changes its size to the specified one. This may result in a
 * block at a new memory position. However, the contents of the block will
 * remain the same.
 * @param  oldptr The pointer of the block to resize.
 * @param  size The new size of the block.
 * @return A pointer to a memory block with the requested new size and the
 *          contents of the old block.
 */
void *mm_realloc(void *oldptr, size_t size)
{
    // Test if pointer is null
    if (oldptr == NULL) {
        return malloc(size);
    }
    // Test is size is zero
    if (size == 0) {
        free(oldptr);
        return NULL;
    }
    
    // Get pointer to start of block.
    block * b = oldptr - BOUNDARY_SIZE;
    assert(!is_block_free(b));
    // Get starting size of block.
    size_t old_size = get_block_size(b);
    // Get adjusted requested size.
    size = roundup(size + 2 * BOUNDARY_SIZE);
    size_t new_size = MAX(size, MIN_BLOCK_SIZE_BYTES);

    if (new_size > old_size) {
        block * new_block = expand_block(b, old_size, new_size);
        return (void *)new_block + BOUNDARY_SIZE;
    }
    else if (old_size - new_size >= MIN_BLOCK_SIZE_BYTES) {
        // Mark used space as used.
        mark_block_used(b, new_size);
        // Get next (remainder) block.
        block * n_block = get_next_block(b);
        // Mark remainder block as free.
        mark_block_free(n_block, old_size - new_size);
        // Coalesce new free block.
        coalesce(n_block);
        return oldptr;
    } else {
        return oldptr;
    }
}
