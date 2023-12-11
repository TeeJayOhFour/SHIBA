#pragma once
#pragma warning(disable:4996)
#ifndef TJSENGINE_H
#define TJSENGINE_H
#define TITLE "SHIBA Engine v1.0 by TJ | Copyright 2023"
// Stupidly Horrendous Implementation of Basic Animations


//TODO for 10th dec:
// Sorted from easy to hard.
// HUD with damage
	// start 830
	// finish 900
// menu
	// start 900
	// end 1000
// enemy spawners
// sound engine
#include <math.h>
#include <iostream>
#include <filesystem>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <queue>
#include <math.h>
#include <chrono>
#include <time.h>
#include <functional>
#include <unordered_map>
#include <string>

float DevHudY = 0.0f;

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

// for FPS calculation
int initTime = time(NULL), finalTime, frameCount = 0;

// Configuration
//-1 = Empty
// 0 = Spawn location.
// 1 = Enemies
// 2 = Objectives (Eggs)
// 3 = Notes
// 4 = Walls/Barriers (you cannot pass through)
// 5 = Doors
// 6 = Bosses
// 7 = Custom 3D models

struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char* data;
};

// This is for the checkered pattern. That's all.
#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageWidth][checkImageHeight][3];

enum tileID {
	Empty = 0,
	SpawnLoc = -1,
	EnemySpawner = 1,
	Objective = 2,
	Notes = 3,
	Wall = 4,
	DoorClosed = 5,
	DoorOpen = -5,
	Boss = 6,
	Custom = 7,
	Bullet = 10
};

struct Position {

	int frontObject = -1;
	float x = 0;
	float y = 0;
	float z = 0;
	float yaw = 0;
	float pitch = 0;
	// used for checking if a specific button is pressed.
	int button = -1;
	int buttonState = -1;

	bool operator == (const Position& a) {
		if (x == a.x && y == a.y &&
			z == a.z && yaw == a.yaw &&
			pitch == a.pitch) return true;
		else return false;
	}

	Position& operator -= (const Position& a) {

		this->x -= a.x;
		this->y -= a.y;
		this->z -= a.z;
		this->yaw -= a.yaw;
		this->pitch -= a.pitch;
		return *this;
	}

	Position& operator += (const Position& a) {

		x += a.x;
		y += a.y;
		z += a.z;
		yaw += a.yaw;
		pitch += a.pitch;
		return *this;
	}

	bool operator <= (const float& a) {

		if (x <= a &&
			y <= a &&
			z <= a &&
			yaw <= a &&
			pitch <= a) return true;
		return false;
	}

	Position& operator / (const float& a) {

		x /= a;
		y /= a;
		z /= a;
		yaw /= a;
		pitch /= a;

		return *this;
	}

	Position& operator - (const float& a) {

		x -= a;
		y -= a;
		z -= a;
		yaw -= a;
		pitch -= a;

		return *this;
	}
	Position& operator += (const float& a) {

		x += a;
		y += a;
		z += a;
		yaw += a;
		pitch += a;

		return *this;
	}

	Position& absolute() {

		x = abs(x);
		y = abs(y);
		z = abs(z);
		yaw = abs(yaw);
		pitch = abs(pitch);

		return *this;

	}

	Position toTile() {
		Position temp;

		temp.x = round(x / 10);
		temp.z = round(z / 10);
		temp.y = round(y / 10);

		return temp;

	}



};

struct ShibaQuad {
	float x;
	float y;
	float z;
	float normal[3];
};

struct Motion {
	bool Forward, Backward, Left, Right;
	float speedUp, speedBack, speedLeft, speedRight;
} motion = { false, false, false, false, 2.0f, 2.0f, 2.0f, 2.0f };

struct Poles {
	bool north, south, east, west;
} facing = { false, false, false, false };

void getColorMod(int id);	//This function is needed by ShibaObject.

// Custom Object Class. All vertexes must be drawn anti-clockwise
class ShibaObject {

public:
	// Tile coordinates
	std::string objectName;
	Position offset;
	bool texture = false;
	std::vector<ShibaQuad> vertexCol;
	std::vector<ShibaQuad> texturePoints;
	int health = 100;

	using loadFunction = void (*)(ShibaObject);

	int color = 0, tileX = 0, tileZ = 0;

	// Using offset the object can be modified from the origin.
	ShibaObject(int x, int y, int z) {

		tileX = x / 10;
		tileZ = z / 10;

		offset.x = 0;
		offset.y = 0;
		offset.z = 0;
		offset.pitch = 0;
		offset.yaw = 0;
	}

	void setLoadGlutFunction(loadFunction func) {
		glutSolids = func;
	}

	void loadGlutSolids() {
		if (glutSolids != nullptr) {
			glutSolids(*this);
		}
		else {
			// std::cout << "This 3D object doesn't have any custom Glut Solids or it wasn't assigned." << std::endl;
		}
	}

	void load() {
		// texture code goes here

		glBegin(GL_QUADS);

		if (!texture) getColorMod(this->color);
		else glColor3f(1, 1, 1);
		int vertexCount = 0;

		// loop through each vertex collection.
		if (!vertexCol.empty())
			for (int v = 0; v < this->vertexCol.size(); v++) {

				if (vertexCount == 4) vertexCount = 0;

				glTexCoord2f(texturePoints.at(vertexCount).x, texturePoints.at(vertexCount).y);
				vertexCount++;

				// Setting texture pinning points

				// setting normals
				glNormal3f(
					this->vertexCol.at(v).normal[0], 	//X
					this->vertexCol.at(v).normal[1], 	//Y
					this->vertexCol.at(v).normal[2]);	//Z

				// setting vertex position
				glVertex3f(
					this->vertexCol.at(v).x + this->offset.x,
					this->vertexCol.at(v).y + this->offset.y,
					this->vertexCol.at(v).z + this->offset.z
				);
			}
		glEnd();

	}

	// allowing to overwrite current object with another
	ShibaObject& operator= (const ShibaObject& a) {
		//! TODO Finish this or code won't compile.
		this->color = a.color;
		this->glutSolids = a.glutSolids;
		this->objectName = a.objectName;
		this->offset = a.offset;
		this->texture = a.texture;
		this->texturePoints = a.texturePoints;
		this->tileX = a.tileX;
		this->tileZ = a.tileZ;
		this->vertexCol = a.vertexCol;

		return *this;
	}

private:
	loadFunction glutSolids = nullptr;

};

// Level class included for easier level creation.
class Level {
public:
	std::string name;
	int(*levelGrid)[20];
	int objectives = 0;
	int enemies = 0;
	std::vector <ShibaObject> customObjects;


	Level(std::string levelName, int map[][20]) {
		this->name = levelName;
		levelGrid = map;
	}

	void printLevel() {
		int  i, j;
		for (i = 0; i < 20; i++)
		{
			for (j = 0; j < 20; j++)
				std::cout << " " << levelGrid[i][j];
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}


};

class Entity {

public:

	int health = 100;
	Position center;
	std::vector<ShibaQuad> texturePoints;

