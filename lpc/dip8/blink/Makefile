TGT = main
CSRC =
ASRC =
ASRC += crt0.S
CSRC += main.c
LDS = ./LPC810/lpc810.ld

OBJS = $(CSRC:.c=.o)
OBJS += $(ASRC:.S=.o)

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
NM=arm-none-eabi-nm
SIZE=arm-none-eabi-size

FLASH = lpc21isp
PORT = /dev/tty.usbserial-AH00RX2S 
BAUD = 115200
KFREQ = 12000

CFLAGS=-O0 -mthumb -mcpu=cortex-m0plus -nostartfiles -Wl,-T$(LDS),-nostdlib,-Map=$(TGT).map

all: $(TGT).hex

%.o: %.S
	$(CC) $(CFLAGS) -c $^ -o $@

$(TGT).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(TGT).hex: $(TGT).elf
	$(OBJCOPY) $< -O ihex $@
	$(OBJDUMP) -D $< > $(TGT).disasm
	$(NM) -n $(TGT).elf > $(TGT).sym
	$(SIZE) $(TGT).elf

clean:
	rm -rf *.elf *.hex *.map *.o *.disasm

flash:
	$(FLASH) $(TGT).hex $(PORT) $(BAUD) $(KFREQ)
