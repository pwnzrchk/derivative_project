#include "dif_tree.h"
#include "dif_tree_parser.h"
#include "math_func.h"
#include "graphviz_log.h"
#include "tex_dump.h"
#include "dif_optimizator.h"

int main(void) {
    printf("====== DERIVATOR TESTS ======\n\n");

    tDerivator main_der;
    tDerivator* main_derivator = &main_der;
    DerCtor(main_derivator);

    tData data1;
    data1.code = kCtg;
    main_derivator->root->left = CreateNode(kOperation, data1, main_derivator->root);

    tData data2;
    data2.value = 6.5;
    main_derivator->root->right = CreateNode(kConst, data2, main_derivator->root);

    tData data3;
    data3.code = kX;
    main_derivator->root->left->left = CreateNode(kVariable, data3, main_derivator->root->left);

    tData data4;
    data4.code = kY;
    main_derivator->root->left->right = CreateNode(kVariable, data4, main_derivator->root->left);

    double tree_value = NodeCaltor(main_derivator, main_derivator->root);
    printf("You're tree equal to a %lf\n", tree_value);

    main_derivator->root = Differentiator(main_derivator->root);
    // main_derivator->root = Differentiator(main_derivator->root);

    TreeOptor(main_derivator);
    // main_derivator->root = Differentiator(main_derivator->root);

    TexDump(main_derivator, "files/9mice.tex");
    GraphDump(main_derivator, LOG_FILE, PNG_FILE, HTML_FILE);


    return  EXIT_SUCCESS;
}

