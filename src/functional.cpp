#include "lib.h"

//================================================================================================================================================================================

static tNode* SeriesSummand(tNode* node, double rank);
static double CalculateFactorial(int depth);
tDerivator* CreateFictDer(void);

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

tDerivator* Series(tDerivator* der, FILE* file) {
    assert(der);
    assert(file);
    if (!der || !file) return NULL;

    if (BeginTex(file) != kNoErrors) ERPRINT("Error begin series file");
    tNode* work_node = CopyNode(der->root->left);
    tDerivator* fict_der = CreateFictDer();

for (int rank = 0; rank <= kDiffLimit; rank++) {
        tNode* summ_node = SeriesSummand(work_node, rank);
        fict_der->root->left = summ_node;
        summ_node->parent = fict_der->root;
        DerOptor(fict_der);
        summ_node = fict_der->root->left;
        NodeTex(summ_node, file);
        TexPrint("\n\n\\noindent", file);
        if (rank != kDiffLimit - 1) fprintf(file, " + ");

        NodeDtor(summ_node);
        fict_der->root->left = NULL;

        tNode* old_work_node = work_node;
        work_node = Differentiator(old_work_node);
        NodeDtor(old_work_node);
    }
    NodeDtor(work_node);
    DerDtor(fict_der);
    if (EndTex(file) != kNoErrors) ERPRINT("Error begin series file");

    return der;
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

tDerivator* CreateFictDer(void) {
    tDerivator* fictive_derivator_ptr = (tDerivator*)calloc(1, sizeof(tDerivator));
    if (fictive_derivator_ptr == NULL) {
        ERPRINT("Allocation error for fictive_derivator");
        return NULL;
    }
    DerCtor(fictive_derivator_ptr);
    fictive_derivator_ptr->constants[kX] = 0;

    return fictive_derivator_ptr;
}
