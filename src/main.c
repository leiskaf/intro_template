#define str(s) #s
#define xstr(s) str(s)

#include <SDL2/SDL.h>

#include "demo.h"

#define CANVAS_WIDTH 1920
#define CANVAS_HEIGHT 1080

__attribute__((__externally_visible__, __section__(".text.startup._start"),
    __noreturn__
))
void _start(void) {
   asm volatile("sub $8, %rsp\n");

#ifndef INLINE_ASSEMBLY    
    SDL_Init(SDL_INIT_VIDEO
#ifdef USE_AUDIO
	    | SDL_INIT_AUDIO
#endif
	    );

    SDL_Window *main_window = SDL_CreateWindow(
	    "",
	    0,
	    0,
	    CANVAS_WIDTH,
	    CANVAS_HEIGHT,
	    SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
	    );
#else
    asm volatile("push $" xstr(SDL_INIT_VIDEO
#ifdef USE_AUDIO       
		| SDL_INIT_AUDIO
#endif	
		) "\n"
	    "pop %%rdi\n"
	    "call *SDL_Init@GOTPCREL(%%rip)"
	    :::"rdi", "rsi");

    SDL_Window *main_window = SDL_CreateWindow(
	    "",
	    0,
	    0,
	    CANVAS_WIDTH,
	    CANVAS_HEIGHT,
	    SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
	    );

#endif
#ifndef INLINE_ASSEMBLY
    SDL_GL_CreateContext(main_window);
    SDL_ShowCursor(0);
    SDL_GL_SetSwapInterval(0);
    on_realize();
#else
    asm volatile("push %[mw]; pop %%rdi\n"
	    "call *SDL_GL_CreateContext@GOTPCREL(%%rip)\n"
	    "push $0; pop %%rdi\n"
	    "call *SDL_ShowCursor@GOTPCREL(%%rip)\n"
	    "push $0; pop %%rdi\n"
	    "call *SDL_GL_SetSwapInterval@GOTPCREL(%%rip)\n"
	    "call on_realize"
	    ::[mw]"r"(main_window)
	    :"rdi");
#endif

    while (1) {
	SDL_Event event;
#ifndef INLINE_ASSEMBLY
	loop_tick();
	SDL_GL_SwapWindow(main_window);
#else
	loop_tick();
	asm volatile("push %[mw]; pop %%rdi\n"
		"call *SDL_GL_SwapWindow@GOTPCREL(%%rip)"
		::[mw]"r"(main_window)
		:"rdi");
#endif

	while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT
		    || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
	       ) {
		asm volatile("push $231; pop %%rax; syscall":::"rax");
		__builtin_unreachable();
	    }

	    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_EXPOSED) {
#ifndef INLINE_ASSEMBLY
		loop_tick();
		SDL_GL_SwapWindow(main_window);
#else
		loop_tick();
		asm volatile("\n"
			"push %[mw]; pop %%rdi\n"
			"call *SDL_GL_SwapWindow@GOTPCREL(%%rip)"
			::[mw]"r"(main_window)
			:"rdi");
#endif
	    }
	}
    }
   __builtin_unreachable();
}