	Entity(Position& a) {
		center = a;
	}


};


// Globals
int pitchLimit = 60;
auto startTime = std::chrono::high_resolution_clock::now();
int currentScene = -1;
struct Position crosshair;
std::queue <Level> levelQueue;
std::queue <std::string> debugInfoQueue;
Position cameraPosition;	//Actual camera cords in 3D space.
Entity player(cameraPosition);

Position levelBounds;
std::vector <Position> possibleSpawns;
std::vector <ShibaObject> objectCollection;
std::vector <int> enemySpawnerLocations;
// std::vector <ShibaObject> bulletCollection;

// stores the ID of the object along with target postion value.
std::unordered_map <int, Position> animationQueue;
std::unordered_map <int, GLuint> textureCollection;
std::unordered_map <int, Position> bulletAnimation;
std::unordered_map <std::string, ShibaObject> bulletCollection;


int lastKey = 0;
bool track = false;
bool wireframe = false;
bool collision = true;
bool light = true;
int lastFPS = 0;
int movemenSpeed = 2.0; //Higher is slower.

// Prototypes
void menu();
void draw();
void shoot();
void camera();
void options();
void lighting();
void devScreen();
void handleMainMenuInteraction(std::string option);
void updateHUD();
void spawnPlayer();
void renderScene();
void idleLoop(int);
void resetCamera();
void bulletPhysics();
void renderWorldBox();
void renderGameElements();
void playQueuedAnimations();
void addSpawns(int x, int z);
int randomInt(int low, int high);
void interactWithObj(int objectID);
void initLevels(std::queue <Level> queue);
void queueAnimation(int id, int x, int z);
void listenForMouseMovement(int x, int y);
void checkForInteraction(Position& entity);
void makeWall(int x, int z, int x2, int z2);
void maintainAspectRatio(int width, int height);
void listenForSpecialKeys(int key, int x, int y);
bool collisionCheck(int direction, float x, float z);
void listenForNormalKeys(unsigned char key, int x, int y);
void listenForMouseClick(int button, int state, int x, int y);
void listenForNormalKeysRelease(unsigned char key, int x, int y);
void renderText(float x, float y, int r, int g, int b, const char* string);

GLuint texture;
// GLuint *texture;

void makeCheckImage(void) {
	int i, j, c;
	for (i = 0; i < checkImageWidth; i++) {
		for (j = 0; j < checkImageHeight; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
		}
	}
}

int ImageLoad(const char* filename, Image* image) {
	FILE* file;
	unsigned long size; // size of the image in bytes.
	unsigned long i; // standard counter.
	unsigned short int planes; // number of planes in image (must be 1)
	unsigned short int bpp; // number of bits per pixel (must be 24)
	char temp; // temporary color storage for bgr-rgb conversion.

	// make sure the file is there.
	if ((file = fopen(filename, "rb")) == NULL) {
		printf("File Not Found : %s\n", filename);
		return 0;
	}

	// seek through the bmp header, up to the width/height:
	fseek(file, 18, SEEK_CUR);

	// read the width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}

	// read the height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}

	// calculate the size (assuming 24 bits or 3 bytes per pixel).
	size = image->sizeX * image->sizeY * 3;

	// read the planes
	if ((fread(&planes, 2, 1, file)) != 1) {
		printf("Error reading planes from %s.\n", filename);
		return 0;
	}
	if (planes != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planes);
		return 0;
	}
	// read the bitsperpixel
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s.\n", filename);
		return 0;
	}
	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}
	// seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);
	// read the data.
	image->data = (char*)malloc(size);
	if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}
	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}
	for (i = 0; i < size; i += 3) { // reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}
	// we're done.
	return 1;
}

Image* loadTexture(const char* filename) {
	Image* result;
	// allocate space for texture
	result = (Image*)malloc(sizeof(Image));
	if (result == NULL) {
		std::cout << "Error allocating space for image" << std::endl;
		exit(0);
	}

	if (!ImageLoad(filename, result)) {
		exit(1);
	}

	// final check after image has been loaded.
	if (result == NULL) {
		std::cout << "Image was not returned from loadTexture. File: " << filename << std::endl;
		exit(0);
	}

	return result;
}

void parseTexture(Image* img, int id) {

	// Create Texture
	glGenTextures(1, &textureCollection.at(id));
	glBindTexture(GL_TEXTURE_2D, textureCollection.at(id));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}

void initTextures() {

	// Loading essential game textures manually.

	// Get the directory of the current source file

	// Adding ID in collection. 
	//The 1 is temporary as it will get overwritten later by the parser
	textureCollection.insert_or_assign(BOUNDARY, 1);
	// Getting image data.
	Image* image1 = loadTexture("assets/textures/wall.bmp");
	// Parsing into texture for OpenGL
	parseTexture(image1, BOUNDARY);

	textureCollection.insert_or_assign(Empty, 1);
	Image* image2 = loadTexture("assets/textures/floor.bmp");
	parseTexture(image2, Empty);

	textureCollection.insert_or_assign(Wall, 1);
	Image* image3 = loadTexture("assets/textures/tileWall.bmp");
	parseTexture(image3, Wall);

	textureCollection.insert_or_assign(DoorClosed, 1);
	Image* image4 = loadTexture("assets/textures/door.bmp");
	parseTexture(image4, DoorClosed);

	//! No need to make checkered patter lol
	// Checkered pattern for last texture
	// makeCheckImage();	//making this for missing textures just in case.
	// glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// glBindTexture(GL_TEXTURE_2D, texture[7]);
	// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	// glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth, checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &checkImage[0][0][0]);
}

void initGLFlags() {

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_COLOR_MATERIAL);

	initTextures();

}

void updateHUD() {

	// So lights don't affect text
	glDisable(GL_LIGHTING);
	// Blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	std::string text = "Press E to interact";
	int x = glutGet(GLUT_WINDOW_WIDTH) / 2 - (text.length() / 2) * 6 - 30, y = glutGet(GLUT_WINDOW_HEIGHT) / 2 - 200;
	int idInFront;

	if (cameraPosition.frontObject == -1) {
		idInFront = -1;
	}
	else idInFront = objectCollection.at(cameraPosition.frontObject).color;

	// HUD elements go here
	if (idInFront == DoorClosed || idInFront == DoorOpen) {
		glColor3f(1, 1, 0);
		glRasterPos2f(x, y);

		const unsigned char* convertedStr = reinterpret_cast <const unsigned char*> (text.c_str());
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, convertedStr);

		glColor4f(0, 0, 0, 0.45f);
		glBegin(GL_QUADS);

		glNormal3f(0, 0, 0);
		glVertex2f(x - 5, y - 5);
		glVertex2f(x + (float)(text.length() * 7) + 22, y - 5);
		glVertex2f(x + (float)(text.length() * 7) + 22, y + 20);
		glVertex2f(x - 5, y + 20);

		glEnd();

	}

	// Player health
	std::string hp = "Health: " + std::to_string(player.health);

	if (player.health >= 70) glColor3f(0, 1, 0);
	else if (player.health >= 30) glColor3f(1, 1, 0);
	else glColor3f(1, 0, 0);


	glRasterPos2f(25.0f, glutGet(GLUT_WINDOW_HEIGHT) - 50);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, reinterpret_cast <const unsigned char*> (hp.c_str()));

	// Disable blending after rendering
	glDisable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// Re-enabling.
	glEnable(GL_LIGHTING);


}

