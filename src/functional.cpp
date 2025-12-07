#include "lib.h"

//=================================================================================================================================================

static tNode* FactorialOp(const int number);
static tNode* SeriesSummand(tDerivator* der, double rank);

//=================================================================================================================================================

tDerivator* DerDiffirentiate(tDerivator* der) {
    assert(der->root->left);

    DerOptor(der);
    der->root->left = Differentiator(der->root->left);
    if (der->root->left == NULL) {
        ERPRINT("Error diffirentiating node in");
        return NULL;
    }

    DerOptor(der);
    return der;
}

//=================================================================================================================================================

tDerivator* DerDiffN(tDerivator* der, int times) {
    assert(der);

    if (times <= 0 || times >= kDiffLimit) {
        ERPRINT("Incorrect input\n");
        return NULL;
    }

    for(int curr_dif = 0; curr_dif < times; curr_dif++) {
        der = DerDiffirentiate(der);
    }

    return der;

}

//=================================================================================================================================================
// DRAFT
// tNode* NodeDiff(tNode* node) {
//     assert(node != NULL);
//
//
// }
//
//=================================================================================================================================================

tDerivator* Series(tDerivator* der, FILE* file) {

    // FILE* file = fopen(filename, "w");
    // if (file == NULL) {
    //     ERPRINT("Error opening TeX file\n");
    //     return NULL;
    // }

    if (NodeTex(der->root->left, file) != kNoErrors) {       // TODO On to maсro
        ERPRINT("Error printing node to TeX file\n");
        return NULL;
    }

    for (int k = 1; k < kDiffLimit; k++) {  // naming "k" caused historical
        der = DerDiffirentiate(der);

        if (NodeTex(SeriesSummand(der, k), file) != kNoErrors) {
            ERPRINT("Error printing node to TeX file\n");
            return NULL;
        }
        TexPrint(" \\ + \\ ", file);

    }
    return der;
}

//=================================================================================================================================================

static tNode* FactorialOp(const int number) {
    double value = tgamma(number + 1); // FIXME - factorial?
    return MakeNum(value);
}

//=================================================================================================================================================

static tNode* SeriesSummand(tDerivator* der, double rank) {
    tNode* mul_x = POW(MakeVar(kX), NUM(rank));
    tNode* denominator = FactorialOp((int)rank);
    tNode* nominator =  MUL(der->root->left, mul_x);

    return DIV(nominator, denominator);
}

//=================================================================================================================================================
