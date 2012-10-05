/*
  Adventure Game Studio source code Copyright 1999-2011 Chris Jones.
  All rights reserved.

  The AGS Editor Source Code is provided under the Artistic License 2.0
  http://www.opensource.org/licenses/artistic-license-2.0.php

  You MAY NOT compile your own builds of the engine without making it EXPLICITLY
  CLEAR that the code has been altered from the Standard Version.

*/

#ifndef __AC_DEBUG_LOG_H
#define __AC_DEBUG_LOG_H

#include "script/cc_instance.h"
#include "ac/runtime_defines.h"
#include "ac/gamestate.h"
#include "platform/base/agsplatformdriver.h"

extern GameState play;
void initialize_debug_system();
void shutdown_debug_system();

#define DEBUG_CONSOLE if (play.debug_mode) debug_write_console

void debug_write_console (char *msg, ...);

/* The idea of this is that non-essential errors such as "sound file not
found" are logged instead of exiting the program.
*/
void debug_log(char*texx, ...);
void quitprintf(char*texx, ...);
bool init_editor_debugging();

// allow LShift to single-step,  RShift to pause flow
void scriptDebugHook (ccInstance *ccinst, int linenum) ;

extern int debug_flags;

struct DebugConsoleText {
    char text[100];
    char script[12];
};

extern DebugConsoleText debug_line[DEBUG_CONSOLE_NUMLINES];
extern int first_debug_line, last_debug_line, display_console;


extern AGSPlatformDriver *platform;

// this function is only enabled for special builds if a startup
// issue needs to be checked
#define write_log_debug(msg) platform->WriteDebugString(msg)
/*extern "C" {
void write_log_debug(const char*msg) {
//if (play.debug_mode == 0)
//return;

char toWrite[300];
sprintf(toWrite, "%02d/%02d/%04d %02d:%02d:%02d (EIP=%4d) %s", sc_GetTime(4), sc_GetTime(5),
sc_GetTime(6), sc_GetTime(1), sc_GetTime(2), sc_GetTime(3), our_eip, msg);

FILE*ooo=fopen("ac.log","at");
fprintf(ooo,"%s\n", toWrite);
fclose(ooo);
}
}*/

#endif // __AC_DEBUG_LOG_H
