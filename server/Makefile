PROGRAM = server
OBJS = main.o server.o daemon.o my_handler.o
SRCS = $(OBJS: %.o=%.c)
CFLAGS = -g -Wall
LDLIBS = -lpthread -lz -lm -ldl
LDFLAGS = 

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) $(OBJS) $(LDLIBS)


