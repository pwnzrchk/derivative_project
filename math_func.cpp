#include "math_func.h"
//Фиктовная нода справа всегда - договоренность, позже исправить //TODO
//=================================================================================================================================================

double NodeCaltor(tDerivator* der, tNode* node) {
    assert(der != NULL);
    if (node == NULL) return 0;

    if (node->type == kConst) {
        return node->data.value;
    }

    if (node->type == kVariable) {
        return der->constants[node->data.code];
    }

    if (node->type == kOperation) {
        return der->funcs[node->data.code](NodeCaltor(der, node->left), NodeCaltor(der, node->right));  //WARNING - индексация
    }

    ERRPRINT(given wrong node)
    return kPoisonValue;
}

//=================================================================================================================================================

double PlusOp (double first_value, double second_value) {   //WARNING - Делать ли сравнение даблов и если говно-нода высирать говно?
    return first_value + second_value;
}

double MinusOp (double first_value, double second_value) {
    return first_value - second_value;
}

double MulOp (double first_value, double second_value) {
    return first_value * second_value;
}

double DivOp (double first_value, double second_value) {
    return first_value / second_value;
}

double PowOp (double first_value, double second_value) {
    return pow(first_value, second_value);
}

double SinOp (double first_value, double second_value) {    //WARNING - как убрать неопределенность?
    return sin(first_value);
}

double CosOp (double first_value, double second_value) {
    return cos(first_value);
}

double ExpOp (double first_value, double second_value) {
    return exp(first_value);
}

double TgOp (double first_value, double second_value) {
    return tan(first_value);
}

double CtgOp (double first_value, double second_value) {
    return 1/tan(first_value);
}

//=================================================================================================================================================



//=================================================================================================================================================
