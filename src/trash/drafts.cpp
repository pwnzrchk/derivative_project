/* Unused functions, may be will use/modify in future

#include "lib.h"

//================================================================================================================================================================================

static bool IsNodesEqual(tNode* first, tNode* second);
static size_t SubTreeSize(tNode* node);

//================================================================================================================================================================================

tTreeError NodeOptor(tNode* node) {
    assert(node != NULL);

    do {
        size_before = SubTreeSize(node);
        Optor
    } while

}

//================================================================================================================================================================================

tTreeError TreeOptor(tDerivator* der) { // FIXME - сравнивать сайз
    assert(der != NULL);

    if(der->root->left == NULL) return kNullPointer;

    tNode* flag_node = CopyNode(der->root->left);
    if (flag_node == NULL) return kNullPointer;

    while(!IsNodesEqual(Optor(der, der->root->left), flag_node)) {
        flag_node = CopyNode(der->root->left);
        if (flag_node == NULL) return kNullPointer;
    }
    NodeDtor(flag_node);

    return kNoErrors;
}

//================================================================================================================================================================================


static bool IsVariable(tNode* node) {
    assert(node != NULL);

    if(node == NULL) return false;

    return node->type == kVariable;
}

//================================================================================================================================================================================

bool EqualTree(tDerivator* der1, tDerivator* der2) {
    if (IsNodesEqual(der1->root->left, der2->root->left)) return true;
    return false;
}

//================================================================================================================================================================================

static bool IsNodesEqual(tNode* first, tNode* second) {

    if(first == NULL && second == NULL) return true;
    if(first == NULL || second == NULL) return false;

    if(first->type != second->type) return false;

    bool is_data_equal = false;

    switch(first->type) {
        case kConst:
        is_data_equal = EqualConstValue(first, second->data.value); // Сделать нормальную функция сравния double
        break;

        case kVariable:
        case kOperation:
            is_data_equal = (first->data.code == second->data.code);
            break;

        default:
            return false;
    }

    if (is_data_equal == false) return false;

    return IsNodesEqual(first->left, second->left) && IsNodesEqual(first->right, second->right);
}

//================================================================================================================================================================================


static size_t SubTreeSize(tNode* node) {
    assert(node);
    size_t counter = 0;
    TreeSize(node, &counter);

    return counter;
}

//================================================================================================================================================================================

tNode* NodeDiff(tNode* node) {
    assert(node != NULL);


}

//================================================================================================================================================================================

static void PrintHtml(const char* html_filename, const char* png_filename) {
    assert(html_filename != NULL);
    assert(png_filename != NULL);

    FILE* html_file = fopen(html_filename, "w");
    if (html_file == NULL) {
        fprintf(stderr, "Error: Could not open HTML log file %s\n", html_filename);
        return;
    }

    fprintf(html_file, "<h2>Tree Dump </h2>\n");
    fprintf(html_file, "<img src=\"%s\" width = 2000px>\n", png_filename);
    fprintf(html_file, "<hr>\n\n");

    fclose(html_file);
}

//================================================================================================================================================================================

static void DeleteNewLine(char* buffer) {
    char* newline_address = strchr(buffer, '\n');
    if (newline_address != NULL) {
        *newline_address = '\0';
    }
}

//================================================================================================================================================================================

static bool IsPriorityHigher(tNode* node) {
    assert(node != NULL);
    if (!node->parent) return false;

    int par_code = FindPriority(node->parent->data.code);
    int node_code = FindPriority(node->data.code);

    if (par_code > node_code) return true;
    return false;
}

//================================================================================================================================================================================

static int FindPriority(int code) {
    if (code == kPlus || code == kMinus) return 1;
    if (code == kMul  || code == kDiv)   return 2;
    if (code == kPow) return 3;

    return 0;
}

//================================================================================================================================================================================

tDerivator* DerDiffirentiate(tDerivator* der) {
    assert(der);
    assert(der->root->left);

    DerOptor(der);
    tNode* old_node = der->root->left;
    if (!old_node) return NULL;
    tNode* new_node = Differentiator(old_node);
    if (new_node == NULL) {
        ERPRINT("Error diffirentiating node in");
        return NULL;
    }
    der->root->left = new_node;
    new_node->parent = der->root;

    DerOptor(der);
    return der;
}

//================================================================================================================================================================================

*/


