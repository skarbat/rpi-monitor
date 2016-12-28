#
# Makefile to build rpi-monitor tool
#

COMMON=../common/
OBJS=main.o gui.o monitor.o \
     $(COMMON)backgroundLayer.o $(COMMON)/font.o \
     $(COMMON)/key.o $(COMMON)/imageGraphics.o $(COMMON)/imageKey.o \
     $(COMMON)/imageLayer.o $(COMMON)/image.o

BIN=rpi-monitor

CFLAGS+=-Wall -g -O3 -I$(COMMON)
LDFLAGS+=-L/opt/vc/lib/ -lbcm_host

INCLUDES+=-I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux

all: $(BIN)

%.o: %.c
	@rm -f $@ 
	$(CC) $(CFLAGS) $(INCLUDES) -g -c $< -o $@ -Wno-deprecated-declarations

$(BIN): $(OBJS)
	$(CC) -o $@ -Wl,--whole-archive $(OBJS) $(LDFLAGS) -pthread -Wl,--no-whole-archive -rdynamic

clean:
	@rm -f $(OBJS)
	@rm -f $(BIN)
