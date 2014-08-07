CC=gcc # I'd use clang, but gcc links to clang on OS X anyways
CFLAGS=-Wall -framework CoreFoundation -framework IOKit # -Weverything is clang-specific

all: tmux-battery

tmux-battery:
	$(CC) $(CFLAGS) tmux-battery.c -o tmux-battery

clean:
	rm -f tmux-battery a.out
