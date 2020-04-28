include config.mk
include cflags.mk

default: all

OBJECTS := $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
HEADERS := $(wildcard include/*.h)
HEADERS += $(wildcard shader/minified/*.h)


shader/minified/effect.h: shader/effect.glsl
	mono ext/shader_minifier.exe --preserve-externals $< -o $@
	sed -i 's/u_time/T/g' $@
		
shader/minified/synth.h: shader/synth.glsl
	mono ext/shader_minifier.exe --preserve-externals $< -o $@
	sed -i 's/ int compute_local_size//g' $@
	
obj/%.o: src/%.c $(HEADERS)
	$(CC) -c $< -o $@ -I include -I shader/minified $(DFLAGS)


bin/main: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@ $(CFLAGS)

obj/%.inc.o: $(OBJECTS)
	$(CC) -r -o "$@" $^ $(CFLAGS) -Wl,--entry -Wl,_start 

obj/%.smol.syms.asm: obj/%.inc.o
	$(PYTHON3) ext/smol/src/smol.py $(LIBS) $^ "$@"

obj/%.smol.stub.o: obj/%.smol.syms.asm
	$(NASM) -f elf64 -DUSE_INTERP -DUSE_DNLOAD_LOADER -I ext/smol/rt/ -o "$@" "$<"

bin/%.smol: obj/%.smol.stub.o obj/%.inc.o
	$(LD) -m elf_x86_64 --oformat=binary -T ext/smol/ld/link.ld -o "$@" $^
	chmod +x "$@"

bin/%.smol.vndh: bin/%.smol
	ext/autovndh.py -g -l -v --vndh ext/vondehi "$<" > "$@"
	chmod +x "$@"

all: shader/minified/effect.h
all: shader/minified/synth.h
all: bin/main
all: bin/main.smol.vndh

.PRECIOUS: bin/main $(OBJECTS) bin/main.smol obj/main.inc.o obj/main.smol.syms.asm obj/main.smol.stub.o 

clean:
	rm -vf bin/* obj/* shader/minified/*
