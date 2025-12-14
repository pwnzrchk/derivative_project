#include "tex_dump.h"
#include <stdbool.h>

//================================================================================================================================================================================

static const char* const phrases[] = {"Нетрудо заметить что:\n",
                                      "Это мошъит\n",
                                      "Petrovich gromko pisaet\n"
                                     };

static size_t global_index = 0;

//================================================================================================================================================================================

static tTreeError OneargumentTex(tNode* node, FILE* file);
static tTreeError BiargumentTex (tNode* node, FILE* file);

static int  GetPriority (int code);
static bool NeedBrackets(tNode* node);

//================================================================================================================================================================================

tTreeError TexDump(tDerivator* der, const char* filename) {
    assert(filename);
    assert(der);

    FILE* file = fopen(filename, "w");
    if (!file) {
        ERPRINT("Error openning file for write latex format");
        return kNullPointer;
    }

    tTreeError error_for_all = kNoErrors;
    if ((error_for_all = BeginTex(file))      != kNoErrors) { fclose(file); return error_for_all; }
    if ((error_for_all = TreeDump(der, file)) != kNoErrors) { fclose(file); return error_for_all; }
    if ((error_for_all = EndTex  (file))      != kNoErrors) { fclose(file); return error_for_all; }
    fclose(file);

    return error_for_all;
}

//================================================================================================================================================================================

tTreeError TreeDump(tDerivator* der, FILE* file) {

    tTreeError err_for_print = kNoErrors;
    if ((err_for_print = NodeTex(der->root->left, file)) != kNoErrors) return err_for_print;

    return kNoErrors;
}

//================================================================================================================================================================================

tTreeError BeginTex(FILE* file) {
    assert(file != NULL);

    if (file == NULL) return kNullPointer;
    fprintf(file, "\\documentclass[12pt]{article}\n"
                  "\\usepackage[utf8]{inputenc}\n"
                  "\\usepackage[russian]{babel}\n"
                  "\\usepackage{amsmath}\n"
                  "\\usepackage{geometry}\n"
                  "\\geometry{verbose,a4paper,tmargin=2cm,bmargin=2cm,lmargin=2.5cm,rmargin=1.5cm}\n"
                  "\n\\begin{document}\n");
    fprintf(file, "\\begin{landscape}\n\n");
    const char* const added_phrase = phrases[ global_index++ % (sizeof(phrases)/sizeof(phrases[0])) ];
    TexPrint(added_phrase, file);
    fprintf(file, "\\begin{equation*} \\begin{aligned}\n");
    return kNoErrors;
}

//================================================================================================================================================================================

tTreeError EndTex(FILE* file) {
    assert(file != NULL);

    if (file == NULL) return kNullPointer;

    fprintf(file, "\n\\end{aligned} \\end{equation*}\n"
                  "\n\\end{landscape}\n"
                  "\n\\end{document}\n");

    return kNoErrors;
}

//================================================================================================================================================================================

tTreeError NodeTex(tNode* node, FILE* file) {
    assert(file != NULL);
    assert(node != NULL);

    if (file == NULL) return kNullPointer;
    if (node == NULL) return kNullPointer;


    if (node->type == kConst) {
        fprintf(file, "%g", node->data.value);

        return kNoErrors;
    }

    if (node->type == kVariable) {
        char var = VariableDecoder(node->data.code);
        fprintf(file, "%c", var);

        return kNoErrors;
    }

    if (node->type == kOperation) {
        bool is_biargument = IsBiargument(node->data.code);
        bool need_brackets = NeedBrackets(node);
        if (need_brackets) fprintf(file, "\\left(");

        if (is_biargument) {
            tTreeError err_for_biprint = kNoErrors;
            if ((err_for_biprint = BiargumentTex(node, file)) != kNoErrors) return err_for_biprint;
        } else {
            tTreeError err_for_oneprint = kNoErrors;
            if ((err_for_oneprint = OneargumentTex(node, file)) != kNoErrors) return err_for_oneprint;
        }

        if (need_brackets) fprintf(file, "\\right)");
        return kNoErrors;

    }
    return kWrongNode;
}

//================================================================================================================================================================================

