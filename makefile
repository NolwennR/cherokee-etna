CC=gcc
CFLAGS=-W -Wall -Wextra -Werror --std=c11 -g -I./include -I./libraries -DLOG_USE_COLOR
RM=rm -rf
MDIR= mkdir -p

TARGET=cherokee

SRCDIR= src/
LIBDIR= libraries/
SRC := $(shell find $(SRCDIR) -name "*.c")
SRC += $(shell find $(LIBDIR) -name "*.c")

OBJDIR= obj/
OBJ=$(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)

all: directories  $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LIBS)

directories: 
	$(MDIR) $(sort $(dir $(OBJ)))

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(TINYLIBS)

clean: 
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(TARGET)

re: fclean all

.PHONY: all clean fclean re hard-re build-lib directories
