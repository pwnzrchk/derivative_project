#include <ctype.h>
#include "console_handler.h"
#include "lib.h"

//================================================================================================================================================================================

static void PrintHelp(void);
static void PrintCmds(void);

static void FlushStdin(void);
static void DeleteNewLine(char* buffer);
static tCmd GetCmd(char* buffer, int buffer_size);

static tTreeError EnterCmd(tDerivator* der);
static void SeriesConsole(tDerivator* der);
static void LatexConsole(tDerivator* der);

static int graph_counter = 0;
static int latex_counter = 0;
static int series_counter = 0;

//================================================================================================================================================================================

tTreeError ConsoleHandler(tDerivator* der) {

    if (!der) {
        ERPRINT("You're tree is uninitialized\n");
        return kNullPointer;
    }

    PrintHelp();

    bool flag_of_continue = true;
    char input_buffer[kMaxCommandLength];

    while (flag_of_continue) {
        printf("> ");
        tCmd command = GetCmd(input_buffer, sizeof(input_buffer));

        switch(command) {

        case kEnterCmd:
            if (EnterCmd(der) == kNoErrors) printf("Expression read succesfully\n");
            break;

        case kDiffCmd:
            der = DerDiffirentiate(der);
            if (!der) ERPRINT("Error diffirentiating tree\n");
            printf("Differentiated successfully.\n");

            break;

        case kLatexCmd:
            LatexConsole(der);
            break;

        case kGraphCmd:
            GraphConsole(der);
            break;

        case kHelpCmd:
            PrintHelp();
            break;

        case kExitCmd:
            flag_of_continue = false;
            printf("Exiting programm..");
            break;

        case kFuncsCmd:
            PrintCmds();
            break;

        case kSeriesCmd:
            SeriesConsole(der);
            break;

        case kUnknownCmd:
            printf("Unknown command. Type \"help\" for assistance.\n");
            break;

        default:
            return kUnkCmd;
        }
    }
    return kNoErrors;
}

//================================================================================================================================================================================

static void PrintHelp(void) {   // TODO - Сделать новое дерево
    printf("\nAvailable commands:\n");
    printf("  enter     - Enter you're expression.\n");
    printf("  diff      - Diffirentiate you're expression(by x).\n");
    printf("  latex     - Create pdf-file with you're expression.\n");
    printf("  graph     - Create graphviz picture with you're expression.\n");
    printf("  help      - Show help message.\n");
    printf("  funcs     - Show how and what to put in.\n");
    printf("  series    - Taylor series for you're function.\n");
    printf("  exit      - Exit the program.\n");
    printf("  You can use any register.\n\n");
}


static void PrintCmds(void) {
    printf("\nAviable functions for input/use:\n");
    printf("->    Standart ariphmetical - [ + / - / / / * ]\n");
    printf("->    Powering              - [^(...)]\n");
    printf("->    Standart trigonometry - [sin(..) / cos(..)] etc\n");
    printf("->    Extended trigonometry - [ sh(..) / ch(..) ]etc\n");
    printf("->    Standart arc-funcs    - [ arcsin(..) / arccos(..) ] etc\n");
    printf("->    Extended arc-funcs    - [ arcsh(..) / arcch(..) ] etc\n");
    printf("->    Natural logarithm     - [ ln(...) ]\n");
    printf("Please! Use brackets where it needeed (specified above).\n\n");
}


//================================================================================================================================================================================

