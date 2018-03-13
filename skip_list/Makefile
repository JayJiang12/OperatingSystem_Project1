SRCS = skip_list.c
OBJS = $(SRCS:.c=.o)
BIN = skip_list
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
