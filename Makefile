CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS = -lm

SOURCES = main.c matrix.c operations.c parser.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = matfun

PREFIX ?= /usr/local
INSTALL_BIN = $(PREFIX)/bin

.PHONY: all clean install uninstall test help

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete"

install: $(TARGET)
	@mkdir -p $(INSTALL_BIN)
	cp $(TARGET) $(INSTALL_BIN)/matfun
	chmod 755 $(INSTALL_BIN)/matfun
	@echo "Installed to $(INSTALL_BIN)/matfun"

uninstall:
	rm -f $(INSTALL_BIN)/matfun
	@echo "Uninstalled"

test: $(TARGET)
	@echo "Running tests..."
	@echo "Test 1: Create matrix"
	@echo "A = [[1,2],[3,4]]" | $(TARGET)
	@echo ""
	@echo "Test 2: Determinant"
	@echo "A = [[1,2],[3,4]]" | $(TARGET)
	@echo "det A" | $(TARGET)
	@echo ""
	@echo "All tests complete!"

help:
	@echo "Matrix Calculator Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build the calculator (default)"
	@echo "  clean    - Remove build files"
	@echo "  install  - Install to $(PREFIX)/bin (requires sudo)"
	@echo "  uninstall - Remove installed calculator"
	@echo "  test     - Run basic tests"
	@echo "  help     - Show this help"
	@echo ""
	@echo "Variables:"
	@echo "  PREFIX   - Installation prefix (default: /usr/local)"
	@echo "            Use PREFIX=~/.local for user installation"