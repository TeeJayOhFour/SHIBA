#pragma once

#include "Config.h"

static int currentNoteCount = 0;
static int maxNotes = 1;

// use this method to collect notes
static void collectNote() {

	if (currentNoteCount == maxNotes) return;

	// add code here for hud to display message about note.

	currentNoteCount++;

}


static void displayNote(int id) {

	// notes can be accessed from the menu.
	// menu will have some buttons added for each note.
	// call this function to display them in a window or something

	switch (currentNoteCount) {
		case 1:  break;

	}


}