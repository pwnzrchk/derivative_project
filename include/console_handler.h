#ifndef CONSOLE_HANDLER_H
#define CONSOLE_HANDLER_H

#include "lib.h"

//================================================================================================================================================================================

enum tCmd {
    kEnterCmd   = 1,
    kDiffCmd    = 2,
    kLatexCmd   = 3,
    kHelpCmd    = 4,
    kExitCmd    = 5,
    kGraphCmd   = 6,
    kFuncsCmd   = 7,
    kSeriesCmd  = 8,
    kUnknownCmd = 10
};

static const int kBufferSizeForCmd = 32;
#define FORMAT_STR "%31s"

static const int kSizeOfFileName   = 64;
static const int kMaxCommandLength = 128;
static const int kBufferLenth = 64;

static const char* const kEnter = "enter";
static const char* const kDiff  = "diff";
static const char* const kLatex = "latex";
static const char* const kGraph = "graph";
static const char* const kExit  = "exit";
static const char* const kHelp  = "help";
static const char* const kFncs  = "funcs";
static const char* const kSeries = "series";

//================================================================================================================================================================================

tTreeError ConsoleHandler(tDerivator* der);
// WARNING
void GraphConsole(tDerivator* der);


//================================================================================================================================================================================

#endif // CONSOLE_HANDLER_H
