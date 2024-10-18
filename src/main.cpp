#include <iostream>
#include <time.h>
#include "game.h"
#include "audio.h"
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#	include <emscripten.h>
#endif

/*
TODO
- New supply model (just tweaks now. food replenishment bias)
- Travelling merchant
- Scribe + table view
- Post game jam ugliness refactor: 
  * Implement nesting maximum in general
  * Make state machines only call one function
  * Reduce hardcoded data
*/

struct Context {
	DrawContext draw_context;
	Input input;
	Assets assets;
	Game game;
};

void loop(void* arg) {
	Context* context = (Context*)arg;

    input_poll(context->input, context->draw_context.pixel_scalar);
    if(context->input.quit.held) {
        context->input.quit_application = true;
    }
    draw_clear_screen(context->draw_context);
    update_game(context->game, context->input, context->assets, context->draw_context);
    draw_present(context->draw_context);
}

int main(int argc, char** argv) {
	printf("Hedooon\n");
	std::cout << "Hedooo!" << std::endl;

	// Initialize
	Context* context = new Context;

    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024);

    init_draw_context(context->draw_context);
    init_input(context->input);
    init_assets(context->assets);
    init_game(context->game, context->assets);

	#ifdef __EMSCRIPTEN__ 
	emscripten_set_main_loop_arg(loop, context, 0, 1);
	#else
	while(!context->input.quit_application) {
		loop(context);
	}
	#endif

	return 0;
}
