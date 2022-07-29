#
# Machine makefile
#

DEFINES		+= -D__ARM32_ARCH__=7 -D__CORTEX_A7__ -D__ARM32_NEON__

ASFLAGS		:= -g -ggdb -Wall -O3
CFLAGS		:= -g -ggdb -Wall -O3
LDFLAGS		:= -T arch/$(ARCH)/$(MACH)/xboot.ld -nostdlib
MCFLAGS		:= -march=armv7-a -mtune=cortex-a7 -mfpu=vfpv3-d16 -mfloat-abi=hard -marm -mno-thumb-interwork -mno-unaligned-access

LIBDIRS		:=
LIBS 		:=
INCDIRS		:=
SRCDIRS		:=

ifeq ($(strip $(HOSTOS)), linux)
BOOT_MERGER	:= arch/$(ARCH)/$(MACH)/tools/linux/boot_merger
endif
ifeq ($(strip $(HOSTOS)), windows)
BOOT_MERGER	:= arch/$(ARCH)/$(MACH)/tools/windows/boot_merger
endif
INIFILE		:= arch/$(ARCH)/$(MACH)/tools/images/rv1106.ini

xend:
	@echo Packing rockchip binrary for irom booting
	@$(BOOT_MERGER) $(INIFILE)
