SRCS:=$(wildcard *.cpp)
EXES:=$(patsubst %.cpp,%,$(SRCS))
all:$(EXES)
%:%.cpp
	g++ $^ -o $@ -g -pthread
clean:
	$(RM) $(EXES)