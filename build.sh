SOURCE="src/*.cpp"

ASSET_PATH="build/assets/@assets"
SOUND_PATH="build/assets/sounds/@assets/sounds"

if [ "$1" = "em" ]; then
	echo "Building program (emscripten)"
	OUT="build/game.html"
	emcc -o $OUT $SOURCE -sUSE_SDL=2 -sUSE_SDL_MIXER=2 --embed-file=$ASSET_PATH --embed-file=$SOUND_PATH --emrun -D __EMSCRIPTEN__
	if [ $? -eq 0 ]; then
		echo "Build succeeded"
	else
		echo "Build failed"
	fi
else
	echo "Building program (linux)"
	OUT="build/caravan"
	g++ -o $OUT $SOURCE `sdl2-config --cflags --libs` -lSDL2_mixer -g
	if [ $? -eq 0 ]; then
		echo "Build succeeded"
	else
		echo "Build failed"
	fi
fi
