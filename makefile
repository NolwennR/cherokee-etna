CC=gcc
CFLAGS=-W -Wall -Wextra -Werror -g -I./include -I./libraries -I/usr/include/python3.6m -L/usr/include/python3.6m -DLOG_USE_COLOR -D_GNU_SOURCE 
RM=rm -rf
MDIR= mkdir -p
TESTLIB= -lcriterion
PLIB:=$(shell /usr/bin/python3-config --ldflags) #--cflags

TARGET=cherokee

SRCDIR= src/
LIBDIR= libraries/
SRC := $(shell find $(SRCDIR) -name "*.c")
LIB := $(shell find $(LIBDIR) -name "*.c")

OBJDIR= obj/
OBJ=$(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)
OBJLIB=$(LIB:$(LIBDIR)%.c=$(OBJDIR)%.o)

all: directories $(OBJLIB) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(OBJLIB) -o $(TARGET) $(PLIB)

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
