CC := clang
CFLAGS := -g -lm -Wno-deprecated-declarations -fsanitize=address

SYSTEM := $(shell uname -s)
ifeq ($(SYSTEM),Darwin)
  CFLAGS += -I$(shell brew --prefix openssl)/include -L$(shell brew --prefix openssl)/lib
else
  CFLAGS += -I/home/curtsinger/.local/include -L/home/curtsinger/.local/lib
endif



all: main

clean:
	rm -f morse utils

main: morse.c utils.h Makefile
	$(CC) $(CFLAGS) -o morse morse.c -lcrypto -lpthread -lm


zip:
	@echo "Generating final.zip file to submit to Gradescope..."
	@zip -q -r final.zip . -x .git/\* .vscode/\* .clang-format .gitignore main final/\*
	@echo "Done. Please upload final.zip to Gradescope."

format:
	@echo "Reformatting source code."
	@clang-format -i --style=file $(wildcard *.c) $(wildcard *.h) $(wildcard *.cu)
	@echo "Done."

.PHONY: all clean zip format

