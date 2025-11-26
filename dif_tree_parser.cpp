#include <sys/stat.h>
#include "dif_tree_parser.h"

//=================================================================================================================================================

static void SettingForOperation(tNode* node, int* index, char* buffer);
static void SettingForVariable(tNode* node, int* index, char* buffer);
static void GetNodeSettings(tNode* node, int* index, char* buffer);
static void SettingForConst(tNode* node, int* index, char* buffer);

static int OperationCode(int* index, char* buffer);
static int VariableCode(int* index, char* buffer);

static const char* OperationDecoder(int code);
static char VariableDecoder(int code);

static bool IsNull(int* index, const char* buffer);
static long long FileSize(FILE* file);

//=================================================================================================================================================

tTreeError ReadDer(tDerivator* der, const char* const filename) {
    assert(der->root != NULL);
    assert(der != NULL);

    int initial_index = 0;
    tTreeError error_for_everything = kNoErrors;

    char* main_buffer;
    if ((error_for_everything = CreateBuffer(&main_buffer, filename)) != kNoErrors) return error_for_everything;

    tNode* buffer_node = ReadNode(NULL, &initial_index, main_buffer);
    if (buffer_node != NULL) {
        der->root = buffer_node;
        return kNoErrors;
    }
    return kNullPointer;

}

//=================================================================================================================================================

tNode* ReadNode(tNode* parent_node, int* index, char* buffer) {
    // assert(parent_node != NULL);
    assert(buffer != NULL);
    assert(index != NULL);

    if(buffer[*index] == '\0') return NULL;

    SkipSpaces(index, buffer);

    if(buffer[*index] == kNodeBegin) {
        (*index)++;

        tData basic_data = {};
        basic_data.code = kBasicSetting;

        tNode* ret_node = CreateNode((tNodeType)kBasicSetting, basic_data, parent_node);    //Caller-allocates
        if (ret_node == NULL) {
            fprintf(stderr, "Error: allocation in ReadNode, index - [%d]\n", *index);
            return NULL;
        }

        GetNodeSettings(ret_node, index, buffer);

        ret_node->left = ReadNode(ret_node, index, buffer);
        ret_node->right = ReadNode(ret_node, index, buffer);

        if (buffer[*index] != kNodeEnd) {
            ERRPRINT(kNodeEnd expected)
        }
        (*index)++;

        return ret_node;
    }

    if (buffer[*index] == kNodeEnd) {
        while(buffer[*index] == kNodeEnd) {
            (*index)++;
        }
        SkipSpaces(index, buffer);
        return NULL;
    }

    if (buffer[*index] == kNilBegin) {
        if(IsNull(index, buffer)) return NULL;
        fprintf(stderr, "Error: unknown symbol at index - [%d]\n", *index);
        return NULL;
    }

    if (buffer[*index] != kNilBegin && buffer[*index] != kNodeEnd) {
        fprintf(stderr, "Error: Expected '%c' at index %d\n", kNodeEnd, *index);
        return NULL;
    }

    fprintf(stderr, "Error: unknown symbol at index - [%d], char: [%c]\n", *index, buffer[*index]);
    return NULL;
}

//=================================================================================================================================================
//=================================================================================================================================================
//=================================================================================================================================================
//Как контролировать выход за границы массива? + очень много утечек
static void GetNodeSettings(tNode* node, int* index, char* buffer) {

    int flag_symb = (int)buffer[*index];
    int next_symb = (int)buffer[(*index) + 1];

    if (isdigit(flag_symb)) {
        SettingForConst(node, index, buffer);
    } else if (isalpha(flag_symb) && ((!isalpha(next_symb)) || (next_symb == kNilBegin))) {
        SettingForVariable(node, index, buffer);
    } else {
        SettingForOperation(node, index, buffer);
    }
    return;
}

//=================================================================================================================================================
//Усложнить stdtod?
static void SettingForConst(tNode* node, int* index, char* buffer) {
    double number  = 0;
    int chars_read = 0;
    int result = sscanf(&buffer[*index], "%lf%n", &number, &chars_read);

    if (result == 1) {
        node->type = kConst;
        node->data.value = number;
        (*index) += chars_read;
        return;
    }

    fprintf(stderr, "Error: couldnt read number in position - [%d]\n", *index);
    node->type = (tNodeType)kBasicSetting;
    node->data.value = 0;
    return;
}


static void SettingForVariable(tNode* node, int* index, char* buffer) {
    node->type = kVariable;

    int flag = VariableCode(index, buffer);
    if (flag == kPoisonValue) {
        fprintf(stderr, "Error: unknown variable at index - [%d]", *index);
        return;
    }
    node->data.code = flag;
    return;
}


static void SettingForOperation(tNode* node, int* index, char* buffer) {
    node->type = kOperation;

    int flag = OperationCode(index, buffer);
    if (flag == kPoisonValue) {
        node->data.code = 0;
        return;
    }
    node->data.code = flag;
    return;

}

//=================================================================================================================================================
//=================================================================================================================================================
//=================================================================================================================================================

static int OperationCode(int* index, char* buffer) {
    const char* op_storage[] = {
        "+", "-", "*", "/", "^", "sin",
        "cos", "exp", "tg", "ctg"
    };

    for (int current_op = 0; current_op < kOperationsAmount; current_op++) {
        size_t len = strlen(op_storage[current_op]);
        char* lowered_string = LoweringString(&buffer[*index], len);            //WARNING я изменяю исходную строку, норм?
        if (strncmp(op_storage[current_op], lowered_string, len) == 0) {
            (*index) += len;                                                    //WARNING - strlen
            return current_op + 1;                                              //+1 Caused by indexation
        }
    }
    fprintf(stderr, "Error: no matching operations at index - [%d]\n", *index);
    return kPoisonValue;
}

