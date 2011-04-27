AUTOTARGETS=hdrboot bootunpack
TARGETS=$(AUTOTARGETS) mkbootimg
DEPS = Makefile shared/bootimg.h
COMMON=

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

$(OBJS): %.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(TARGETS) *.o
