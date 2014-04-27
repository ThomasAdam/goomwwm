CFLAGS?=-Wall -Os -D_GNU_SOURCE -Wno-parentheses -ggdb
LDADD?= $(shell pkg-config --cflags --libs x11 xinerama xft) -I. -iquote.

SRC= $(shell echo *.c)
SRC_OBJ= $(patsubst %.c,%.o,$(SRC))

.c.o:
	$(CC) $(CFLAGS) $(LDADD) -c -o $@ $<

normal: $(SRC_OBJ)
	$(CC) $(LDFLAGS) $(LDADD) $(CFLAGS) -o goomwwm $+
#$(SRC) $(CFLAGS) $(LDADD) $(LDFLAGS)

debug: $(SRC)
	$(CC) -o goomwwm-debug $(SRC_OBJ) $(CFLAGS) -g -DDEBUG $(LDADD)

all: normal debug

clean:
	rm -f goomwwm goomwwm-debug *.o
