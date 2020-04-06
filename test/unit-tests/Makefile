CC=gcc
CFLAGS=-W -Wall -Wextra -Werror --std=c11 -g -I../../include -I../../libraries -lcriterion
RM=rm -rf
MDIR= mkdir -p

TARGET=unit-tests

TESTDIR= ./
SRCDIR= ../../src/
LIBDIR= ../../libraries/
TEST := $(shell find $(TESTDIR) -name "*.c")
SRC := $(SRCDIR)/parser.c
LIB := $(shell find $(LIBDIR) -name "*.c")

OBJDIR= obj/
OBJ=$(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)
OBJLIB=$(LIB:$(LIBDIR)%.c=$(OBJDIR)%.o)
OBJTEST=$(TEST:$(TESTDIR)%.c=$(OBJDIR)%.o)

run-tests: all
	./$(TARGET)

all: directories $(OBJLIB) $(OBJ) $(OBJTEST)
	$(CC) $(CFLAGS) $(OBJ) $(OBJLIB) $(OBJTEST) -o $(TARGET)

directories: 
	$(MDIR) $(sort $(dir $(OBJ)))
	$(MDIR) $(sort $(dir $(OBJLIB)))
	$(MDIR) $(sort $(dir $(OBJTEST)))

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(TINYLIBS)

$(OBJDIR)%.o: $(LIBDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(TINYLIBS)

$(OBJDIR)%.o: $(TESTDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(TINYLIBS)

clean: 
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(TARGET)

re: fclean all