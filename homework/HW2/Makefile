#CC = clang
CFLAGS += -Wall
 
all: checker coordinator
 
checker: Checker.c
	$(CC) $(CFLAGS) $< -o $@
 
coordinator: Coordinator.c
	$(CC) $(CFLAGS) $< -o $@
 
clean:
	rm -f checker coordinator