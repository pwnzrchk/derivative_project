#ifndef MATH_FUNC_H
#define MATH_FUNC_H

#include "dif_tree.h"

//================================================================================================================================================================================

#define NUM(x) MakeNum(x)

#define L NodeCaltor(der, node->left)
#define R NodeCaltor(der, node->right)

#define cL CopyNode(node->left)
#define cR CopyNode(node->right)
#define dL Differentiator(node->left)
#define dR Differentiator(node->right)

#define ADD(left, right) MakeNode(kPlus, left, right)
#define SUB(left, right) MakeNode(kMinus, left, right)
#define MUL(left, right) MakeNode(kMul, left, right)
#define DIV(left, right) MakeNode(kDiv, left, right)
#define POW(left, right) MakeNode(kPow, left, right)

#define SIN(left)  MakeNode(kSin, left, NULL)
#define COS(left)  MakeNode(kCos, left, NULL)
#define EXP(left)  MakeNode(kExp, left, NULL)
#define TG(left)   MakeNode(kTg,  left, NULL)
#define CTG(left)  MakeNode(kCtg, left, NULL)
#define LN(left)   MakeNode(kLn,  left, NULL)
#define SH(left)   MakeNode(kSh,  left, NULL)
#define CH(left)   MakeNode(kCh,  left, NULL)
#define TGH(left)  MakeNode(kTgh, left, NULL)
#define Ctgh(left) MakeNode(kCtgh, left, NULL)

//================================================================================================================================================================================

tNode* MakeNode(int op_code, tNode* left, tNode* right);
tNode* Differentiator (tNode* node);
tNode* MakeNum(double value);
tNode* MakeVar(int code);

double NodeCaltor(tDerivator* der, tNode* node);

int VariableCode(char var);

//================================================================================================================================================================================


#endif // MATH_FUNC_H

