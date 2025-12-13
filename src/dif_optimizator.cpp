#include "dif_optimizator.h"
#include "math_func.h"

//================================================================================================================================================================================

static tTreeError ReplaceNode(tNode* source, tNode* replacement);
static size_t DerivatorSize(tDerivator* der);

static void ConstFolding(tNode* node);
static void PowFolding(tNode* node);
static void MulFolding(tNode* node);
static void PlusMinusFolding(tNode* node);
static void DivFolding(tNode* node);

static void TreeSize(tNode* node, size_t* curr_size);
static tNode* Optor (tDerivator* der, tNode* node);

static bool EqualConstValue(tNode* node, double value);
static bool IsOperation(tNode* node);
static bool IsConst(tNode* node);

//================================================================================================================================================================================

//================================================================================================================================================================================

tTreeError DerOptor(tDerivator* der) {
    assert(der != NULL);

    if(der->root->left == NULL) return kNullPointer;
    size_t size_before = 0, size_after = 0;

    do {
        size_before = DerivatorSize(der);
        Optor(der, der->root->left);
        size_after  = DerivatorSize(der);

    } while(size_before != size_after);
    return kNoErrors;
}

//================================================================================================================================================================================

//================================================================================================================================================================================

static bool IsOperation(tNode* node) {
    assert(node != NULL);

    if(node == NULL) return false;

    return node->type == kOperation;
}

static bool IsConst(tNode* node) {

    if (node == NULL) return false;

    return node->type == kConst;
}

//================================================================================================================================================================================

static tNode* Optor(tDerivator* der, tNode* node) {
    assert(der != NULL);

    if (node == NULL) return NULL;

    ConstFolding(der, node);
    if (IsOperation(node) && node->data.code  == kPow) PowFolding(node);
    if (IsOperation(node) && node->data.code  == kMul) MulFolding(node);
    if (IsOperation(node) && (node->data.code == kPlus || node->data.code == kMinus)) PlusMinusFolding(node);
    if (IsOperation(node) && node->data.code  == kDiv) DivFolding(node);

    Optor(der, node->left);
    Optor(der, node->right);

    return node;
}

//================================================================================================================================================================================

static tTreeError ReplaceNode(tNode* source, tNode* replacement) { // WARNING - Тут не работает Caller-allocates
    assert(source != NULL);                                        // Не caller удаляет замененную ноду
    assert(replacement != NULL);

    if (source == NULL)         return kNullPointer;
    if (replacement == NULL)    return kNullPointer;

    if (replacement->parent == source) {
        if (source->left == replacement)  source->left = NULL;
        if (source->right == replacement) source->right = NULL;
    }

    if (source == source->parent->left) {
        source->parent->left = replacement;
    } else {
        source->parent->right = replacement;
    }
    replacement->parent = source->parent;

    source->left = NULL;
    source->right = NULL;
    source->parent = NULL;

    // NodeDtor(source);         // How to avoid memory leak
    // source = replacement;

    return kNoErrors;
}

//================================================================================================================================================================================

static void ConstFolding(tDerivator* der, tNode* node) {
    assert(der != NULL);

    if(node->left == NULL || node->right == NULL) return;

    if (IsConst(node->left) && IsConst(node->right)) {
        double value = NodeCaltor(der, node);
        if (ReplaceNode(node, NUM(value)) != kNoErrors) ERRPRINT(replacing nodes in folding)
    } else if (IsConst(node->left) && node->type == kOperation && !IsBiargument(node->data.code)) {
        double value = NodeCaltor(der, node);
        if (ReplaceNode(node, NUM(value)) != kNoErrors) ERRPRINT(replacing nodes in folding)
    }
}

//================================================================================================================================================================================

static bool EqualConstValue(tNode* node, double value) {
    assert(node != NULL);
    assert(IsConst(node) != false);

    if (IsConst(node) == false) {
        ERRPRINT(wrong input given in compaaring kConst value)
        return false;
    }

    return CompareDouble(node->data.value, value);
}

