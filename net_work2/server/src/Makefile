SRCS:= $(wildcard *.cpp)
OBJS:= $(patsubst %.cpp,%.o,$(SRCS))
CC:= g++
server: $(OBJS)
	$(CC) $^ -o $@ -pthread -g -lmysqlclient -lpthread -lcrypt
%.o:%.cpp
	$(CC) -c $^ -o $@ -g
clean:
	$(RM) $(OBJS) server
rebuild:clean server