#include "dif_optimizator.h"
#include "math_func.h"

//=================================================================================================================================================

static tTreeError ReplaceNode(tNode* source, tNode* replacement);
static void ConstFolding(tDerivator* der, tNode* node);
static void PowFolding(tDerivator* der, tNode* node);
static void MulFolding(tDerivator* der, tNode* node);
static void PlusMinusFolding(tDerivator* der, tNode* node);
static void DivFolding(tDerivator* der, tNode* node);
static void Optor (tDerivator* der, tNode* node);

//=================================================================================================================================================

tTreeError TreeOptor(tDerivator* der) {
    Optor(der, der->root);
    return kNoErrors;
}

//=================================================================================================================================================

static void Optor(tDerivator* der, tNode* node) {
    assert(der != NULL);

    if (node == NULL) return;

    ConstFolding(der, node);
    if (node->type == kOperation && node->data.code == kPow) PowFolding(der, node);
    if (node->type == kOperation && node->data.code == kMul) MulFolding(der, node);
    if (node->type == kOperation && (node->data.code == kPlus || node->data.code == kMinus)) PlusMinusFolding(der, node);
    if (node->type == kOperation && node->data.code == kDiv) DivFolding(der, node); //WARNING - null divisiong, what to do?

    Optor(der, node->left);
    Optor(der, node->right);
}

//=================================================================================================================================================

static tTreeError ReplaceNode(tNode* source, tNode* replacement) {
    assert(source != NULL);
    assert(replacement != NULL);

    if (source == NULL)      return kNullPointer;
    if (replacement == NULL) return kNullPointer;

    (source == source->parent->left ? source->parent->left : source->parent->right) = replacement;
    return kNoErrors;
}

//=================================================================================================================================================

static void ConstFolding(tDerivator* der, tNode* node) {
    assert(der != NULL);
    assert(node != NULL);

    if(node->left == NULL || node->right == NULL) return;
    if (node->left->type == kConst && node->right->type == kConst) {
        double value = NodeCaltor(der, node);
        tNode* buffer_node = NUM(value);
        if (ReplaceNode(node, buffer_node) != kNoErrors) ERRPRINT(replacing nodes in folding)
    }

    if (node->left->type == kConst && node->type == kOperation && !IsBiargument(node->data.code)) {
        double value = NodeCaltor(der, node);
        tNode* buffer_node = NUM(value);
        if (ReplaceNode(node, buffer_node) != kNoErrors) ERRPRINT(replacing nodes in folding)
    }
}

//=================================================================================================================================================

static void PowFolding(tDerivator* der, tNode* node) {
    assert(der != NULL);
    assert(node != NULL);

    if(node->left == NULL || node->right == NULL) return;
    if ((node->left->type == kConst && node->left->data.value  == 1) ||
       (node->right->type == kConst && node->right->data.value == 1)) {   //TODO - функция сравнения даблов

            tNode* replace_node = ((node->right->type == kConst && node->right->data.value == 1) ? node->left : node->right);
            if (ReplaceNode(node, replace_node) != kNoErrors) ERRPRINT(replacing nodes in pow_folding)
        }

    if ((node->left->type == kConst && node->left->data.value  == 0) ||
       (node->right->type == kConst && node->right->data.value == 0)) {

        if (ReplaceNode(node, NUM(1)) != kNoErrors) ERRPRINT(replacing nodes in pow_folding)
    }

}

//=================================================================================================================================================

static void MulFolding(tDerivator* der, tNode* node) {
    assert(der != NULL);
    assert(node != NULL);

    if(node->left == NULL || node->right == NULL) return;
    if ((node->left->type == kConst && node->left->data.value  == 1) ||
       (node->right->type == kConst && node->right->data.value == 1)) {

            tNode* replace_node = ((node->right->type == kConst && node->right->data.value == 1) ? node->left : node->right);
            if (ReplaceNode(node, replace_node) != kNoErrors) ERRPRINT(replacing nodes in mul_folding)
        }

    if ((node->left->type == kConst && node->left->data.value  == 0) ||
       (node->right->type == kConst && node->right->data.value == 0)) {

        if (ReplaceNode(node, NUM(0)) != kNoErrors) ERRPRINT(replacing nodes in mul_folding)
    }
}

//=================================================================================================================================================

static void PlusMinusFolding(tDerivator* der, tNode* node) {
    assert(der != NULL);
    assert(node != NULL);

    if(node->left == NULL || node->right == NULL) return;
    if ((node->left->type == kConst && node->left->data.value  == 0) ||
       (node->right->type == kConst && node->right->data.value == 0)) {

            tNode* replace_node = ((node->right->type == kConst && node->right->data.value == 0) ? node->left : node->right);
            if (ReplaceNode(node, replace_node) != kNoErrors) ERRPRINT(replacing nodes in plus_minus_folding)
        }
}

//=================================================================================================================================================

static void DivFolding(tDerivator* der, tNode* node) {
    assert(der != NULL);
    assert(node != NULL);

    if(node->left == NULL || node->right == NULL) return;
    if ((node->left->type == kConst && node->left->data.value  == 1) ||
       (node->right->type == kConst && node->right->data.value == 1)) {

            tNode* replace_node = ((node->right->type == kConst && node->right->data.value == 1) ? node->left : node->right);
            if (ReplaceNode(node, replace_node) != kNoErrors) ERRPRINT(replacing nodes in mul_folding)
        }
}

//=================================================================================================================================================
