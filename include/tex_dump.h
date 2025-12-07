#ifndef TEX_DUMP_H
#define TEX_DUMP_H

#include "dif_tree.h"

#define TEX_FILENAME "files/9mice.tex"

//=================================================================================================================================================

tTreeError TexDump(tDerivator* der, const char* filename);
tTreeError NodeTex(tNode* node, FILE* file);
tTreeError TexPrint(const char* str, FILE* file);

//=================================================================================================================================================

#endif
