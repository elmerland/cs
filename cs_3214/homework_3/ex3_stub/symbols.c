// The symbol is in the uninitialized data section (BSS)
static int b;
// the symbol is in the initialized data section
static int d = 10;
// The symbol is in a read only data section
static const int r = 20;

// The symbol is undefined
extern int U;
// Symbol is common
void * C;
// Symbol is in the initialized data section
void * D = &d;

// The symbol is in the text (code) section
void T (void) {
	U = 33;
}
// The symbol is in the text (code) section
static void t(void) {}
