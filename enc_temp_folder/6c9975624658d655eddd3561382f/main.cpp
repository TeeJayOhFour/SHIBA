
//including IO stream or stdlib not required as it's in Shiba.h already.
#include "SHIBA.h"

using namespace std;

static void drawHuman(ShibaObject a) {

    // any custom object must have a single ShibaQuad point as center.

    ShibaQuad center;
    center = a.vertexCol.front();

    glPushMatrix();
    glTranslatef(center.x + a.offset.x, center.y + a.offset.y + 4.0f, center.z + a.offset.z);
    glScalef(10, 10, 10);

    // Draw the head
    glColor3f(1.0, 0.8, 0.6); // Skin color
    glutSolidSphere(0.25, 100, 100);

    // Draw the body
    glColor3f(0.5, 0.5, 1.0); // Blue shirt
    glPushMatrix();
    glTranslatef(0.0, -0.4, 0.0);
    glutSolidCube(0.4);
    glPopMatrix();

    // Draw the arms
    glColor3f(1.0, 0.8, 0.6); // Skin color
    glPushMatrix();
    glTranslatef(0.5, -0.3, 0.0);
    glutSolidCube(0.1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.5, -0.3, 0.0);
    glutSolidCube(0.1);
    glPopMatrix();

    // Draw the legs
    glColor3f(0.0, 0.0, 1.0); // Blue pants
    glPushMatrix();
    glTranslatef(0.2, -0.8, 0.0);
    glutSolidCube(0.15);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.2, -0.8, 0.0);
    glutSolidCube(0.15);
    glPopMatrix();

    glPopMatrix();

}

static void drawRocket(ShibaObject a) {

    ShibaQuad center;
    center = a.vertexCol.front();


    // Draw Rocket Body
    glPushMatrix();
    glTranslatef(center.x + a.offset.x, center.y + a.offset.y + 12.0f, center.z + a.offset.z);
    glScalef(.5, .5, .5);


    glPushMatrix();


    GLUquadricObj* rocketBody = gluNewQuadric();
    gluQuadricDrawStyle(rocketBody, GLU_FILL);
    gluQuadricNormals(rocketBody, GLU_SMOOTH);
    gluQuadricOrientation(rocketBody, GLU_OUTSIDE);

    glColor3f(0.694, 0.694, 0.686);
    glTranslated(0, -18, 0);
    glRotated(-90, 1, 0, 0);
    gluCylinder(rocketBody, 4.5, 4.5, 30, 32, 32);

    // Draw Windows on Rocket Body
    glColor3f(0, 0, 0);
    int windows[5] = { -5, -10, -15, -20, -25 };
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glRotated(90, 0, 0, 1);
        glRotated(-90, 1, 0, 0);
        glTranslated(0, windows[i], 0);
        glScalef(6.45, 1, 1);
        glutSolidCube(1.4);
        glPopMatrix();
    }

    glPopMatrix();

    // Draw Rocket Bottom
    glColor3f(0.564, 0.541, 0.517);
    glPushMatrix();
    glTranslated(0, -18, 0);
    glRotated(-90, 1, 0, 0);
    glutSolidSphere(4.5, 32, 32);
    glPopMatrix();

    // Draw Rocket Top
    glColor3f(0.6, 0.6, 0.6);
    glPushMatrix();
    glTranslated(0, 12, 0);
    glRotated(-90, 1, 0, 0);
    glutSolidCone(4.5, 10.0, 32, 32);
    glPopMatrix();

    // Draw Rocket Plates
    GLUquadricObj* rocketPlate = gluNewQuadric();
    gluQuadricDrawStyle(rocketPlate, GLU_FILL);
    gluQuadricNormals(rocketPlate, GLU_SMOOTH);
    gluQuadricOrientation(rocketPlate, GLU_OUTSIDE);
    glColor3f(0.427, 0.180, 0.094);

    // Draw the right most plate
    glPushMatrix();
    glTranslated(0, -15.5, 0);
    glRotatef(45, 0, 1, 0);
    glRotatef(160, 1, 0, 0);
    gluPartialDisk(rocketPlate, 4.5, 18, 10, 5, 0, 30);
    glPopMatrix();

    // Draw the back plate
    glPushMatrix();
    glTranslated(0, -15.5, 0);
    glRotatef(160, 1, 0, 0);
    glRotatef(-90, 0, 1, 0);
    gluPartialDisk(rocketPlate, 4.5, 18, 10, 5, 0, 30);
    glPopMatrix();

    // Draw the left most plate
    glPushMatrix();
    glTranslated(0, -15.5, 0);
    glRotatef(225, 0, 1, 0);
    glRotatef(160, 1, 0, 0);
    gluPartialDisk(rocketPlate, 4.5, 18, 10, 5, 0, 30);
    glPopMatrix();
    glPopMatrix();

}

