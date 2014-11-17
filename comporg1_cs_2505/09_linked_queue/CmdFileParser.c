#include "CmdFileParser.h"
#include <inttypes.h>
#include <string.h>

static FILE* cmds;
static bool hasParameter(char* word);

bool setCmdFile(char* fName) {

   cmds = fopen(fName, "r");
   return (cmds != NULL);
}

void closeCmdFile() {

   fclose(cmds);
}

Command getNextCommand() {

   char trailingstuff[100];
   Command nextCmd;
   fscanf(cmds, "%s\t", nextCmd.word);
   if ( hasParameter(nextCmd.word) )
        fscanf(cmds, "%"PRId32"\n", &nextCmd.parameter);
   else
        fscanf(cmds, "%s\n", trailingstuff);
   return nextCmd;
}

static bool hasParameter(char* word) {

   if ( strcmp(word, "push") == 0 ) return true;
   return false;
}
