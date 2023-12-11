CC=clang
CFLAGS=-Wall \
	-Wextra \
	-Wconversion \
	-Wshorten-64-to-32 \
	-Wconstant-conversion \
	-Wliteral-range \
	-Woverflow \
	-Wimplicit-int \
	-Wreserved-identifier \
	-pedantic-errors \
	-Werror \
	-std=c17 \
	-fsanitize=address,undefined \
	-g \
	-O1

BIN_DIR=bin

# Deduplicating is necessary for the case where we're compiling the utils projects, to avoid including the util lib.c file twice which would lead to duplicate symbol errors.
SOURCES = $(shell echo ./*.c ../utils/*/lib.c | xargs -n 1 realpath 2>/dev/null | sort | uniq)

$(BIN_DIR)/test: $(SOURCES)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CFLAGS) -o $@ $^

test: $(BIN_DIR)/test
	@$<
