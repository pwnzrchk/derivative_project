#include "math_func.h"
#include <ctype.h>
//WARNING Фиктовная нода справа всегда - договоренность
//Корневая нода - фиктивная, первая нода дерева - левая нода корня
//================================================================================================================================================================================

static tNode* DiffPlus (tNode* node);
static tNode* DiffMinus(tNode* node);
static tNode* DiffMul  (tNode* node);
static tNode* DiffDiv  (tNode* node);
static tNode* DiffPow  (tNode* node);
static tNode* DiffSin  (tNode* node);
static tNode* DiffCos  (tNode* node);
static tNode* DiffExp  (tNode* node);
static tNode* DiffTg   (tNode* node);
static tNode* DiffCtg  (tNode* node);
static tNode* DiffLn   (tNode* node);
static tNode* DiffSh   (tNode* node);
static tNode* DiffCh   (tNode* node);
static tNode* DiffTgh  (tNode* node);
static tNode* DiffCtgh (tNode* node);
static tNode* DiffArcSin(tNode* node);
static tNode* DiffArcCos(tNode* node);
static tNode* DiffArcTg (tNode* node);
static tNode* DiffArcCtg(tNode* node);
static tNode* DiffArcSh (tNode* node);
static tNode* DiffArcCh (tNode* node);
static tNode* DiffArcTh (tNode* node);
static tNode* DiffArcCth(tNode* node);

typedef tNode* (*tDiffFuncPtr)(tNode* node);
static tDiffFuncPtr dif_func[kOperationsAmount + 1] = {     //+1 caused by indexation
    NULL,
    DiffPlus,
    DiffMinus,
    DiffMul,
    DiffDiv,
    DiffPow,
    DiffSin,
    DiffCos,
    DiffExp,
    DiffTg,
    DiffCtg,
    DiffLn,
    DiffSh,
    DiffCh,
    DiffTgh,
    DiffCtgh,
    DiffArcSin,
    DiffArcCos,
    DiffArcTg,
    DiffArcCtg,
    DiffArcSh,
    DiffArcCh,
    DiffArcTh,
    DiffArcCth
};

//================================================================================================================================================================================

static double PlusOp (tDerivator* der, tNode* node);
static double MinusOp(tDerivator* der, tNode* node);
static double MulOp  (tDerivator* der, tNode* node);
static double DivOp  (tDerivator* der, tNode* node);
static double PowOp  (tDerivator* der, tNode* node);
static double SinOp  (tDerivator* der, tNode* node);
static double CosOp  (tDerivator* der, tNode* node);
static double ExpOp  (tDerivator* der, tNode* node);
static double TgOp   (tDerivator* der, tNode* node);
static double CtgOp  (tDerivator* der, tNode* node);
static double LnOp   (tDerivator* der, tNode* node);
static double ShOp   (tDerivator* der, tNode* node);
static double ChOp   (tDerivator* der, tNode* node);
static double TghOp  (tDerivator* der, tNode* node);
static double CtghOp (tDerivator* der, tNode* node);
static double ArcSinOp(tDerivator* der, tNode* node);
static double ArcCosOp(tDerivator* der, tNode* node);
static double ArcTgOp (tDerivator* der, tNode* node);
static double ArcCtgOp(tDerivator* der, tNode* node);
static double ArcSh   (tDerivator* der, tNode* node);
static double ArcCh   (tDerivator* der, tNode* node);
static double ArcTh   (tDerivator* der, tNode* node);
static double ArcCth  (tDerivator* der, tNode* node);

typedef double (*tMathFuncPtr)(tDerivator* der, tNode* node);
static const tMathFuncPtr math_func[kOperationsAmount + 1] = {
    NULL,
    PlusOp,
    MinusOp,
    MulOp,
    DivOp,
    PowOp,
    SinOp,
    CosOp,
    ExpOp,
    TgOp,
    CtgOp,
    LnOp,
    ShOp,
    ChOp,
    TghOp,
    CtghOp,
    ArcSinOp,
    ArcCosOp,
    ArcTgOp,
    ArcCtgOp,
    ArcSh,
    ArcCh,
    ArcTh,
    ArcCth
};

//================================================================================================================================================================================

double NodeCaltor(tDerivator* der, tNode* node) {    //WARNING вынести массив с переменными из структуры в глобал?
                                                     // Это заебно и не хочется, придется переписывать парсер
    if (node == NULL) return 0;

    if (node->type == kConst) {
        return node->data.value;
    }

    if (node->type == kVariable) {      //TODO - для любой переменной
        return der->constants[node->data.code];
    }

    if (node->type == kOperation) {
        return math_func[node->data.code](der, node);
    }

    ERRPRINT(given wrong node)
    return kPoisonValue;
}

//================================================================================================================================================================================

