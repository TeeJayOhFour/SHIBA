#pragma once

#include "Config.h"
#include "DataStructures.h"

// Globals
//Player navigation info
Motion motion = { false, false, false, false };

struct Position crosshair;
std::queue <Level> levelQueue;
std::queue <Menu> menuQueue;
std::queue <std::string> debugInfoQueue;
Position cameraPosition;	//camera cords

Entity player(cameraPosition);	//incorporate this throughout the whole code. remove cameraPostion



//? OR just add convert the camera into a ShibaObject.

Position levelBounds;
std::vector <Position> possibleSpawns;
std::vector <ShibaObject> objectCollection;
std::vector <int> enemySpawnerLocations;

// stores the ID of the object along with target postion value.
std::unordered_map <int, Position> animationQueue;
std::unordered_map <int, GLuint> textureCollection;
std::unordered_map <int, int> globalAnimationDelay;

std::unordered_map <std::string, ShibaObject> enemyCollection;
std::unordered_map <std::string, ShibaObject> bulletMap;

bool levelSpawning = false;

bool playerInCombat = false;

irrklang::ISound* combatMusic = soundEngine->play2D(BATTLE_MUSIC_0, false, true, true);

//!TODO YOU CAN't INTERACT WITH ENEMY SPAWNERS FOR WHATEVER REASON. CHECK INTERACTION FUNCTION IS NOT THE ISSUE

// Prototypes
void menu();
void draw();
void camera();
void lighting();
void devScreen();
void updateHUD();
void showSplash(int splash);
void spawnPlayer();
void renderScene();
void idleLoop(int);
void resetCamera();
void enemyPathing();
void bulletPhysics();
void renderWorldBox();
void renderGameElements();
void shoot(Position entity);
void playQueuedAnimations();
void addSpawns(int x, int z);
void enemyModel(ShibaObject a);
int randomInt(int low, int high);
void interactWithObj(int objectID);
void initLevels(std::queue <Level> queue);
void queueAnimation(int id, int x, int z);
void listenForMouseMovement(int x, int y);
void checkForInteraction(Position& entity);
void maintainAspectRatio(int width, int height);
void listenForSpecialKeys(int key, int x, int y);
bool collisionCheck(int direction, float x, float z);
void listenForNormalKeys(unsigned char key, int x, int y);
void handleOptionInteraction(std::string option, int value);
void handleMainMenuInteraction(std::string option, int value);
void listenForMouseClick(int button, int state, int x, int y);
void listenForNormalKeysRelease(unsigned char key, int x, int y);
std::queue<ShibaQuad> aStarImplementation(ShibaObject& start, Position& goal, int range);
void renderText(float x, float y, int r, int g, int b, const char* string);


#include "TextureLoader.h"

// init all possible menus in the game screen
void initMenu() {

	std::vector<MenuOption> menuOptions;
	menuOptions.push_back({ "EXIT", BUTTON });
	menuOptions.push_back({ "OPTIONS", BUTTON });
	menuOptions.push_back({ "START GAME", BUTTON });

	Menu Main(100, 150, crosshair);

	Main.setHandler(handleMainMenuInteraction);
	Main.options = menuOptions;

	// Reusing the same vector because why not
	menuOptions.clear();

	menuOptions.push_back({ "BACK", BUTTON });
	menuOptions.push_back({ "MASTER SOUND VOLUME", MULTI_BUTTON, 0 });

	// recursively adding all the options possible from 0-100 lol
	// There could have been an easier way to achieve this howwever ther wasn't enough time to fledge out the menu system.

	for (int i = 0; i < 10; i++) {
		menuOptions.back().value.push_back((i + 1) * 10);
	}
	menuOptions.back().head = (int) (MAX_VOLUME * 10.0f);
	
	menuOptions.push_back({ "MOUSE SENSITIVITY", MULTI_BUTTON, (int) mouseSpeed });
	menuOptions.back().value.push_back((int) SENSITIVITY_HIGH);
	menuOptions.back().value.push_back((int) SENSITIVITY_LOW);


	menuOptions.push_back({ "FULLSCREEN", TOGGLE_BUTTON, 0 });			//off by default
	menuOptions.push_back({ "SHOW BLOOD", TOGGLE_BUTTON, 1 });			//on by default
	menuOptions.push_back({ "[LEFT CLICK]  for decrements", TEXT });	
	menuOptions.push_back({ "[RIGHT CLICK] for increments", TEXT });	


	Menu Options(100, 150, crosshair);
	Options.setHandler(handleOptionInteraction);
	Options.options = menuOptions;

	menuQueue.push(Main);
	menuQueue.push(Options);

}

void initTextures() {

	// Loading essential game textures manually.

	// Get the directory of the current source file

	// Adding ID in collection. 
	//The 1 is temporary as it will get overwritten later by the parser
	textureCollection.insert_or_assign(BOUNDARY, 1);
	// Getting image data.
	Image* image = loadTexture("assets/textures/wall.bmp");
	// Parsing into texture for OpenGL
	parseTexture(image, BOUNDARY);

	textureCollection.insert_or_assign(Empty, 1);
	image = loadTexture("assets/textures/floor.bmp");
	parseTexture(image, Empty);

	textureCollection.insert_or_assign(Wall, 1);
	image = loadTexture("assets/textures/tileWall.bmp");
	parseTexture(image, Wall);

	textureCollection.insert_or_assign(DoorClosed, 1);
	image = loadTexture("assets/textures/door.bmp");
	parseTexture(image, DoorClosed);

	textureCollection.insert_or_assign(SplashArt0, 1);
	image = loadTexture("assets/textures/splash0.bmp");
	parseTexture(image, SplashArt0);	
	
	textureCollection.insert_or_assign(SplashArt1, 1);
	image = loadTexture("assets/textures/splash1.bmp");
	parseTexture(image, SplashArt1);


}

