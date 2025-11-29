#include <string.h>
#include "dif_tree.h"
#include "math_func.h"

//=================================================================================================================================================

tNode* CreateNode(tNodeType type, tData data, tNode* parent) {

    tNode* new_node = (tNode*)calloc(1, sizeof(tNode));
    if (!new_node) {
        free(new_node);
        new_node = NULL;
        fprintf(stderr, "Allocation error in creating node\n");
        return NULL;
    }

    new_node->type = type;
    new_node->data = data;

    new_node->left   = NULL;
    new_node->right  = NULL;
    new_node->parent = parent;

    return new_node;
}

//=================================================================================================================================================


//=================================================================================================================================================

static void NodeDtor (tNode* ref_node) {
    if(ref_node == NULL) return;

    NodeDtor(ref_node->left);
    NodeDtor(ref_node->right);

    ref_node->data.value = NAN;
    ref_node->type = (tNodeType)kPoisonValue;

    ref_node->left   = NULL;
    ref_node->right  = NULL;
    ref_node->parent = NULL;

    free(ref_node);
    ref_node = NULL;
}

//=================================================================================================================================================

tTreeError DerCtor(tDerivator* der) {       //WARNING - как быть с конструктором:
    assert(der != NULL);                    //Чем заполнять нулевую/фиктивную ноду?

    tData init_data;
    init_data.code = kMul;
    der->root = CreateNode(kOperation, init_data, NULL);    //Как использовать фиктивную ноду?

    der->constants = (double*)calloc(kConstsCount + 1, sizeof(double)); //+1 caused by indexation
    der->size = kInitialDerivatorSize;

    return kNoErrors;
}

//=================================================================================================================================================

tTreeError DerDtor (tDerivator* der) {
    assert(der != NULL);

    NodeDtor(der->root);
    der->size = kPoisonValue;
    free(der->constants);

    return kNoErrors;
}

//=================================================================================================================================================

tNode* CopyNode(tNode* source) {
    if (source == NULL) return NULL;

    tNode* copy_node = CreateNode(source->type, source->data, NULL);

    if(copy_node == NULL) {
        ERRPRINT(allocation)
        return NULL;
    }

    copy_node->left = CopyNode(source->left);
    copy_node->right = CopyNode(source->right);

    if(copy_node->left != NULL) copy_node->left->parent = copy_node;
    if(copy_node->right != NULL) copy_node->right->parent = copy_node;

    return copy_node;
}

//=================================================================================================================================================

char* OperationDecoder(int code) {      //TODO - Доделать
    if (code < 0 || code > kOperationsAmount) {
        fprintf(stderr, "Error: incorrect code was given to OperationDecoder\n");
        return NULL;
    }

    char* op_storage[] = {
        "+", "-", "*", "/", "^", "sin",
        "cos", "exp", "tg", "ctg", "ln",
        "sh", "ch", "tgh", "ctgh"
    };
    return op_storage[code - 1]; //-1 caused by indexation
}

//=================================================================================================================================================

char VariableDecoder(int code) {
    if (code < 0 || code > kVariablesAmount) {
        fprintf(stderr, "Error: incorrect code was given to VariableDecoder\n");
        return '\0';                                                    //WARNING - хуйня или нет?
    }

    const char var_storage[] = {
        'x', 'y', 'z'
    };
    return var_storage[code - 1];
}

//=================================================================================================================================================

bool IsBiargument(int code) {
    if (code >= kPlus && code <= kPow) return true;
    else return false;
}

//=================================================================================================================================================
