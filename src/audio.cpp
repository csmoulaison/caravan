#include "audio.h"

void play_sound(Mix_Chunk* sound) {
	Mix_PlayChannel(-1, sound, 0);
}
