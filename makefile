# directories
SRCDIR = ./src
INCDIR = ./inc

# complier to use
CC = gcc

# Include paths for header files
INCLUDES = -I $(INCDIR) -I /usr/local/include

# Compiler flags
CFLAGS = -Wall -Wextra  -g $(INCLUDES) -std=c99 #-pthread

# All source files
SRCS = $(wildcard $(SRCDIR)/*.c)

# All object files
OBJS := $(SRCS:$(SRCDIR)/%.c=%.o)

# Executable name
MAIN=pipeline

.PHONY: all
all: $(MAIN)

$(MAIN): $(OBJS)
	@echo "Compiling: $@"
	@$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBS)

# Automatically builds all object files from source files
# -c option compiles but does not link (create object files)
# -o is output filename
$(OBJS): %.o : $(SRCDIR)/%.c
	@echo "Compiling object file: $@"
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo




.PHONY: clean
clean:
	@if [ `find . -name "*.o" | head -1` ]; then rm *.o; fi;
	@if [ `find . -name "temp" | head -1` ]; then rm temp; fi;
	@if [ `find . -name ".DS_Store" | head -1` ]; then rm -rf .DS_Store; fi;
	rm -rf $(MAIN)
	rm -rf temp
