########################## Makefile ##########################
# https://github.com/CELTAB/AVR-Util/blob/master/tools/Makefile

LIBS = -lGLEW -lSDL2 -lSDL2_image -lSDL2_ttf
FRAMEWORKS = -framework OpenGL

CC = clang++
CXXFLAGS = -std=c++11
INCLUDE_DIR = /usr/local/include
LIB_DIR = /usr/local/lib
LDFLAGS = -L$(LIB_DIR) $(LIBS) -I$(INCLUDE_DIR) $(FRAMEWORKS)

MAIN = main.cpp

EXTRA_SOURCE_DIR =
EXTRA_SOURCE_FILES =

LOCAL_SOURCE = $(wildcard *.cpp)

TARGET = $(strip $(basename $(MAIN)))
EXEC = $(TARGET)

EXTRA_SOURCE = $(addprefix $(EXTRA_SOURCE_DIR), $(EXTRA_SOURCE_FILES))
SRC = $(EXTRA_SOURCE)
SRC += $(subst ${MAIN},,${LOCAL_SOURCE})

OBJ = $(SRC:.cpp=.o)

HEADERS = $(SRC:.cpp=.h)


# all: ftest
# ftest: $(CPPSOURCES:.cpp=.o)
# 	g++ -o $@ $^ $(LDFLAGS)
# %.o: %.cpp
# 	g++ -c $< $(CPPFLAGS) -o $@

all: $(OBJ)
	$(CC) $(MAIN) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $(EXEC)

%.o: %.cpp
	@echo $<
	$(CC) -c $(CXXFLAGS) $< $(LDFLAGS) -o $@

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -f $(EXEC)

remade:
	$(MAKE) mrproper
	$(MAKE)