static void PowFolding(tNode* node) {
    assert(node != NULL);

    assert(node->left != NULL);
    assert(node->right != NULL);        // Ассерты выключатся в релиз режиме, поэтому ставим ешё проверку

    if (node->left == NULL || node->right == NULL) {
        ERRPRINT(null-pointer in pow folding)
        return;
    }

    bool is_left_1  = IsConst(node->left)  && EqualConstValue(node->left, 1);
    bool is_right_1 = IsConst(node->right) && EqualConstValue(node->right, 1);

    bool is_right_0 = IsConst(node->right) && EqualConstValue(node->right, 0);
    bool is_left_0  = IsConst(node->left)  && EqualConstValue(node->left, 0);


    if (is_right_1) {
        ReplaceNode(node, node->left);
    } else if (is_left_1) {
        ReplaceNode(node, NUM(1));
    } else if(is_right_0) {
        ReplaceNode(node, NUM(1));
    } else if (is_left_0) {
        ReplaceNode(node, NUM(0));
    }
    // Как free(node)?
}

//================================================================================================================================================================================

static void MulFolding(tNode* node) {
    assert(node != NULL);

    if(node->left == NULL || node->right == NULL) return;

    bool is_left_1  = IsConst(node->left)  && EqualConstValue(node->left, 1);
    bool is_right_1 = IsConst(node->right) && EqualConstValue(node->right, 1);

    bool is_left_0  = IsConst(node->left)  && EqualConstValue(node->left, 0);
    bool is_right_0 = IsConst(node->right) && EqualConstValue(node->right, 1);


    if (is_left_1 == true) {
        if (ReplaceNode(node, node->right) != kNoErrors) ERRPRINT(replacing nodes in mul_folding)
    } else if (is_right_1 == true) {
        if (ReplaceNode(node, node->left) != kNoErrors) ERRPRINT(replacing nodes in mul_folding)
    } else if (is_left_0 == true || is_right_0 == true) {
        if (ReplaceNode(node, NUM(0)) != kNoErrors) ERRPRINT(replacing nodes in mul_folding)
    }
}

//================================================================================================================================================================================
// TODO - проверка на NULL
static void PlusMinusFolding(tNode* node) {
    assert(node != NULL);

    bool is_left_0 = IsConst(node->left) && EqualConstValue(node->left, 0);
    bool is_right_0 = IsConst(node->right) && EqualConstValue(node->right, 0);

    if (is_left_0 == true) {
        if (ReplaceNode(node, node->right) != kNoErrors) ERRPRINT(replacing nodes in plusminus_folding)
    } else if(is_right_0 == true) {
        if (ReplaceNode(node, node->left) != kNoErrors) ERRPRINT(replacing nodes in plus minus_folding)
    }
}

//================================================================================================================================================================================

static void DivFolding(tNode* node) {
    assert(node != NULL);

    if (node->left  == NULL || node->right == NULL) {
        ERPRINT("Null pointer in div folding");
        return;
    }

    bool is_left_1  = IsConst(node->left) && EqualConstValue(node->left, 1);
    bool is_right_1 = IsConst(node->right) && EqualConstValue(node->right, 1);

    if (is_right_1 == true) {
        if (ReplaceNode(node, node->left) != kNoErrors) ERRPRINT(replacing nodes in div_folding)
    } else if (is_left_1 == true) {
        if (ReplaceNode(node, node->right) != kNoErrors) ERRPRINT(replacing nodes in div_folding)
    }
}

//================================================================================================================================================================================

//
//================================================================================================================================================================================

//================================================================================================================================================================================

static void TreeSize(tNode* node, size_t* curr_size) {
    assert(curr_size);
    assert(node);

    (*curr_size)++;
    if (node->left != NULL)  TreeSize(node->left, curr_size);
    if (node->right != NULL) TreeSize(node->right, curr_size);

}

//================================================================================================================================================================================

static size_t DerivatorSize(tDerivator* der) {
    assert(der);

    if (!der->root || !der->root->left) {       // DEBUG
        ERPRINT("Null pointer of der.root or der root left");
        return 1;
    }

    size_t counter = 1;
    TreeSize(der->root->left, &counter);

    return counter;
}

//================================================================================================================================================================================