void devScreen() {

	glDisable(GL_LIGHTING);

	std::vector <std::string> debugHUDInfo;
	float x = 5.0, y = 5.0;

	//? Add a time elasped in the game for the hud
	auto endTime = std::chrono::high_resolution_clock::now();
	double elaspedTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

	int sec = elaspedTime / 1000, min = sec / 60, hr = min / 60;
	std::string timestamp = (std::to_string(hr) + " hrs " + std::to_string(min - hr * 60) + " mins " + std::to_string(sec - min * 60) + " secs");

	int finalX = (array_size(*levelQueue.front().levelGrid) - 1) - cameraPosition.toTile().x;
	int finalZ = cameraPosition.toTile().z;

	int tileValue;

	if (
		(finalX < 0 || finalX >(array_size(*levelQueue.front().levelGrid) - 1))
		||
		(finalZ < 0 || finalZ >(array_size(*levelQueue.front().levelGrid) - 1))
		) tileValue = Wall;
	else tileValue = levelQueue.front().levelGrid[finalX][finalZ];

	std::string cameraFace;

	if (facing.north) cameraFace = "North";
	if (facing.south) cameraFace = "South";
	if (facing.east) cameraFace = "East";
	if (facing.west) cameraFace = "West";

	std::string frontObject;

	if (cameraPosition.frontObject == -1) {
		frontObject = "None";
	}
	else frontObject = std::to_string(objectCollection.at(cameraPosition.frontObject).color);



	// DevHUD Data
	debugHUDInfo.push_back("Mouse tracking: " + std::to_string(track));
	debugHUDInfo.push_back("Collision: " + std::to_string(collision));
	debugHUDInfo.push_back("Keyboard Key: " + std::to_string(lastKey));
	debugHUDInfo.push_back("Boundary X: " + std::to_string(levelBounds.x) + " Z: " + std::to_string(levelBounds.z));
	debugHUDInfo.push_back("X: " + std::to_string(cameraPosition.x) + " Y: " + std::to_string(cameraPosition.y) + " Z: " + std::to_string(cameraPosition.z));
	debugHUDInfo.push_back("TileX: " + std::to_string(cameraPosition.toTile().x) + " TileY: " + std::to_string(cameraPosition.toTile().y) + " TileZ: " + std::to_string(cameraPosition.toTile().z) + " Tvalue: " + std::to_string(tileValue));
	debugHUDInfo.push_back("Facing: " + cameraFace + " Object ID: " + std::to_string(cameraPosition.frontObject) + " Object Value: " + frontObject);
	debugHUDInfo.push_back("Looking at: Yaw: " + std::to_string(cameraPosition.yaw) + " Pitch: " + std::to_string(cameraPosition.pitch));
	debugHUDInfo.push_back("Level: " + levelQueue.front().name);
	debugHUDInfo.push_back("Time elasped: " + timestamp);
	debugHUDInfo.push_back("FPS: " + std::to_string(lastFPS));
	debugHUDInfo.push_back(TITLE);

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

	int longestText = 0;

	// Set the position of the text
	for (std::string str : debugHUDInfo) {
		glColor3f(1, 1, 0);
		glRasterPos2f(x, y);

		if (longestText < str.length()) longestText = str.length();

		const unsigned char* convertedStr = reinterpret_cast <const unsigned char*> (str.c_str());
		glutBitmapString(GLUT_BITMAP_HELVETICA_12, convertedStr);
		y += 15.0f; //line spacing
	}

	glColor4f(0, 0, 0, 0.45f);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 0);
	glVertex2f((float)(longestText * 6) + 10, 0.0f);
	glVertex2f((float)(longestText * 6) + 10, (float)16.0f * debugHUDInfo.size());
	glVertex2f(0.0f, (float)16.0f * debugHUDInfo.size());
	glVertex2f(0.0f, 0.0f);

	glEnd();

	// Disable blending after rendering
	glDisable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);

}

int launch(int argc, char** argv) {

	std::cout << TITLE << std::endl;

	// Initializing position data.
	cameraPosition.x = 0.0;
	cameraPosition.y = 0.0;
	cameraPosition.z = 0.0;
	cameraPosition.pitch = 0.0;
	cameraPosition.yaw = 270.0; //facing negative X

	if (possibleSpawns.size() == 0)
		throw std::invalid_argument(levelQueue.front().name + " doesn't have a spawn location. SHIBA shutting down...");
	else spawnPlayer();

	if (levelQueue.empty()) throw std::invalid_argument("No levels in Queue. SHIBA shutting down");
	std::cout << (DEBUGMODE ? "Developer mode: ON" : "Developer mode: OFF") << std::endl;


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// Window properties
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow(TITLE);

	std::cout << "Initializing Window..." << std::endl;
	std::cout << "Rendering First Scene..." << std::endl;

	initGLFlags();	//sets flags for various functions

	//function that contains drawing functions
	glutDisplayFunc(renderScene);

	// function that handles window size changes.
	glutReshapeFunc(maintainAspectRatio);

	// keyboard listeners
	std::cout << "Adding Hardware Listners..." << std::endl;
	glutKeyboardFunc(listenForNormalKeys);
	glutKeyboardUpFunc(listenForNormalKeysRelease);
	glutSpecialFunc(listenForSpecialKeys);

	// Mouse listners
	glutMotionFunc(listenForMouseMovement);
	glutPassiveMotionFunc(listenForMouseMovement);
	glutMouseFunc(listenForMouseClick);
	// glutMotionFunc()

	// rerender display at the rate of FPS.
	glutTimerFunc(1000 / FPS, idleLoop, 0);

	// Loops the display function until terminated.
	auto endTime = std::chrono::high_resolution_clock::now();
	double elaspedTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

	std::cout << "SHIBA is ready! Took " << elaspedTime << " ms." << std::endl;
	glutMainLoop();
	return 1;
}

// Makes a 2D wall along the Y axis with respect to the passed x and z.
void makeWall(int x, int z, int x2, int z2) {

	glBegin(GL_QUADS);
	glColor3f(0.7, 0.7, 0.7);	//gray-ish walls

	// W1
	glVertex3f(x, GROUNDLEVEL, z);
	// W2
	glVertex3f(x2, GROUNDLEVEL, z2);
	// W3
	glVertex3f(x2, GROUNDLEVEL + WALLSIZE, z2);
	// W4
	glVertex3f(x, GROUNDLEVEL + WALLSIZE, z);

	glEnd();

}

void idleLoop(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, idleLoop, 0);
}

void renderScene(void) {

	lighting();

	// mouse tracking
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D);

	// Any number is releated to a menu Scene. -1 indicates the game has started.
	if (currentScene >= 0)
		switch (currentScene) {
			// case 0 should be pause.
		case 0:
			menu();
			break;
			// Options.
		case 1:
			options();
		default:
			renderText(0, glutGet(GLUT_WINDOW_HEIGHT) / 2, 1, 0, 0, "Error Assigning Scene. | Scene is null");
			break;
		}
	else renderGameElements();
	// Game has started so start iterating through each level in a FIFO manner


	glutSwapBuffers();

	// FPS calulation.
	frameCount++;
	finalTime = time(NULL);
	if (finalTime - initTime > 0) {
		lastFPS = frameCount / (finalTime - initTime);
		frameCount = 0;
		initTime = finalTime;
	}

}

