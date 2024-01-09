#pragma once
#include "Config.h"

struct Motion {
	bool Forward, Backward, Left, Right;

	bool isMoving() const {
		return (this->Forward || this->Backward || this->Left || this->Right);
	}

};


enum gameState {
	GAME_OVER = -1,
	RESET_ENGINE = 0,
	GAME_FINISHED = 1,
	GAME_OPTIONS = 2,
	GAME_PAUSED = 3,
	GAME_EXIT = 4
};

enum AnimID {

	// Enemy texture ids
	ENEMY_IDLE_STATE_0 = 100,
	ENEMY_IDLE_STATE_1 = 101,
	ENEMY_IDLE_STATE_2 = 102,
	ENEMY_IDLE_STATE_3 = 103,
	ENEMY_IDLE_STATE_4 = 104,
	ENEMY_IDLE_STATE_5 = 105,

	SPLASH_ART_GLOW = 200,
	SPLASH_BROKEN_GLASS = 201,
	SplashArt0 = 202,
	SplashArt1 = 203,

	SPAWNER_1 = 301,
	SPAWNER_2 = 302,
	SPAWNER_3 = 303,

};

enum tileID {
	Enemy = -5,
	Player = -4,
	SpawnLoc = -1,
	Empty = 0,
	EnemySpawner = 1,
	Objective = 2,
	ObjectiveCollected = 20,
	Notes = 3,
	Wall = 4,
	DoorClosed = 5,
	DoorOpen = -5,
	Boss = 6,
	Custom = 7,
	LevelExit = 8,
	LevelExitOpen = 80,
	Bullet = 10
};

enum Directions {
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3
};

struct Path {

	std::string id;
	int cost;
	int direction;
	bool visited = false;
	int objectID = -1;

};

struct Poles {
	bool north = false, south = false, east = false, west = false;

	void printFacing() const {
		std::cout << "N: " << north;
		std::cout << " S: " << south;
		std::cout << " E: " << east;
		std::cout << " W: " << west << std::endl;
	}

	//	returns UP/DOWN/LEFT or RIGHT depending on where the entity is facing.
	int toDirection() const {

		if (north) return UP;
		else if (south) return DOWN;
		else if (east) return RIGHT;
		else if (west) return LEFT;

	}
};

struct Position {

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	float yaw = 0.0;
	float pitch = 0.0;
	int identifier;

	// used for checking if a specific button is pressed.
	int button = -1;
	int buttonState = -1;
	int frontObject = -1;

	// north, south, east, west. All false by default
	Poles facing;
	Motion motion = { false,false,false,false };

	bool operator == (const Position& a) {
		if (x == a.x && y == a.y &&
			z == a.z) return true;
		else return false;
	}

	Position& operator -= (const Position& a) {

		this->x -= a.x;
		this->y -= a.y;
		this->z -= a.z;
		return *this;
	}	

	Position& operator += (const Position& a) {

		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}

	bool operator <= (const float& a) {

		if (x <= a &&
			y <= a &&
			z <= a) return true;
		return false;
	}

	Position& operator / (const float& a) {

		this->x /= a;
		this->y /= a;
		this->z /= a;

		return *this;
	}

	Position& operator - (const float& a) {

		x -= a;
		y -= a;
		z -= a;

		return *this;
	}

	Position& operator - (const Position& a) {

		x -= a.x;
		y -= a.y;
		z -= a.z;

		return *this;
	}	

	Position& operator + (const Position& a) {

		x += a.x;
		y += a.y;
		z += a.z;

		return *this;
	}

	Position& operator += (const float& a) {

		x += a;
		y += a;
		z += a;

		return *this;
	}

	Position& absolute() {

		x = fabs(x);
		y = fabs(y);
		z = fabs(z);

		return *this;

	}

	Position toTile() {
		Position temp;

		temp.x = round(x / 10);
		temp.z = round(z / 10);
		temp.y = round(y / 10);

		return temp;

	}

	std::string toString() {
		return (
			"X: " + std::to_string(this->x) +
			" Y: " + std::to_string(this->y) +
			" Z: " + std::to_string(this->z) +
			" Yaw: " + std::to_string(this->yaw) +
			" Pitch: " + std::to_string(this->pitch)
			);
	}

