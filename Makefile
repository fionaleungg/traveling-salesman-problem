CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic -gdwarf-4 -g
LFLAGS = -lm
#TESTS= tests
EXEC = tsp
HEADERS = graph.h path.h stack.h vertices.h
OBJS = graph.o path.o stack.o 

all: $(EXEC)

#tsp: $(EXEC)

#$(TESTS): $(TESTS).o $(OBJS)
#	$(CC) $^ $(LFLAGS) -o $@

$(EXEC): $(EXEC).o $(OBJS) 
	$(CC) $^ $(LFLAGS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXEC) *.o 

format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean format