void menu() {

	glDisable(GL_LIGHTING);

	std::vector<std::string> menuOptions;
	menuOptions.push_back("EXIT");
	menuOptions.push_back("OPTIONS");
	menuOptions.push_back("START GAME");

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

	float padding = 5.0f;
	int y = 150.0f;
	int x = 100.0f;
	int textHeight = 20.0f;

	for (std::string item : menuOptions) {
		glNormal3f(2, 0, 0);
		glColor3f(1, 1, 1);

		glRasterPos2f(x, y);

		const unsigned char* convertedStr = reinterpret_cast <const unsigned char*> (item.c_str());
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, convertedStr);

		// checking if mouse is within menu text area.
		if (crosshair.x > x - padding && crosshair.x < x + (item.length() * 13)
			&& crosshair.y > y - padding && crosshair.y < y + textHeight) {
			glColor3f(0, 1, 0);
			// handle click
			if (crosshair.buttonState == 0 && crosshair.button == 0) {
				std::cout << "Clicked on: " << item << std::endl;
				handleMainMenuInteraction(item);
			}
		}
		else glColor3f(1, 0, 0);

		glBegin(GL_QUADS);
		glNormal3f(0, 0, 0);

		glVertex2f(x - padding, y - padding);
		glVertex2f(x + (item.length() * 13), y - padding);
		glVertex2f(x + (item.length() * 13), y + textHeight);
		glVertex2f(x - padding, y + textHeight);

		glEnd();

		y += 30.0f;
	}

	// glutSolidCube();

	// Disable blending after rendering
	glDisable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);

}

// reads and writes for various game settings
void options() {

	//TODO: Create a separate class system for MENUs


}

// Contains all the logic for displaying menu options and all
void handleMainMenuInteraction(std::string option) {

	if (option == "EXIT") exit(0);

	if (option == "OPTIONS")  currentScene = 1;

	if (option == "START GAME") currentScene = -1;

}



// Draws the level based on the item in queue.
//! TODO: THIS CODE ONLY WORKS WITH AN ARRAY OF A FIXED SIZE.
//! Utilize vectors to bypass the limit of arrays
void draw() {

	// animation Queue to play step by step animations for different objects.
	playQueuedAnimations();
	bulletPhysics();

	int size = array_size(*levelQueue.front().levelGrid);

	// loops through a collection of 3D models and draws them in level.
	for (int i = 0; i < objectCollection.size(); i++) {
		// Setting the texture if available.
		if (objectCollection.at(i).texture == true) {

			glBindTexture(GL_TEXTURE_2D, textureCollection.at(objectCollection.at(i).color));

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glEnable(GL_TEXTURE_2D);

		}

		// Loads the object in the world.
		objectCollection.at(i).load();
		glDisable(GL_TEXTURE_2D);

		// loading bullets (if any)
		for (auto& b : bulletCollection) {
			b.second.loadGlutSolids();
		}


	}

	renderWorldBox();

	return;
}

void renderWorldBox() {

	glBindTexture(GL_TEXTURE_2D, textureCollection.at(Empty));

	// setting texture to repeat
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// glEnable(GL_TEXTURE_2D);

	// this is the ceiling.
	// Not rendering this for now
	// Rendering it in chunks so that lighting can spread.

	int size = array_size(*levelQueue.front().levelGrid);
	glEnable(GL_TEXTURE_2D);
	int xAxis = 0, zAxis = 0;

	// Rendering along the Z axis first and increase X after each pass.
	for (int xCounter = 0; xCounter < size; xCounter++) {

		for (int zCounter = 0; zCounter < size; zCounter++) {

			// drawing tiles CCW
			glBegin(GL_QUADS);
			glNormal3f(0, -1, 0);

			glColor3f(1, 1, 1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(xAxis - TILESIZE, (WALLSIZE + GROUNDLEVEL), zAxis - TILESIZE);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(xAxis + TILESIZE, (WALLSIZE + GROUNDLEVEL), zAxis - TILESIZE);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(xAxis + TILESIZE, (WALLSIZE + GROUNDLEVEL), zAxis + TILESIZE);

			glTexCoord2f(0.0, 1.0);
			glVertex3f(xAxis - TILESIZE, (WALLSIZE + GROUNDLEVEL), zAxis + TILESIZE);

			glEnd();
			xAxis += TILESIZE * 2;
		}
		zAxis += TILESIZE * 2;
		xAxis = 0;
	}

	// rendering floor.
	xAxis = 0; zAxis = 0;
	for (int xCounter = 0; xCounter < size; xCounter++) {

		for (int zCounter = 0; zCounter < size; zCounter++) {

			// drawing tiles CCW
			glBegin(GL_QUADS);
			glNormal3f(0, 1, 0);

			glColor3f(1, 1, 1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(xAxis - TILESIZE, (GROUNDLEVEL), zAxis + TILESIZE);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(xAxis + TILESIZE, (GROUNDLEVEL), zAxis + TILESIZE);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(xAxis + TILESIZE, (GROUNDLEVEL), zAxis - TILESIZE);

			glTexCoord2f(0.0, 1.0);
			glVertex3f(xAxis - TILESIZE, (GROUNDLEVEL), zAxis - TILESIZE);

			glEnd();
			xAxis += TILESIZE * 2;
		}
		zAxis += TILESIZE * 2;
		xAxis = 0;
	}



	glBindTexture(GL_TEXTURE_2D, textureCollection.at(BOUNDARY));

	// setting texture to repeat
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBegin(GL_QUADS);
	// Walls for level
	// West side wall
	glNormal3f(0, 1.0f, 0);
	glColor3f(1, 1, 1);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-TILESIZE, GROUNDLEVEL, -TILESIZE);

	glTexCoord2f(10.0, 1.0);
	glVertex3f(levelBounds.x - TILESIZE, GROUNDLEVEL, -TILESIZE);

	glTexCoord2f(10.0, 0.0);
	glVertex3f(levelBounds.x - TILESIZE, (GROUNDLEVEL + WALLSIZE), -TILESIZE);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-TILESIZE, (GROUNDLEVEL + WALLSIZE), -TILESIZE);

	// North Wall
	glTexCoord2f(0.0, 0.0);
	glVertex3f(levelBounds.x - TILESIZE, GROUNDLEVEL, -TILESIZE);

	glTexCoord2f(10.0, 0.0);
	glVertex3f(levelBounds.x - TILESIZE, GROUNDLEVEL, levelBounds.z - TILESIZE);

	glTexCoord2f(10.0, 1.0);
	glVertex3f(levelBounds.x - TILESIZE, (GROUNDLEVEL + WALLSIZE), levelBounds.z - TILESIZE);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(levelBounds.x - TILESIZE, (GROUNDLEVEL + WALLSIZE), -TILESIZE);

	// South Wall
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-TILESIZE, (GROUNDLEVEL + WALLSIZE), -TILESIZE);

	glTexCoord2f(10.0, 0.0);
	glVertex3f(-TILESIZE, (GROUNDLEVEL + WALLSIZE), levelBounds.z - TILESIZE);

	glTexCoord2f(10.0, 1.0);
	glVertex3f(-TILESIZE, GROUNDLEVEL, levelBounds.z - TILESIZE);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-TILESIZE, GROUNDLEVEL, -TILESIZE);

	// East Wall
	glTexCoord2f(0.0, 0.0);
	glVertex3f(levelBounds.x - TILESIZE, GROUNDLEVEL, levelBounds.z - TILESIZE);

	glTexCoord2f(10.0, 0.0);
	glVertex3f(-TILESIZE, GROUNDLEVEL, levelBounds.z - TILESIZE);

	glTexCoord2f(10.0, 1.0);
	glVertex3f(-TILESIZE, (GROUNDLEVEL + WALLSIZE), levelBounds.z - TILESIZE);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(levelBounds.x - TILESIZE, (GROUNDLEVEL + WALLSIZE), levelBounds.z - TILESIZE);

	glEnd();

	// GROUND IS RENDERED BY TILES.

	glDisable(GL_TEXTURE_2D);
}



