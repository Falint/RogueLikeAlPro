# ══════════════════════════════════════════════════════════════════════════════
# Makefile — Roguelike Dungeon Crawler
#
# Build targets:
#   make          → build debug
#   make release  → build optimized release
#   make clean    → remove build artifacts
#   make run      → build and run
# ══════════════════════════════════════════════════════════════════════════════

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic

# Debug flags (default)
DEBUG_FLAGS   := -g -O0 -DDEBUG
# Release flags
RELEASE_FLAGS := -O2 -DNDEBUG

TARGET   := roguelike

# Source files
SRCDIR   := src
SOURCES  := $(shell find $(SRCDIR) -name '*.cpp')
OBJECTS  := $(SOURCES:.cpp=.o)

# ─── Default target: debug build ─────────────────────────────────────────────
.PHONY: all
all: CXXFLAGS += $(DEBUG_FLAGS)
all: $(TARGET)

# ─── Release build ───────────────────────────────────────────────────────────
.PHONY: release
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

# ─── Link ─────────────────────────────────────────────────────────────────────
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# ─── Compile ──────────────────────────────────────────────────────────────────
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ─── Run ──────────────────────────────────────────────────────────────────────
.PHONY: run
run: all
	./$(TARGET)

# ─── Clean ────────────────────────────────────────────────────────────────────
.PHONY: clean
clean:
	find $(SRCDIR) -name '*.o' -delete
	rm -f $(TARGET)
