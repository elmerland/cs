#include <stdlib.h>
#include "Untangle.h"

static uint16_t read_int(uint8_t first, uint8_t second);
static char read_char(uint16_t offset, const uint8_t *pBuffer);
static void analyze_record(FILE *log, uint16_t offset, const uint8_t *pBuffer);

static uint16_t current_offset = 0;
static char *current_word;

void Untangle(const uint8_t *pBuffer, FILE *log) {
	// Get initial offset
	current_offset = read_int(pBuffer[0], pBuffer[1]);
	// Set word counter
	uint32_t total_words = 0;
	// Untangle the file
	while (current_offset != 0) {
		analyze_record(log, current_offset, pBuffer);
		total_words++;
	}
	// Print the total number of words
	//fprintf(log, "\nNumber of words reported: %4d\n", total_words);
}

/**
 * Takes two byte-long unsigned integers and using the 'little-endian' order
 * converts them into a single word-long integer.
 *
 * Pre:
 * 	first	The first byte-long unsigned integer
 * 	second	The second byte-long unsigned integer
 *
 * Returns:
 * 	Returns a 2 byte-long unsigned integer. This integer is constructed by using the
 * 	second integer as its least significant bits and using the first integer
 * 	as its most significant bits.
 */
uint16_t read_int(uint8_t first, uint8_t second) {
	uint16_t most_significant = second << 8;
	uint16_t least_significant = first;
	uint16_t result = most_significant + least_significant;
	return result;
}

/**
 * Takes a binary buffer and interprets the byte at the specified offset as a character.
 *
 * Pre:
 * 	offset	The offset in the binary buffer where the character is located.
 * 	pBuffer	The binary buffer.
 *
 * Returns:
 * 	Returns character value that is obtain from the binary buffer located at the
 * 	position specified by the offset.
 */
char read_char(uint16_t offset, const uint8_t *pBuffer) {
	char result = (char) pBuffer[offset];
	return result;
}

/**
 * Takes a binary buffer and a pointer to log file, it then extracts the word located
 * at the specified offset in the binary file and writes the word to the log file.
 *
 * Pre:
 * 	log	The log file where the word will be written to.
 * 	offset	The offset in the binary file where the word will be found.
 * 	pBuffer	The binary buffer where the information is read from.
 *
 * Post:
 * 	The word specified the offset has being written to the log file.
 * 	The 'current_offset' global variable has being updated to the next
 * 	offset specified by the current record.
 */
void analyze_record(FILE *log, uint16_t offset, const uint8_t *pBuffer) {
	// Get all indexes and offsets
	uint16_t rcd_length = read_int(pBuffer[offset], (uint8_t)0);
	uint16_t wrd_length = rcd_length - 3;
	uint16_t wrd_start = offset + 1;
	uint16_t wrd_end = wrd_start + wrd_length;
	uint16_t new_offset = read_int(pBuffer[wrd_end], pBuffer[wrd_end + 1]);
	
	// Update offset
	current_offset = new_offset;
		
	// Get word
	current_word = (char *) malloc(wrd_length + 1);
	int index = 0;
	for(int i = wrd_start; i < wrd_end; i++, index++) {
		current_word[index] = read_char(i, pBuffer);
	}
	
	// Add null terminator to string
	current_word[index] = '\0';
	
	// Print word to file
	fprintf(log, "%4.4X:  %s\n", offset, current_word);
	
	// Free memory used by malloc
	free(current_word);
}

//On my honor: 
//
//- I have not discussed the C language code in my program with 
//anyone other than my instructor or the teaching assistants 
//assigned to this course. 
//
//- I have not used C language code obtained from another student, 
//or any other unauthorized source, either modified or unmodified. 
//
//- If any C language code or documentation used in my program 
//was obtained from another source, such as a text book or course 
//notes, that has been clearly noted with a proper citation in 
//the comments of my program. 
//
//- I have not designed this program in such a way as to defeat or 
//interfere with the normal operation of the Curator System. 
// 
// Elmer Landaverde
