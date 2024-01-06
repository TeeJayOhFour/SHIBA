#pragma once
#include "Config.h"

// returning 1: animation is finished.
// returning 0: it's being played
// returning -1: doesn't exist or called yet 
// returning a int value if frames is available
int animateTex(int id, int delay = 0, bool frames = false) {

	// if there's no frames then just switch to another texture after a delay.
	// check if there's already a delay. in which case update it.
	if (globalAnimationDelay.count(id) != 0) {

		int value = globalAnimationDelay.at(id) - ANIMATIONSTEP;

		if (value <= 0) {
			globalAnimationDelay.erase(id);
			return 1;
		}
		globalAnimationDelay.insert_or_assign(id, value);
		if (frames) return value;
		
	}
	else if (globalAnimationDelay.count(id) == 0 && delay == 0) {
		return -1;
	} else globalAnimationDelay.insert_or_assign(id, delay);

	return 0;

}