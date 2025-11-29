CC = g++
CFLAGS = -Og -ggdb -Wall -std=c++17 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations \
		 -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy     		 \
		 -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2     		 \
		 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith       		 \
		 -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo           		 \
		 -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef   		 \
		 -Wunreachable-code -Wunused -Wvariadic-macros -Wno-literal-range 			     		 \
		 -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast 			 		 \
		 -Wno-varargs -Wstack-protector -Wsuggest-override -Wbounds-attributes-redundant 		 \
		 -Wlong-long -Wopenmp  -fsized-deallocation -fstack-protector 		 			 		 \
		 -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-protector  		 \
		 -fPIE -Werror=vla -fsanitize=address


BUILD_DIR = build

TARGET = derivator

SRCS = main.cpp dif_tree.cpp dif_tree_parser.cpp math_func.cpp graphviz_log.cpp tex_dump.cpp dif_optimizator.cpp

OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

EXECUTABLE = $(BUILD_DIR)/$(TARGET)


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@echo "Linking project..."
	@$(CC) $(CFLAGS) $^ -o $@ -lm


$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@


$(BUILD_DIR):
	@mkdir -p $@


clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)


.PHONY: all clean