	Position& moveForward() {
		if (facing.north) this->x += TILESIZE * 2;
		else if (facing.south) this->x -= TILESIZE * 2;
		else if (facing.east) this->z += TILESIZE * 2;
		else if (facing.west) this->z -= TILESIZE * 2;
		return *this;
	}

	Position& moveBackward() {
		if (facing.north) this->x -= TILESIZE * 2;
		else if (facing.south) this->x += TILESIZE * 2;
		else if (facing.east) this->z -= TILESIZE * 2;
		else if (facing.west) this->z += TILESIZE * 2;
		return *this;
	}

	Position& moveLeft() {
		if (facing.north) this->z -= TILESIZE * 2;
		else if (facing.south) this->z += TILESIZE * 2;
		else if (facing.east) this->x += TILESIZE * 2;
		else if (facing.west) this->x -= TILESIZE * 2;
		return *this;
	}

	Position& moveRight() {
		if (facing.north) this->z += TILESIZE * 2;
		else if (facing.south) this->z -= TILESIZE * 2;
		else if (facing.east) this->x -= TILESIZE * 2;
		else if (facing.west) this->x += TILESIZE * 2;
		return *this;
	}
};

struct ShibaQuad {

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	float normal[3];

	ShibaQuad& translateTile(const int direction) {

		switch (direction) {

			case UP: this->x += 10.0f; break;

			case DOWN: this->x -= 10.0f; break;

			case LEFT: this->z -= 10.0f; break;

			case RIGHT: this->z += 10.0f; break;

		}

		return *this;

	}

	ShibaQuad& operator - (const ShibaQuad& a) {

		x -= a.x;
		y -= a.y;
		z -= a.z;

		return *this;
	}

	ShibaQuad& operator / (const ShibaQuad& a) {

		this->x /= a.x;
		this->y /= a.y;
		this->z /= a.z;

		return *this;
	}

	Position toPosition() const {
		Position obj;

		obj.x = this->x;
		obj.y = this->y;
		obj.z = this->z;

		return obj;

	}

	std::string toString() {
		return (
			"X: " + std::to_string(this->x) +
			" Y: " + std::to_string(this->y) +
			" Z: " + std::to_string(this->z));
	}



};


//This function is needed by ShibaObject, so declaring it first.
static void getColorMod(int id) {

	switch (id) {
	case SpawnLoc:	//floor: Grey
		glColor3f(0.5, 0.5, 0.5);
		break;
	case Empty:		//spawn points: Purple
		glColor3f(0.6, 0.2, 0.6);
		break;
	case EnemySpawner:		//enemy spawner: Red
		glColor3f(1.0, 0.0, 0.0);
		break;
	case Objective:		//Objectives: Blue
		glColor3f(0.0, 0.0, 1.0);
		break;	
	case ObjectiveCollected:		//Objectives collected: dark blue
		glColor3f(0, 0, 0.5);
		break;
	case Notes:		//Notes: Cyan
		glColor3f(0.0, 1, 1);
		break;
	case Wall:		//Walls : Grey
		glColor3f(1.0, 1.0, 1.0);
		break;
	case DoorClosed:		//Doors: Yellow
	case DoorOpen:		//Doors: Yellow
		glColor3f(1.0, 1.0, 0.0);
		break;
	case Boss:		//Boss Location: Orange
		glColor3f(1.0, 0.60, 0.0);
		break;
	case LevelExit:	//white
		glColor3f(1.0, 1.0, 1.0);	
		break;
	case Custom:		//Custom 3D model unique to map: Green
		glColor3f(0.0, 1.0, 0.0);
		break;
	default:
		glColor3f(0.0, 0.0, 0.0);
		break;
	}
}

// Custom Object Class. All vertexes must be drawn anti-clockwise
class ShibaObject {

public:

	// Tile coordinates
	std::string objectName;
	Position offset;
	bool texture = false;

	std::vector <ShibaQuad> vertexCol;
	std::vector <ShibaQuad> texturePoints;
	std::vector <int> rangeIDCol;
	std::vector <int> pathIDCol;


	int health = 100;

	bool loopPath = false;
	int state = -1;
	int currentState = 0;

	std::queue <ShibaQuad> pathing;

	using loadFunction = void (*)(ShibaObject);
	loadFunction glutSolids = nullptr;

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

