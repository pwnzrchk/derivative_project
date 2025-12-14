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
    tDerivator* copy_der = CopyDer(der);
    copy_der->constants[kX] = 0.0;
    tDerivator* fict_der = CreateFictDer();

    for (int rank = 0; rank <= kDiffLimit; rank++) {
        tNode* calculated_node = NUM(NodeCaltor(copy_der, copy_der->root->left));
        fict_der->root->left = SeriesSummand(calculated_node, rank);

        // GraphConsole(fict_der);  // WARNING

        DerOptor(fict_der);
        TexPrint("\n\n\\noindent", file);
        if (fict_der->root->left != NULL) NodeTex(fict_der->root->left, file);
        if (rank != kDiffLimit - 1) fprintf(file, " + ");
        NodeDtor(fict_der->root->left);
        fict_der->root->left = NULL;
        DerDiffirentiate(copy_der);
    }
    if (EndTex(file) != kNoErrors) ERPRINT("Error begin series file");

    return copy_der;
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

    return fictive_derivator_ptr;
}
