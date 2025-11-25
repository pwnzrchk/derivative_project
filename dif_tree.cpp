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

tTreeError DerCtor(tDerivator* der) {       //WARNING - как быть с конструктором?
    assert(der != NULL);                    //Чем заполнять нулевую ноду?

    tData init_data;
    init_data.code = kPlus;
    der->root = CreateNode(kOperation, init_data, NULL);    //Как использовать фиктивную ноду?

    der->constants = (double*)calloc(kConstsCount + 1, sizeof(double)); //+1 caused by indexation
    der->size = kInitialDerivatorSize;

    der->funcs = (math_func*)calloc(kOperationsAmount + 1, sizeof(math_func));  //+1 Caused by indexation

    der->funcs[kPlus]  = PlusOp;
    der->funcs[kMinus] = MinusOp;
    der->funcs[kMul]   = MulOp;
    der->funcs[kDiv]   = DivOp;
    der->funcs[kPow]   = PowOp;
    der->funcs[kSin]   = SinOp;
    der->funcs[kCos]   = CosOp;
    der->funcs[kExp]   = ExpOp;
    der->funcs[kTg]    = TgOp;
    der->funcs[kCtg]   = CtgOp;

    return kNoErrors;
}

//=================================================================================================================================================

tTreeError DerDtor (tDerivator* der) {
    assert(der != NULL);

    NodeDtor(der->root);
    der->size = kPoisonValue;
    free(der->funcs);
    free(der->constants);

    return kNoErrors;
}

//=================================================================================================================================================



//=================================================================================================================================================
