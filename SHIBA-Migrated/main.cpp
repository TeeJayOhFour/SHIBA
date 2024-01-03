
//including IO stream or stdlib not required as it's in engine.hpp already.
#include "SHIBA.h"

using namespace std;

static void drawHuman(ShibaObject a) {

    // any custom object must have a single ShibaQuad point as center.

    ShibaQuad center;
    center = a.vertexCol.front();

    //// Draw the head
    glColor3f(1.0, 0.8, 0.6); // Skin color

    glPushMatrix();
        glTranslatef(
            center.x + a.offset.x,
            center.y + a.offset.y,
            center.z + a.offset.z);
    glutSolidSphere(0.25, 100, 100);
    glPopMatrix();

    // Draw the body
    glColor3f(0.5, 0.5, 1.0); // Blue shirt

    glPushMatrix();
        glTranslatef(
            center.x + a.offset.x,
            center.y + a.offset.y - 0.4f,
            center.z + a.offset.z);
    glutSolidCube(0.4);
    glPopMatrix();

    // Draw the arms
    glColor3f(1.0, 0.8, 0.6); // Skin color
    glPushMatrix();
        glTranslatef(
            center.x + a.offset.x + 0.5f,
            center.y + a.offset.y - 0.3f,
            center.z + a.offset.z
        );
    glutSolidCube(0.1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(
            center.x + a.offset.x - 0.5f,
            center.y + a.offset.y - 0.3f,
            center.z + a.offset.z);
    glutSolidCube(0.1);
    glPopMatrix();

    // Draw the legs
    glColor3f(0.0, 0.0, 1.0); // Blue pants
    glPushMatrix();
        glTranslatef(
            center.x + a.offset.x + 0.2f,
            center.y + a.offset.y - 0.8f,
            center.z + a.offset.z);
    glutSolidCube(0.15);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(
            center.x + a.offset.x - 0.2f,
            center.y + a.offset.y - 0.8f,
            center.z + a.offset.z);
    glutSolidCube(0.15);
    glPopMatrix();

}

static void drawRocket(ShibaObject a) {

    // Rocket body
    glColor3f(0.694, 0.694, 0.686);
    glPushMatrix();
    glTranslated(7.5, -18, -120);
    glRotated(-90, 1, 0, 0);
    glutSolidCylinder(4.5, 30, 32, 32);
    glPopMatrix();

    // Rocket bottom
    glColor3f(0.564, 0.541, 0.517);
    glPushMatrix();
    glTranslated(7.5, -18, -120);
    glRotated(-90, 1, 0, 0);
    glutSolidSphere(4.5, 32, 32);
    glPopMatrix();

    // Rocket plates
    glColor3f(0.427, 0.180, 0.094);

    glPushMatrix();
    glTranslated(7.5, -15.5, -120);
    glRotatef(45, 0, 1, 0);
    glRotatef(160, 1, 0, 0);
    glutSolidTorus(1, 8, 10, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(7.5, -15.5, -120);
    glRotatef(160, 1, 0, 0);
    glRotatef(-90, 0, 1, 0);
    glutSolidTorus(1, 8, 10, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(7.5, -15.5, -120);
    glRotatef(225, 0, 1, 0);
    glRotatef(160, 1, 0, 0);
    glutSolidTorus(1, 8, 10, 5);
    glPopMatrix();

}

static void Batman(ShibaObject a) {


    // Cowl
    glColor3f(1, 0, 0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glutSolidCube(0.4);
    glPopMatrix();

    // Suit
    glColor3f(0, 1, 0);
    glPushMatrix();
    glTranslatef(0.0, -0.4, 0.0);
    glutSolidCube(0.5);
    glPopMatrix();

    // arms
    glColor3f(0.4, 0, 0);
    glPushMatrix();
    glTranslatef(0.3, -0.2, 0.0);
    glutSolidCube(0.2);
    glPopMatrix();
    glColor3f(0.4, 0, 0);
    glPushMatrix();
    glTranslatef(0.34, -0.3, 0.0);
    glutSolidCube(0.12);
    glPopMatrix();
    glColor3f(0.4, 0, 0);
    glPushMatrix();
    glTranslatef(0.34, -0.4, 0.0);
    glutSolidCube(0.12);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.3, -0.2, 0.0);
    glutSolidCube(0.2);
    glPopMatrix();
    glColor3f(0.4, 0, 0);
    glPushMatrix();
    glTranslatef(-0.34, -0.3, 0.0);
    glutSolidCube(0.12);
    glPopMatrix();
    glColor3f(0.4, 0, 0);
    glPushMatrix();
    glTranslatef(-0.34, -0.4, 0.0);
    glutSolidCube(0.12);
    glPopMatrix();

    // pants
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0.1, -0.7, 0.0);
    glutSolidCube(0.15);
    glPopMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0.1, -0.8, 0.0);
    glutSolidCube(0.15);
    glPopMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0.1, -0.9, 0.0);
    glutSolidCube(0.15);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1, -0.7, 0.0);
    glutSolidCube(0.15);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.1, -0.8, 0.0);
    glutSolidCube(0.15);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.1, -0.9, 0.0);
    glutSolidCube(0.15);
    glPopMatrix();

}

