#ifndef GRAPHVIZ_LOG_H
#define GRAPHVIZ_LOG_H

#include "dif_tree_parser.h"

//=================================================================================================================================================

#define LOG_FILE  "files/9mice.dot"
#define PNG_FILE  "files/9mice.png"
#define HTML_FILE "files/9mice.html"

static const int kMaxCommandLen = 128;


void GraphDump(tDerivator* der, const char* dot_file_name, const char* png_file_name, const char* html_filename);
//=================================================================================================================================================

#endif // GRAPHVIZ_LOG_H
