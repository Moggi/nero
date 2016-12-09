########################## Makefile ##########################
# https://github.com/CELTAB/AVR-Util/blob/master/tools/Makefile

LIBS = -lGLEW -lSDL2 -lSDL2_image -lSDL2_ttf
FRAMEWORKS = -framework OpenGL

CC = clang++
CXXFLAGS = -c -std=c++11
INCLUDE_DIR = /usr/local/include
LIB_DIR = /usr/local/lib
LDFLAGS = -L$(LIB_DIR) $(LIBS) -I$(INCLUDE_DIR) $(FRAMEWORKS)
EXEC = main

MAIN = main.cpp

EXTRA_SOURCE_DIR =
EXTRA_SOURCE_FILES =

LOCAL_SOURCE = $(wildcard *.cpp)

TARGET = $(strip $(basename $(MAIN)))

SRC = $(TARGET).cpp
EXTRA_SOURCE = $(addprefix $(EXTRA_SOURCE_DIR), $(EXTRA_SOURCE_FILES))
SRC += $(EXTRA_SOURCE)
SRC += $(LOCAL_SOURCE)

OBJ = $(SRC).o

HEADERS = $(SRC:.cpp=.h)


# all: ftest
# ftest: $(CPPSOURCES:.cpp=.o)
# 	g++ -o $@ $^ $(LDFLAGS)
# %.o: %.cpp
# 	g++ -c $< $(CPPFLAGS) -o $@



all: $(SRC).o
	$(CC) $^ $(LDFLAGS) -o $(EXEC)

%.o: $(SRC)
	$(CC) $(CXXFLAGS) $< $(LDFLAGS) -o $@

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -f $(EXEC)
