#pragma once
#include <math.h>
#include <queue>
#include <chrono>
#include <time.h>
#include <functional>
#include <unordered_map>
#include <string>
#include <stdlib.h>
#include <vector>
#include <GL/freeglut.h>
#include <iostream>

#define SENSITIVITY 50.0f	//Higher is slower.
#define FPS 60
#define TILESIZE 5.0f	//should be 5
#define WALLSIZE 20.0f	//should be 20
#define DEBUGMODE TRUE
#define GROUNDLEVEL -5.0f
#define TO_RADIANS 3.141592/180.0
#define array_size(arr) (sizeof(arr) / sizeof(*(arr)))
#define ANIMATIONSTEP 10.0f		//Higher value is slower/smoother
#define BOUNDARY 69
#define FLOOR 70
#define TRAVEL 300
#define WIDTH 16*80
#define HEIGHT 9*80

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