void maintainAspectRatio(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (float)w / (float)h, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);

}

void listenForNormalKeys(unsigned char key, int xx, int yy) {
	lastKey = key;
	if (currentScene < 0) switch ((int)key) {

	case (int)'l':
		levelQueue.pop();
		initLevels(levelQueue);
		break;

	case (int)'m':
		int tpX, tpZ;
		std::cout << "Enter new tile cords:";
		std::cin >> tpX >> tpZ;
		std::cout << "Teleported to tile " << tpX << " " << tpZ << std::endl;
		cameraPosition.x = tpX * 10;
		cameraPosition.z = tpZ * 10;

		break;

	case (int)'q':

		for (int id : enemySpawnerLocations) {
			interactWithObj(id);
		}
		break;

	case (int)'[':
		player.health -= 10;
		break;

	case (int)']':
		player.health += 10;
		break;

	case (int)'e':
		interactWithObj(cameraPosition.frontObject);
		break;

	case (int)'f':
		light = !light;
		break;

	case (int)'g':
		cameraPosition.y = cameraPosition.y > 1.0f ? 0.0f : 120.0f;
		break;

	case (int)'r':
		resetCamera();
		break;

	case (int)'t':
		track = !track;
		track ? glutSetCursor(GLUT_CURSOR_NONE) : glutSetCursor(GLUT_CURSOR_INHERIT);

		break;
	case (int)'p':
		levelQueue.front().printLevel();
		break;

	case (int)'y':
		wireframe = !wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
		break;
	case 'W':
	case 'w':
		motion.Forward = true;
		break;
	case 'A':
	case 'a':
		motion.Left = true;
		break;
	case 'S':
	case 's':
		motion.Backward = true;
		break;
	case 'D':
	case 'd':
		motion.Right = true;
		break;
	case 'c':
		collision = !collision;
		break;
	default:
		break;
	}

}

void listenForNormalKeysRelease(unsigned char key, int x, int y) {

	if (currentScene < 0) switch (key) {
	case 'W':
	case 'w':
		motion.Forward = false;
		break;
	case 'A':
	case 'a':
		motion.Left = false;
		break;
	case 'S':
	case 's':
		motion.Backward = false;
		break;
	case 'D':
	case 'd':
		motion.Right = false;
		break;
	}
}

void listenForSpecialKeys(int key, int x, int y) {

	lastKey = key;

	switch (key) {
	case 101: //UP
		cameraPosition.y += 0.10f;
		std::cout << cameraPosition.y << std::endl;
		break;
	case 102: //RIGHT
		DevHudY += 10.00f;
		std::cout << DevHudY << std::endl;
		break;
	case 103: //DOWN
		cameraPosition.y -= 0.10f;
		std::cout << cameraPosition.y << std::endl;
		break;
	case 100: //LEFT
		DevHudY -= 10.00f;
		std::cout << DevHudY << std::endl;
		break;

	default:
		break;
	}

}

void listenForMouseClick(int button, int state, int x, int y) {

	switch (button) {
	case 0:
		// incase it's being held down, don't do anything.
		if (state && track) shoot();
		// else do menu stuff
		break;
	case 1:
		currentScene--;
		break;

	case 2:
		currentScene++;
		break;
	}

	crosshair.button = button;
	crosshair.buttonState = state;


	if (DEBUGMODE) {
		std::cout << "[Method] listenForMouseClick: " << button << " " << crosshair.x << ":" << crosshair.y << std::endl;
	}

}

void listenForMouseMovement(int x, int y) {

	//checking if mouse position has changed from previous recorded position.
	// Prevents pointless reloads.
	if (crosshair.x == x && crosshair.y == y) return;

	if (track) {

		int dev_x, dev_y;
		dev_x = (WIDTH / 2) - x;
		dev_y = (HEIGHT / 2) - y;

		/* apply the changes to pitch and yaw*/

		//checking if yaw goes over 360 or under -360. resetting back to 0.
		if (cameraPosition.yaw + dev_x / SENSITIVITY >= 360.0) cameraPosition.yaw += (float)dev_x / SENSITIVITY - 360.0;
		else cameraPosition.yaw += (float)dev_x / SENSITIVITY;

		if (cameraPosition.yaw + dev_x / SENSITIVITY < 0) cameraPosition.yaw += (float)dev_x / SENSITIVITY + 360.0;
		else cameraPosition.yaw += (float)dev_x / SENSITIVITY;

		// Setting pole orientation
		Poles finalFacing;
		if (cameraPosition.yaw < 270.0 + 45 && cameraPosition.yaw > 270.0 - 45) finalFacing.north = true;
		else if (cameraPosition.yaw < 180 + 45 && cameraPosition.yaw > 180 - 45) finalFacing.east = true;
		else if (cameraPosition.yaw < 90 + 45 && cameraPosition.yaw > 90 - 45) finalFacing.south = true;
		else finalFacing.west = true;

		cameraPosition.pitch += (float)dev_y / SENSITIVITY;
		facing = finalFacing;

	}

	// updating mouse position for 2D projection

	crosshair.x = x;
	crosshair.y = glutGet(GLUT_WINDOW_HEIGHT) - y;
}

void renderText(float x, float y, int r, int g, int b, const char* string) {
	glDisable(GL_LIGHTING);
	int j = strlen(string);

	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for (int i = 0; i < j; i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);

	glEnable(GL_LIGHTING);
}

