#include "graphviz_log.h"

//=================================================================================================================================================

static void PrintNodes(tNode* ref_node, FILE* log_file) {
    assert(ref_node != NULL);
    assert(log_file != NULL);

    if (ref_node == NULL) return;

    const char* font_color = "#FFFFFF";
    if (ref_node->type == kOperation) font_color = "#C2ADFF";
    if (ref_node->type == kVariable)  font_color = "#EBADFF";
    if (ref_node->type == kConst)     font_color = "#FFADEB";

    const char* color = "#FFFFFF";
    if (ref_node->type == kOperation) color = "#FFEBAD";
    if (ref_node->type == kVariable)  color = "#EBFFAD";
    if (ref_node->type == kConst)     color = "#C2FFAD";

    if (ref_node->type == kOperation) {
        char* operation = OperationDecoder(ref_node->data.code);
        fprintf(log_file, "\tNode%p [shape = Mrecord, style = \"rounded\", color = \"%s\", fontcolor = \"%s\", label= \"{ {<p_parent> parent: %p} | {address: %p} "
                          "| {type: \\\"%s\\\"} | { data: \\\"%s\\\" } | {<p_left> left: %p | <p_right> right: %p} } \"];\n",
                ref_node, color, font_color, (ref_node->parent), ref_node, "Operation",  operation, (ref_node->left), (ref_node->right));

    }


    if (ref_node->type == kVariable) {
        char variable = VariableDecoder(ref_node->data.code);
        fprintf(log_file, "\tNode%p [shape = Mrecord, style = \"rounded\", color = \"%s\", fontcolor = \"%s\", label= \"{ {<p_parent> parent: %p} | {address: %p} "
                          "| {type: \\\"%s\\\"} | { data: \\\"%c\\\" } | {<p_left> left: %p | <p_right> right: %p} } \"];\n",
                ref_node, color, font_color, (ref_node->parent), ref_node, "Variable", variable, (ref_node->left), (ref_node->right));

    }


    if (ref_node->type == kConst) {
    fprintf(log_file, "\tNode%p [shape = Mrecord, style = \"rounded\", color = \"%s\", fontcolor = \"%s\", label= \"{ {<p_parent> parent: %p} | {address: %p} "
                      "| {type: \\\"%s\\\"} | { data: %.2lf } | {<p_left> left: %p | <p_right> right: %p} } \"];\n",
            ref_node, color, font_color, (ref_node->parent), ref_node, "Value", ref_node->data.value, (ref_node->left), (ref_node->right));

    }


    if (ref_node->left != NULL) {
        PrintNodes(ref_node->left, log_file);
    }
    if (ref_node->right != NULL) {
        PrintNodes(ref_node->right, log_file);
    }

}

//=================================================================================================================================================

static void BeginGraph(FILE* log_file) {
    assert(log_file != NULL);

    fprintf(log_file, "digraph DifDump {\n"
                      "\tgraph [bgcolor = \"#0e1017\", rankdir=TB, ranksep=1.2, nodesep=0.8];\n"
                      "\tnode [shape=record, style=\"rounded,filled\", fillcolor=\"#FAEBD7\"];\n"
                      "\tedge [fontsize=18];\n\n");

}

//=================================================================================================================================================

static void EndGraph(FILE* log_file) {
    assert(log_file != NULL);

    fprintf(log_file, "}\n");
    fflush(log_file);
}

//=================================================================================================================================================

static void PrintEdges (tNode* ref_node, FILE* log_file) {
    assert(ref_node != NULL);
    assert(log_file != NULL);


    if (ref_node->left  != NULL) {
        fprintf(log_file, "\n\t\tNode%p:p_left->Node%p [label=\"Left\", arrowhead=normal,"
                          "fontcolor=\"#191970\", fontname=\"Arial\", color=\"#191970\", style=bold, weight = 1];", ref_node, (ref_node->left));

        fprintf(log_file, "\n\t\tNode%p:p_parent->Node%p:p_left [label=\"parent\", style=dashed, constrain=false,"
                          " arrowhead=normal,"
                          "fontcolor=\"#006400\", fontname=\"Times New Roman\", color=\"#006400\"];\n", (ref_node->left),  ref_node);

        PrintEdges(ref_node->left, log_file);
    }

    if (ref_node->right != NULL) {
        fprintf(log_file, "\n\t\tNode%p:p_right->Node%p [label=\"Right\", arrowhead=normal,"
                          "fontcolor=\"#800000\", fontname=\"Arial\", color=\"#800000\", style=bold, weight = 1];", ref_node, (ref_node->right));

        fprintf(log_file, "\n\t\tNode%p:p_parent->Node%p:p_right [label=\"parent\", arrowhead=normal,"
                          "fontcolor=\"#006400\", fontname=\"Times New Roman\", color=\"#006400\", style=dashed, "
                          "constrain = false];\n", (ref_node->right), ref_node);
                          if (ref_node->right != NULL) PrintEdges(ref_node->right,log_file);
    }

    fprintf(log_file, "\n");
}

//=================================================================================================================================================

static void CreateGraphImage(const char* dot_file_name, const char* png_file_name) {
    assert(dot_file_name != NULL);
    assert(png_file_name != NULL);

    char command[kMaxCommandLen];

    snprintf(command, kMaxCommandLen, "dot -Tpng %s -o %s", dot_file_name, png_file_name);

    int result = system(command);
    if (result == 0) {
        printf("Successfully created %s\n", png_file_name);
    } else {
        fprintf(stderr, "Error: Failed to execute dot command. Is Graphviz installed and in your PATH?\n");
    }
}

//=================================================================================================================================================

static void PrintHtml(const char* html_filename, const char* png_filename) {
    assert(html_filename != NULL);
    assert(png_filename != NULL);

    FILE* html_file = fopen(html_filename, "w");
    if (html_file == NULL) {
        fprintf(stderr, "Error: Could not open HTML log file %s\n", html_filename);
        return;
    }

    fprintf(html_file, "<h2>Tree Dump </h2>\n");
    fprintf(html_file, "<img src=\"%s\" width = 2000px>\n", png_filename);
    fprintf(html_file, "<hr>\n\n");

    fclose(html_file);
}

//=================================================================================================================================================

void GraphDump(tDerivator* der, const char* dot_file_name, const char* png_file_name, const char* html_filename) {
    assert(dot_file_name != NULL);
    assert(png_file_name != NULL);
    assert(html_filename != NULL);
    assert(der != NULL);

    FILE* log_file = fopen(dot_file_name, "w");

    if (log_file == NULL) {
        ERRPRINT(couldnt open log file);
    return;
    }

    BeginGraph(log_file);
    PrintNodes(der->root, log_file);
    PrintEdges(der->root, log_file);
    EndGraph(log_file);
    fclose(log_file);

    CreateGraphImage(dot_file_name, png_file_name);
    PrintHtml(html_filename, png_file_name);
    return;
}

//=================================================================================================================================================
