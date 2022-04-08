CC_C = arm-linux-gnueabihf-gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -pthread
CFILES := $(wildcard ./src/*/**.c)
GUEST_MAIN := $(wildcard ./src/alarm_guest.c)
HOST_MAIN := $(wildcard ./src/alarm_host.c)

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

all: alarm_guest

alarm_guest: $(GUEST_MAIN) $(CFILES)
	$(CC_C) $(CFLAGS) $(GUEST_MAIN) $(CFILES) -o alarm_guest $(LFLAGS) -lpthread -lasound
	cp alarm_guest $(HOME)/cmpt433/public/myApps/

clean:
	rm -f *.o *.obj $(ALL)
