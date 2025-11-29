#include "parser.h"
#include "math_func.h"

//=================================================================================================================================================

tNode* GetN (char* buffer, size_t* index) {
    int accuracy = 0;
    double remain = 0;
    double val = 0;
    bool divisor_flag = false;
    while ((*buffer[*index] <= '9' && *buffer[*index] >= '0') || *buffer[*index] == '.') {

        char symb = *buffer[*index];
        (*index)++;
        if (symb == '.') divisor_flag = true;
        if (divisor_flag == false) {
            val = val*10 + symb - '0';
        }

        if (divisor_flag == true) {
            remain = remain + (symb - '0')/pow(10, ++accuracy);
        }

    }
    ret_value = val + remain;
    return NUM(ret_value);

}


tNode* GetF(char* buffer, size_t* index) {
    
}
