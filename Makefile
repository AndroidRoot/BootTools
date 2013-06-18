AUTOTARGETS=hdrboot bootunpack
TARGETS=$(AUTOTARGETS) mkbootimg
DEPS = Makefile shared/bootimg.h
COMMON=
PREFIX ?= usr/local


CC=gcc
CFLAGS=-I. -Ishared -Isrc -Wall
LDFLAGS=

OBJS	= $(COMMON) $(addsuffix .o, $(TARGETS))

all: $(TARGETS)

$(AUTOTARGETS): %: %.o $(COMMON) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $< $(COMMON) $(LDFLAGS)

sha.o: src/mincrypt/sha.c src/mincrypt/sha.h
	$(CC) $(CFLAGS) -c -o sha.o src/mincrypt/sha.c

mkbootimg: mkbootimg.o sha.o $(COMMON) $(DEPS)
	$(CC) $(CFLAGS) -o mkbootimg mkbootimg.o sha.o $(COMMON) $(LDFLAGS)

install: $(TARGETS)
	install -d -m 0755 $(DESTDIR)/$(PREFIX)/bin
	install -D -m 0755 $(TARGETS) $(DESTDIR)/$(PREFIX)/bin

$(OBJS): %.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(TARGETS) *.o
