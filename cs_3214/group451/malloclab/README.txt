/*
 * Scott Sines (ssines)
 * Elmer Landaverde (elmer)
 * 
 * This mm.c file contains our memory manager implementation implementing 
 * malloc, free, and realloc. Our memory implementation uses boundary tags in
 * headers and footers, fences to mark the boundaries of the heap, and an array
 * of linked lists as the segregated free lists. All payloads are 8 byte aligned
 * and the minimum block size is 16 bytes in 32-bit and 36 bytes in 64-bit.  We
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
 * We leveraged this fact to store the size and allocated bit in the same 32-bit
 * number using bit-wise operations (info = size | allocated).
 *
 * Free List Element:
 * In addition to the boundary tags, the block contains a list_elem, which
 * contains a pointer to a next and prev element to be stored in our free list.
 * Since the heap space of a free block is not being used by the user, we
 * decided to store these pointers in the heap space so that we don't waste 
 * space outside of the heap. Because of this design decision, the minimum block
 * size is 16 bytes (4 byte header + 4 byte footer + 4 byte next + 4 byte prev)
 * in a 32-bit machine. When the block is allocated for the user, the user will
 * write over this next and prev pointers with their own data.
 *
 * # # # # # # # # # # # # # # # # FREE LIST # # # # # # # # # # # # # # # # # #
 * We decided to implement a segregated free list which is an array of linked
 * lists using the list.c list impelementation.  Our segregated list has 20
 * lists that each represent a range of block sizes.  The sizes start at 16 and
 * increase by powers of two.  We use a log function to hash a block into its
 * respective list based on the block size.  When a free block is created via
 * extending the heap or freeing a previously allocated block, the new free
 * block's list_elem is pushed to the back of its respective list.  When a free
 * block is needed, either my malloc or realloc, the free list is searched
 * starting at the beginning of the list that corresponds to the needed size. If
 * the search reaches the end of the list without finding a block large enough
 * to fill the request, the search proceeds to the beginning of the next larger
 * list. If all lists after unsuccessfully exhausted, the heap is extending by
 * the size of the request to create a free block of the necessary size. If a
 * free block is found in the free list, we check if that free block can be
 * split to both fill the request and leave behind a block of at least the
 * minimum block size of 16 bytes.
 *
 * When a block is freed, we attempt to coalesce the freed block with the block
 * immediately to the left and right of the block. We use the footers in this
 * situation so that we access the block previous to any block in the heap. We
 * use fences, a boundary tag signifying size 0 and allocated, one on either end
 * of the heap to eliminate edge cases in coalescing.  The memory manager will
 * simply see these as allocated blocks.
 */