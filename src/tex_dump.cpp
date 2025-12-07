#include "tex_dump.h"
#include <stdbool.h>

// static const char* const phrases[] = {"Нетрудо заметить что:",
//                                "Это мошъит",
//                                "Petrovich gromko pisaet"
//                               };
//
// static int global_index = 0;

//=================================================================================================================================================

static tTreeError BeginTex(FILE* file);
static tTreeError EndTex(FILE* file);

static tTreeError OneargumentTex(tNode* node, FILE* file);
static tTreeError BiargumentTex(tNode* node, FILE* file);

static bool IsPriorityHigher(tNode* node);
static int FindPriority(int code);

//=================================================================================================================================================

tTreeError TexDump(tDerivator* der, const char* filename) {
    assert(filename != NULL);

    FILE* file = fopen(filename, "w");
    if (!file) return kNullPointer;

    tTreeError error_for_all = kNoErrors;

    if ((error_for_all = BeginTex(file)) != kNoErrors)      return error_for_all;  //В макрос
    if ((error_for_all = TreeDump(der, file)) != kNoErrors) return error_for_all;
    if ((error_for_all = EndTex  (file)) != kNoErrors)      return error_for_all;

    return kNoErrors;
    }

//=================================================================================================================================================

tTreeError TreeDump(tDerivator* der, FILE* file) {
    assert(file);
    assert(der);

    tTreeError err_for_print = kNoErrors;
    TexPrint("Petrovich pisaet", file);
    if ((err_for_print = NodeTex(der->root->left, file)) != kNoErrors) return err_for_print;

    return kNoErrors;

}

//=================================================================================================================================================

static tTreeError BeginTex(FILE* file) {
    assert(file != NULL);

    if (file == NULL) return kNullPointer;
    fprintf(file, "\\documentclass[12pt, litterpaper]{article}\n"
                  "\\usepackage{graphicx}\n"
                  "\\usepackage{mathtext}\n"
                  "\\usepackage[utf8]{inputenc}\n"
                  "\\usepackage[english, russian]{babel}\n"
                  "\\usepackage{float}\n"
                  "\\usepackage{caption}\n"
                  "\\usepackage{lscape}\n"
                  "\\usepackage{float}\n"
                  "\\usepackage{tabularx}\n"
                  "\\usepackage{pgfplots}\n"
                  "\\pgfplotsset{compat=1.18}\n"

                  "\n\\begin{document}\n"
                  "\n\\[ \n");
    return kNoErrors;
}

//=================================================================================================================================================

static tTreeError EndTex(FILE* file) {
    assert(file != NULL);

    if (file == NULL) return kNullPointer;

    fprintf(file, "\n\\]\n"
                  "\n\\end{document}\n");

    return kNoErrors;
}

//=================================================================================================================================================

tTreeError NodeTex(tNode* node, FILE* file) {
    assert(file != NULL);
    assert(node != NULL);

    if (file == NULL) return kNullPointer;
    if (node == NULL) return kNullPointer;


    if (node->type == kConst) {
        fprintf(file, "%.2lf", node->data.value);

        return kNoErrors;
    }

    if (node->type == kVariable) {
        char var = VariableDecoder(node->data.code);
        fprintf(file, "%c", var);

        return kNoErrors;
    }

    if (node->type == kOperation) {
        bool is_biargument = IsBiargument(node->data.code);

        if (is_biargument == true) {
            tTreeError err_for_biprint = kNoErrors;
            if ((err_for_biprint = BiargumentTex(node, file)) != kNoErrors)
                return err_for_biprint;

            return kNoErrors;
        }

        if (is_biargument == false) {
            tTreeError err_for_oneprint = kNoErrors;
            if ((err_for_oneprint = OneargumentTex(node, file)) != kNoErrors)
                return err_for_oneprint;

            return kNoErrors;
        }

    }
    return kWrongNode;
}

//=================================================================================================================================================

tTreeError TexPrint(const char* str, FILE* file) {
    assert(file);
    assert(str);

    if (!str)  return kNullPointer;
    if (!file) return kNullPointer;

    fprintf(file, "%s", str);

    return kNoErrors;
}

//=================================================================================================================================================

//=================================================================================================================================================