static double PlusOp (tDerivator* der, tNode* node) {
    return L + R;
}

static double MinusOp (tDerivator* der, tNode* node) {
    return L - R;
}

static double MulOp (tDerivator* der, tNode* node) {
    return L * R;
}

static double DivOp (tDerivator* der, tNode* node) {
    return L / R;
}

static double PowOp (tDerivator* der, tNode* node) {
    return pow(L, R);
}

static double SinOp (tDerivator* der, tNode* node) {
    return sin(L);
}

static double CosOp (tDerivator* der, tNode* node) {
    return cos(L);
}

static double ExpOp (tDerivator* der, tNode* node) {
    return exp(L);
}

static double TgOp (tDerivator* der, tNode* node) {
    return tan(L);
}

static double CtgOp (tDerivator* der, tNode* node) {
    return 1/tan(L);
}

static double LnOp (tDerivator* der, tNode* node) {
    return log(L);
}

static double ShOp (tDerivator* der, tNode* node) {
    return sinh(L);
}

static double ChOp (tDerivator* der, tNode* node) {
    return cosh(L);
}

static double TghOp (tDerivator* der, tNode* node) {
    return tanh(L);
}

static double CtghOp (tDerivator* der, tNode* node) {
    return 1/tanh(L);
}

static double ArcSinOp (tDerivator* der, tNode* node) {
    return asin(L);
}

static double ArcCosOp (tDerivator* der, tNode* node) {
    return acos(L);
}

static double ArcTgOp (tDerivator* der, tNode* node) {
    return atan(L);
}

static double ArcCtgOp (tDerivator* der, tNode* node) {
    return M_PI_2 - atan(L);
}

static double ArcSh (tDerivator* der, tNode* node) {
    return asinh(L);
}

static double ArcCh (tDerivator* der, tNode* node) {
    return acosh(L);
}

static double ArcTh (tDerivator* der, tNode* node) {
    return atanh(L);
}

static double ArcCth (tDerivator* der, tNode* node) {
    return atanh(1/L);
}

//================================================================================================================================================================================

tNode* Differentiator(tNode* node) {
    // int code = VariableCode(var);                        //TODO - дифференциирование по переменной
    if (node->type == kConst) return MakeNum(0);

    if (node->type == kVariable) {
        // if (code == node->data.code) return MakeNum(1);  //TODO - дифференциирование по переменной
        return MakeNum(1);
    }

    if (node->type == kOperation) {
        return dif_func[node->data.code](node);
    }

    ERRPRINT(unknown noda)
    return NULL;
}

//================================================================================================================================================================================

int VariableCode(char var) {
    const char var_storage[] = {    //TODO сделать глобальным массивов для всех функций?
        'x', 'y', 'z'
    };

    char variable = (char)tolower(var);

    for (int current_variable = 0; current_variable < kVariablesAmount; current_variable++) {
        if (variable == var_storage[current_variable]) return current_variable + 1; //+1 сaused by indexation
    }

    ERRPRINT(no matching variable in var_storage)
    return kPoisonValue;
}

//================================================================================================================================================================================

tNode* MakeVar(int var_code) {
    if (var_code < 1 || var_code > kVariablesAmount) {     // 1 caused by enumeration of variables
        ERPRINT("Incorrect input code in makevar\n");
        return NULL;
    }

    tData data;
    data.code = var_code;
    tNode* ret_node = CreateNode(kVariable, data, NULL);
    if (ret_node == NULL) ERPRINT("Error creating node\n");

    return ret_node;
}

//================================================================================================================================================================================

tNode* MakeNum(double value) {
    tData data;
    data.value = value;
    tNode* ret_node = CreateNode(kConst, data, NULL);
    if (ret_node == NULL) ERRPRINT(null pointer to node)

    return ret_node;
}

//================================================================================================================================================================================

tNode* MakeNode(int op_code, tNode* left, tNode* right) {
    tData data;
    data.code = op_code;
    tNode* node = CreateNode(kOperation, data, NULL);
    if (node == NULL) {
        ERRPRINT(null pointer to node)
        return NULL;
    }

    node->left  = left;
    node->right = right;

    if(left != NULL) left->parent = node;
    if(right != NULL) right->parent = node;

    return node;
}

//================================================================================================================================================================================

static tNode* DiffPlus(tNode* node) {
    return ADD(dL, dR);
}

static tNode* DiffMinus(tNode* node) {
    return SUB(dL, dR);
}

static tNode* DiffMul(tNode* node) {
    tNode* mul1 = MUL(dL, cR);
    tNode* mul2 = MUL(cL, dR);
    return ADD(mul1, mul2);
}

