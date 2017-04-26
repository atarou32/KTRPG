PROGRAM = main
OBJS = main.o resultcheck.o
SRCS = $(OBJS: %.o=%.c)
CFLAGS = -g -Wall
LDLIBS = -lpthread -lz -lm -ldl
LDFLAGS = 

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) $(OBJS) $(LDLIBS)


