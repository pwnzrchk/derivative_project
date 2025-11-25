#ifndef DIF_TREE_PARSER_H
#define DIF_TREE_PARSER_H

#include <ctype.h>
#include "dif_tree.h"

//=================================================================================================================================================

static const char* const kNilString = "nil";
static const int kNilStringLen = 3;
static int kBasicSetting = 1;

//=================================================================================================================================================

tNode* ReadNode(tNode* parent_node, int* index, char* buffer);

tTreeError PrintNode   (tNode* node, FILE* printing_file, int* depth);
tTreeError CreateBuffer(char** buffer, const char* const filename);
tTreeError ReadDer     (tDerivator* der, const char* const filename);
tTreeError PrintTree   (tDerivator* der, FILE* printing_file);

void  SkipSpaces    (int* index, const char* buffer);
char* LoweringString(char* string, size_t len);

//=================================================================================================================================================


#endif // DIF_TREE_PARSER_H

//TODO - сделать макрос
/*
if (buffer_for_number == NULL) {
        fprintf(stderr, "Error: allocation in %s at %s:%d\n",
                __FUNCTION__, __FILE__, __LINE__);
        return;
    }
*/
