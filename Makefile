CXX      := /usr/local/bin/g++
CXXFLAGS := -Wall -Wextra -O3 -mtune=native -march=native --std=c++23
SRCS     := main.cpp malbolge.cpp malbolge_machine_state.cpp
TARGET   := malbolge-hello.out

$(TARGET): $(SRCS)
	$(CXX) $^ $(CXXFLAGS) -o $@

.PHONY: test
test: $(TARGET)
	./$<

.PHONY: check
check: $(SRCS)
	$(CXX) $^ $(CXXFLAGS) -fsyntax-only

.PHONY: clean
clean:
	$(RM) $(TARGET)