tTreeError TexPrint(const char* str, FILE* file) {
    assert(file);
    assert(str);

    if (!str)  return kNullPointer;
    if (!file) return kNullPointer;

    fprintf(file, "%s", str);

    return kNoErrors;
}

//================================================================================================================================================================================

//================================================================================================================================================================================

static tTreeError BiargumentTex(tNode* node, FILE* file) {
    switch(node->data.code) {

        case kPlus:
            NodeTex(node->left, file);
            fprintf(file, " + ");
            NodeTex(node->right, file);
            break;

        case kMinus:
            NodeTex(node->left, file);
            fprintf(file, " - ");
            NodeTex(node->right, file);
            break;

        case kMul:
            NodeTex(node->left, file);
            fprintf(file, " \\cdot ");
            NodeTex(node->right, file);
            break;

        case kDiv:
            fprintf(file, "\\frac{");
            NodeTex(node->left, file);
            fprintf(file, "}{");
            NodeTex(node->right, file);
            fprintf(file, "}");
            break;

        case kPow:
            fprintf(file, "{");
            NodeTex(node->left, file);
            fprintf(file, "}^{");
            NodeTex(node->right, file);
            fprintf(file, "}");
            break;

        default:
            return kOneArgWrongCode;
    }
    return kNoErrors;

}

//================================================================================================================================================================================

static tTreeError OneargumentTex(tNode* node, FILE* file) {
    const char* func_name = NULL;
    switch(node->data.code) {
case kSin:    func_name = "\\sin"; break;
        case kCos:    func_name = "\\cos";    break;
        case kTg:     func_name = "\\tg";     break;
        case kCtg:    func_name = "\\ctg";    break;
        case kLn:     func_name = "\\ln";     break;
        case kExp:    func_name = "\\e^";     break;
        case kArcSin: func_name = "\\arcsin"; break;
        case kArcCos: func_name = "\\arccos"; break;
        case kArcTg:  func_name = "\\arctg";  break;
        case kArcCtg: func_name = "\\arcctg"; break;
        case kSh:     func_name = "\\sinh";   break;
        case kCh:     func_name = "\\cosh";   break;
        case kTgh:    func_name = "\\tanh";   break;
        case kCtgh:   func_name = "\\coth";   break;
        default:      func_name = "unknown";  return kTwoArgWrongCode;
    }

    if (node->data.code == kExp) {
        fprintf(file, "e^{");
        NodeTex(node->left, file);
        fprintf(file, "}");
    } else {
        fprintf(file, "%s\\left(", func_name);
        NodeTex(node->left, file);
        fprintf(file, "\\right)");
    }
    return kNoErrors;
}

//================================================================================================================================================================================

static int GetPriority(int code) {
    switch (code) {
        case kPlus:
        case kMinus:
            return 1;

        case kMul:
        case kDiv:
            return 2;

        case kPow:
            return 3;

        default:
            return 10;
    }
}

//================================================================================================================================================================================

static bool NeedBrackets(tNode* node) {
    if (!node->parent) return false;
    if (node->parent->type != kOperation) return false;

    int parent_code = node->parent->data.code;
    int node_code   = node->data.code;

    if (node_code == kDiv) return false;
    if (!IsBiargument(parent_code) && parent_code != kExp) return false;
    if (parent_code == kExp) return false;

    int parent_pri = GetPriority(parent_code);
    int node_pri   = GetPriority(node_code);

    if (parent_pri > node_pri) return true;
    if (parent_pri < node_pri) return false;

    if (parent_pri == node_pri) {
        if (parent_code == kMinus) {
            if (node == node->parent->right) return true;
        }
        if (parent_code == kPow) {
            if (node == node->parent->left) return true;
        }
    }
    return false;
}

//================================================================================================================================================================================

//================================================================================================================================================================================

//================================================================================================================================================================================

int LatexToPdf(const char* tex_filename) {
    assert(tex_filename);

    char sys_command[kSysCommandSize];
    snprintf(sys_command, sizeof(sys_command), "pdflatex -interaction=nonstopmode -output-directory=files %s > /dev/null", tex_filename);
    printf("Executing: %s\n", sys_command);
    int result = system(sys_command);

    if (result != 0) {
        ERPRINT("Error in generating sys_command for Latex");
    } else {
        printf("PDF generated successfully.\n");
    }

    return result;
}

//================================================================================================================================================================================