//=================================================================================================================================================

char* LoweringString(char* string, size_t len) {
    assert(string != NULL);

    for (size_t current_op = 0; current_op < len; current_op++) {
        string[current_op] = (char)tolower((unsigned char)string[current_op]);
    }
    return string;
}

//=================================================================================================================================================

static int VariableCode(int* index, char* buffer) {
    const char var_storage[] = {
        'x', 'y', 'z'
    };

    char variable = (char)tolower(buffer[*index]);

    for (int current_variable = 0; current_variable < kVariablesAmount; current_variable++) {
        if (variable == var_storage[current_variable]) {
            (*index)++;
            return current_variable + 1; //+1 сaused by indexation
        }
    }

    fprintf(stderr, "Error: no matching variable at index - [%d]\n", *index);
    return kPoisonValue;
}

//=================================================================================================================================================

//=================================================================================================================================================

void SkipSpaces(int* index, const char* buffer) {
    while (isspace((unsigned char)buffer[*index])) {
        (*index)++;
    }
}

//=================================================================================================================================================

static bool IsNull(int* index, const char* buffer) {
    assert(index != NULL);
    assert(buffer != NULL);

    if (strncmp(&buffer[*index], kNilString, kNilStringLen) == 0) {
        (*index) += kNilStringLen;
        return true;
    }
    return false;
}

//=================================================================================================================================================
//=================================================================================================================================================
//=================================================================================================================================================

static const char* OperationDecoder(int code) {
    if (code < 0 || code > kOperationsAmount) {
        fprintf(stderr, "Error: incorrect code was given to OperationDecoder\n");
        return NULL;
    }

    const char* op_storage[] = {
        "+", "-", "*", "/", "^", "sin",
        "cos", "exp", "tg", "ctg"
    };
    return op_storage[code - 1]; //-1 caused by indexation
}

//=================================================================================================================================================

static char VariableDecoder(int code) {
    if (code < 0 || code > kVariablesAmount) {
        fprintf(stderr, "Error: incorrect code was given to VariableDecoder\n");
        return '\0';                                                    //WARNING - хуйня или нет?
    }

    const char var_storage[] = {
        'x', 'y', 'z'
    };
    return var_storage[code - 1];
}

//=================================================================================================================================================
//=================================================================================================================================================
//=================================================================================================================================================

tTreeError PrintNode (tNode* node, FILE* printing_file, int* depth) {
    assert(node != NULL && __LINE__);
    assert(printing_file != NULL && __LINE__);

    if (node->data.code == kPoisonValue || (int)node->type == kPoisonValue) return kWrongNode;
                //Можно ли написать node->data?

    // fputc('\n', printing_file);
    // for (int tabs = 0; tabs < *depth; tabs++) fputc('\t', printing_file);    //WARNING - убрать
    // (*depth)++;

    fputc(kNodeBegin, printing_file);
    if (node->type == kConst) {
        fprintf(printing_file, "%lf", node->data.value);
    }

    if (node->type == kOperation) {
        const char* printable_operation = OperationDecoder(node->data.code);
        if  (printable_operation == NULL) return kNullPointer;

        fprintf(printing_file, "%s", printable_operation);
    }

    if (node->type == kVariable) {
        char printable_var = VariableDecoder(node->data.code);
        if (printable_var == '\0') return kUnkVar;

        fprintf(printing_file, "%c", printable_var);
    }

    tTreeError err_for_print = kNoErrors;
    if (node->left != NULL) {
        if ((err_for_print = PrintNode(node->left, printing_file, depth)) != kNoErrors) return err_for_print;
    }

    if (node->right != NULL) {
        if ((err_for_print = PrintNode(node->right, printing_file, depth)) != kNoErrors) return err_for_print;
    }

    if(node->left == NULL){
        fprintf(printing_file, kNilString);
    }
    if(node->right == NULL){
        fprintf(printing_file, kNilString);
    }

    fputc(kNodeEnd, printing_file);
    return kNoErrors;
}
//=================================================================================================================================================

tTreeError PrintTree(tDerivator* der, FILE* printing_file) {
    assert(der != NULL );
    assert(printing_file != NULL);

    int init_depth = 0;
    return PrintNode(der->root, printing_file, &init_depth);
}

//=================================================================================================================================================

static long long FileSize(FILE* file) {
    struct stat file_info;
    fstat(fileno(file), &file_info);
    return file_info.st_size;
}

//=================================================================================================================================================

tTreeError CreateBuffer(char** buffer, const char* const filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return kNullPointer;

    size_t file_size = (size_t)FileSize(file) + 1;                      //+1 for '\0' in the end
    char* template_buffer = (char*)calloc(file_size, sizeof(char));
    if(template_buffer == NULL) return kNullPointer;

    size_t symbols_read = fread(template_buffer, sizeof(char), file_size, file);
    if (symbols_read != file_size - 1) return kFreadError;              //-1 caused reason upper
    fclose(file);

    template_buffer[file_size - 1] = '\0';
    *buffer = template_buffer;
    return kNoErrors;
}

//=================================================================================================================================================
