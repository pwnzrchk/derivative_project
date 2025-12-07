#include "lib.h"

//=================================================================================================================================================

static bool IsNodesEqual(tNode* first, tNode* second);
static size_t SubTreeSize(tNode* node);

//=================================================================================================================================================

tTreeError NodeOptor(tNode* node) {
    assert(node != NULL);

    do {
        size_before = SubTreeSize(node);
        Optor
    } while

}

//=================================================================================================================================================

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

//=================================================================================================================================================


static bool IsVariable(tNode* node) {
    assert(node != NULL);

    if(node == NULL) return false;

    return node->type == kVariable;
}

//=================================================================================================================================================

bool EqualTree(tDerivator* der1, tDerivator* der2) {
    if (IsNodesEqual(der1->root->left, der2->root->left)) return true;
    return false;
}

//=================================================================================================================================================

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

//=================================================================================================================================================


static size_t SubTreeSize(tNode* node) {
    assert(node);
    size_t counter = 0;
    TreeSize(node, &counter);

    return counter;
}

//=================================================================================================================================================

tNode* NodeDiff(tNode* node) {
    assert(node != NULL);


}

//=================================================================================================================================================

