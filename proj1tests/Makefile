SRCS = test_mailbox_system.c
OBJS = $(SRCS:.c=.o)
BIN = test_mailbox_system
CFLAGS = -g
LDFLAGS = -g

# You shouldn't need to touch anything below this line...                       
all: $(BIN)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	-rm -f $(OBJS)
	-rm -f $(BIN)


# source code to build Kernel

#obj-y := mailbox_system.o
