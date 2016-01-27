#############################################################################
#
prefix := /usr/local

# Detect the Raspberry Pi by the existence of the bcm_host.h file
BCMLOC=/opt/vc/include/bcm_host.h

ARCH=armv6zk
ifeq "$(shell uname -m)" "armv7l"
ARCH=armv7-a
endif

ifneq ("$(wildcard $(BCMLOC))","")
# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=$(ARCH) -mtune=arm1176jzf-s
endif

# define all programs
#PROGRAMS = scanner pingtest gettingstarted
#PROGRAMS = gettingstarted gettingstarted_call_response transfer pingpair_dyn
PROGRAMS = receive
SOURCES = ${PROGRAMS:=.cpp}

all: ${PROGRAMS}

${PROGRAMS}: ${SOURCES}
	g++ ${CCFLAGS} -Wall -I../ -lrf24-bcm $@.cpp -o $@

clean:
	rm -rf $(PROGRAMS)

install: all
	test -d $(prefix) || mkdir $(prefix)
	test -d $(prefix)/bin || mkdir $(prefix)/bin
	for prog in $(PROGRAMS); do \
	  install -m 0755 $$prog $(prefix)/bin; \
	done

.PHONY: install
