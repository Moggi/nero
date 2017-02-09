########################## Makefile ##########################
# thanks to this guys:
# https://github.com/CELTAB/AVR-Util/blob/master/tools/Makefile

# Just remember
# CURRENT_DIR = $(CURDIR)

LIBS = -lGLEW -lSDL2 -lSDL2_image -lSDL2_ttf
FRAMEWORKS = -framework OpenGL

CC = clang++
CXXFLAGS = -w -std=c++11
INCLUDE_DIR = /usr/local/include
LIB_DIR = /usr/local/lib
LDFLAGS = -L$(LIB_DIR) $(LIBS) -I$(INCLUDE_DIR) $(FRAMEWORKS)

MAIN = ${NAME}

OBJ_DIR = obj
SRC_DIR = .
BIN_DIR = bin

FOLDERS = $(OBJ_DIR) $(SRC_DIR) $(BIN_DIR)

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TARGET = $(strip $(basename $(MAIN)))

EXEC = $(addprefix $(BIN_DIR)/, $(TARGET))


all: $(OBJECTS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $(EXEC)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) -c $(CXXFLAGS) $< $(LDFLAGS) -o $@

install:
	mkdir -p $(FOLDERS)

clean:
	rm -rf $(OBJECTS)

mrproper: clean
	rm -f $(EXEC)

remade:
	$(MAKE) mrproper
	$(MAKE)