void initLevels(std::queue <Level> queue) {

	// clearing spawn locations of previous level.
	possibleSpawns.clear();
	objectCollection.clear();

	int size = array_size(*queue.front().levelGrid);
	float x = 0.0, y = GROUNDLEVEL, z = 0.0;

	if (DEBUGMODE)
		std::cout << "[Method] initLevels: Number of levels is " << queue.size() << std::endl;

	levelQueue = queue;
	std::cout << "Initializing Level..." << std::endl;

	int repeatScale = 1;

	// Columns (X)
	for (int actualX = 0; actualX < size; actualX++) {

		// Rows (Z)
		for (int actualZ = size - 1; actualZ > -1; actualZ--) {

			y = GROUNDLEVEL;

			// adding to spawn collection if it's a valid spawn loaction.
			if (queue.front().levelGrid[actualZ][actualX] == SpawnLoc)
				addSpawns((size - 1) - actualZ, actualX);

			// Points are below.
			// Setting origin of object.
			ShibaObject tile(x, y, z);
			tile.objectName = std::to_string(levelQueue.front().levelGrid[actualZ][actualX]);

			// saving the color (not necessary when loading textures)
			tile.color = levelQueue.front().levelGrid[actualZ][actualX];

			// counting number of objectives
			if (tile.color == Objective) levelQueue.front().objectives++;
			// counting enemy spawners
			if (tile.color == EnemySpawner) {
				levelQueue.front().enemies++;
				// adding the ID of the object to a separate vector for
				// controling spawning.
				enemySpawnerLocations.push_back(objectCollection.size());
			}

			if (tile.color == Wall || abs(tile.color) == DoorClosed) tile.texture = true;
			else tile.texture = false;

			// Ground vertexes:
			// Bottom view
			// Doors will have some extra vertexes.
			if (tile.color == DoorClosed || DoorOpen) {
				// P3 = top left vertex
				tile.vertexCol.push_back({ x + TILESIZE, y, z - TILESIZE, {0, 1.0f, 0} });
				// P2 = top right vertex
				tile.vertexCol.push_back({ x + TILESIZE, y, z + TILESIZE, {0, 1.0f, 0} });
				// P1 = bottom right vertex
				tile.vertexCol.push_back({ x - TILESIZE, y, z + TILESIZE, {0, 1.0f, 0} });
				// P4 = bottom left vertexeee
				tile.vertexCol.push_back({ x - TILESIZE, y, z - TILESIZE, {0, 1.0f, 0} });
			}

			// Wall vertexes
			float wallModifer;

			// Incase it's a note, it will be scaled down to be of only 0.1f height
			if (tile.color == Notes) wallModifer = 0.1f;
			// else if (tile.color == Empty) wallModifer = 0.01f;
			else wallModifer = 20.0f;

			tile.texturePoints.push_back({ 0.0, 0.0 });
			tile.texturePoints.push_back({ 0.0, (float)repeatScale });
			tile.texturePoints.push_back({ (float)repeatScale, (float)repeatScale });
			tile.texturePoints.push_back({ (float)repeatScale, 0.0 });

			// Top view (only visible in god mode)
			// P1 = bottom right vertex
			tile.vertexCol.push_back({ x - TILESIZE, y + wallModifer, z + TILESIZE, {0, 1.0f, 0} });
			// P2 = top right vertex
			tile.vertexCol.push_back({ x + TILESIZE, y + wallModifer, z + TILESIZE, {0, 1.0f, 0} });
			// P3 = top left
			tile.vertexCol.push_back({ x + TILESIZE, y + wallModifer, z - TILESIZE, {0, 1.0f, 0} });
			// P4 = bottom left
			tile.vertexCol.push_back({ x - TILESIZE, y + wallModifer, z - TILESIZE, {0, 1.0f, 0} });


			if (levelQueue.front().levelGrid[actualZ][actualX] > 0) {
				// -X facing wall Quad

					// P1 = bottom right vertex
				tile.vertexCol.push_back({ x - TILESIZE, y, z + TILESIZE, {0, -1.0f, 0} });
				tile.vertexCol.push_back({ x - TILESIZE, y + wallModifer, z + TILESIZE, {0, -1.0f, 0} });

				// P4 = bottom left
				tile.vertexCol.push_back({ x - TILESIZE, y + wallModifer, z - TILESIZE, {0, -1.0f, 0} });
				tile.vertexCol.push_back({ x - TILESIZE, y, z - TILESIZE, {0, -1.0f, 0} });

				// +X facing wall Quad
					// P3 = top left
				tile.vertexCol.push_back({ x + TILESIZE, y, z - TILESIZE, {0, -1.0f, 0} });
				tile.vertexCol.push_back({ x + TILESIZE, y + wallModifer, z - TILESIZE, {0, -1.0f, 0} });
				// P2 = top right vertex
				tile.vertexCol.push_back({ x + TILESIZE, y + wallModifer, z + TILESIZE, {0, -1.0f, 0} });
				tile.vertexCol.push_back({ x + TILESIZE, y, z + TILESIZE, {0, -1.0f, 0} });

				// -Z facing wall Quad

					// P4 = bottom left
				tile.vertexCol.push_back({ x - TILESIZE, y, z - TILESIZE, {0, -1.0, 0} });
				tile.vertexCol.push_back({ x - TILESIZE, y + wallModifer, z - TILESIZE, {0, -1.0, 0} });
				// P3 = top left
				tile.vertexCol.push_back({ x + TILESIZE, y + wallModifer, z - TILESIZE, {0, -1.0, 0} });
				tile.vertexCol.push_back({ x + TILESIZE, y, z - TILESIZE, {0, -1.0, 0} });

				// +Z facing wall Quad

					// P2 = top right vertex
				tile.vertexCol.push_back({ x + TILESIZE, y, z + TILESIZE, {0, -1.0f, 0,} });
				tile.vertexCol.push_back({ x + TILESIZE, y + wallModifer, z + TILESIZE, {0, -1.0f, 0} });
				// P1 = bottom right vertex
				tile.vertexCol.push_back({ x - TILESIZE, y + wallModifer, z + TILESIZE, {0, -1.0f, 0} });
				tile.vertexCol.push_back({ x - TILESIZE, y, z + TILESIZE, {0, -1.0f, 0} });

			}

			//! TODO: Assign glut functions outside of engine.
			// This is for testing only and it works!
			// tile.setLoadGlutFunction(test);

			// finally add the object to the level collection.
			objectCollection.push_back(tile);

			x += TILESIZE * 2;
		}
		z += TILESIZE * 2;
		levelBounds.x = x;
		x = 0.0;
	}
	levelBounds.z = z;

	objectCollection.shrink_to_fit();
	possibleSpawns.shrink_to_fit();
	resetCamera();

}

void renderGameElements() {

	if (track) glutWarpPointer(WIDTH / 2, HEIGHT / 2);

	camera();	//player
	draw();		//level/world
	checkForInteraction(cameraPosition);

	//shows engine stats. ALWAYS on top
	devScreen();

	// Player HUD elements
	updateHUD();

	// Pathing algorithms for enemies.


}

void camera() {

	float newX = 0.0, newZ = 0.0;

	// Calculate new coordinate depending on key directions held.
	if (motion.Forward) {
		newX += cos((cameraPosition.yaw + 90) * TO_RADIANS) / movemenSpeed;
		newZ -= sin((cameraPosition.yaw + 90) * TO_RADIANS) / movemenSpeed;
	}

	if (motion.Backward) {
		newX += cos((cameraPosition.yaw + 90 + 180) * TO_RADIANS) / movemenSpeed;
		newZ -= sin((cameraPosition.yaw + 90 + 180) * TO_RADIANS) / movemenSpeed;
	}

	if (motion.Left) {
		newX += cos((cameraPosition.yaw + 90 + 90) * TO_RADIANS) / movemenSpeed;
		newZ -= sin((cameraPosition.yaw + 90 + 90) * TO_RADIANS) / movemenSpeed;
	}

	if (motion.Right) {
		newX += cos((cameraPosition.yaw + 90 - 90) * TO_RADIANS) / movemenSpeed;
		newZ -= sin((cameraPosition.yaw + 90 - 90) * TO_RADIANS) / movemenSpeed;
	}

	if (collisionCheck(1, newX, newZ)) cameraPosition.x += newX;
	if (collisionCheck(2, newX, newZ)) cameraPosition.z += newZ;

	//limit the values of pitch between -60 and 60
	cameraPosition.y > GROUNDLEVEL + 5.0f ? GROUNDLEVEL : 120.0f;

	if (cameraPosition.pitch >= (cameraPosition.y > GROUNDLEVEL + 5.0f ? 90.0f : pitchLimit))
		cameraPosition.pitch = (cameraPosition.y > GROUNDLEVEL + 5.0f ? 90.0f : pitchLimit);

	if (cameraPosition.pitch <= -(cameraPosition.y > GROUNDLEVEL + 5.0f ? 90.0f : pitchLimit))
		cameraPosition.pitch = -(cameraPosition.y > GROUNDLEVEL + 5.0f ? 90.0f : pitchLimit);


	glRotatef(-cameraPosition.pitch, 1.0, 0.0, 0.0); // Along X axis
	glRotatef(-cameraPosition.yaw, 0.0, 1.0, 0.0);    //Along Y axis

	glTranslatef(-cameraPosition.x, -cameraPosition.y, -cameraPosition.z);  //new position
}

