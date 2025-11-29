#ifndef DIF_TREE_H
#define DIF_TREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

//=================================================================================================================================================

#define ERRPRINT(x) fprintf(stderr, "Error: " #x " in %s at %s:%d\n", \
                            __func__, __FILE__, __LINE__);

//=================================================================================================================================================

enum tTreeError {
    kNoErrors     = 0,
    kNullPointer  = 1,
    kWrongAddress = 2,
    kWrongNode    = 3,
    kUserError    = 4,
    kFreadError   = 5,
    kUnkCmd       = 6,
    kScanfError   = 7,
    kUnkVar       = 8,
    kOneArgWrongCode = 9,
    kTwoArgWrongCode = 10
};

enum tConst {
    kX = 1,
    kY = 2,
    kZ = 3
};

enum tOpCode {
    kPlus  = 1,
    kMinus = 2,
    kMul   = 3,
    kDiv   = 4,
    kPow   = 5,
    kSin   = 6,
    kCos   = 7,
    kExp   = 8,
    kTg    = 9,
    kCtg  = 10,
    kLn   = 11,
    kSh   = 12,
    kCh   = 13,
    kTgh  = 14,
    kCtgh = 15  //TODO Добавить арк функции
};

enum tNodeType {
    kOperation = 1,
    kVariable  = 2,
    kConst     = 3
};

//=================================================================================================================================================

typedef union {
    double value;
    int code;
} tData;

struct Node {
    tNodeType type;
    tData     data;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
};
typedef struct Node tNode;

typedef struct {
    tNode* root;
    int size;
    double* constants;
} tDerivator;

//=================================================================================================================================================

static const int kInitialDerivatorSize  = 1;
static const int kPoisonValue = (int)0xDEADBEEF;
static const int kConstsCount = 10;
static const int kMaxNumberSize = 100;
static const int kVariablesAmount = 3;
static const int kOperationsAmount = 15;
static const int ONE = 1;

static const char kNodeBegin = '(';
static const char kNodeEnd = ')';
static const char kNilBegin = 'n';


//=================================================================================================================================================

tTreeError DerCtor(tDerivator* der);
tTreeError DerDtor(tDerivator* der);

tNode* CopyNode  (tNode* source);
tNode* CreateNode(tNodeType type, tData data, tNode* parent);

char  VariableDecoder (int code);   //WARNING - const char?
char* OperationDecoder(int code);

bool IsBiargument(int code);

//=================================================================================================================================================

#endif // DIF_TREE_H
