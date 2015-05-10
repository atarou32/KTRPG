PROGRAM = server
OBJS = main.o server.o daemon.o my_handler.o my_mysql.o
SRCS = $(OBJS: %.o=%.c)
CFLAGS = -g -Wall
LDLIBS = -lmysqlclient -lpthread -lz -lm -ldl
LDFLAGS = 

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) $(OBJS) $(LDLIBS)


