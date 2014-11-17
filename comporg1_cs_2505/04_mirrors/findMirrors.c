#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "findMirrors.h"

static bool findRightHandSideMirror(const int32_t list[], uint32_t size, 
	uint32_t l_start, uint32_t l_end, uint32_t *r_start, uint32_t *r_end);
static bool isMirror(const int32_t list[], uint32_t l_start, uint32_t l_end, 
	uint32_t r_start, uint32_t r_end);

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
uint32_t findMirrors(const int32_t list[], uint32_t size) {
	// Initialize all the mirror pointers
	uint32_t l_start = 0, l_end = 0, r_start = 0, r_end = 0;
	bool foundMirror = false;
	// Generate all possible left hand side mirrors and try to find 
	// the right hand side matches.
	for (int i = 0; i < size - 1; i++) {
		for (int j = size - 1; j > i; j--) {
			// Try to find the right hand side mirror that 
			// matches this left hand side mirror.
			l_start = i;
			l_end = j;
			if (findRightHandSideMirror(list, size, l_start, 
				l_end, &r_start, &r_end))
			{
				foundMirror = true;
				break;
			}
		}
		if(foundMirror) break;
	}
	// Format the results.
	uint32_t result = 0;
	if (foundMirror)
	{
		// add length
		result += (l_end - l_start + 1) * 1000000;
		// add left mirror start
		result += l_start * 1000;
		// add right mirror start
		result += r_start;
	}
	return result;
}

/**
 * Given the coordinates for a left hand side mirror it finds the right 
 * hand side mirror. If no mirror is found then it returns false.
 * 
 * post:
 * 	the varaibles r_start, and r_end will contain the coordinates 
 * 	for the right hand side mirror.
 *
 * returns: 
 * 	True if a mirror was found. Otherwise returns false.
 */
static bool findRightHandSideMirror(const int32_t list[], uint32_t size, 
	uint32_t l_start, uint32_t l_end, uint32_t *r_start, uint32_t *r_end) {
	// Get the size of the mirror
	uint32_t mirrorSize = l_end - l_start + 1;
	if ((size - mirrorSize) < mirrorSize)
	{
		// If remaining array length is less than mirror lenght, 
		// return false.
		return false;
	}
	// Generate every possible conbination on the right hand side,
	// and determine if it is a mirror.
	for (int i = size - 1; i > l_end + 1; i--)
	{
		for (int j = l_end + 1; j < i; j++)
		{
			if (isMirror(list, l_start, l_end, i, j))
			{
				// If the coordinates actualy represent 
				// and mirror,then store the starting 
				// and ending points of the mirror and 
				// return true.
				*r_start = i;
				*r_end = j;
				return true;
			}
		}
	}
	return false;
}

/**
 * Determines if the given mirror corrdinates actually represent a mirror.
 * 
 * returns: 
 * 	If the mirror coordinates represent a mirror in the given array it 
 * 	returns true. Otherwise it returns false.
 */
static bool isMirror(const int32_t list[], uint32_t l_start, uint32_t l_end, 
	uint32_t r_start, uint32_t r_end) {
	uint32_t mirrorSize = l_end - l_start + 1;
	uint32_t r_mirrorSize = r_start - r_end + 1;
	if (mirrorSize != r_mirrorSize)
	{
		// Return false if the sizes of the two mirrors don't match.
		return false;
	}
	// Look at every value of the two mirrors and make sure that they match.
	for(int i = 0; i < mirrorSize; i++)
	{
		if (list[l_start + i] != list[r_start - i])
		{
			return false;
		}
	}
	return true;
}

// On my honor: 
//
// - I have not discussed the C language code in my program with 
// anyone other than my instructor or the teaching assistants 
// assigned to this course. 
// 
// - I have not used C language code obtained from another student, 
// or any other unauthorized source, either modified or unmodified. 
// 
// - If any C language code or documentation used in my program 
// was obtained from an allowed source, such as a text book or course 
// notes, that has been clearly noted with a proper citation in 
// the comments of my program. 
// 
// Elmer Landaverde 
