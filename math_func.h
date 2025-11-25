#ifndef MATH_FUNC_H
#define MATH_FUNC_H

#include "dif_tree.h"

//=================================================================================================================================================

double NodeCaltor(tDerivator* der, tNode* node);
double PlusOp  (double first_value, double second_value);
double MinusOp (double first_value, double second_value);
double MulOp (double first_value, double second_value);
double DivOp (double first_value, double second_value);
double PowOp (double first_value, double second_value);
double SinOp (double first_value, double second_value);
double CosOp (double first_value, double second_value);
double ExpOp (double first_value, double second_value);
double TgOp  (double first_value, double second_value);
double CtgOp (double first_value, double second_value);

//=================================================================================================================================================

#endif // MATH_FUNC_H

