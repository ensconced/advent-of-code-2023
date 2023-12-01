CC=clang
CFLAGS=-Wall \
	-Wextra \
	-Wconversion \
	-Wshorten-64-to-32 \
	-Wconstant-conversion \
	-Wliteral-range \
	-Woverflow \
	-Wimplicit-int \
	-Werror \
	-pedantic-errors \
	-std=c17 \
	-fsanitize=address,undefined \
	-g \
	-O1

BIN_DIR=bin

# Deduplicating is necessary for the case where we're compiling the test-utils project, where lib.c
# actually resolves to the same file as test-utils/lib.c, which would otherwise mean we'd get
# duplicate symbol errors.
SOURCES = $(shell echo test.c lib.c ../test-utils/lib.c | xargs -n 1 realpath 2>/dev/null | sort | uniq)

$(BIN_DIR)/test: $(SOURCES)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

test: $(BIN_DIR)/test
	$<
