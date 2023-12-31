#pragma once
#include <math.h>
#include <queue>
#include <chrono>
#include <time.h>
#include <ctime>
#include <functional>
#include <unordered_map>
#include <string>
#include <stdlib.h>
#include <vector>
#include <list>
#include <GL/freeglut.h>
#include <iostream>
#include <irrKlang.h>
#include <iomanip>

#define TITLE "SHIBA Engine v1.7 by TJ | Copyright 2023"
// Stupidly Horrendous Implementation of Basic Animations


#define array_size(arr) (sizeof(arr) / sizeof(*(arr)))
constexpr auto SENSITIVITY_HIGH = 120.0f;
constexpr auto SENSITIVITY_MID = 80.0f;	//Higher is slower.
constexpr auto SENSITIVITY_LOW = 50.0f;
constexpr auto FPS = 60;
constexpr auto PLAYER_DAMAGE = 100;
constexpr auto TILESIZE = 5.0f;	//should be 5
constexpr auto WALLSIZE = 20.0f;	//should be 20
constexpr auto DEBUGMODE = TRUE;
constexpr auto GROUNDLEVEL = -5.0f;
constexpr auto TO_RADIANS = 3.141592 / 180.0;
constexpr auto ANIMATIONSTEP = 10.0f;		//Higher value is slower/smoother;
constexpr auto BOUNDARY = 69;
constexpr auto FLOOR = 70;
constexpr auto TRAVEL = 300;
constexpr auto WIDTH = 16*80;
constexpr auto HEIGHT = 9*80;




static float mouseSpeed = SENSITIVITY_MID;

static int lastKey = 0;
static bool track = false;
static bool wireframe = false;
static bool collision = true;
static bool light = true;
static int lastFPS = 0;
static int movemenSpeed = 2.0; //Higher is slower.

static int pitchLimit = 60;
static auto startTime = std::chrono::high_resolution_clock::now();
static int currentScene = -1;
static float DevHudY = 0.0f;		//WHY IS THIS STILL HERE?! REMOVE IT FFS IT SERVES NO PURPOSE


static float MAX_VOLUME = 0.3f; //0.3f or 30% by default
static irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();

// Sound files
static irrklang::ISoundSource* BATTLE_MUSIC_0 = soundEngine->addSoundSourceFromFile("assets/sound/battle_0.mp3");
static irrklang::ISoundSource* BATTLE_MUSIC_1 = soundEngine->addSoundSourceFromFile("assets/sound/battle_1.mp3");
static irrklang::ISoundSource* GLASS_CRACK = soundEngine->addSoundSourceFromFile("assets/sound/glass-crack.mp3");


// for FPS calculation
static int initTime = time(NULL), finalTime, frameCount = 0;

static void toggleOverlayMode(bool toggle) {
	

	if (toggle) {
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
	}
	else {
		// Disable blending after rendering

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);

	}

}

static void toggleTransparency(bool toggle) {


	if (toggle) {

		// Enable blending for transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

	}
	else {
		// Disable blending after rendering
		glDisable(GL_BLEND);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

}