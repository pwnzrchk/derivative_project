#ifndef TEX_DUMP_H
#define TEX_DUMP_H

#include "dif_tree.h"

#define TEX_FILENAME "files/9mice.tex"

//=================================================================================================================================================

static const int kSysCommandSize = 256;

//=================================================================================================================================================

tTreeError TexDump(tDerivator* der, const char* filename);
tTreeError TreeDump(tDerivator* der, FILE* file);
tTreeError NodeTex(tNode* node, FILE* file);
tTreeError TexPrint(const char* str, FILE* file);
int LatexToPdf(const char* tex_filename);

//=================================================================================================================================================

#endif
