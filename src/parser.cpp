#include "parser.h"
#include "math_func.h"

//=================================================================================================================================================

static tNode* GetNumber    (char* buffer, size_t* index);
static tNode* GetPriority  (char* buffer, size_t* index);
static tNode* GetFunction  (char* buffer, size_t* index);
static tNode* GetVariable  (char* buffer, size_t* index);
static tNode* GetPow       (char* buffer, size_t* index);
static tNode* GetTerm      (char* buffer, size_t* index);
static tNode* GetExpression(char* buffer, size_t* index);

static int OperationCode(char* buffer, size_t* index);
static int VariableCode (char* buffer, size_t* index);

//=================================================================================================================================================

static tNode* GetNumber (char* buffer, size_t* index) { // TODO maybe strtod
    if(isdigit(buffer[*index]) == true) {
        double const_data = 0;
        int data_len = 0;
        tData ret_data;

        if (sscanf(buffer + *index, "%lg%n", &const_data, &data_len) == 1) {
            ret_data.value = const_data;

            tNode* ret_node = CreateNode(kConst, ret_data, NULL);
            if(ret_node == NULL) ERRPRINT(allocation error in GetNumber)

            (*index) += (size_t)data_len;
            return ret_node;
        }
    }
    return NULL;
}

//=================================================================================================================================================

static tNode* GetFunction(char* buffer, size_t* index) {
    if(isalpha(buffer[*index]) == true) {
        int func_code = OperationCode(buffer, index);
        if (func_code == 0) return NULL;

        if (buffer[*index] != '(') return NULL;
        tNode* exp_node = GetExpression(buffer, index);
        if (exp_node == NULL) return NULL;

        return MakeNode(func_code, exp_node, NULL);
    }
    return NULL;
}

static int OperationCode(char* buffer, size_t* index) {
    const char* op_storage[] = {
    "sin",    "cos",    "exp",
    "tg",     "ctg",    "ln",     "sh",
    "ch",     "tgh",    "ctgh",   "arcsin",
    "arccos", "arctg",  "arcctg", "arcsh",
    "arcch",  "arcth",  "arccth"
    };

    for (int curr_op = 0; curr_op < kOperationsAmount - kSin; curr_op++) {  // -kSin caused by enumeration of operations
        size_t len = strlen(op_storage[curr_op]);
        char* low_str = LoweringString(&buffer[*index], len);
        if (strncmp(op_storage[curr_op], low_str, len) == 0) {
            (*index) += len;
            return curr_op + kSin;                                          // +kSin caused by enumeration of operations
        }
    }
    return 0;
}

//=================================================================================================================================================

static tNode* GetVariable(char* buffer, size_t* index) {
    if (isalpha(buffer[*index]) == true) {
        int var = VariableCode(buffer, index);
        if (var == 0) return NULL;

        tData ret_data;
        ret_data.code = var;
        tNode* ret_node = CreateNode(kVariable, ret_data, NULL);
        if (ret_node == NULL) ERRPRINT(allocation error in GetVariable)

        return ret_node;
    }

    return NULL;
}


static int VariableCode(char* buffer, size_t* index) {
    const char* var_storage[] = {
    "x", "y", "z" };

    for (int curr_var = 0; curr_var < kVariablesAmount ; curr_var++) {  // -kDiv caused by enumeration of operations
        size_t len = strlen(var_storage[curr_var]);
        char* low_str = LoweringString(&buffer[*index], len);
        if(strncmp(var_storage[curr_var], low_str, len) == 0) {
            (*index) += len;
            return curr_var + 1;  // +1 caused by enumeration of operations
        }
    }
    return 0;
}

//=================================================================================================================================================

static tNode* GetPriority (char* buffer, size_t* index) {

    if(buffer[*index] == '(') {
        tNode* ret_node = NULL;
        (*index)++;
        ret_node = GetExpression(buffer, index);
        if (buffer[*index] != ')') {
            ERPRINT("Missing end quote");
            NodeDtor(ret_node);
            return NULL;
        }
        (*index)++;
        return ret_node;
    }

    if(isdigit(buffer[*index]) == true) {
        return GetNumber(buffer, index);
    }

    if(isalpha(buffer[*index])) {
        size_t fict_ind = *index;
        while(isalpha(buffer[fict_ind]) == true) fict_ind++;
        if (buffer[fict_ind] == '(') return GetFunction(buffer, index);
        return GetVariable(buffer, index);
    }

    return NULL;
}

//=================================================================================================================================================

static tNode* GetTerm(char* buffer, size_t* index) {
    tNode* left_node = GetPow(buffer, index);
    if(left_node == NULL) return NULL;

    while(buffer[*index] == '*' || buffer[*index] == '/') {
        int op_code = (buffer[*index] == '*' ? kMul : kDiv);
        (*index)++;
        tNode* right_node = GetPow(buffer, index);
        if(right_node == NULL) {
            ERPRINT("Missing operand after '*' '/'");
            NodeDtor(left_node);
            return NULL;
        }

        left_node = MakeNode(op_code, left_node, right_node);
    }
    return left_node;

}

//=================================================================================================================================================

static tNode* GetPow(char* buffer, size_t* index) {
    tNode* left_node = GetPriority(buffer, index);
    if (left_node == NULL) return NULL;

    if(buffer[*index] == '^') {
        (*index)++;
        tNode* right_node = GetPow(buffer, index);
        if (right_node == NULL) {
            ERPRINT("Missing operand for powering");
            NodeDtor(left_node);
            return NULL;
        }
        left_node = MakeNode(kPow, left_node, right_node);
    }
    return left_node;

}

//=================================================================================================================================================

static tNode* GetExpression(char* buffer, size_t* index) {
    tNode* left_node = GetTerm(buffer, index);
    if(left_node == NULL) return NULL;

    while(buffer[*index] == '+' || buffer[*index] == '-') {
        int op_code = (buffer[*index] == '+' ? kPlus : kMinus);
        (*index)++;
        tNode* right_node = GetTerm(buffer, index);
        if (right_node == NULL) {
            ERPRINT("Missing operand after '+' '-' in GetExpression");
            NodeDtor(left_node);
            return NULL;
        }

        left_node = MakeNode(op_code, left_node, right_node);
    }
    return left_node;
}

//=================================================================================================================================================

tNode* GetG(char* buffer, size_t* index) {
    tNode* ret_node = GetExpression(buffer, index);
    if(ret_node == NULL) {
        ERPRINT("Syntax error parsing GetG");
        return NULL;
    }

    if(buffer[*index] != '\0') {
        ERPRINT("Syntax error end-symb in GetG");    // TODO - переделать макрос под строчку
        return NULL;
    }

    return ret_node;
}

//=================================================================================================================================================
