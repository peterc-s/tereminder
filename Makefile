CC = gcc
CFLAGS ?= -Wall -Wextra -pedantic -O2 -D_XOPEN_SOURCE=600 -D_FORTIFY_SOURCE=3 -Werror=format-security -fstack-clash-protection -fstack-protector-all -fcf-protection -s -Wl,-z,relro,-z,now
CFLAGS += $(CFLAGS_EX)

SRCDIR = src
BUILDDIR = build

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

INSTALL_DIR = /usr/sbin

NAME = tereminder

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

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

.PHONY: all clean run install uninstall
