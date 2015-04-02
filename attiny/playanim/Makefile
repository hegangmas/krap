include make.vars

ISP=avrdude -c usbtiny -p $(MCU)
CFLAGS+=-I.

TARGET=playanim
SRC=playanim.c anim.c

all: $(TARGET).hex

program : $(TARGET).hex
	$(ISP) -e -U flash:w:$(TARGET).hex

getfuse:
	$(ISP) -U hfuse:r:fuse-h.txt:b -U lfuse:r:fuse-l.txt:b \
         -U efuse:r:fuse-e.txt:b

$(TARGET).elf: $(SRC:.c=.o) uWireM/uWireM.a matrix8x8/matrix8x8.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

uWireM/uWireM.a: uWireM/*.[ch]
	make -C uWireM

matrix8x8/matrix8x8.a: matrix8x8/*.[ch]
	make -C matrix8x8

%.hex : %.elf
	avr-objcopy -R .eeprom -O ihex $< $@
	avr-size $@

include make.rules

clean::
	rm -f *.hex *.elf fuse-*.txt $(TARGET)
	make -C uWireM clean
	make -C matrix8x8 clean
