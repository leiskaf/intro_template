CFLAGS := -fno-plt -O2 -std=gnu11 -Wall -Wextra -flto -ffast-math -nostartfiles \
    	-fno-stack-check -fno-unwind-tables -fno-asynchronous-unwind-tables -fomit-frame-pointer \
	-no-pie -fno-pic -fno-PIE -fno-PIC -ffunction-sections -fdata-sections \
	-Wl,--build-id=none -z norelro -Wl,-z,noseparate-code -Wl,--no-eh-frame-hdr \
	-Wl,--no-ld-generated-unwind-info -Wl,--hash-style=sysv -Wl,-z,nodynamic-undefined-weak \
    	-Wl,--gc-sections 

CFLAGS += -m64 -march=core2

DFLAGS := -DCANVAS_WIDTH=$(CANVAS_WIDTH)
DFLAGS += -DCANVAS_HEIGHT=$(CANVAS_HEIGHT)
ifeq ($(INLINE_ASSEMBLY), 1)
DFLAGS += -DINLINE_ASSEMBLY
endif
ifeq ($(USE_AUDIO), 1)
DFLAGS += -DUSE_AUDIO
endif

DFLAGS += -DINLINE_ASSEMBLY_TEST


CFLAGS += $(DFLAGS)
