PART=atmega168
SHORTPART=m168
CC=avr-gcc
LD=avr-gcc
CFLAGS:=-mmcu=$(PART) -Og -mcall-prologues
LDFLAGS:= -Wl,-Map,main.map
CRT0O=/usr/local/avr/avr/lib/avr5/crt$(SHORTPART).o

main.hex: main.elf
	avr-objcopy -O ihex main.elf main.hex
main.elf: $(CRT0O) main.o driver.o
	$(CC) $(LDFLAGS) $^ -o $@ -T lds.x
	avr-size $@

%.o: %.c %.lst
	$(CC) -c $(CFLAGS) -o $@ $<
%.o: %.s
	$(CC) -c $(CFLAGS) -x assembler-with-cpp $< -o $@

%.lst: %.c
	$(CC) -S -o $@ $(CFLAGS) -fverbose-asm $<

program: main.hex
	avrdude -c um232h -p m168 -U flash:w:main.hex 

clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.hex
