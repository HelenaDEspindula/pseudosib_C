# Makefile do pseudosib

CFLAGS = -Wall -g
LDFLAGS = -lm

objs = pseudosib.o

# Regra default

all: pseudosib $(objs)

# Regras de ligação

pseudosib: pseudosib.c
	gcc pseudosib.c -o pseudosib $(CFLAGS) $(LDFLAGS)

# Regras de compilação

pseudosib.o: pseudosib.c
	gcc -c pseudosib.c $(CFLAGS)



