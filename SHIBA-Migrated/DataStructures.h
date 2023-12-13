#pragma once
#include "Config.h"

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

		x = fabs(x);
		y = fabs(y);
		z = fabs(z);
		yaw = fabs(yaw);
		pitch = fabs(pitch);

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
};

struct Poles {
	bool north, south, east, west;
};

//This function is needed by ShibaObject, so declaring it first.
static void getColorMod(int id) {

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

enum menuTypes {

	BUTTON = 1,
	SLIDER_BUTTON = 2,
	INPUT_BUTTON = 3,
	TOGGLE_BUTTON = 4,

};

class MenuOption {

public:
	std::string text;
	int type = BUTTON;
	std::vector <int> value;

	MenuOption(std::string text, int type, int value = -1) {
		this->text = text;
		this->type = type;
		this->value.push_back(value);
	}

	void render(int x, int y, int padding, int textHeight) {

		glBegin(GL_QUADS);
		
		switch (type) {

		case BUTTON:
		case TOGGLE_BUTTON:
		case SLIDER_BUTTON:

			glNormal3f(0, 0, 0);

			glVertex2f(x - padding, y - padding);
			glVertex2f(x + (text.length() * 13), y - padding);
			glVertex2f(x + (text.length() * 13), y + textHeight);
			glVertex2f(x - padding, y + textHeight);

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

	using funcP = void (*)(std::string);
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

			for (MenuOption item : options) {

				glNormal3f(2, 0, 0);
				glColor3f(1, 1, 1);
				glRasterPos2f(x, y);

				const unsigned char* convertedStr = reinterpret_cast <const unsigned char*> (item.text.c_str());
				glutBitmapString(GLUT_BITMAP_HELVETICA_18, convertedStr);

				// checking if mouse is within menu text area.
				if (cursor->x > x - padding && cursor->x < x + (item.text.length() * 13)
					&& cursor->y > y - padding && cursor->y < y + textHeight) {
					glColor3f(0, 1, 0);

					// handle click
					if (cursor->buttonState == 0 && cursor->button == 0) {
						std::cout << "Clicked on: " << item.text << std::endl;
						handlerFunction(item.text);
					}
				}
				else glColor3f(1, 0, 0);

				item.render(x, y, padding, textHeight);

				y += 30.0f;
			}

			// Restoring y
			y = tempY;
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