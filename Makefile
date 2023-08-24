CXX      := /usr/local/bin/g++
CXXFLAGS := -Wall -Wextra -O3 -mtune=native -march=native --std=c++23
LDFLAGS  := -Wl,-rpath="$(shell dirname $(shell readlink -f $(shell which $(CXX))))/../lib64/"
SRCS     := main.cpp malbolge.cpp malbolge_machine_state.cpp
TARGET   := malbolge-hello.out

$(TARGET): $(SRCS)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

.PHONY: test
test: $(TARGET)
	./$<

.PHONY: check
check: $(SRCS)
	$(CXX) $^ $(CXXFLAGS) -fsyntax-only

.PHONY: clean
clean:
	$(RM) $(TARGET)
