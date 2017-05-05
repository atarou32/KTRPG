default: PROGRAM subsystem

.PHONY : subsystem
PROGRAM : main
OBJS = main.o
SRCS = $(OBJS: %.o=%.c)
CFLAGS = -g -Wall
LDLIBS = -lpthread -lz -lm -ldl
LDFLAGS = 
$(PROGRAM): $(OBJS) $(SUBTEST)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) $(OBJS) $(LDLIBS)
SRCROOT = .
UEDIR = ../
SRCDIRS  = $(shell find $(SRCROOT) -type d)
TESTDIRS = $(patsubst ./TEST_%, ./TEST_%/, $(SRCDIRS))
TESTDIRS2 = $(filter-out . %/TEST/ %/temp/, $(TESTDIRS))
SEARCHDIR = cd $(dr) && $(MAKE) && cd $(UEDIR);
subsystem : sub
sub : 
	$(foreach dr, $(TESTDIRS2), $(SEARCHDIR))
