#!/bin/make
CC = g++
# FLAGS FOR MacOS
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

# FLAGS FOR LINUX
# -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat
# -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy
# -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op
# -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow
# -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn
# -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef
# -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers
# -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector
# -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla
# -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attrib
# ute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

# FLAGS FOR WINDOWS
# -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code
# -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe
# -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers
# -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo
# -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG
# -D_EJUDGE_CLIENT_SIDE



BUILD_DIR = build

TARGET = derivator

SRCS = src/main.cpp src/dif_tree.cpp src/dif_tree_parser.cpp src/math_func.cpp src/graphviz_log.cpp \
       src/tex_dump.cpp src/dif_optimizator.cpp src/parser.cpp src/functional.cpp console_handler.cpp

OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cpp=.o)))

EXECUTABLE = $(BUILD_DIR)/$(TARGET)


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
# 	@echo "Linking project..."
	@$(CC) $(CFLAGS) $^ -o $@ -lm



$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
# 	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -Iinclude -c $< -o $@


$(BUILD_DIR):
	@mkdir -p $@


clean:
# 	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)


.PHONY: all clean
