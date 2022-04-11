CXX = gcc
#CXX = clang
CXXFLAGS = -Wall -pedantic -Wextra -Werror -g -O0 -I./
LDFLAGS = -g -O0 -I./

PROG = emulator
OBJDIR = ./obj/
HEADDIR = ./include/
SRCDIR = ./src/
HEADS = $(wildcard $(HEADDIR)*.h)
SRCS = $(wildcard $(SRCDIR)*.c)
OBJS = $(SRCS:./src/%.c=./obj/%.o)

$(PROG): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

$(OBJDIR)%.o: $(SRCDIR)%.c $(HEADS)
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(PROG)

.PHONY: run
run: $(PROG)
	./$(PROG)