static tNode* DiffDiv(tNode* node) {
    tNode* mul1 = MUL(dL, cR);
    tNode* mul2 = MUL(cL, dR);
    tNode* nominator = SUB(mul1, mul2);
    tNode* denominator = POW(cR, NUM(2));
    return DIV(nominator, denominator);
}

static tNode* DiffPow(tNode* node) {
    tNode* pow_mul  = POW(cL, cR);

    tNode* frst_smmnd = MUL(LN(cL), dR);

    tNode* reverse_r = POW(cL, NUM(-1));
    tNode* scnd = MUL(reverse_r, cR);
    tNode* scnd_smmnd = MUL(scnd, dL);

    tNode* sum = ADD(frst_smmnd, scnd_smmnd);

    return MUL(pow_mul, sum);
}

static tNode* DiffSin(tNode* node) {
    return MUL(COS(cL), dL);
}

static tNode* DiffCos(tNode* node) {
    tNode* neg_sin = MUL(SIN(cL), NUM(-1));
    return MUL(neg_sin, dL);
}

static tNode* DiffExp(tNode* node) {
    return MUL(EXP(cL), dL);
}

static tNode* DiffTg(tNode* node) {
    tNode* powered_cos = POW(COS(cL), NUM(-2));
    tNode* diff_arg = dL;
    return MUL(powered_cos, diff_arg);
}

static tNode* DiffCtg(tNode* node) {
    tNode* powered_sin = POW(SIN(cL), MakeNum(-2));
    tNode* neg_pow_sin = MUL(powered_sin, NUM(-1));
    tNode* diff_arg = dL;
    return MUL(neg_pow_sin, diff_arg);
}

static tNode* DiffLn(tNode* node) {
    tNode* denominator = POW(cL, NUM(-1));
    return MUL(denominator, dL);
}

static tNode* DiffSh(tNode* node) {
    return MUL(CH(cL), dL);
}

static tNode* DiffCh(tNode* node) {
    return MUL(SH(cL), dL);
}

static tNode* DiffTgh(tNode* node) {
    tNode* powered_ch = POW(CH(cL), NUM(-2));
    tNode* diff_arg = dL;
    return MUL(powered_ch, diff_arg);
}

static tNode* DiffCtgh(tNode* node) {
    tNode* powered_sh = POW(SH(cL), MakeNum(-2));
    tNode* neg_pow_sh = MUL(powered_sh, NUM(-1));
    tNode* diff_arg = dL;
    return MUL(neg_pow_sh, diff_arg);
}

static tNode* DiffArcSin(tNode* node) {
    tNode* powered_arg =  POW(cL, NUM(2));
    tNode* difference  =  SUB(NUM(1), powered_arg);
    tNode* diff = POW(difference, NUM(-0.5));
    return MUL(diff, dL);
}

static tNode* DiffArcCos(tNode* node) {
    tNode* powered_arg =  POW(cL, NUM(2));
    tNode* difference  = SUB(NUM(1), powered_arg);
    tNode* diff = POW(difference, NUM(-0.5));
    tNode* neg_diff = MUL(diff, NUM(-1));
    return MUL(neg_diff, dL);
}

static tNode* DiffArcTg(tNode* node) {
    tNode* powered_arg =  POW(cL, NUM(2));
    tNode* summ = ADD(NUM(1), powered_arg);
    tNode* pow  = POW(summ, NUM(-1));
    return MUL(pow, dL);
}

static tNode* DiffArcCtg(tNode* node) {
    tNode* powered_arg =  POW(cL, NUM(2));
    tNode* summ  = ADD(NUM(1), powered_arg);
    tNode* pow  = POW(summ, NUM(-1));
    tNode* neg_diff = MUL(pow, NUM(-1));
    return MUL(neg_diff, dL);
}

static tNode* DiffArcSh(tNode* node) {
    tNode* powered_arg =  POW(cL, NUM(2));
    tNode* sum  = ADD(powered_arg, NUM(1));
    tNode* diff = POW(sum, NUM(-0.5));
    return MUL(diff, dL);
}

static tNode* DiffArcCh(tNode* node) {
    tNode* powered_arg =  POW(cL, NUM(2));
    tNode* sub  = SUB(powered_arg, NUM(1));
    tNode* diff = POW(sub, NUM(-0.5));
    return MUL(diff, dL);
}

static tNode* DiffArcTh(tNode* node) {
    tNode* powered_arg =  POW(cL, NUM(2));
    tNode* sub  = SUB(NUM(1), powered_arg);
    tNode* diff = POW(sub, NUM(-1));
    return MUL(diff, dL);
}

static tNode* DiffArcCth(tNode* node) {
    tNode* powered_arg =  POW(cL, NUM(2));
    tNode* sub  = SUB(NUM(1), powered_arg);
    tNode* diff = POW(sub, NUM(-1));
    return MUL(diff, dL);
}





//================================================================================================================================================================================
