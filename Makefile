# Makefile do pseudosib

CFLAGS = -Wall -g
LDFLAGS = -lm

objs = library.o pseudosib.o

# Regra default

all: pseudosib $(objs)

# Regras de ligação

pseudosib: pseudosib.c library.c
	gcc pseudosib.c library.c -o pseudosib $(CFLAGS) $(LDFLAGS)

# Regras de compilação

pseudosib.o: pseudosib.c library.h
	gcc -c pseudosib.c $(CFLAGS)

library.o: library.c library.h
	gcc -c library.c $(CFLAGS)

#remove arquivos temporários
clean:
	-rm $(objs) *~

#remove tudo o que não for código fonte
purge: clean
		-rm programa
