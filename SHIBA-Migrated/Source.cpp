#include <iostream>
#include <irrKlang.h>

using namespace irrklang;

static int notmainss() {

	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine) {
		return 0;
	}
	engine->play2D("assets/sound/battle.mp3", true);
	while (true) {

	}
	engine->drop();

	std::cout << "Your mom is gay" << std::endl;

	return 0;
}