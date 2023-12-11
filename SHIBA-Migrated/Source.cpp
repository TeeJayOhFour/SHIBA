#include <iostream>
#include <irrKlang.h>


using namespace irrklang;

int main(int argc, char** argv) {

	
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine) {
		return 0;
	}
	engine->play2D("media/getout.ogg", true);
	while (true) {

	}
	engine->drop();

	std::cout << "Your mom is gay" << std::endl;

	return 0;
}