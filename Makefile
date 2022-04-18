SHELL = /bin/sh
CC = gcc
#CC = clang
CCFLAGS = -Wall -pedantic -Wextra -Werror -g -O0 -I.
LDFLAGS = -g -O0 -I.

PROG = emulator
OBJDIR := ./obj/
HEADDIR := ./include/
SRCDIR := ./src/
HEADS = $(wildcard $(HEADDIR)*.h)
SRCS = $(wildcard $(SRCDIR)*.c)
OBJS = $(SRCS:./src/%.c=./obj/%.o)

$(PROG): $(OBJS)
	@echo -e '\033[0;1m  [$(CC)]  $@\033[0m'
	@$(CC) $(LDFLAGS) $(OBJS) -o $@

$(OBJDIR)%.o: $(SRCDIR)%.c $(HEADS) | $(OBJDIR)
	@echo -e '\033[0;1m  [$(CC)]  $@\033[0m'
	@$(CC) $(CCFLAGS) -c $< -o $@

.PHONY: $(OBJDIR)
$(OBJDIR):
	@mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	@rm -rf $(OBJS) $(PROG)
	@echo -e '\033[0;1m  Object files and binary removed\033[0m'

.PHONY: run
run: $(PROG)
	@./$(PROG)