void getColorMod(int id) {

	switch (id) {
	case -1:	//floor: Grey
		glColor3f(0.5, 0.5, 0.5);
		break;
	case 0:		//spawn points: Purple
		glColor3f(0.6, 0.2, 0.6);
		break;
	case 1:		//enemy spawner: Red
		glColor3f(1.0, 0.0, 0.0);
		break;
	case 2:		//Objectives: Blue
		glColor3f(0.0, 0.0, 1.0);
		break;
	case 3:		//Notes: Cyan
		glColor3f(0.0, 1, 1);
		break;
	case 4:		//Walls : Grey
		glColor3f(1.0, 1.0, 1.0);
		break;
	case 5:		//Doors: Yellow
		glColor3f(1.0, 1.0, 0.0);
		break;
	case 6:		//Boss Location: Orange
		glColor3f(1.0, 0.60, 0.0);
		break;
	case 7:		//Custom 3D model unique to map: Green
		glColor3f(0.0, 1.0, 0.0);
		break;
	default:
		glColor3f(0.0, 0.0, 0.0);
		break;
	}
}

// set camera back to spawn and default Y axis
void resetCamera() {

	spawnPlayer();
	cameraPosition.y = 0.0;
	cameraPosition.pitch = 0.0;
	cameraPosition.yaw = 270.0; //facing negative X

}

void addSpawns(int x, int z) {
	Position temp;
	temp.x = x;
	temp.z = z;
	possibleSpawns.push_back(temp);
}

bool collisionCheck(int direction, float x, float z) {

	// 1 = x
	// 2 = z
	if (!collision) return true;

	float offset = 0.30;	//prevent camera seeing through collision walls

	// Converting X and Z coords to index for level map
	int finalX =
		(int)(array_size(*levelQueue.front().levelGrid) - 1)
		- round((cameraPosition.x
			+ (direction == 1 ? x : 0)	//only factor X if traveling towards Z.
			+ (x > 0 ? offset : -offset)) / 10);

	int finalZ = (int)round((cameraPosition.z
		+ (direction == 2 ? z : 0)	//only factor Z if traveling towards Z.
		+ (z > 0 ? offset : -offset)) / 10);

	int tileVal;

	//Level boundary check
	if (
		(finalX < 0 || finalX >(array_size(*levelQueue.front().levelGrid) - 1))
		||
		(finalZ < 0 || finalZ >(array_size(*levelQueue.front().levelGrid) - 1))
		) tileVal = Wall;
	else tileVal = levelQueue.front().levelGrid[finalX][finalZ];

	//X Axis check
	if (
		(direction == 1)
		&& //checking if within bounds
		(tileVal == Wall || tileVal == Boss || tileVal == Custom || tileVal == DoorClosed || tileVal == EnemySpawner || tileVal == Objective)
		) return false; //stopping movement in that axis.


	//Z Axis check
	if (
		(direction == 2)
		&&
		(tileVal == Wall || tileVal == Boss || tileVal == Custom || tileVal == DoorClosed || tileVal == EnemySpawner || tileVal == Objective)
		) return false; //stopping movement in that axis.

	return true;

}


void spawnPlayer() {

	//checking if there's only 1 spawn point.
	if (possibleSpawns.size() == 1) {
		cameraPosition.x = possibleSpawns.at(0).x * 10;
		cameraPosition.z = possibleSpawns.at(0).z * 10;

	}
	else {

		Position temp = possibleSpawns.at(randomInt(0, possibleSpawns.size() - 1));

		cameraPosition.x = temp.x * 10;
		cameraPosition.z = temp.z * 10;

	}


}

// generate a random number between the low and high range, inclusive
int randomInt(int low, int high) {
	return (rand() % (high - low + 1)) + low;
}


void checkForInteraction(Position& entity) {

	int size = array_size(*levelQueue.front().levelGrid);
	int xModifier = 1, zModifier = 20;
	//int worldX = cameraPosition.toTile().x, worldZ = cameraPosition.toTile().z;

	int id = (((entity.toTile().z) * size) + (entity.toTile().x));

	// out of bounds prevention
	if (facing.north) {	// X+
		// worldX++;
		id += xModifier;
	}
	else if (facing.south) {	// X-
		// worldX--;
		id -= xModifier;
	}
	else if (facing.east) {	// Z+
		// worldZ++;
		id += zModifier;
	}
	else {	// Z- (West)
		// worldZ--;
		id -= zModifier;
	}

	if (id < 0 || id >= (size * size))
		id = -1;

	if ((facing.east && entity.toTile().z == size - 1) || (facing.west && entity.toTile().z == 0))
		id = -1;

	// finally setting the object
	entity.frontObject = id;

}

void interactWithObj(int objectID) {

	int size = array_size(*levelQueue.front().levelGrid);

	if (objectID == -1) {
		std::cout << "No interaction available" << std::endl;
		return;
	}

	std::cout << "Interacting with: ";
	std::cout << objectID << " ";
	std::cout << objectCollection.at(objectID).tileX << " ";
	std::cout << objectCollection.at(objectID).tileZ << std::endl;


	queueAnimation(objectID, ((size - 1) - objectCollection.at(objectID).tileX), objectCollection.at(objectID).tileZ);

	// int size = array_size(*levelQueue.front().levelGrid);
	// int xModifier = 1, zModifier = 20, worldX = cameraPosition.toTile().x, worldZ = cameraPosition.toTile().z;

	// int id = (((cameraPosition.toTile().z) * size) + (cameraPosition.toTile().x));

	// // out of bounds prevention
	// if (facing.north) {	// X+
	// 	worldX++;
	// 	id += xModifier;	
	// } else if (facing.south) {	// X-
	// 	worldX--;
	// 	id -= xModifier;	
	// }  else if (facing.east) {	// Z+
	// 	worldZ++;
	// 	id += zModifier;		
	// } else {	// Z- (West)
	// 	worldZ--;
	// 	id -= zModifier;	
	// } 

	// if (id < 0 || id >= (size * size)) 
	// 	id = -1;

	// if ((facing.east && cameraPosition.toTile().z == size - 1) || (facing.west && cameraPosition.toTile().z == 0)) 
	// 	id = -1;

	// if (id != -1) {

	// 	// Interaction animation
	// 	std::cout << "ID: "<< id << " : " << objectCollection.at(id).color << std::endl;
	// 	std::cout << "X: "<< worldX << " Z: " << worldZ << std::endl;
	// 	std::cout << "VAL: "<< levelQueue.front().levelGrid[worldX][worldZ] << std::endl;

	// 	if (lastKey == (int) 'e')
	// 	queueAnimation(id, ((size-1) - worldX), worldZ);

	// } else {
	// 	std::cout << "Out of bounds" << std::endl;
	// }


}

