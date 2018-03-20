# Makefile do pseudosib

CFLAGS = -Wall -g
LDFLAGS = -lm

objs = pseudosib.o library.o liballoc.o libinout.o

ifeq ($(PREFIX),)
	PREFIX := /usr/bin
endif

# Regra default

all: pseudosib $(objs)

# Regras de ligação

pseudosib: pseudosib.c library.c liballoc.c libinout.c
	gcc pseudosib.c library.c liballoc.c libinout.c -o pseudosib $(CFLAGS) $(LDFLAGS)

# Regras de compilação

pseudosib.o: pseudosib.c library.h liballoc.h libinout.h global.h
	gcc -c pseudosib.c $(CFLAGS)

library.o: library.c library.h liballoc.h libinout.h global.h
	gcc -c library.c $(CFLAGS)

liballoc.o: liballoc.c library.h liballoc.h libinout.h global.h
	gcc -c liballoc.c $(CFLAGS)

libinout.o: libinout.c library.h liballoc.h libinout.h global.h
	gcc -c libinout.c $(CFLAGS)

#remove arquivos temporários
clean:
	-rm $(objs) *~

#remove tudo o que não for código fonte
purge: clean
		-rm programa

install: pseudosib
	install -d $(PREFIX)/
	install -m 755 pseudosib $(PREFIX)/