static void initGLFlags() {

	glColor4f(1, 1, 1, 1);

	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_COLOR_MATERIAL);
	initTextures();

}

static void updateHUD() {

	toggleOverlayMode(true);
	toggleTransparency(true);

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
	std::string score = "Kills: " + std::to_string(player.kills);

	glColor3f(1, 1, 1);

	glRasterPos2f(25.0f, glutGet(GLUT_WINDOW_HEIGHT) - 100.0f);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, reinterpret_cast <const unsigned char*> (score.c_str()));


	if (player.health >= 70) glColor3f(0, 1, 0);
	else if (player.health >= 30) glColor3f(1, 1, 0);
	else glColor3f(1, 0, 0);


	glRasterPos2f(25.0f, glutGet(GLUT_WINDOW_HEIGHT) - 50.0f);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, reinterpret_cast <const unsigned char*> (hp.c_str()));


	// crosshair let it be a small cube for now.

	glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(glutGet(GLUT_WINDOW_WIDTH)/2.0f, glutGet(GLUT_WINDOW_HEIGHT)/2.0f, 1);
		glutWireCube(2);
	glPopMatrix();
	
	toggleOverlayMode(false);
	toggleTransparency(false);

}

static void devScreen() {

	toggleOverlayMode(true);

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

	if (cameraPosition.facing.north) cameraFace = "North";
	if (cameraPosition.facing.south) cameraFace = "South";
	if (cameraPosition.facing.east) cameraFace = "East";
	if (cameraPosition.facing.west) cameraFace = "West";

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
	
	toggleTransparency(true);

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

	toggleTransparency(false);
	toggleOverlayMode(false);


}

