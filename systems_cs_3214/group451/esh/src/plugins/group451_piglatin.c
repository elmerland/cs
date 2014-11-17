
#include <stdio.h>
#include <readline/history.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "../esh.h"

#define BUFF_SIZE 256

/* File where the history commands are stored */
#define HIST_FILENAME ".esh_history"

/* Maximum number of history entries that can be stored */
#define MAX_HISTORY 100

/**
 * Prints the provided string to standard out using signal
 * safe functions
 * @param str The string to be printed.
 */
static void print_str(char *str);

/**
 * Takes a command structure and analyses the command.
 * @param cmd The command structure.
 */
static void process_words(struct esh_command * cmd);

/**
 * Translates the given word into pig latin.
 * @param word The word to be translated.
 */
static void translate_word(char * word);

/**
 * Determines if the given character is a vowel.
 * @param  c         The character.
 * @param  include_y True if 'y' is to be considered a vowel.
 * @return           True if the character is vowel.
 */
static bool is_vowel(char c, bool include_y);

/**
 * Appends 'way' to the end of the word and prints it.
 * @param word The word.
 */
static void append_way(char * word);

/**
 * Appends the first vowel_start consonants to the word and then
 * appends 'ay'. The first consonants are not printed.
 * @param word        The word.
 * @param vowel_start The index where vowels start.
 */
static void append_ay(char * word, int vowel_start);

/**
 * Initialize the history by reading in from the history file.
 */
static bool init_plugin(struct esh_shell *shell)
{
    printf("Plugin 'pig_latin' initialized...\n");
    return true;
}

static bool pig_latin(struct esh_command * cmd) {
	// Get argv length
	int size = 0;
	int i;
	for (i = 0; cmd->argv[i] != NULL; i++) {
		size++;
	}

	// Check if size is zero.
	if (size == 0) {
		return false;
	} else if (strcmp(cmd->argv[0], "piglatin") == 0) {	// Check if first word is equal to "piglatin"
		if (size == 1) {	// If size is one then print usage
			char buff[BUFF_SIZE];
			snprintf(buff, BUFF_SIZE, "Usage:\n\tpiglatin ... WORD ...\n");
			print_str(buff);
		} else {
			process_words(cmd);
		}
		return true;
	} else {
		return false;
	}
}

static void print_str(char *str) {
	write(STDOUT_FILENO, str, strlen(str));
}

static void process_words(struct esh_command * cmd) {
	int i;
	for (i = 1; cmd->argv[i] != NULL; i++){
		translate_word(cmd->argv[i]);
	}
	print_str("\n");
}

static void translate_word(char * word) {
	// Test if word starts with vowel
	if (is_vowel(word[0], false)) {
		append_way(word);
	} else {
		// Get consonant count.
		int cons_ct = 0;
		int i;
		for (i = 0; word[i] != '\0'; i++) {
			if (is_vowel(word[i], true)) {
				break;
			}
			cons_ct++;
		}

		append_ay(word, cons_ct);
	}
}

static bool is_vowel(char c, bool include_y) {
	switch (c) {
		case 'a': return true;
		case 'A': return true;
		case 'e': return true;
		case 'E': return true;
		case 'i': return true;
		case 'I': return true;
		case 'o': return true;
		case 'O': return true;
		case 'u': return true;
		case 'U': return true;
		default: break;
	}

	if (include_y) {
		switch (c) {
			case 'y': return true;
			case 'Y': return true;
			default: break;
		}
	}

	return false;
}

static void append_way(char * word) {
	char result[BUFF_SIZE];
	strncpy(result, word, BUFF_SIZE);

	int i = strlen(result);
	result[i] = 'w';
	result[i + 1] = 'a';
	result[i + 2] = 'y';
	result[i + 3] = ' ';

	print_str(result);
}

static void append_ay(char * word, int vowel_start) {
	char result[BUFF_SIZE];
	strncpy(result, &word[vowel_start], BUFF_SIZE);

	int size = strlen(result);
	int i;
	for (i = 0; i < vowel_start; i++) {
		result[size + i] = word[i];
	}
	result[size + i] = 'a';
	result[size + i + 1] = 'y';
	result[size + i + 2] = ' ';

	print_str(result);
}

struct esh_plugin esh_module = {
    .rank = 1,
    .init = init_plugin,
    .process_builtin = pig_latin
};