	void loadGlutSolids() const {
		if (glutSolids != nullptr) {
			glutSolids(*this);
		}
		else {
			// std::cout << "This 3D object doesn't have any custom Glut Solids or it wasn't assigned." << std::endl;
		}
	}

	void cycleState() {

		if (state == -1) {
			std::cout << "Error: Cycle State called when no state was declared." << std::endl;
			return;
		}
		if (currentState == state) currentState = 0;
		else currentState++;

		std::cout << "State is now: " << currentState << "/" << state << std::endl;

	}

	int getState() {
		return currentState;
	}

	void load() {
		// texture code goes here

		if (glutSolids != nullptr) {
			loadGlutSolids();
			return;
		}

		glBegin(GL_QUADS);

		if (!texture) getColorMod(abs(this->color));
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

	void updateTileCoords() {

		//checking if it's a custom object.
		if (vertexCol.size() == 1) {
			int xMod = 0, zMod = 0;
			float rawX = (vertexCol.at(0).x + this->offset.x), rawZ = (vertexCol.at(0).z + this->offset.z);

			int iX = rawX / 10;
			int iZ = rawZ / 10;

			if (rawX > iX * 10 + TILESIZE) xMod = 1;
			
			if (rawZ > iZ * 10 + TILESIZE) zMod = 1;

			if (rawX < iX * 10 - TILESIZE) xMod = -1;

			if (rawZ < iZ * 10 - TILESIZE) zMod = -1;


			tileX = iX + xMod;
			tileZ = iZ + zMod;
		}
		else {
			std::cerr << "SHIBA attempted to update tile coordinate of a non-custom object." << std::endl;
			std::cerr << "This action is not supported and has been discarded." << std::endl;
		}

	}

	Position getRawCoords() {

		Position r;

		r = offset;
		r.x += vertexCol.at(0).x;
		r.y += vertexCol.at(0).y;
		r.z += vertexCol.at(0).z;

		return r;
	}

};

// Level class included for easier level creation.
class Level {
public:
	std::string name;
	int levelGrid[20][20];
	int objectives = 0;
	int enemies = 0;
	std::vector <ShibaObject> customObjects;

	Level(std::string levelName, int map[][20]) {
		this->name = levelName;
		int  i, j;
		for (i = 0; i < 20; i++)
		{
			for (j = 0; j < 20; j++)
				levelGrid[i][j] = map[i][j];
		}
	}

	void printLevel() const {
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

	int health = PLAYER_HP + PLAYER_HP_MOD;
	Position center;
	std::vector<ShibaQuad> texturePoints;
	int kills = 0;
	int objectives = 0;

	Entity(Position& a) {
		center = a;
	}

};

enum menuTypes {

	BUTTON = 1,
	MULTI_BUTTON = 2,
	INPUT_BUTTON = 3,
	TOGGLE_BUTTON = 4,
	TEXT = 5

};

class MenuOption {

public:
	std::string text;
	int type = BUTTON;
	std::vector <int> value;
	bool hovering = false;
	int gap = 300;
	int head = 0;

	MenuOption(std::string text, int type, int value = -1) {
		this->text = text;
		this->type = type;
		this->value.push_back(value);
	}

	void render(int x, int y, int padding, int textHeight, Position* cursor) {

		// checking if mouse is within menu text area.

		if (cursor->x > x - padding && cursor->x < x + (this->text.length() * 13)
			&& cursor->y > y - padding && cursor->y < y + textHeight && type != TEXT) {

			glColor4f(0.694f, 0.78f, 0.561f, .8f);
			hovering = true;
		}
		else {
			glColor4f(0, 0, 0, 0);
			hovering = false;
		}
		
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 0);
		int spacing = 0;