static int launch(int argc, char** argv) {

	std::cout << TITLE << std::endl;

	// Initializing position data.
	cameraPosition.x = 0.0;
	cameraPosition.y = 0.0;
	cameraPosition.z = 0.0;
	cameraPosition.pitch = 0.0;
	cameraPosition.yaw = 270.0; //facing negative X
	cameraPosition.facing.north = true;
	cameraPosition.identifier = Player;

	// Sound Engine config.
	soundEngine->setSoundVolume(MAX_VOLUME);

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
	initMenu();

	std::cout << "SHIBA is ready! Took " << elaspedTime << " ms." << std::endl;
	glutMainLoop();
	return 1;
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
		case 1:
			//pause();
			break;
		default:
			renderText(0, glutGet(GLUT_WINDOW_HEIGHT) / 2.0f, 1, 0, 0, "Error Assigning Scene. | Scene is null");
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

void showSplash(int splash) {

	glColor3f(1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, textureCollection.at(splash));


	// setting texture to repeat
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glEnable(GL_TEXTURE_2D);
	
	glBegin(GL_QUADS);

		glNormal3f(0, -1, 0);

		glTexCoord2f(0.0, 0.0);
		glVertex2f(0, 0);

		glTexCoord2f(1.0, 0.0);
		glVertex2f(glutGet(GLUT_WINDOW_WIDTH), 0);
		
		glTexCoord2f(1.0, 1.0);
		glVertex2f(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0, glutGet(GLUT_WINDOW_HEIGHT));
		
	glEnd();

	glDisable(GL_TEXTURE);
}

void menu() {

	toggleOverlayMode(true);
	toggleTransparency(true);
		
		int result = animateTex(SplashArt0);
		//	checking if animation playback has started.

		if (result == -1) {
			menuQueue.front().show();
			showSplash(SplashArt0);
		}
		else if (result == 1) currentScene = -1;
		else showSplash(SplashArt1);
		

	toggleTransparency(false);
	toggleOverlayMode(false);


}

// reads and writes for various game settings
void handleOptionInteraction(std::string option, int value = -1) {

	//TODO: Create a separate class system for MENUs
	if (option == "BACK") {
		std::cout << "menus before: " << menuQueue.size() << std::endl;

		//Putting the main menu back on queue before going to options.
		menuQueue.push(menuQueue.front());
		menuQueue.pop();

		std::cout << "menus after: " << menuQueue.size() << std::endl;

	}

	if (option == "FULLSCREEN") {

		glutFullScreenToggle();
		
	}

	if (option == "MOUSE SENSITIVITY") {

		std::cout << "Mouse speed set to: " << value << std::endl;
		mouseSpeed = value;

	}

	if (option == "MASTER SOUND VOLUME") {

		MAX_VOLUME = ((float)value) / 100.0;
		std::cout << "Volume set to: " << MAX_VOLUME << std::endl;
		soundEngine->setSoundVolume(MAX_VOLUME);
	}

	
	// List of options
	// 
	// Fullscreen	ON/OFF	default(off)
	// Cursor Sensitivity	0-10	Slider?	(default 5)
	// Cencorship	ON/OFF	default (off)
	// Master Sound Volume	0-100	Slider	(default 100)
	
	//renderText(0, glutGet(GLUT_WINDOW_HEIGHT) / 2, 1, 0, 0, "Options go here");


}

// Contains all the logic for displaying menu options and all
void handleMainMenuInteraction(std::string option, int value = -1) {

	//? Maybe make an exit confirmation?
	if (option == "EXIT") exit(0);

	if (option == "OPTIONS") {

		std::cout << "menus before: " << menuQueue.size() << std::endl;

		//Putting the main menu back on queue before going to options.
		menuQueue.push(menuQueue.front());
		menuQueue.pop();

		std::cout << "menus after: " << menuQueue.size() << std::endl;


	}

	if (option == "START GAME") {

		currentScene = -1;	//	remove this code later
		animateTex(SplashArt0, 600);
		// playing the crack sound.
		soundEngine->play2D(GLASS_CRACK);


	}

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
		if (objectCollection.at(i).texture && !wireframe) {

			glBindTexture(GL_TEXTURE_2D, textureCollection.at(objectCollection.at(i).color));

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glEnable(GL_TEXTURE_2D);

		}


		objectCollection.at(i).load();
		
		// Loads the object in the world.
		glDisable(GL_TEXTURE_2D);

		// loading bullets (if any)
		for (auto& b : bulletMap) b.second.loadGlutSolids();
		

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
	
	//Esc key
	case 27:
		currentScene = 0;
		track = false;
		track ? glutSetCursor(GLUT_CURSOR_NONE) : glutSetCursor(GLUT_CURSOR_INHERIT);
		break;

	case (int)'l':
		levelQueue.pop();
		initLevels(levelQueue);
		break;

	case (int)'z':

		int tpX, tpZ;
		std::cout << "Enter new tile cords:";
		std::cin >> tpX >> tpZ;

		std::cout << "Teleported to tile " << tpX << " " << tpZ << std::endl;
		cameraPosition.x = tpX * 10;
		cameraPosition.z = tpZ * 10;

		break;

	case (int)'m':
		/*std::cout << "SOUNDS STOPPED" << std::endl;
		soundEngine->removeAllSoundSources();*/

		playerInCombat = !playerInCombat;
		std::cout << "Combat: " << playerInCombat << std::endl;

		break;

	case (int)'q':

		for (int id : enemySpawnerLocations) {
			//triggering all enemy spawners at once
			//players can interact and trigger as well

			//! Don't allow players to interact with spawners. Only destroy.

			std::cout << "All enemy spawners triggered via hotkey" << std::endl;
			std::cout << "ID: " << id << std::endl;
			interactWithObj(id);	//This starts an animation.
			levelSpawning = true;
		}
		break;

	case (int)'[':
		player.health -= 10;
		break;

	case (int)']':
		player.health += 10;
		break;
	case (int)'/':

		int id;

		std::cout << "Enter Object ID:";
		std::cin >> id;

		objectCollection.at(id).color = Boss;

		break;
	case (int)'e':
		interactWithObj(cameraPosition.frontObject);
		break;

	case (int)'h':
		for (auto& item : enemyCollection) {
			item.second.health = 100;
		}
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

		movementSpeed = wireframe ? 0.5 : 2.0;
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
	case 101: //UP KEY
		cameraPosition.y += 0.10f;
		std::cout << cameraPosition.y << std::endl;
		break;
	case 102: //RIGHT KEY
		DevHudY += 10.00f;
		std::cout << DevHudY << std::endl;

		break;
	case 103: //DOWN KEY
		cameraPosition.y -= 0.10f;
		std::cout << cameraPosition.y << std::endl;
		break;
	case 100: //LEFT KEY
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
		if (state && track) shoot(cameraPosition);
		// else do menu stuff
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
		if (cameraPosition.yaw + dev_x / mouseSpeed >= 360.0) cameraPosition.yaw += (float)dev_x / mouseSpeed - 360.0;
		else cameraPosition.yaw += (float)dev_x / mouseSpeed;

		if (cameraPosition.yaw + dev_x / mouseSpeed < 0) cameraPosition.yaw += (float)dev_x / mouseSpeed + 360.0;
		else cameraPosition.yaw += (float)dev_x / mouseSpeed;

		// Setting pole orientation
		Poles finalFacing = { false,false,false,false };
		if (cameraPosition.yaw < 270.0 + 45 && cameraPosition.yaw > 270.0 - 45) finalFacing.north = true;
		else if (cameraPosition.yaw < 180 + 45 && cameraPosition.yaw > 180 - 45) finalFacing.east = true;
		else if (cameraPosition.yaw < 90 + 45 && cameraPosition.yaw > 90 - 45) finalFacing.south = true;
		else finalFacing.west = true;

		cameraPosition.pitch += (float)dev_y / mouseSpeed;
		cameraPosition.facing = finalFacing;

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
	enemySpawnerLocations.clear();

	int size = array_size(*queue.front().levelGrid);
	float x = 0.0, y = GROUNDLEVEL, z = 0.0;

	if (DEBUGMODE) std::cout << "[Method] initLevels: Number of levels is " << queue.size() << std::endl;

	levelQueue = queue;
	std::cout << "Initializing Level..." << std::endl;

	int repeatScale = 1;

	// Columns (X)
	for (int actualX = 0; actualX < size; actualX++) {

		// Rows (Z)
		for (int actualZ = size - 1; actualZ > -1; actualZ--) {

			y = GROUNDLEVEL;

			// adding to spawn collection if it's a valid spawn loaction.
			if (levelQueue.front().levelGrid[actualZ][actualX] == SpawnLoc)
				addSpawns((size - 1) - actualZ, actualX);

			// Points are below.
			// Setting origin of object.
			ShibaObject tile(x, y, z);

			//! IN CASE THINGS BREAK REMOVE THIS CODE:
			//! 
			//tile.objectName = std::to_string(levelQueue.front().levelGrid[actualZ][actualX]);
			tile.objectName = std::to_string(actualZ) + std::to_string(actualX);

			// saving the color (not necessary when loading textures)
			tile.color = queue.front().levelGrid[actualZ][actualX];

			if (tile.color != Custom || levelQueue.front().customObjects.empty()) {
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
				if (tile.color == DoorClosed || tile.color == DoorOpen) {
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

			}
			else {

				tile.vertexCol.push_back({ x, 0, z, {0, 1.0f, 0} });
				tile.setLoadGlutFunction(levelQueue.front().customObjects.at(0).glutSolids);
				
			}

			// finally add the object to the level collection.
			objectCollection.push_back(tile);

			x += TILESIZE * 2;
		}

		z += TILESIZE * 2;
		levelBounds.x = x;
		x = 0.0;

	}

	levelBounds.z = z;

	//objectCollection.shrink_to_fit();
	//possibleSpawns.shrink_to_fit();
	resetCamera();

}

void renderGameElements() {

	//	Locks or unlocks the cursor
	if (track) glutWarpPointer(WIDTH / 2, HEIGHT / 2);

	// Game sound management
	if (playerInCombat) {

		// assigning new sound if nothing is there.
		if (!combatMusic) combatMusic = soundEngine->play2D(BATTLE_MUSIC_0, false, true, true);

		//start audio
		combatMusic->setIsPaused(false);

		// checking if the build up is done playing
		if (combatMusic->isFinished()) 
			combatMusic = soundEngine->play2D(BATTLE_MUSIC_1, true, false, true);

	}
	else {

		//checking which sound is currently loaded
		//if sound 0, go to next part then fade it out.
		if (combatMusic->getSoundSource()->getName() == BATTLE_MUSIC_0->getName()) {
			
			// Set to sound 1.
			if (combatMusic->isFinished()) 
				combatMusic = soundEngine->play2D(BATTLE_MUSIC_1, false, false, true);

		}
		else if (combatMusic->getPlayPosition() != 0 || combatMusic->getPlayPosition() != -1) {

			float i = (float) animateTex(0, 1000, true);
			
			// checking if fade out or sound has finished playing
			if ((int) i != 1) combatMusic->setVolume(i / 1000.0f);

			if (combatMusic->isFinished() || (int) i == 1) {
				combatMusic = soundEngine->play2D(BATTLE_MUSIC_0, false, true, true);

				//std::cout << "Sound finished" << std::endl;

			}

			//std::cout << "No combat | current volume: " << combatMusic->getVolume() << " : " << i << std::endl;
		}
		

		
	}
	

	camera();	//player
	draw();		//level/world
	checkForInteraction(cameraPosition);

	enemyPathing();

	//shows engine stats. ALWAYS on top
	devScreen();

	// Player HUD elements
	updateHUD();
	

}

void camera() {

	float newX = 0.0, newZ = 0.0;

	// Calculate new coordinate depending on key directions held.
	if (motion.Forward) {
		newX += cos((cameraPosition.yaw + 90) * TO_RADIANS) / movementSpeed;
		newZ -= sin((cameraPosition.yaw + 90) * TO_RADIANS) / movementSpeed;
	}

	if (motion.Backward) {
		newX += cos((cameraPosition.yaw + 90 + 180) * TO_RADIANS) / movementSpeed;
		newZ -= sin((cameraPosition.yaw + 90 + 180) * TO_RADIANS) / movementSpeed;
	}

	if (motion.Left) {
		newX += cos((cameraPosition.yaw + 90 + 90) * TO_RADIANS) / movementSpeed;
		newZ -= sin((cameraPosition.yaw + 90 + 90) * TO_RADIANS) / movementSpeed;
	}

	if (motion.Right) {
		newX += cos((cameraPosition.yaw + 90 - 90) * TO_RADIANS) / movementSpeed;
		newZ -= sin((cameraPosition.yaw + 90 - 90) * TO_RADIANS) / movementSpeed;
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

	// Level boundary check
	if (
		(finalX < 0 || finalX >(array_size(*levelQueue.front().levelGrid) - 1))
		||
		(finalZ < 0 || finalZ >(array_size(*levelQueue.front().levelGrid) - 1))
		) tileVal = Wall;
	else tileVal = levelQueue.front().levelGrid[finalX][finalZ];

	// X Axis check
	if (
		(direction == 1)
		&& //checking if within bounds
		(tileVal == Wall || tileVal == Boss || tileVal == Custom || tileVal == DoorClosed || tileVal == EnemySpawner || tileVal == Objective)
		) return false; //stopping movement in that axis.


	// Z Axis check
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
void checkForInteraction(Position& entity) {


	int size = array_size(*levelQueue.front().levelGrid);
	int xModifier = 1, zModifier = 20;
	//int worldX = cameraPosition.toTile().x, worldZ = cameraPosition.toTile().z;

	int id = (((entity.toTile().z) * size) + (entity.toTile().x));

	// out of bounds prevention
	if (entity.facing.north) {	// X+
		// worldX++;
		id += xModifier;
	}
	else if (entity.facing.south) {	// X-
		// worldX--;
		id -= xModifier;
	}
	else if (entity.facing.east) {	// Z+
		// worldZ++;
		id += zModifier;
	}
	else if (entity.facing.west) {	// Z- (West) 
		// worldZ--;
		id -= zModifier;
	}
	// not using else because this function can also be used to get the object ID of the tile you're standing on.

	if (id < 0 || id >= (size * size)) id = -1;

	if ((entity.facing.east && entity.toTile().z >= size - 1) || (entity.facing.west && entity.toTile().z <= 0)) id = -1;

	if ((entity.facing.north && entity.toTile().x >= size - 1) || (entity.facing.south && entity.toTile().x <= 0)) id = -1;

	// finally setting the object
	entity.frontObject = id;


}

void interactWithObj(int objectID) {

	//! TODO Check if player is trying to interact with object, in which case don't let them or make them do a melee attack.

	int size = array_size(*levelQueue.front().levelGrid);

	if (objectID == -1) {
		std::cout << "No interaction available" << std::endl;
		return;
	}

	std::cout << "Interacting with: ";
	std::cout << objectID << " X: ";

	std::cout << objectCollection.at(objectID).tileX << " Z: ";
	std::cout << objectCollection.at(objectID).tileZ << std::endl;

	queueAnimation(objectID, ((size - 1) - objectCollection.at(objectID).tileX), objectCollection.at(objectID).tileZ);

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
		target.y = -18.0f;	//door open position.

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
		GLfloat ambient[] = { 0.30f, 0.30f, 0.30f, 1.0f };  // Adjust ambient brightness
		GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Adjust diffuse brightness

		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	}
	else {
		glDisable(GL_LIGHT0);
	}

}

void bulletModel(ShibaObject a) {

	// any custom object must have a single ShibaQuad point as center.
	ShibaQuad center;
	center = a.vertexCol.at(0);

	// moving the render point to elsewhere
	glPushMatrix();
	glColor3f(1, 0, 0);	//bullets are red for now
	glTranslatef(
		center.x + a.offset.x,
		center.y + a.offset.y,
		center.z + a.offset.z);
	glutSolidCube(0.5f);
	glPopMatrix();
	
}

// handles bullet animation and collision
void bulletPhysics() {

	if (bulletMap.empty()) return;

	//std::cout << "Bullets in motion: " << bulletMap.size() << std::endl;
	
	std::unordered_map <std::string, ShibaObject> tempMap = bulletMap;

	for (auto& item : tempMap) {

		ShibaQuad current = {
			item.second.vertexCol.at(0).x,
			item.second.vertexCol.at(0).y,
			item.second.vertexCol.at(0).z
		};

		//float newX = cos((cameraPosition.yaw + 90) * TO_RADIANS) * TRAVEL;
		//float newZ = sin((cameraPosition.yaw + 90) * TO_RADIANS) * TRAVEL;
		//float newY = tan((cameraPosition.pitch) * TO_RADIANS) * TRAVEL;

		Position target, difference;
		target.x = (cos((item.second.offset.yaw + 90.0f) * TO_RADIANS)) * ANIMATIONSTEP;
		target.z = -(sin((item.second.offset.yaw + 90.0f) * TO_RADIANS)) * ANIMATIONSTEP;
		target.y = tan((item.second.offset.pitch) * TO_RADIANS) * ANIMATIONSTEP;

		//difference = target - current.toPosition();

		// For now making bullets slow.
		item.second.offset += target;

		// Updating coords to new position
		item.second.updateTileCoords();
		item.second.objectName = std::to_string(item.second.tileX) + std::to_string(item.second.tileZ);

		// update tile ID for bullet.
		bulletMap.erase(item.first);
		bulletMap.insert_or_assign(item.second.objectName, item.second);

		int finalX = (array_size(*levelQueue.front().levelGrid) - 1) - item.second.tileX;


		// Check if bullet collides with any walls or obstacles here
		if (levelQueue.front().levelGrid[finalX][item.second.tileZ] == Wall
			|| 
			levelQueue.front().levelGrid[finalX][item.second.tileZ] == Objective
			||
			levelQueue.front().levelGrid[finalX][item.second.tileZ] == Custom
			) {
			//if (levelQueue.front().levelGrid[finalX][item.second.tileZ] == EnemySpawner) 
			// Implement this later
			bulletMap.erase(item.second.objectName);
			//std::cout << "Bullet killed by wall in tile (X:Z):  " << item.second.tileX << " : " << item.second.tileZ << std::endl;
			continue;	// No need to check for level bound collisions afterwards
		}

		if (levelQueue.front().levelGrid[finalX][item.second.tileZ] == DoorClosed) {

			int size = array_size(*levelQueue.front().levelGrid);
			int id = ((item.second.tileZ * size) + (item.second.tileX));

			if (id < 0 || id >= (size * size)) 
				id = -1;

			std::cout << "Bullet hit door:  " << item.second.tileX << " : " << item.second.tileZ << std::endl;

			interactWithObj(id);
		}

		//bullets will be deleted once they hit the map edge.
		if (current.toPosition().x + item.second.offset.x < 0.0f - TILESIZE || current.toPosition().x + item.second.offset.x > levelBounds.x
			|| current.toPosition().z + item.second.offset.z < 0.0f - TILESIZE || current.toPosition().x + item.second.offset.z > levelBounds.z) {
			
			bulletMap.erase(item.second.objectName);
			//std::cout << "Bullet killed in tile (X:Z):  " << item.second.tileX << " : " << item.second.tileZ << std::endl;
		}

	}

	tempMap.clear();

}

void shoot(Position entity) {

	// calculating next coord.
	ShibaObject bullet(entity.x, entity.y, entity.z);

	bullet.objectName = std::to_string(bullet.tileX) + std::to_string(bullet.tileZ);
	bullet.offset.identifier = entity.identifier;

	// adding only 1 quad as the object center/bullet hitbox
	bullet.vertexCol.push_back({ entity.x, entity.y, entity.z, {0, 1.0f, 0} });

	// bind the bullet model to the object.
	bullet.setLoadGlutFunction(bulletModel);

	bullet.offset.yaw = entity.yaw;
	bullet.offset.pitch = entity.pitch;

	// adding to collection to later iterate through using ID
	bulletMap.insert_or_assign(bullet.objectName, bullet);

}

void enemyPathing() {

	//	Spawn new enemies if needed.
	//	change this to spawn enemies proportional to game level and amount of spawners.
	if (levelSpawning && enemyCollection.size() < 1) {

		int id = enemySpawnerLocations.at(randomInt(0, enemySpawnerLocations.size() - 1));

		float spawnerX = objectCollection.at(id).tileX * 10;
		float spawnerZ = objectCollection.at(id).tileZ * 10;
		
		//	making a list of empty tiles the enemy can get out to from the spawner.
		std::vector <int> emptySpots;
		
		Position tempEnemy;
		tempEnemy.x = spawnerX;
		tempEnemy.z = spawnerZ;

		for (int i = 0; i < 4; i++) {

			tempEnemy.facing = { i == 0, i == 1, i == 2, i == 3 };
			checkForInteraction(tempEnemy);


			if (tempEnemy.frontObject != -1 && objectCollection.at(tempEnemy.frontObject).color == Empty) {
					
				tempEnemy.facing.printFacing();
				emptySpots.push_back(tempEnemy.frontObject);
				//std::cout << "\nFront ID " << tempEnemy.frontObject << std::endl;

			}

		}

		if (emptySpots.size() == 0) {
			std::cout << "\nError for spawner in X: " << spawnerX << " Z:" << spawnerZ << " ObjectID: " << id;
			std::cout << "\nNo free space available for enemies to spawn. Skipping this spawner" << std::endl;
		}
		else {

			//std::cout << emptySpots.size() << std::endl;
			int randID = emptySpots.size() == 1 ? 0 : randomInt(0, emptySpots.size() - 1);

			spawnerX = objectCollection.at(emptySpots.at(randID)).tileX * 10;
			spawnerZ = objectCollection.at(emptySpots.at(randID)).tileZ * 10;

			ShibaObject tempEnemy(
				spawnerX, GROUNDLEVEL, spawnerZ
			);
			tempEnemy.offset.identifier = Enemy;

			tempEnemy.vertexCol.push_back({ 
				spawnerX, GROUNDLEVEL, spawnerZ, {0.0, -1.0f, 0.0} 
			});

			tempEnemy.objectName = std::to_string(tempEnemy.vertexCol.at(0).x) + std::to_string(tempEnemy.vertexCol.at(0).z);
			tempEnemy.setLoadGlutFunction(enemyModel);

			// initialize with with a path towards player. ONLY if they're within 5 tiles
			tempEnemy.pathing = aStarImplementation(tempEnemy, cameraPosition, RANGED_ENEMY_DETECTION_RANGE);

			//tempEnemy.pathing.push(tempEnemy.vertexCol.at(0).translateTile(DOWN));
			//tempEnemy.pathing.push(tempEnemy.vertexCol.at(0).translateTile(RIGHT));
			//tempEnemy.pathing.push(tempEnemy.vertexCol.at(0).translateTile(UP));
			//tempEnemy.pathing.push(tempEnemy.vertexCol.at(0).translateTile(LEFT));
			//tempEnemy.pathing.push(tempEnemy.vertexCol.at(0).translateTile(DOWN));

			//tempEnemy.loopPath = true;

			enemyCollection.insert_or_assign(tempEnemy.objectName, tempEnemy);
			//std::cout << "Added enemy: " << tempEnemy.objectName << std::endl;
		}


	}

	// Enemy path finding algo goes here
	// exit if there's nothing to iterate

	if (enemyCollection.empty()) return;

	for (auto& item : enemyCollection) {

		// checking if pathing is empty.
		if (!item.second.pathing.empty()) {

			ShibaQuad front = item.second.pathing.front();
			ShibaQuad current = {
				item.second.vertexCol.at(0).x,
				item.second.vertexCol.at(0).y,
				item.second.vertexCol.at(0).z
			};

			Position difference = front.toPosition() - current.toPosition();

			if (difference.x < 0) item.second.offset.facing.south = true;
			else if (difference.x > 0) item.second.offset.facing.north = true;
			else if (difference.z < 0) item.second.offset.facing.west = true;
			else if (difference.z > 0) item.second.offset.facing.east = true;

			item.second.offset += (difference / (ANIMATIONSTEP * 10.0f));

			//TODO  the concept works but the forumla is incorrect.
			
			//float adjacent, opposite, theta;

			//adjacent = (item.second.offset.x + current.x) - cameraPosition.x;
			//opposite = (item.second.offset.z + current.z) - cameraPosition.z;

			//theta = tan(fabs(opposite) / fabs(adjacent));

			//item.second.offset.yaw = theta * 360;

			//if (lastKey == (int)'x') {
			//	std::cout << theta * 360 << std::endl;

			//	shoot(item.second.getRawCoords());
			//	lastKey = -1;
			//}

			if ((front.toPosition() - (current.toPosition() + item.second.offset)).absolute() <= 0.01f) {

				item.second.vertexCol.at(0) = item.second.pathing.front();
				item.second.offset = { 0.0f, 0.0f, 0.0f };

				if (item.second.loopPath)
					item.second.pathing.push(front);

				item.second.pathing.pop();
			}

		}

		item.second.updateTileCoords();
		
		std::string id = std::to_string(item.second.tileX) + std::to_string(item.second.tileZ);
		item.second.loadGlutSolids();
		
		// collision check for enemies with bullets
		if (bulletMap.count(id) != 0 && bulletMap.at(id).offset.identifier != item.second.offset.identifier) {
			
			//std::cout << "BULLET" << std::endl;
			//std::cout << "x: " << bulletMap.at(id).getRawCoords().x << "\ty: " << bulletMap.at(id).getRawCoords().y << "\tz:" << bulletMap.at(id).getRawCoords().z <<std::endl;

			//std::cout << "ENEMY" << std::endl;
			//std::cout << "x: " << item.second.getRawCoords().x << "\ty: " << item.second.getRawCoords().y << "\tz:" << item.second.getRawCoords().z << std::endl;

			bool xPass1 = bulletMap.at(id).getRawCoords().x >= item.second.getRawCoords().x - 5.0f;
			bool xPass2 = bulletMap.at(id).getRawCoords().x <= item.second.getRawCoords().x + 5.0f;
																							  
			bool zPass1 = bulletMap.at(id).getRawCoords().z >= item.second.getRawCoords().z - 5.0f;
			bool zPass2 = bulletMap.at(id).getRawCoords().z <= item.second.getRawCoords().z + 5.0f;

			bool yPass1 = bulletMap.at(id).getRawCoords().y <= item.second.getRawCoords().y + 10.0f;
			bool yPass2 = bulletMap.at(id).getRawCoords().y >= item.second.getRawCoords().y;

			//std::cout << "PASSES:" << std::endl;
			//std::cout << "xPass 1: " << xPass1 << std::endl;
			//std::cout << "xPass 2: " << xPass2 << std::endl;

			//std::cout << "yPass 1: " << yPass1 << std::endl;
			//std::cout << "yPass 2: " << yPass2 << std::endl;

			//std::cout << "zPass 1: " << zPass1 << std::endl;
			//std::cout << "zPass 2: " << zPass2 << std::endl;


			// if it exists, check if within hitbox of enemy.
			// !TODO: X axis detection doesn't work properly for some reason. FIX it
			// !TODO: Switch to melee attack if enemy and player are in the same tile.
			if (xPass1 && xPass2 && zPass1 && zPass2 && yPass1 && yPass2) {
				//std::cout << "HIT" << std::endl;
				item.second.health -= PLAYER_DAMAGE;

				if (item.second.health <= 0) {
					enemyCollection.erase(item.first);
					player.kills++;
					return;
				}

				if (!BULLET_PIERCING) bulletMap.erase(id);
					
			}

		}


		//updating range data.
		aStarImplementation(item.second, cameraPosition, RANGED_ENEMY_DETECTION_RANGE);


	}


}

std::queue<ShibaQuad> aStarImplementation(ShibaObject& entity, Position& goal, int range) {
	
	// enemy range for detection.
	std::vector <int> objectIDRange;

	std::vector <int> pathID;
	std::queue <ShibaQuad> rr;

	Position tempPos = entity.getRawCoords();

	// SHOOT a bullet trajectory towards player, if there's any obstacles between enemy and player within the trajectory. Then visible (green) else in range for detection (blue)
	
	/*
	// taking the enemy towards the edge of the range
	tempPos.z -= range * 10;

	//	Only check north and south sides.
	for (int r = 0; r < range * 2 + 1; r++) {

		// adding currently standing tile.
		tempPos.facing = { false, false, false, false };
		checkForInteraction(tempPos);

		for (int i = 0; i < 2; i++) {

			if (tempPos.frontObject != -1 && objectCollection.at(tempPos.frontObject).color != Wall &&
				objectCollection.at(tempPos.frontObject).color != Boss && objectCollection.at(tempPos.frontObject).color != Objective &&
				objectCollection.at(tempPos.frontObject).color != EnemySpawner)
				objectIDRange.push_back(tempPos.frontObject);

			tempPos.facing = { i == 0 , i == 1, false, false };
		
			// adding either north or south from current tile times range.
			for (int j = 0; j < range; j++) {

				if (i) tempPos.x -= 10 * j;
				else tempPos.x += 10 * j;

				checkForInteraction(tempPos);

				if (tempPos.frontObject != -1 && objectCollection.at(tempPos.frontObject).color != Wall &&
					objectCollection.at(tempPos.frontObject).color != Boss && objectCollection.at(tempPos.frontObject).color != Objective &&
					objectCollection.at(tempPos.frontObject).color != EnemySpawner)
					objectIDRange.push_back(tempPos.frontObject);

				tempPos.x = entity.getRawCoords().x;
			}


		}

		tempPos.z += 10;

	}
	*/

	tempPos.moveBackward();

	for (int i = 0; i <= range; i++) {

		// add the current tile.
		checkForInteraction(tempPos);

		if (tempPos.frontObject != -1)
			objectIDRange.push_back(tempPos.frontObject);

		tempPos.moveForward();

		//// count * range + 1 for cone of vision
		//for (int j = i * range + 1; j > 0; j++) {


		//}

	}
	
	tempPos = entity.getRawCoords();
	tempPos.facing = { false, false, false, false };
	checkForInteraction(tempPos);

	// temporary variable for tracing out and backtracking
	Position trace = tempPos;

	pathID.push_back(tempPos.frontObject);


	// using this as a stack to construct pathing from Source to Goal.
	std::vector <Path> pathStack;

	// making a map to store all moves possible from each tile.
	std::unordered_map <std::string, Path> pathMap;


	//adding the current position as the SOURCE tile.
	Path source = {
		std::to_string(trace.toTile().x) + std::to_string(trace.toTile().z),	//ID
		pathStack.size() + abs(trace.toTile().x - cameraPosition.toTile().x) + abs(trace.toTile().z - cameraPosition.toTile().z)	// Cost = current - goal + heuristics.
		- 1, //direction
		true,	//visited = true
		trace.frontObject
	};

	pathMap.insert_or_assign(source.id, source);
	// heuristics increases as number of steps needed does.
	int heuristics;
	int visitedTiles = 1;

	// recursively add paths to the map as the enemy explores the tiles.
	// termination condition: If all of the paths are visited and we're back in the source. There is no path to goal.
	// loop runs indefinite number of times. Will only terminate once num of visited tiles == size of map.
	std::cout << "Tracing path to player...." << std::endl;

	int iterations = 0;

	while (iterations < 15) {

		heuristics = pathStack.size();
		int lowestCost = -1;
		std::string lowestCostID;

		// For now ignoring obstacles and just tracing a path to player position.
		// adding possible moves from current postion to map by looking around all poles.
		for (int i = 0; i < 4; i++) {

			trace.facing = { i == 0, i == 1, i == 2, i == 3 };
			checkForInteraction(trace);

			if (trace.frontObject != -1) {	// TODO: Add other obstacle checks here

				// creating a new path tile.
				int diffX = objectCollection.at(trace.frontObject).tileX - cameraPosition.toTile().x;
				int diffZ = objectCollection.at(trace.frontObject).tileZ - cameraPosition.toTile().z;

				std::string id = objectCollection.at(trace.frontObject).objectName;

				int direction = trace.facing.toDirection();
				int cost;

				// calculating cost.
				cost = abs(diffX) + abs(diffZ);

				Path p = { id, cost, direction, false, trace.frontObject };

				//std::cout << trace.frontObject << " : " << " Cost: " << cost << " Direction: " << direction << std::endl;
				//std::cout <<  "dX" << ":" << "dZ: " << diffX << " : " << diffZ << std::endl;

				// finally adding as a path for selection if it doesn't exist already.
				if (pathMap.count(id) == 0) pathMap.insert_or_assign(id, p);
				
				// store the lowest cost among the available spots.
				if (lowestCost == -1 || lowestCost > cost) {
					lowestCost = cost;
					lowestCostID = id;
				}
				
			}

		}

		//std::cout << "Lowest Cost: " << lowestCost << std::endl;
		//std::cout << "Lowest Cost ID: " << lowestCostID << std::endl;

		bool backtrack = false;

		if (lowestCostID.empty() && pathMap.at(lowestCostID).visited) backtrack = true;
			
		// backtracking to previous tile by reversing the direction travelled.
		std::cout << "going " << pathMap.at(lowestCostID).direction << " Cost:" << pathMap.at(lowestCostID).cost << std::endl;
		switch (pathMap.at(lowestCostID).direction) {

			case UP:
				trace.moveForward();
				
				break;

			case DOWN:
				trace.moveBackward();
				
				break;

			case LEFT:
				trace.moveLeft();
				
				break;

			case RIGHT:
				trace.moveRight();
				
				break;
		}



		// marking as visited
		pathMap.at(lowestCostID).visited = true;
		pathStack.push_back(pathMap.at(lowestCostID));

		// removing last path tile if it was a backtrack.
		if (backtrack) pathStack.pop_back();
		
		// moving the trace point
		
		// checking if we arrived at the destination
		//if (!pathStack.empty() && pathStack.back().cost == 0) break;

		iterations++;

	}

	std::cout << "Path trace complete." << std::endl;
	
	for (Path i : pathStack) pathID.push_back(i.objectID);
	

	if (!objectIDRange.empty()) entity.rangeIDCol = objectIDRange;
	if (!pathID.empty()) entity.pathIDCol = pathID;

	return rr;
	
}

void enemyModel(ShibaObject a) {

	float enemyWidth = 5;

	// any custom object must have a single ShibaQuad point as center.
	ShibaQuad center;

	center = a.vertexCol.at(0);

	//	For now rendering a white plane as enemy. Apply texture over it later.
	if (a.health <= 10) glColor3f(1, 0, 0);
	else if (a.health <= 50) glColor3f(1, 1, 0);
	else glColor3f(1, 1, 1);

	glPushMatrix();

		glTranslatef(
			center.x + a.offset.x,
			center.y + a.offset.y, 
			center.z + a.offset.z
		);
		glRotatef(cameraPosition.yaw + 90, 0, 1, 0);	//rotate the object with respect to camera yaw

		glBegin(GL_QUADS);
			glNormal3f(0, -1, 0);

			glVertex3f(0, 0, -enemyWidth / 2.0f);
			glVertex3f(0, 0, enemyWidth / 2.0f);
			glVertex3f(0, 10, enemyWidth / 2.0f);
			glVertex3f(0, 10, -enemyWidth / 2.0f);

		glEnd();

	glPopMatrix();

	// drawing range of enemies
	if (!a.rangeIDCol.empty()) for (int i : a.rangeIDCol) {

			// checking if player is within range.
			if (objectCollection.at(i).tileX == cameraPosition.toTile().x && objectCollection.at(i).tileZ == cameraPosition.toTile().z) {
				glColor3f(1, 0, 0);
				int res = animateTex(EnemySpawner, 120);
				if (res == 1) player.health -= 1;
			}
			else {
				glColor3f(0, 1, 0);
			}

			glPushMatrix();
				
				
				glTranslatef(objectCollection.at(i).tileX * 10, GROUNDLEVEL, objectCollection.at(i).tileZ * 10);
				
				glPushMatrix();
						glScalef(1, 0.01, 1);
						glutSolidCube(TILESIZE * 2);
				glPopMatrix();


			glPopMatrix();
		}

	// drawing path enemy will is going to take.
	if (!a.pathIDCol.empty()) for (int i : a.pathIDCol) {

		glColor3f(0,0,1);
		glPushMatrix();

		glTranslatef(objectCollection.at(i).tileX * 10, 5.0f, objectCollection.at(i).tileZ * 10);

			glPushMatrix();
				glScalef(1, 0.01, 1);
				glutSolidCube(TILESIZE * 2);
			glPopMatrix();

		glPopMatrix();
	}


}

