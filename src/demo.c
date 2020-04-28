#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <SDL2/SDL_timer.h>

#include "effect.h"
#ifdef USE_AUDIO
#include <SDL2/SDL_audio.h>
#include "synth.h"
#endif

#define str(s) #s
#define xstr(s) str(s)

#define SAMPLE_RATE 44100
#define FORMAT AUDIO_F32LSB
#define CHANNELS 2
#define SAMPLES 1024
#define BUFFER_SIZE 1024
#define DATA_SIZE CHANNELS * BUFFER_SIZE
#define DATA_SIZE_BYTES DATA_SIZE * 4
#define AUDIO_END SAMPLE_RATE * 2

unsigned int prgm;
unsigned int loc;
float T;

#ifdef USE_AUDIO
SDL_AudioSpec want;
unsigned int dev;
unsigned int ssbo;
float data[DATA_SIZE];


void audio_init() {
/*  
    Works without memset but unsafe

    SDL_memset(&want, 0, sizeof(want));
*/
    want.freq = SAMPLE_RATE;
    want.format = FORMAT;
/*  
    Don't need to set these as default values are sane

    want.channels = CHANNELS;
    want.samples = SAMPLES;
    want.callback = NULL;
*/  
    dev = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
}
#endif

void shader_init(void *s, unsigned char v) {
    prgm = glCreateShaderProgramv(
#ifdef USE_AUDIO
	    v == 0 ? GL_COMPUTE_SHADER : 
#endif
	    GL_FRAGMENT_SHADER, 1, s);
    
    glUseProgram(prgm);
    loc = glGetUniformLocation(prgm, "T");
#ifdef USE_AUDIO
    if(v == 0) {
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, DATA_SIZE_BYTES, data, GL_DYNAMIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    }
#endif
}

void on_render(float u, unsigned char v) {
#ifndef INLINE_ASSEMBLY//_TEST
    glUniform1f(loc, u);
#ifdef USE_AUDIO
    if(v == 0) {
	glDispatchCompute(BUFFER_SIZE, 1, 1);
	glGetNamedBufferSubData(ssbo, 0, DATA_SIZE_BYTES, data);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	SDL_QueueAudio(dev, data, DATA_SIZE_BYTES);
    }
    else
#endif // USE_AUDIO
    	glRecti(-1, -1, 1, 1);
#else  // INLINE_ASSEMBLY

	asm volatile("mov %[loc], %%edi; mov %[u], %%esi\n"
		//"pop %%rdi; pop %%rsi\n"
		"call *glUniform1f@GOTPCREL(%%rip)\n"
		::[u]"r"(u),
		[loc]"r"(loc)
		:"rax", "rdi", "rsi", "rdx");

#ifdef USE_AUDIO
	//asm volatile("cmp $0x00, %%al; jne end_on_render"::[v]"r"(v):);
	if(v == 0) {
	asm volatile("push $1; push $1; push $" xstr(BUFFER_SIZE)"\n"
		"pop %%rdi; pop %%rsi; pop %%rdx\n"
		"call *glDispatchCompute@GOTPCREL(%%rip)\n"
		:::);

	asm volatile("mov %[ssbo], %%edi;"
		"push %[data]; push $" xstr(DATA_SIZE_BYTES)"; push $0;"
		"pop %%rsi; pop %%rdx; pop %%rcx\n"
		"call *glGetNamedBufferSubData@GOTPCREL(%%rip)"
		::[ssbo]"r"(ssbo),
		[data]"r"(data)
		:);
		
	//asm volatile("mov $" xstr(GL_ALL_BARRIER_BITS)", %%edi; call *glMemoryBarrier@GOTPCREL(%%rip)":::);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	asm volatile("mov %[dev], %%edi;"
		"push $" xstr(DATA_SIZE_BYTES)"; push %[data];"
		"pop %%rsi; pop %%rdx;"
		"sub $8, %%rsp;"
		"call *SDL_QueueAudio@GOTPCREL(%%rip);"
		"sub $8, %%rsp"
		::[dev]"r"(dev),
		[data]"r"(data)
		:);
	
	//SDL_QueueAudio(dev, data, DATA_SIZE_BYTES);
	//asm volatile("end_on_render:");
	} else
#endif //USE_AUDIO
	    asm volatile("push $1; push $1; push $-1; push $-1\n"
		    "pop %%rdi; pop %%rsi; pop %%rdx; pop %%rcx\n"
		    "call *glRecti@GOTPCREL(%%rip)\n"
		    :::);

#endif //INLINE_ASSEMBLY
}

void on_realize() {
#ifdef USE_AUDIO
    shader_init(&synth_glsl, 0);
    audio_init();
    while(T < AUDIO_END) {
	on_render(T, 0);
	T += BUFFER_SIZE;
    }
    SDL_PauseAudioDevice(dev, 0);
#endif

    shader_init(&effect_glsl, 1);
    T = SDL_GetTicks(); 
}

void loop_tick() {
    on_render((SDL_GetTicks() - T) / 1000.0, 1);
}
