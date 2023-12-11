#pragma once
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

