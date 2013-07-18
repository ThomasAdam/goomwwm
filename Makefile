CFLAGS?=-Wall -Os
LDADD?=`pkg-config --cflags --libs x11 xinerama xft`

normal:
	$(CC) -o goomwwm goomwwm.c $(CFLAGS) $(LDADD) $(LDFLAGS)

debug:
	$(CC) -o goomwwm-debug goomwwm.c $(CFLAGS) -g -DDEBUG $(LDADD)

docs:
	pandoc -s -w man goomwwm.md -o goomwwm.1

all: normal debug docs

clean:
	rm -f goomwwm goomwwm-debug