static tTreeError BiargumentTex(tNode* node, FILE* file) {
    bool flag_of_quotes = IsPriorityHigher(node);
    if (flag_of_quotes) fprintf(file, "(");

    switch(node->data.code) {

        case kPlus:
            NodeTex(node->left, file);
            fprintf(file, " \\ + \\ ");
            NodeTex(node->right, file);
            break;

        case kMinus:
            NodeTex(node->left, file);
            fprintf(file, " \\ - \\ ");
            NodeTex(node->right, file);
            break;

        case kMul:
            NodeTex(node->left, file);
            fprintf(file, " \\ * \\ ");
            NodeTex(node->right, file);

            break;

        case kDiv:
            fprintf(file, "\\frac{ ");
            NodeTex(node->left, file);
            fprintf(file, " }{ ");
            NodeTex(node->right, file);
            fprintf(file, " }");
            break;

        case kPow:
            fprintf(file, "{ ");
            NodeTex(node->left, file);
            fprintf(file, " }^{ ");
            NodeTex(node->right, file);
            fprintf(file, " }");
            break;

        default:
            return kOneArgWrongCode;
    }
    if (flag_of_quotes) fprintf(file, ")");
    return kNoErrors;

}

//=================================================================================================================================================

static tTreeError OneargumentTex(tNode* node, FILE* file) {

    switch(node->data.code) {

        case kSin:
            fprintf(file, "sin({ ");
            NodeTex(node->left, file);
            fprintf(file, " }) ");
            break;

        case kCos:
            fprintf(file, "cos({ ");
            NodeTex(node->left, file);
            fprintf(file, " }) ");

            break;

        case kExp:
            fprintf(file, "e^{ ");
            NodeTex(node->left, file);
            fprintf(file, " }");

            break;

        case kTg:
            fprintf(file, "tg({ ");
            NodeTex(node->left, file);
            fprintf(file, " }) ");

            break;

        case kCtg:
            fprintf(file, "ctg({ ");
            NodeTex(node->left, file);
            fprintf(file, " }) ");

            break;

        case kLn:
            fprintf(file, "ln({ ");
            NodeTex(node->left, file);
            fprintf(file, " })");

            break;

        case kSh:
            fprintf(file, "sh({ ");
            NodeTex(node->left, file);
            fprintf(file, " }) ");

            break;

        case kCh:
            fprintf(file, "ch({ ");
            NodeTex(node->left, file);
            fprintf(file, " })");

            break;

        case kTgh:
            fprintf(file, "tgh({ ");
            NodeTex(node->left, file);
            fprintf(file, " })");

            break;

        case kCtgh:
            fprintf(file, "ctgh({ ");
            NodeTex(node->left, file);
            fprintf(file, " } )");

        case kArcSin:
            fprintf(file, "arcsin({ ");
            NodeTex(node->left, file);
            fprintf(file, " } )");


        case kArcCos:
            fprintf(file, "arccos({ ");
            NodeTex(node->left, file);
            fprintf(file, " } )");
            break;

        case kArcTg:
            fprintf(file, "arctg({ ");
            NodeTex(node->left, file);
            fprintf(file, " } )");
            break;

        case kArcCtg:
            fprintf(file, "arcctg({ ");
            NodeTex(node->left, file);
            fprintf(file, " } )");
            break;

        case kArcSh:
            fprintf(file, "arcsh({ ");
            NodeTex(node->left, file);
            fprintf(file, " } )");
            break;

        case kArcCh:
            fprintf(file, "arcch({ ");
            NodeTex(node->left, file);
            fprintf(file, " } )");
            break;

        case kArcTh:
            fprintf(file, "arcth({ ");
            NodeTex(node->left, file);
            fprintf(file, " } )");
            break;

        case kArcCth:
            fprintf(file, "arccth({ ");
            NodeTex(node->left, file);
            fprintf(file, " } )");
            break;

        default:
            return kTwoArgWrongCode;
    }
    return kNoErrors;

}

//=================================================================================================================================================

static int FindPriority(int code) {
    if (code == kPlus || code == kMinus) return 1;
    if (code == kMul  || code == kDiv)   return 2;
    if (code == kPow) return 3;

    return 0;
}

//=================================================================================================================================================
// Если приоритет родитяля выше -> печатаем
static bool IsPriorityHigher(tNode* node) {
    assert(node != NULL);
    if (!node->parent) return false;

    int par_code = FindPriority(node->parent->data.code);
    int node_code = FindPriority(node->data.code);

    if (par_code > node_code) return true;
    return false;
}

//=================================================================================================================================================

int LatexToPdf(const char* tex_filename) {
    assert(tex_filename);

    char sys_command[kSysCommandSize];
    snprintf(sys_command, sizeof(sys_command), "pdflatex %s", tex_filename);

    int result = system(sys_command);

    if (result != 0) {
        ERPRINT("Error in generatint sys_command for Latex\n");
    }

    return result;
}

//=================================================================================================================================================
