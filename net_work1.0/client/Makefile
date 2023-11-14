SRCS:= $(wildcard *.cpp)
OBJS:= $(patsubst %.cpp,%.o,$(SRCS))
CC:= g++
client: $(OBJS)
	$(CC) $^ -o $@ -pthread -g
%.o:%.cpp
	$(CC) -c $^ -o $@ -g
clean:
	$(RM) $(OBJS) client
rebuild:clean client