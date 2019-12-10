CC = xtensa-lx106-elf-gcc
AR = xtensa-lx106-elf-ar

SDK = /Volumes/esp-open-sdk/esp-open-sdk/sdk

CFLAGS = -Iinclude -I$(SDK)/include -Os -g -O2 -Wpointer-arith -Wundef -Werror -Wl,-EL \
		 -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals   \
		 -D__ets__ -DICACHE_FLASH

LINKER_SCRIPT = $(SDK)/ld/eagle.app.v6.ld


LDFLAGS = -T$(LINKER_SCRIPT) -L$(SDK)/lib -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static
LIBS = -Wl,--start-group -lc -lgcc -lhal -lpp -lphy -lnet80211 -llwip -lwpa -lmain
LIBS_END = -Wl,--end-group

ESPTOOL		?= esptool.py


##### Build Targets

firmware: blinky
	mkdir -p $@
	$(ESPTOOL) elf2image -o $@/ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

src/blinky.a: src/blinky.o
	$(AR) cru $@ $<

blinky: src/blinky.o
	$(CC) $(LDFLAGS) $(LIBS) $< $(LIBS_END) -o $@

# - Flashing

FW_FILE_1_ADDR	= 0x00000
FW_FILE_2_ADDR	= 0x10000

FW_FILE_1	:= firmware/$(FW_FILE_1_ADDR).bin
FW_FILE_2	:= firmware/$(FW_FILE_2_ADDR).bin

.PHONY: flash
flash:
	$(ESPTOOL) --port $(ESPPORT) write_flash $(FW_FILE_1_ADDR) $(FW_FILE_1) $(FW_FILE_2_ADDR) $(FW_FILE_2)

clean:
	rm -f src/*.o src/*.a blinky
