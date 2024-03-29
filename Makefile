INCLUDE_DIR = ./include
SOURCE_DIR = ./source
OBJECT_DIR = ./object

CC = gcc
CFLAGS=-g -I$(INCLUDE_DIR)
OUT = prodline

_DEPS = AssemblageStage.h CheckStage.h Component.h ErrorCodes.h ManufactureStage.h PaintshopStage.h SharedMem.h
#pattern matching from  _DEPS to include directory
DEPS = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJ = AssemblageStage.o CheckStage.o Component.o main.o ManufactureStage.o PaintshopStage.o SharedMem.o
#same pattern matching principe
OBJ = $(patsubst %,$(OBJECT_DIR)/%,$(_OBJ))

.PHONY: clean

############goals#######################

#general rule for all object files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#default goal
$(OUT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

count:
	wc $(SOURCE_DIR)/*.c $(DEPS)

clean:
	rm -f $(OBJECT_DIR)/*.o ./prodline