void queueAnimation(int id, int x, int z) {

	int tileValue = levelQueue.front().levelGrid[x][z];
	std::cout << tileValue << " From Queue" << std::endl;
	Position target;

	switch (tileValue) {

	case DoorOpen:

		target.y = 0.0f;	//door open position.

		// adding to animation queue.
		animationQueue.insert_or_assign(id, target);

		// update the tile value
		levelQueue.front().levelGrid[x][z] = DoorClosed;

		break;

	case EnemySpawner:
		target.y = 19.9f;	//door open position.

		// adding to animation queue.
		animationQueue.insert_or_assign(id, target);
		break;

	case DoorClosed:
		target.y = 19.9f;	//door open position.

		// adding to animation queue.
		animationQueue.insert_or_assign(id, target);

		// update the tile value
		levelQueue.front().levelGrid[x][z] = DoorOpen;
		break;

	default:
		break;
	}




}


void playQueuedAnimations() {

	// Checking if queue is empty
	if (animationQueue.empty()) return;

	std::vector<int> IDsToRemove;

	for (const auto& item : animationQueue) {

		Position difference = item.second;
		objectCollection.at(item.first).offset;
		// first contains the ID.
		// second contains the target position of the object.

		// getting difference between object and target.
		difference -= objectCollection.at(item.first).offset;


		objectCollection.at(item.first).offset += difference / ANIMATIONSTEP;

		// checking if animation is finished.
		//! TODO: This is jank and doesn't translate well when dealing.
		//! This never gets called because it's too busy doing microcalculations
		//* Fixed by just converting all negative values to positive lmao
		//* This has an unintended effect of easing the animation out.

		if (difference.absolute() <= 0.01) {

			objectCollection.at(item.first).offset = item.second;
			IDsToRemove.push_back(item.first);	// dequeue this object
		}


	}

	// recursively removing all ids.
	for (int i : IDsToRemove)
		animationQueue.erase(i);

	// trimming and freeing memory
	IDsToRemove.clear();
	IDsToRemove.shrink_to_fit();

}

void lighting() {
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	if (light) {
		glEnable(GL_LIGHT0);

		// Set light position
		GLfloat lightpos[] = { cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

		// Increase ambient and diffuse components for brightness
		GLfloat ambient[] = { 0.35f, 0.35f, 0.35f, 1.0f };  // Adjust ambient brightness
		GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Adjust diffuse brightness

		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	}
	else {
		glDisable(GL_LIGHT0);
	}

	// // Set global ambient light
	// GLfloat globalAmbient[] = {0.4f, 0.4f, 0.4f, 1.0f};
	// glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	// // Set light position
	// GLfloat lightPosition[] = {levelBounds.x/2, GROUNDLEVEL+2.0f, levelBounds.z/2, 1.0f};
	// glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// // Set material properties
	// GLfloat ambientMaterial[] = {1.0f, 1.0f, 1.0f, 0.0f};
	// GLfloat diffuseMaterial[] = {1.0f, 1.0f, 1.0f, 0.0f};
	// GLfloat specularMaterial[] = {1.0f, 1.0f, 1.0f, 0.0f};
	// GLfloat shininess = 10.0f;

	// glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterial);
	// glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial);
	// glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial);
	// glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

}

void bulletModel(ShibaObject a) {

	// any custom object must have a single ShibaQuad point as center.
	ShibaQuad center;

	center = a.vertexCol.at(0);

	// moving the render point to elsewhere
	glPushMatrix();
	glColor3f(1, 0, 0);	//bullets are red for now
	glTranslated(
		center.x + a.offset.x,
		center.y + a.offset.y,
		center.z + a.offset.z);
	glutSolidCube(1.0f);
	glPopMatrix();
	//putting it back to scene origin

}

// handles bullet animation and collision
void bulletPhysics() {

	// exit if there's nothing to iterate
	if (bulletAnimation.empty()) return;

	std::vector <int> IDsToRemove;

	for (const auto& item : bulletAnimation) {

		Position difference = item.second;
		std::string bulletID = "Bullet#" + std::to_string(item.first);
		bulletCollection.at(bulletID).offset;

		difference -= bulletCollection.at(bulletID).offset;

		bulletCollection.at(bulletID).offset += difference / (ANIMATIONSTEP * 5);

		if (difference.absolute() <= 0.01) {

			bulletCollection.at(bulletID).offset = item.second;
			IDsToRemove.push_back(item.first);	// dequeue this object

			// removing the bullet from the collection if the names match
			bulletCollection.erase(bulletID);
			std::cout << "Bullet #" << item.first << " dequeued from animation." << std::endl;
			std::cout << "Bullets in Collection: " << bulletCollection.size() << std::endl;
		}

	}

	// recursively removing all ids if animation is complete.
	for (int i : IDsToRemove) {
		bulletAnimation.erase(i);
	}

	// trimming and freeing memory
	IDsToRemove.clear();
	IDsToRemove.shrink_to_fit();

}

void shoot() {

	// calculating next coord.
	ShibaObject bullet(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	bullet.objectName = "Bullet#" + std::to_string(bulletCollection.size());

	// adding only 1 quad as the object center/bullet hitbox
	bullet.vertexCol.push_back({ cameraPosition.x, cameraPosition.y, cameraPosition.z, {0, 1.0f, 0} });

	// bind the bullet model to the object.
	bullet.setLoadGlutFunction(bulletModel);

	int id = bulletCollection.size();
	// adding to collection to later iterate through using ID
	bulletCollection.insert_or_assign(bullet.objectName, bullet);

	// queuing for animation.
	// target is not the destination coordinates!
	// this is how many points it should move in either direction.
	Position target;

	// for now shooting in straight lines.
	// when firing, the bullet will continue to travel until it hits the level bound.
	float newX = cos((cameraPosition.yaw + 90) * TO_RADIANS) * TRAVEL;
	float newZ = sin((cameraPosition.yaw + 90) * TO_RADIANS) * TRAVEL;
	float newY = tan((cameraPosition.pitch) * TO_RADIANS) * TRAVEL;

	target.x = newX;
	target.z = -newZ;
	target.y = newY;
	bulletAnimation.insert_or_assign(id, target);

	// if (facing.north) {
	// 	target.x = levelBounds.x;
	// 	bulletAnimation.insert_or_assign(id, target);
	// } 
	// if (facing.south) {
	// 	target.x = -levelBounds.x;
	// 	bulletAnimation.insert_or_assign(id, target);
	// } 
	// if (facing.east) {
	// 	target.z = levelBounds.z;
	// 	bulletAnimation.insert_or_assign(id, target);
	// } 
	// if (facing.west) {
	// 	target.z = -levelBounds.z;
	// 	bulletAnimation.insert_or_assign(id, target);
	// } 


}


#endif