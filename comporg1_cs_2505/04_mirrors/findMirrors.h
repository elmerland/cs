#ifndef FINDMIRRORS_C
#define FINDMIRRORS_C
#include <stdint.h>

/**
 *  Determines whether list[] contains two segments that are "mirrors".
 *  Mirrors are nonoverlapping consecutive sequences of values, with length
 *  greater than 1, which contain the same values, where one is in reverse
 *  order relative to the other.
 *
 *  Pre:
 *        0 < size <= 1000
 *        list[0:size-1] has been initialized
 *  Post:
 *        list[] is unmodified
 *  Returns:
 *        An integer of up to 9 digits, formatted as xxxyyyzzz, where
 *        xxx is the length of the mirrors, yyy is the index of the
 *        left-most entry in the left mirror and zzz is the index of
 *        the right-most entry in the right mirror; or 0 if no mirrors
 *        are found.
 *  Notes:
 *        Some of the digits in the return value will probably be 
 *        leading zeros.  If the array contains more than one pair of
 *        mirrors, you should report the left-most left mirror and the
 *        right-most corresponding right mirror.
 *
 *  Restrictions:
 *        You may not use a secondary array, or any other data structures.
 *        You may not write output or read input of any sort.
 *
 *  Examples:
 *        0  1  2  3  4  5  6  7  8
 *       ---------------------------
 *       {1, 3, 2, 1, 1, 2, 3, 5, 2}  3001006
 *       {1, 7, 3, 5, 4, 2, 5, 3, 7}  3001008
 *       {1, 2, 3, 4, 4, 3, 7, 2, 1}  2000008
 *       {7, 2, 8, 5, 7, 2}                 0
 */
uint32_t findMirrors(const int32_t list[], uint32_t size);

#endif
