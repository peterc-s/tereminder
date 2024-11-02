CC = gcc
CFLAGS ?= -Wall -Wextra -pedantic -O2
CFLAGS += $(CFLAGS_EX)

SRCDIR = src
BUILDDIR = build

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

INSTALL_DIR = /usr/sbin

NAME = tereminder

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $^ -o $@

$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run : $(NAME)
	./$(NAME) $(ARGS)

clean:
	-rm -r $(BUILDDIR)
	-rm $(NAME)

install : $(NAME)
	sudo cp $(NAME) $(INSTALL_DIR)/$(NAME)

uninstall :
	sudo rm $(INSTALL_DIR)/$(NAME)

.PHONY: all clean run
