CXX      := /usr/local/bin/g++
CXXFLAGS := -Wall -Wextra -O3 -mtune=native -march=native --std=c++23
CPPFLAGS := -MMD -MP
SRCS     := main.cpp malbolge.cpp malbolge_machine_state.cpp
OBJS     := $(SRCS:.cpp=.o)
DEPS     := $(SRCS:.cpp=.d)
TARGET   := malbolge-hello.out

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

-include $(DEPS)

.PHONY: test
test: $(TARGET)
	./$<

.PHONY: clean
clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET)