int main(int argc, char** argv) {


    int L3Map[20][20] = {
        {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4},
        {4, 4, 4, 4, 4, 4, 4, 0, 0, 4, 4, 4, 0, 0, 4, 0, 0, 0, 1, 4},
        {4, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 4, 0, 0, 4, 0, 0, 0, 2, 4},
        {4, 2, 0, 6, 0, 0, 4, 0, 4, 4, 0, 4, 0, 0, 5, 0, 0, 0, 0, 4},
        {4, 2, 0, 1, 0, 0, 5, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 4},
        {4, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 1, 4},
        {4, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 4, 0, 0, 4, 4, 4, 4, 4, 4},
        {4, 4, 4, 4, 4, 4, 4, 0, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, -1, 4},
        {4, 4, 4, 4, 4, 4, 4, 4, 0, 4, 0, 4, 4, 4, 4, 4, 4, 4, 0, 4},
        {4, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4},
        {4, 4, 0, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 4, 0, 4},
        {4, 4, 1, 4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4},
        {4, 4, 4, 4, 0, 0, 4, 4, 0, 0, 4, 4, 4, 4, 0, 4, 4, 4, 0, 4},
        {4, 4, 0, 0, 0, 0, 4, 1, 0, 0, 4, 0, 0, 5, 0, 0, 0, 0, 0, 4},
        {4, 4, 4, 4, 4, 5, 4, 0, 0, 0, 4, 0, 0, 4, 5, 4, 4, 4, 4, 4},
        {4, 4, 4, 4, 4, 0, 4, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 4},
        {4, 4, 4, 4, 4, 0, 4, 4, 4, 0, 4, 0, 0, 4, 0, 0, 1, 0, 0, 4},
        {4, 0, 0, 0, 0, 0, -1, 1, 4, 0, 4, 0, 0, 4, 0, 0, 0, 0, 2, 4},
        {4, 1, 3, 4, 4, 4, 4, 4, 4, 0, 4, 1, 3, 4, 0, 0, 0, 0, 1, 4},
        {4, 4, 4, 4, 4, 4, 4, 4, 4, -1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
    };

    int L1Map[20][20] = {
        {4, 4, 4, 4, 4, 4, 4, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
        {4, 0, 0, 0, 0, 0, 0, 4, 3, 4, 0, 0, 0, 0, 1, 0, 0, 2, 7, 4},
        {4, 0, 4, 4, 4, 0, 0, 4, 0, 4, 0, 0, 1, 0, 0, 0, 1, 0, 7, 4},
        {4, 0, 0, 1, 0, 0, 0, 5, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
        {4, 0, 4, 4, 4, 0, 0, 4, 1, 0, 1, 0, 0, 0, 1, 0, 2, 0, 0, 4},
        {4, 0, 0, 0, 0, 0, 0, 4, 0, 4, 4, 4, 4, 5, 4, 4, 4, 5, 4, 4},
        {4, 4, 4, 4, 5, 4, 4, 4, 0, 4, 7, 1, 5, 0, 1, 0, 0, 1, 0, 4},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0, 1, 0, 4},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 1, 0, 4, 0, 0, 1, 0, 0, 0, 4},
        {4, 4, 4, 4, 5, 4, 4, 4, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4},
        {0, 0, 0, 0, 0, 1, 0, 4, 0, 4, 0, 0, 5, 2, 0, 0, 1, 2, 3, 4},
        {2, 0, 1, 0, 0, 0, 0, 4, 0, 4, 2, 7, 4, 0, 0, 0, 0, 0, 7, 4},
        {0, 0, 0, 0, 2, 1, 3, 5, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 4, 4, 5, 4, 4, 4, 4, 0, 4, 4, 4, 4, 0, 0, 4, 4, 1, 0, 4},
        {0, 0, 1, 0, 0, 1, 0, 4, 0, 4, 0, 0, 2, 0, 0, 0, 4, 0, 0, 4},
        {2, 1, 0, 0, 0, 0, 0, 5, 0, 4, 0, 1, 0, 0, 0, 1, 4, 0, 1, 4},
        {0, -1, 0, 0, 2, 0, 0, 4, 0, 4, 0, 0, 0, 1, 0, 0, 5, 0, 0, 4},
        {0, 0, 1, 0, 0, 0, 3, 4, 3, 4, 0, 2, 0, 3, 0, 0, 4, 1, -1, 4},
        {4, 4, 4, 4, 4, 4, 4, 4, 5, 4, 0, 0, 0, 0, 0, 1, 4, 0, 2, 4}
    };

    int L5Map[20][20] = {
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},

    };

    int L2Map[20][20] = {
        {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
        {4, 0, 4, 0, 0, 0, 0, 0, 0, 2, 4, 0, 5, 0, 4, 0, 5, 0, 3, 4},
        {4, 0, 5, 0, 4, 4, 4, 4, 4, 4, 4, 0, 4, 0, 4, 0, 4, 4, 0, 4},
        {4, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 0, 4},
        {4, 0, 4, 0, 4, 4, 6, 0, 0, 4, 4, 4, 4, 0, 4, 0, 4, 0, 0, 4},
        {4, 0, 4, 0, 0, 4, 0, 0, 0, 1, 4, 3, 4, 0, 4, 0, 4, 4, 0, 4},
        {4, 0, 4, 0, 4, 4, 4, 0, 0, 0, 4, 0, 5, 0, 4, 0, 4, 4, 0, 4},
        {4, 0, 4, 0, 0, 4, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 4, 0, 0, 4},
        {4, 0, 4, 0, 4, 4, 0, 0, 0, 4, 4, 0, 4, 0, 4, 0, 4, 0, 4, 4},
        {4, 0, 4, 0, 0, 4, 0, 4, 0, 0, 0, 0, 4, 0, 5, 0, 4, 0, 0, 4},
        {4, 0, 4, 0, 4, 4, 0, 4, 0, 4, 4, 4, 4, 4, 4, 0, 4, 0, 4, 4},
        {4, 0, 4, 0, 0, 4, 2, 4, 0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4},
        {4, 0, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 4, 4, 0, 4},
        {4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 4},
        {4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 4, 4, 0, 4},
        {4, 0, 4, 1, 4, 4, 0, 4, 4, 0, 4, 4, 0, 4, 4, 0, 4, 0, 0, 4},
        {4, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 4, 4, 0, 4},
        {4, 0, 4, 4, 0, 4, 4, 0, 4, 4, 0, 4, 4, 0, 4, 0, 4, 0, 0, 4},
        {4, 0, 4, 2, 0, 4, 0, 0, 4, 3, 0, 4, 0, 0, 4, 2, 4, -1, 0, 4},
        {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
    };

    // 1. Level class queue (queue each level)
    queue<Level> LevelQueue;

    Level meow1 = Level("Level 0", L5Map);
    Level meow2 = Level("Level 1", L1Map);
    Level meow3 = Level("Level 2", L2Map);
    Level meow4 = Level("Level 3", L3Map);


    //! TODO Allow shibaobject to handle glutsolids
    ShibaObject zaki(0, 0, 0);
    zaki.setLoadGlutFunction(Batman);
     meow1.customObjects.push_back(zaki);

    // batman.setLoadGlutFunction(&kyakya);
    // batman.loadGlutSolids();

    // return 0;

    LevelQueue.push(meow1);
    LevelQueue.push(meow2);
    LevelQueue.push(meow3);
    LevelQueue.push(meow4);

    // 2. Pathfinding callback function (for deciding next tile)
    // 3. Enemy Prioritization callback function (for targetting closest threat)

    // launches the engine with configured settings.
    initLevels(LevelQueue);

    // Shows startup screen
    currentScene = 0;

    try {
        launch(argc, argv); //starts game
        // add some logic here to handle level completion.
        // isLevelComplete() == true

    }
    catch (const invalid_argument& e) {
        cerr << e.what() << '\n';
    }


    return 0;
}


