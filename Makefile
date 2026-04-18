INCLUDEFLAGS := -I./include -I./imgui -I./imgui/backends/ 
LIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
COMMON_COMPILE_FLAGS := -Wall -Wextra $(INCLUDEFLAGS) $(LIBS) -std=c++23 
RELEASE := $(COMMON_COMPILE_FLAGS) -O2
DEBUG := $(COMMON_COMPILE_FLAGS) -ggdb -O0

COMPILEFLAGS := $(RELEASE)
ifeq ($(mode), debug)
	COMPILEFLAGS := $(DEBUG)
endif

SOURCES := $(shell find ./src/ -type f -name '*.cpp')
IMGUI_SOURCES := $(wildcard ./imgui/*.cpp) 
IMGUI_BACKEND := ./imgui/backends/imgui_impl_opengl3.cpp

OBJ_FILES := $(patsubst %.cpp, obj/%.o, $(SOURCES) $(IMGUI_SOURCES) $(IMGUI_BACKEND))


all: ./bin/main

./bin/main: setup ./main.cpp $(OBJ_FILES) 
	g++ $(OBJ_FILES) ./main.cpp $(COMPILEFLAGS) -o ./bin/main 

./obj/%.o: %.cpp
	g++ -c $< $(COMPILEFLAGS) -o $@

setup: 
	mkdir -p bin/ obj/imgui/backends obj/src/engine

clean:
	rm ./bin/main $(OBJ_FILES)
