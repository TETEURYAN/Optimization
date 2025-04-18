# Caminho base do CPLEX
CPLEX_DIR = /opt/ibm/ILOG/CPLEX_Studio_Community2212

# Includes
INCLUDES = \
  -I$(CPLEX_DIR)/cplex/include \
  -I$(CPLEX_DIR)/concert/include

# Bibliotecas
LIBDIRS = \
  -L$(CPLEX_DIR)/cplex/lib/x86-64_linux/static_pic \
  -L$(CPLEX_DIR)/concert/lib/x86-64_linux/static_pic

# Flags de compilação
CXX = g++
CXXFLAGS = -O2 -Wall $(INCLUDES)
LDFLAGS = $(LIBDIRS) -lilocplex -lconcert -lcplex -lm -lpthread -ldl

# Source files
SRCS = $(wildcard src/*.cpp)
# Target executables (in bin/)
TARGETS = $(patsubst src/%.cpp,bin/%,$(SRCS))

# Ensure bin directory exists
$(shell mkdir -p bin)

# Default target - build all executables
all: $(TARGETS)

# Rule to build each executable
bin/%: src/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# Run all executables
run: all
	@for exe in $(TARGETS); do \
		echo "Running $$exe"; \
		$$exe; \
	done

# Clean all executables
clean:
	rm -f $(TARGETS)