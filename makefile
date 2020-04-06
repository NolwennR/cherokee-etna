CC=gcc
CFLAGS=-W -Wall -Wextra -Werror --std=c11 -g -I./include -I./libraries -DLOG_USE_COLOR
RM=rm -rf
MDIR= mkdir -p
TESTLIB= -lcriterion

TARGET=cherokee

SRCDIR= src/
LIBDIR= libraries/
SRC := $(shell find $(SRCDIR) -name "*.c")
LIB := $(shell find $(LIBDIR) -name "*.c")

OBJDIR= obj/
OBJ=$(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)
OBJLIB=$(LIB:$(LIBDIR)%.c=$(OBJDIR)%.o)

all: directories $(OBJLIB) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(OBJLIB) -o $(TARGET) $(LIBS)

directories: 
	$(MDIR) $(sort $(dir $(OBJ)))
	$(MDIR) $(sort $(dir $(OBJLIB)))

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(TINYLIBS)

$(OBJDIR)%.o: $(LIBDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(TINYLIBS)

clean: 
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(TARGET)

re: fclean all

tests_run: 
	make -C test/unit-tests/ run-tests

.PHONY: all clean fclean re hard-re build-lib directories
