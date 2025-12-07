CC = g++
CFLAGS = -Og -ggdb -Wall -std=c++17 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations \
         -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy             \
         -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2             \
         -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith               \
         -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo                   \
         -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef           \
         -Wunreachable-code -Wunused -Wvariadic-macros -Wno-literal-range                        \
         -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast                      \
         -Wno-varargs -Wstack-protector -Wsuggest-override -Wbounds-attributes-redundant         \
         -Wlong-long -Wopenmp  -fsized-deallocation -fstack-protector                            \
         -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-protector          \
         -fPIE -Werror=vla -fsanitize=address


BUILD_DIR = build

TARGET = derivator

SRCS = src/main.cpp src/dif_tree.cpp src/dif_tree_parser.cpp src/math_func.cpp src/graphviz_log.cpp src/tex_dump.cpp src/dif_optimizator.cpp src/parser.cpp src/functional.cpp console_handler.cpp

OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cpp=.o)))

EXECUTABLE = $(BUILD_DIR)/$(TARGET)


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@echo "Linking project..."
	@$(CC) $(CFLAGS) $^ -o $@ -lm



$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -Iinclude -c $< -o $@


$(BUILD_DIR):
	@mkdir -p $@


clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)


.PHONY: all clean