static tCmd GetCmd(char* buffer, int buffer_size) {
    assert(buffer != NULL);
    assert(buffer_size > 0);

    if (scanf(FORMAT_STR, buffer) <= 0) {
        ERPRINT("Error in reading command\n");
        return kUnknownCmd;
    }

    // char* buffer_for_cmd = (char*)calloc(buffer_size, sizeof(char));
    // if (fgets(buffer, buffer_size, stdin) == NULL) return kExitCmd;
    // DeleteNewLine(buffer);
    //Надо ли флашить stdin?
    // char buffer_for_cmd[kBufferSizeForCmd] = "";    // TODO - поработать над оптимальностью
    // sscanf(buffer, FORMAT_STR, buffer_for_cmd);     // универсальностью ввода

    for (size_t current_symbol = 0; current_symbol < sizeof(buffer_size); current_symbol++) {
        buffer[current_symbol] = (char)tolower(buffer[current_symbol]);
    }

    if (strcmp(buffer, kEnter) == 0)  return kEnterCmd;
    if (strcmp(buffer, kDiff)  == 0)  return kDiffCmd;
    if (strcmp(buffer, kLatex) == 0)  return kLatexCmd;
    if (strcmp(buffer, kGraph) == 0)  return kGraphCmd;
    if (strcmp(buffer, kExit)  == 0)  return kExitCmd;
    if (strcmp(buffer, kHelp)  == 0)  return kHelpCmd;
    if (strcmp(buffer, kFncs)  == 0)  return kFuncsCmd;
    if (strcmp(buffer, kSeries) == 0) return kSeriesCmd;

    return kUnknownCmd;
}

//================================================================================================================================================================================

void GraphConsole(tDerivator* der) {
    assert(der);

    char dot_buf[kBufferLenth], png_buf[kBufferLenth];
    graph_counter++;
    snprintf(dot_buf, kBufferLenth, "files/dot_file_%d.dot", graph_counter);
    snprintf(png_buf, kBufferLenth, "files/picture_%d.svg",  graph_counter);

    GraphDump(der, dot_buf, png_buf);
}

//================================================================================================================================================================================

static void LatexConsole(tDerivator* der) {
    assert(der);

    char tex_buf[kBufferLenth];
    snprintf(tex_buf, kBufferLenth, "files/file_%d.tex", ++latex_counter);
    TexDump(der, tex_buf);

    char* tex_filename = strdup(tex_buf);
    LatexToPdf(tex_filename);
}

//===============================================================================================================================================================================

static tTreeError EnterCmd(tDerivator* der) {
    printf("Please enter you're expression in input format.\n");

    char* buffer = (char*)calloc(kBufferLenth, sizeof(char));
    FlushStdin();
    if (fgets(buffer, kBufferLenth, stdin) == NULL) {
        ERPRINT("Error getting expression\n");
        return kFgetsError;
    }
    DeleteNewLine(buffer);

    size_t index = 0;
    der->root->left = GetGeneral(buffer, &index);

    if (!der->root->left) {
        ERPRINT("Error reading tree");
        return kUserError;
    }
    return kNoErrors;
}

//===============================================================================================================================================================================

static void FlushStdin(void) {
    int buffer_cleaner_value = '\0';
    while ((buffer_cleaner_value = getchar()) != '\n' && buffer_cleaner_value != EOF);
}

//===============================================================================================================================================================================

static void DeleteNewLine(char* buffer) {
    char* newline_address = strchr(buffer, '\n');
    if (newline_address != NULL) {
        *newline_address = '\0';
    }
}

//===============================================================================================================================================================================

static void SeriesConsole(tDerivator* der) {
    assert(der);

    if (!der->root->left) {
        printf("Error: derivator is uninitialized\n");
        return;
    }

    char series_buf[kBufferLenth];
    snprintf(series_buf, kBufferLenth, "files/series_%d.tex", ++series_counter);
    FILE* file_for_series = fopen(series_buf, "w");
    if (!file_for_series) {
        ERPRINT("Error openning TeX file for print series\n");
        return;
    }
    tDerivator* diffed_der = Series(der, file_for_series);

    GraphConsole(diffed_der);   // DEBUG

    fclose(file_for_series);

    char* series_filename = strdup(series_buf);
    LatexToPdf(series_filename);
    return;
}

//===============================================================================================================================================================================
