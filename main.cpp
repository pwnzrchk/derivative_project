#include "dif_tree.h"
#include "dif_tree_parser.h"
#include "math_func.h"

// Тест ручного построения дерева
static int TestManualTree(void);
// Тест парсера из файла
static int TestParser(void);
// Вспомогательная функция создания тестового файла
static void CreateTestFile(const char* filename, const char* content);

//=================================================================================================================================================

int main(void) {
    printf("====== DERIVATOR TESTS ======\n\n");

    int errors = 0;

    errors += TestManualTree();
    errors += TestParser();

    printf("\n====== RESULTS ======\n");
    if (errors == 0) {
        printf("All tests PASSED!\n");
    } else {
        printf("FAILED: %d test(s)\n", errors);
    }

    return errors == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

//=================================================================================================================================================

static int TestManualTree(void) {
    printf("--- Test 1: Manual Tree Building ---\n");

    tDerivator der;
    tTreeError err = DerCtor(&der);
    if (err != kNoErrors) {
        fprintf(stderr, "FAIL: DerCtor returned %d\n", err);
        return 1;
    }
    printf("[OK] DerCtor\n");

    der.root->type = kOperation;
    der.root->data.code = kMul;

    tData plus_data;
    plus_data.code = kPlus;
    der.root->left = CreateNode(kOperation, plus_data, der.root);
    if (der.root->left == NULL) {
        fprintf(stderr, "FAIL: CreateNode for '+'\n");
        DerDtor(&der);
        return 1;
    }

    tData y_data;
    y_data.code = kY;
    der.root->right = CreateNode(kVariable, y_data, der.root);
    if (der.root->right == NULL) {
        fprintf(stderr, "FAIL: CreateNode for 'y'\n");
        DerDtor(&der);
        return 1;
    }

    tData x_data;
    x_data.code = kX;
    der.root->left->left = CreateNode(kVariable, x_data, der.root->left);

    tData five_data;
    five_data.value = 5.0;
    der.root->left->right = CreateNode(kConst, five_data, der.root->left);

    printf("[OK] Tree built: (x + 5) * y\n");

    printf("\n--- Tree structure ---\n");
    err = PrintTree(&der, stdout);
    printf("\n----------------------\n");
    if (err != kNoErrors) {
        fprintf(stderr, "FAIL: PrintTree returned %d\n", err);
        DerDtor(&der);
        return 1;
    }
    printf("[OK] PrintTree\n");

    der.constants[kX] = 2.0;
    der.constants[kY] = 3.0;
    der.constants[kZ] = 0.0;

    double result = NodeCaltor(&der, der.root);
    double expected = 21.0;

    printf("Calculation: x=2, y=3\n");
    printf("Expected: %.2f, Got: %.2f\n", expected, result);

    if (fabs(result - expected) > 0.0001) {
        fprintf(stderr, "FAIL: Wrong calculation result\n");
        DerDtor(&der);
        return 1;
    }
    printf("[OK] NodeCaltor\n");


    tDerivator der2;
    DerCtor(&der2);

    der2.root->type = kOperation;
    der2.root->data.code = kPlus;

    tData sin_data;
    sin_data.code = kSin;
    der2.root->left = CreateNode(kOperation, sin_data, der2.root);

    tData cos_data;
    cos_data.code = kCos;
    der2.root->right = CreateNode(kOperation, cos_data, der2.root);

    // sin(x) - аргумент в левом поддереве
    tData x_data2;
    x_data2.code = kX;
    der2.root->left->left = CreateNode(kVariable, x_data2, der2.root->left);

    // cos(y) - аргумент в левом поддереве
    tData y_data2;
    y_data2.code = kY;
    der2.root->right->left = CreateNode(kVariable, y_data2, der2.root->right);

    der2.constants[kX] = 0.0;  // sin(0) = 0
    der2.constants[kY] = 0.0;  // cos(0) = 1

    result = NodeCaltor(&der2, der2.root);
    expected = 1.0;  // sin(0) + cos(0) = 0 + 1 = 1

    printf("\nTrig test: sin(0) + cos(0)\n");
    printf("Expected: %.2f, Got: %.2f\n", expected, result);

    if (fabs(result - expected) > 0.0001) {
        fprintf(stderr, "FAIL: Trig calculation wrong\n");
        DerDtor(&der);
        DerDtor(&der2);
        return 1;
    }
    printf("[OK] Trig functions\n");

    // Cleanup
    DerDtor(&der);
    DerDtor(&der2);
    printf("[OK] DerDtor\n");

    printf("--- Test 1 PASSED ---\n\n");
    return 0;
}

//=================================================================================================================================================

static int TestParser(void) {
    printf("--- Test 2: Parser ---\n");

    // Создаём тестовый файл с выражением: x * (y + 6)
    // Формат: (* (x nil nil) (+ (y nil nil) (6 nil nil)))
    const char* test_content = "(* (x nil nil) (+ (y nil nil) (6 nil nil)))";
    const char* test_filename = "test_expr.txt";

    CreateTestFile(test_filename, test_content);
    printf("[OK] Created test file: %s\n", test_filename);
    printf("     Content: %s\n", test_content);

    // Парсим
    tDerivator der;
    DerCtor(&der);

    tTreeError err = ReadDer(&der, test_filename);
    if (err != kNoErrors) {
        fprintf(stderr, "FAIL: ReadDer returned %d\n", err);
        DerDtor(&der);
        return 1;
    }
    printf("[OK] ReadDer\n");

    // Печатаем распарсенное дерево
    printf("\n--- Parsed tree ---\n");
    PrintTree(&der, stdout);
    printf("\n-------------------\n");

    // Вычисляем: x=2, y=4 → 2 * (4 + 6) = 20
    der.constants[kX] = 2.0;
    der.constants[kY] = 4.0;

    double result = NodeCaltor(&der, der.root);
    double expected = 20.0;

    printf("Calculation: x=2, y=4\n");
    printf("Expression: x * (y + 6) = 2 * (4 + 6) = 20\n");
    printf("Expected: %.2f, Got: %.2f\n", expected, result);

    if (fabs(result - expected) > 0.0001) {
        fprintf(stderr, "FAIL: Parser calculation wrong\n");
        DerDtor(&der);
        return 1;
    }
    printf("[OK] Parsed expression calculates correctly\n");

    DerDtor(&der);

    // Удаляем тестовый файл
    remove(test_filename);

    printf("--- Test 2 PASSED ---\n\n");
    return 0;
}

//=================================================================================================================================================

static void CreateTestFile(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Cannot create test file: %s\n", filename);
        return;
    }
    fprintf(file, "%s", content);
    fclose(file);
}

//=================================================================================================================================================
