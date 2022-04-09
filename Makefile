CC_C = arm-linux-gnueabihf-gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -pthread
CFILES := $(wildcard ./src/*/**.c)
GUEST_MAIN := $(wildcard ./src/alarm_guest.c)
HOST_MAIN := $(wildcard ./src/alarm_host.c)
GUEST_TARGET_NAME = guest_alarm
HOST_TARGET_NAME = host_alarm
TARGETS = $(GUEST_TARGET_NAME) $(HOST_TARGET_NAME)

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

all: $(TARGETS)

$(GUEST_TARGET_NAME): $(GUEST_MAIN) $(CFILES)
	$(CC_C) $(CFLAGS) $(GUEST_MAIN) $(CFILES) -o $(GUEST_TARGET_NAME) $(LFLAGS) -lpthread -lasound
	cp $(GUEST_TARGET_NAME) $(HOME)/cmpt433/public/myApps/
	cp ./src/alarm/alarm_sound.wav $(HOME)/cmpt433/public/myApps/

$(HOST_TARGET_NAME): $(HOST_MAIN) $(CFILES)
	$(CC_C) $(CFLAGS) $(HOST_MAIN) $(CFILES) -o $(HOST_TARGET_NAME) $(LFLAGS) -lpthread -lasound
	cp $(HOST_TARGET_NAME) $(HOME)/cmpt433/public/myApps/
	cp ./src/alarm/alarm_sound.wav $(HOME)/cmpt433/public/myApps/

clean:
	rm -f *.o *.obj $(ALL)
