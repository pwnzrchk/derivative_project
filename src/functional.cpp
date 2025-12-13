#include "lib.h"

//================================================================================================================================================================================

// static tNode* FactorialOp(const int number);

static tNode* SeriesSummand(tNode* node, double rank);
static double CalculateFactorial(int depth);

// static tNode* CreateTermNode(double coefficient, int power);

//================================================================================================================================================================================

tDerivator* DerDiffirentiate(tDerivator* der) {
    assert(der);
    assert(der->root->left);

    DerOptor(der);
    tNode* old_node = der->root->left;
    if (old_node == NULL) return NULL;

    tNode* new_node = Differentiator(old_node);
    if (!new_node) {
        ERPRINT("Error in differentiating node");
        return NULL;
    }
    der->root->left = new_node;
    new_node->parent = der->root;

    NodeDtor(old_node);
    DerOptor(der);

    return der;
}

//================================================================================================================================================================================

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

//================================================================================================================================================================================

static double CalculateFactorial(int depth) {
    if (depth <= 0) return 1;
    double res = 1.0;
    for (int i = 1; i <= depth; i++) res *= i;
    return res;
}

//================================================================================================================================================================================

tNode* Series(tNode* node, FILE* file) {
    assert(node);
    assert(file);
    if (!node || !file) return NULL;

    if (BeginTex(file) != kNoErrors) ERPRINT("Error begin series file");
    tNode* work_node = CopyNode(node);
    tDerivator* tmp

    for (int rank = 0; rank <= kDiffLimit; rank++) {
        tNode* summ_node = SeriesSummand(work_node, rank);

        NodeTex(summ_node, file);
        if (rank != kDiffLimit - 1) fprintf(file, " + ");
        NodeDtor(summ_node);
        work_node = Differentiator(work_node);
    }
    if (EndTex(file) != kNoErrors) ERPRINT("Error begin series file");

    return node;
}

//================================================================================================================================================================================

//================================================================================================================================================================================

static tNode* SeriesSummand(tNode* node, double rank) {
    if (!node) { // DEBUG
        ERPRINT("Null pointer of node");
        return NULL;
    }

    tNode* mul_x = POW(MakeVar(kX), NUM(rank));
    tNode* denominator = NUM(CalculateFactorial((int)rank));
    tNode* node_cpy = CopyNode(node);
    tNode* nominator =  MUL(node_cpy, mul_x);

    return DIV(nominator, denominator);
}

//================================================================================================================================================================================