		switch (type) {

		case MULTI_BUTTON:
			//Highlights option
			glVertex2f(x - padding, y - padding);
			glVertex2f(x + (text.length() * 13), y - padding);
			glVertex2f(x + (text.length() * 13), y + textHeight);
			glVertex2f(x - padding, y + textHeight);

		break;


		case BUTTON:

			glVertex2f(x - padding, y - padding);
			glVertex2f(x + (text.length() * 13), y - padding);
			glVertex2f(x + (text.length() * 13), y + textHeight);
			glVertex2f(x - padding, y + textHeight);

		break;

		case TOGGLE_BUTTON:

			// text with a gap followed by a button that you can press.
			glVertex2f(x - padding, y - padding);
			glVertex2f(x + (text.length() * 13), y - padding);
			glVertex2f(x + (text.length() * 13), y + textHeight);
			glVertex2f(x - padding, y + textHeight);

			if (!value.empty())
			if (value.front() == 0)
				glColor3f(1, 0, 0);
			else
				glColor3f(0, 1, 0);

			//toggle box
			glVertex2f(x + (text.length() * 13) + gap, y);
			glVertex2f(x + (text.length() * 13) + gap + 15, y);
			glVertex2f(x + (text.length() * 13) + gap + 15, y + 15);
			glVertex2f(x + (text.length() * 13) + gap, y + 15);


		break;

		}
		
		glEnd();

	}

};

class Menu {

public:

	int x = 0;
	int y = 0;
	int hotkey = -1;
	float padding = 5.0f;
	bool displayed = true;
	int textHeight = 20.0f;
	Position* cursor = nullptr;
	std::vector <MenuOption> options;

	using funcP = void (*)(std::string, int);
	funcP handlerFunction = nullptr;

	Menu(int x, int y, Position& a) {
		this->x = x;
		this->y = y;
		this->cursor = &a;
	}

	void setHandler(funcP func) {
		handlerFunction = func;
	}

	void show() {

		toggleOverlayMode(true);

		if (displayed) {

			int tempY = y;

			for (MenuOption &item : options) {

				glNormal3f(2, 0, 0);

				//Show text
				std::string finalText = item.text;

				if (item.type == MULTI_BUTTON && item.value.size() != 0) {
			
					if (item.hovering) glColor4f(0.694f, 0.78f, 0.561f, 1.0f);
					else glColor3f(1, 1, 1);

					std::string optionVal = "[" + std::to_string(item.value.at(item.head)) + " %]";
					const unsigned char* convertedVal = reinterpret_cast <const unsigned char*> (optionVal.c_str());
					glRasterPos2f(x + 420, tempY);
					glutBitmapString(GLUT_BITMAP_HELVETICA_18, convertedVal);
				}

				if (item.type == TEXT) glColor3f(0.95f, 0.95f, 0);
				else glColor3f(1, 1, 1);

				glRasterPos2f(x, tempY);
				const unsigned char* convertedStr = reinterpret_cast <const unsigned char*> (finalText.c_str());

				if (item.type == TEXT) glutBitmapString(GLUT_BITMAP_HELVETICA_10, convertedStr);
				else glutBitmapString(GLUT_BITMAP_HELVETICA_18, convertedStr);

				// Render the button itself.
				if (item.hovering) {

					// handle click
					//if (cursor->buttonState == 0 && cursor->button == 0) {

						if (item.type == BUTTON && cursor->buttonState == 0 && cursor->button == 0) {
							handlerFunction(item.text, -1);
						}
						else if (item.type == TOGGLE_BUTTON && cursor->buttonState == 0 && cursor->button == 0) {
							item.value.front() = item.value.front() == 0 ? 1 : 0;
							handlerFunction(item.text, item.value.front());
						}
						else if (item.type == MULTI_BUTTON && cursor->buttonState == 0) {

							if (cursor->button == 0) {
								if ((item.head + 1) != item.value.size()) item.head++;
								else item.head = 0;
							}
							else if (cursor->button == 2) {
								if (!(item.head - 1 <= -1)) item.head--;
								else item.head = item.value.size() - 1;
							}

							handlerFunction(item.text, item.value.at(item.head));
						}

						//std::cout << "Clicked on: " << item.text << std::endl;
						cursor->button = -1;
						cursor->buttonState = -1;
					//}

				}

				item.render(x, tempY, padding, textHeight, cursor);
				if (item.type == TEXT) tempY += 15.0f;
				else tempY += 30.0f;
			}

		}

		toggleOverlayMode(false);

	}

	void setHotKey(int key) {
		hotkey = key;
		displayed = false;
	}
};

// generate a random number between the low and high range, inclusive
static int randomInt(int low, int high) {
	return (rand() % (high - low + 1)) + low;
}