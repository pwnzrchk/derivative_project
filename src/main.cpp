#include "dif_tree.h"
#include "math_func.h"
#include "dif_optimizator.h"
#include "graphviz_log.h"
#include "tex_dump.h"
#include "parser.h"

void RunParserTests() {
    printf("====== PARSER TESTS ======\n\n");

    const char* test_expressions[] = {
        "10+2*3",
        "(10+2)*3",
        "sin(x)/(2+cos(y))",
        "3*4*5",
        "12/3/2",
        "10-5+3",
        "2^sin(3*x)",
        "5+xyz",
        "5+",
        "5*(3+1",
        "2^3^2",
        NULL
    };


    for (int i = 0; test_expressions[i] != NULL; ++i) {
        printf("--- Parsing expression: \"%s\" ---\n", test_expressions[i]);

        char* buffer = strdup(test_expressions[i]);
        size_t index = 0;

        tNode* root = GetG(buffer, &index);

        if (root) {
            printf("Parsing successful!\n");

            tDerivator temp_der;
            temp_der.root = CreateNode(kConst, {}, NULL);
            temp_der.root->left = root;
            root->parent = temp_der.root;

            char dot_buf[128], png_buf[128], tex_buf[128];
            sprintf(dot_buf, "files/parser_test_%d.dot", i);
            sprintf(png_buf, "files/parser_test_%d.png", i);
            sprintf(tex_buf, "files/parser_test_%d.tex", i);

            GraphDump(&temp_der, dot_buf, png_buf);
            TexDump(&temp_der, tex_buf);
            printf("Dump created: %s\n", png_buf);

            NodeDtor(temp_der.root);
        } else {
            printf("Parsing FAILED (as expected for some tests).\n");
        }

        free(buffer);
        printf("\n");
    }
}

int main() {
    RunParserTests();
    return EXIT_SUCCESS;
}