static void Batman(ShibaObject a) {

    ShibaQuad center = a.vertexCol.at(0);
    int scaleFactor = 7.0;

    glPushMatrix();

    glTranslatef(center.x + a.offset.y, center.y + a.offset.y+2.0f, center.z + a.offset.z);
    glScalef(scaleFactor, scaleFactor, scaleFactor);


    // Cowl
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glScalef(0.5, 1, .5);
    texturedCube(0.4 / 2.0);

    glScalef(1, .5, 1);
    glutSolidCube(0.4);

    glPopMatrix();

    // Suit

    glPushMatrix();
    glTranslatef(0.0, -0.4, 0.0);
    glScalef(1, 1, .5);
    glutSolidCube(0.5);
    texturedCube(0.5 / 2.0, SUIT);

    glPopMatrix();

    // arms

    glPushMatrix();
    glTranslatef(0.3, -0.2, 0.0);
    glutSolidCube(0.2);
    texturedCube(0.2 / 2.0, SHOULDERR);

    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.34, -0.3, 0.0);
    glutSolidCube(0.12);
    texturedCube(0.12 / 2.0, ARMR);

    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.34, -0.4, 0.0);
    glutSolidCube(0.12);
    texturedCube(0.12 / 2.0, HANDR);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.3, -0.2, 0.0);
    glutSolidCube(0.2);
    texturedCube(0.2 / 2.0, SHOULDERL);

    glPopMatrix();


    glPushMatrix();
    glTranslatef(-0.34, -0.3, 0.0);
    glutSolidCube(0.12);
    texturedCube(0.12 / 2.0, ARML);

    glPopMatrix();


    glPushMatrix();
    glTranslatef(-0.34, -0.4, 0.0);
    glutSolidCube(0.12);
    texturedCube(0.12 / 2.0, HANDL);

    glPopMatrix();

    // pants


    glPushMatrix();
    glTranslatef(0.1, -0.7, 0.0);
    glutSolidCube(0.15);
    texturedCube(0.15 / 2.0, UPLEGR);

    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.1, -0.8, 0.0);
    glutSolidCube(0.15);
    texturedCube(0.15 / 2.0, BLEGR);

    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.1, -0.9, 0.0);
    glutSolidCube(0.15);
    texturedCube(0.15 / 2.0, BOOTR);

    glPopMatrix();


    glPushMatrix();
    glTranslatef(-0.1, -0.7, 0.0);
    glutSolidCube(0.15);
    texturedCube(0.15 / 2.0, UPLEGL);

    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.1, -0.8, 0.0);
    glutSolidCube(0.15);
    texturedCube(0.15 / 2.0, BLEGL);

    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.1, -0.9, 0.0);
    glutSolidCube(0.15);
    texturedCube(0.15 / 2.0, BOOTL);

    glPopMatrix();

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
        {4, 7, 0, 0, 0, 0, -1, 1, 4, 0, 4, 0, 0, 4, 0, 0, 0, 0, 2, 4},
        {4, 1, 3, 4, 4, 4, 4, 4, 4, 0, 4, 1, 3, 4, 0, 0, 0, 0, 1, 4},
        {4, 4, 4, 4, 4, 4, 4, 4, 4, -1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
    };

    int L1Map[20][20] = {
        {4, 4, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
        {4, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 3, 0, 0, 0, 5, 0, 0, 0, 0, 4, 0, 4, 4, 5, 4, 4, 4},
        {4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 2, 0, 4, 5, 4, 4, 4, 4, 4, 4},
        {4, 4, 4, 4, 5, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 9, 0, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 2, 0, 0, 4},
        {4, 4, 4, 4, 5, 4, 4, 4, 0, 9, 0, 9, 0, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 5, 4, 4, 4, 4, 4},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 1, 4},
        {4, 4, 4, 5, 4, 4, 4, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 0, 0, 0, 0, 4, 0, 3, 0, 0, 5, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 4, 0, 2, 0, 0, 0, 0, 4},
        {4, 20, -1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4},
        {4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4},
        {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}
    };

    int L5Map[20][20] = {
        {4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 8},
        {4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 8, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},

    };

    int L2Map[20][20] = {
        {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
        {4, 0, 4, 0, 0, 0, 0, 0, 0, 2, 4, 0, 5, 0, 4, 0, 5, 0, 3, 4},
        {4, 0, 5, 0, 4, 4, 4, 4, 4, 4, 4, 0, 4, 0, 4, 0, 4, 4, 0, 4},
        {4, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 0, 4},
        {4, 0, 4, 0, 4, 4, 6, 0, 0, 4, 4, 4, 4, 0, 4, 0, 4, 0, 4, 4},
        {4, 0, 4, 0, 1, 4, 0, 0, 0, 1, 4, 3, 4, 0, 4, 0, 4, 0, 0, 4},
        {4, 0, 4, 0, 4, 4, 4, 0, 0, 0, 4, 0, 5, 0, 4, 0, 4, 4, 0, 4},
        {4, 0, 4, 0, 0, 4, 7, 0, 0, 0, 4, 0, 4, 0, 4, 0, 4, 0, 0, 4},
        {4, 0, 4, 0, 4, 4, 0, 0, 0, 4, 4, 0, 4, 0, 4, 0, 4, 0, 4, 4},
        {4, 0, 4, 0, 0, 4, 0, 4, 0, 0, 0, 0, 4, 0, 5, 0, 4, 0, 0, 4},
        {4, 0, 4, 0, 4, 4, 0, 4, 0, 4, 4, 4, 4, 4, 4, 0, 4, 0, 4, 4},
        {4, 0, 4, 0, 0, 4, 2, 4, 1, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4},
        {4, 0, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 4, 4, 0, 4},
        {4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 1, 0, 4},
        {4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 4},
        {4, 0, 4, 0, 4, 4, 0, 4, 4, 0, 4, 4, 0, 4, 4, 0, 4, 0, 0, 4},
        {4, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 4, 4, 0, 4},
        {4, 0, 4, 4, 0, 4, 4, 0, 4, 4, 0, 4, 4, 0, 4, 0, 4, 0, 0, 4},
        {4, 8, 4, 2, 0, 4, 1, 0, 4, 3, 0, 4, 1, 0, 4, 2, 4, -1, 0, 4},
        {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
    };

    // 1. Level class queue (queue each level)
    queue<Level> LevelQueue;

    Level meow1 = Level("Level 0", L5Map);
    Level meow2 = Level("Level 1", L1Map);
    Level meow3 = Level("Level 2", L2Map);
    Level meow4 = Level("Level 3", L3Map);


    //! TODO Allow shibaobject to handle glutsolids
    ShibaObject custom(0, 0, 0);

    custom.setLoadGlutFunction(drawHuman);
    meow3.customObjects.push_back(custom);


    custom.setLoadGlutFunction(drawRocket);
    meow2.customObjects.push_back(custom);    
    
    custom.setLoadGlutFunction(Batman);
    meow4.customObjects.push_back(custom);


    // batman.setLoadGlutFunction(&kyakya);
    // batman.loadGlutSolids();

    // return 0;

    LevelQueue.push(meow1);
    LevelQueue.push(meow2);
    LevelQueue.push(meow3);
    LevelQueue.push(meow4);    
    

    backupLevelQueue.push(meow1);
    backupLevelQueue.push(meow2);
    backupLevelQueue.push(meow3);
    backupLevelQueue.push(meow4);

    // 2. Pathfinding callback function (for deciding next tile)
    // 3. Enemy Prioritization callback function (for targetting closest threat)

    //pushing the initial levels.
    initLevels(LevelQueue);

    // Shows startup screen
    currentScene = 0;

    try {

        // launches the engine with configured settings.
        int state = launch(argc, argv);

    }
    catch (const invalid_argument& e) {
        cerr << e.what() << '\n';
    }


    return 0;
}


