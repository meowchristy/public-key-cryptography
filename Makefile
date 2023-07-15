CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -lm $(shell pkg-config --libs gmp)

OBJS_K = keygen.o randstate.o numtheory.o ss.o
OBJS_E = encrypt.o randstate.o numtheory.o ss.o
OBJS_D = decrypt.o randstate.o numtheory.o ss.o

debug: CFLAGS+= -g -gdwarf-4
debug: encrypt decrypt keygen

all: keygen encrypt decrypt
	
encrypt: $(OBJS_E)
	$(CC) $(LDFLAGS) -o encrypt $(OBJS_E)
		
decrypt: $(OBJS_D)
	$(CC) $(LDFLAGS) -o decrypt $(OBJS_D)
	
keygen: $(OBJS_K)
	$(CC) $(LDFLAGS) -o keygen $(OBJS_K)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f keygen encrypt decrypt $(OBJS_K) $(OBJS_E) $(OBJS_D)

format:
	clang-format -i -style=file *.[ch]
