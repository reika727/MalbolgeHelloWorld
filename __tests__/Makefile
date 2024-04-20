CXX      := /usr/local/bin/g++
CXXFLAGS := -Wall -Wextra --std=c++23
CPPFLAGS := -MMD -MP
SRCS     := main.cpp ../malbolge.cpp malbolge_machine.cpp
OBJS     := $(SRCS:.cpp=.o)
DEPS     := $(SRCS:.cpp=.d)
TARGET   := malbolge.out

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

-include $(DEPS)

.PHONY: test
test: $(TARGET)
	./$< hello.mb

.PHONY: clean
clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET)
