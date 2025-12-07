#include "tex_dump.h"
#include <stdbool.h>

//=================================================================================================================================================

static tTreeError BeginTex(FILE* file);
static tTreeError EndTex(FILE* file);
static tTreeError OneargumentTex(tNode* node, FILE* file);
static tTreeError BiargumentTex(tNode* node, FILE* file);

//=================================================================================================================================================

tTreeError TexDump(tDerivator* der, const char* filename) {
    assert(filename != NULL);

    FILE* file = fopen(filename, "w");
    if (!file) return kNullPointer; 

    tTreeError error_for_all = kNoErrors;

    if ((error_for_all = BeginTex(file)) != kNoErrors) return error_for_all;  //В макрос
    if ((error_for_all = NodeTex (der->root->left, file)) != kNoErrors) return error_for_all;
    if ((error_for_all = EndTex  (file)) != kNoErrors) return error_for_all;

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

    switch(node->data.code) {

        case kPlus:
            fprintf(file, "(");
            // fprintf(file, "(");
            NodeTex(node->left, file);
            // fprintf(file, ")");

            fprintf(file, " \\ + \\ ");

            // fprintf(file, "(");
            NodeTex(node->right, file);
            // fprintf(file, ")");
            fprintf(file, ")");

            break;

        case kMinus:

            fprintf(file, "( ");
            NodeTex(node->left, file);
            // fprintf(file, ")");

            fprintf(file, " \\ - \\ ");

            // fprintf(file, "(");
            NodeTex(node->right, file);
            fprintf(file, " )");


            break;

        case kMul:
            fprintf(file, "( ");
            NodeTex(node->left, file);
            // fprintf(file, ") ");

            fprintf(file, " \\ * \\ ");

            // fprintf(file, "(");
            NodeTex(node->right, file);
            fprintf(file, " )");

            break;

        case kDiv:
            // fprintf(file, "( ");
            fprintf(file, "\\frac{ ");
            NodeTex(node->left, file);
            fprintf(file, " }{ ");
            NodeTex(node->right, file);
            fprintf(file, " }");
            // fprintf(file, " )");
            break;

        case kPow:
            // fprintf(file, "( ");
            fprintf(file, "{ ");
            NodeTex(node->left, file);
            fprintf(file, " }^{ ");
            NodeTex(node->right, file);
            fprintf(file, " }");
            // fprintf(file, " )");
            break;

        default:
            return kOneArgWrongCode;
    }
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

            break;

        default:
            return kTwoArgWrongCode;
    }
    return kNoErrors;

}

//=================================================================================================================================================

bool IsPriorityHighre(tNode* node) {}
