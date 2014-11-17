#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINE_SZ 256
#define MASK 0xf

char scanner (void);
int  mask    (int num);

int E  (void);
int EE (int subtotal);
int A  (void);
int AA (int subtotal);
int B  (void);
int BB (int subtotal);
int C  (void);

char token = '\n';

int main(int argc, char ** argv) {
	if (argc == 2 && argv[1][0] == 'v') {
		#define VERBOSE
	}
	while (scanner() != '\0') {
		int result = E();
		printf("Ans: 0x%x\n\n", result);
		// printf("%x\n", result);
	}
	return 0;
}

char scanner(void) {
	static char * line = NULL;
	static int pos;
	if (line == NULL) { // Initialize line
		line = (char *) malloc(LINE_SZ * sizeof(char));
	}
	if (token == '\n' || token == '\0') { // Reached end of line
		pos = 0;
		do {
			char * l = fgets(line, LINE_SZ, stdin); // Read new line
			if (l == NULL) { // If EOF return with null terminator
				free(line);
				puts("EOF");
				return '\0';
			}
		} while (line[pos] == '\n');
		char last_char = line[strlen(line) - 1];
		printf("Exp: %s%s", line, last_char == '\n' ? "" : "\n");
		token = line[pos];
	} else { // Read next token
		pos++;
		token = line[pos];
	}
	return token;
}

int mask(int num) {
	return MASK & num;
}

int E(void) {
	int subtotal = A();
	int value = EE(subtotal);
	return value;
}

int EE(int subtotal) {
	int value;
	if (token == '|') {
		scanner(); // Consume bar
		int st = subtotal | A();
		value = EE(st);
	} else {
		value = subtotal;
	}
	return mask(value);
}

int A(void) {
	int subtotal = B();
	int value = AA(subtotal);
	return value;
}

int AA(int subtotal) {
	int value;
	if (token == '^') {
		scanner(); // Consume caret
		int st = subtotal ^ B();
		value = AA(st);
	} else {
		value = subtotal;
	}
	return mask(value);
}

int B(void) {
	int subtotal = C();
	int value = BB(subtotal);
	return value;
}

int BB(int subtotal) {
	int value;
	if (token == '&') {
		scanner(); // Consume ampersand
		int st = subtotal & C();
		value = BB(st);
	} else {
		value = subtotal;
	}
	return mask(value);
}

int C(void) {
	int value;
	char my_token = token; // Store current token
	scanner(); // Consume next token for all subcalls
	switch (my_token) {
		case '<':
			value = C() << 1;
			break;
		case '>':
			value = C() >> 1;
			break;
		case '~':
			value = ~ C();
			break;
		case '(': {
			value = E();
			if (token != ')') { // Test for closing parenthesis
				puts("Error: Missing parenthesis.");
				exit(1);
			}
			scanner(); // Consume right parenthesis
			break;
		}
		default: {
			char buff[2] = {my_token, 0};
			value = strtol(buff, NULL, 16);
			break;
		}
	}
	return mask(value);
}
