CXX      := /usr/local/bin/g++
CXXFLAGS := -Wall -Wextra --std=c++23
SRCS     := main.cpp ../malbolge.cpp malbolge_machine.cpp
TARGET   := malbolge.out

$(TARGET): $(SRCS)
	$(CXX) $^ $(CXXFLAGS) -o $@

.PHONY: test
test: $(TARGET)
	./$< hello.mb

.PHONY: check
check: $(SRCS)
	$(CXX) $^ $(CXXFLAGS) -fsyntax-only

.PHONY: clean
clean:
	$(RM) $(TARGET)
