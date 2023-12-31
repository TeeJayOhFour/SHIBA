#include <iostream>
#include <GL/freeglut.h>

using namespace std;

float tx = 0, ty = 0, z_dis = 100, z_viewPoint = -120;
unsigned char mainKey = 'r';
GLUquadricObj *rocketBody, *rocketPlate;
GLfloat pos[] = {-35, -8, 5, 1};
GLfloat amb[] = {0.3, 0.3, 0.3, 1.0};
GLfloat front_amb_diff[] = {0.7, 0.5, 0.1, 1.0};
GLfloat back_amb_diff[] = {0.4, 0.7, 0.1, 1.0};
GLfloat spe[] = {0.25, 0.25, 0.25, 1.0};
GLfloat theta = 0, alpha = 0, dt = 0.1; // Adjusted dt for slower rotation

void rocket_body() {
    glPushMatrix();
    rocketBody = gluNewQuadric();
    gluQuadricDrawStyle(rocketBody, GLU_FILL);
    gluQuadricNormals(rocketBody, GLU_SMOOTH);
    gluQuadricOrientation(rocketBody, GLU_OUTSIDE);

    glColor3f(0.694, 0.694, 0.686);
    glTranslated(7.5, -18, z_viewPoint);
    glRotated(-90, 1, 0, 0);
    gluCylinder(rocketBody, 4.5, 4.5, 30, 32, 32);

    glColor3f(0, 0, 0);
    int windows[5] = {-5, -10, -15, -20, -25};
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
}

void rocket_bottom() {
    glColor3f(0.564, 0.541, 0.517);
    glPushMatrix();
    glTranslated(7.5, -18, z_viewPoint);
    glRotated(-90, 1, 0, 0);
    glutSolidSphere(4.5, 32, 32);
    glPopMatrix();
}

void rocket_top() {
    glColor3f(0.6, 0.6, 0.6);
    glPushMatrix();
    glTranslated(7.5, 12, z_viewPoint);
    glRotated(-90, 1, 0, 0);
    glutSolidCone(4.5, 10.0, 32, 32);
    glPopMatrix();
}

void rocket_plates() {
    rocketPlate = gluNewQuadric();
    gluQuadricDrawStyle(rocketPlate, GLU_FILL);
    gluQuadricNormals(rocketPlate, GLU_SMOOTH);
    gluQuadricOrientation(rocketPlate, GLU_OUTSIDE);
    glColor3f(0.427, 0.180, 0.094);

    glPushMatrix();
    glTranslated(7.5, -15.5, z_viewPoint);
    glRotatef(45, 0, 1, 0);
    glRotatef(160, 1, 0, 0);
    gluPartialDisk(rocketPlate, 4.5, 18, 10, 5, 0, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslated(7.5, -15.5, z_viewPoint);
    glRotatef(160, 1, 0, 0);
    glRotatef(-90, 0, 1, 0);
    gluPartialDisk(rocketPlate, 4.5, 18, 10, 5, 0, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslated(7.5, -15.5, z_viewPoint);
    glRotatef(225, 0, 1, 0);
    glRotatef(160, 1, 0, 0);
    gluPartialDisk(rocketPlate, 4.5, 18, 10, 5, 0, 30);
    glPopMatrix();
}

void rocket_assembly() {
    rocket_plates();
    rocket_top();
    rocket_body();
    rocket_bottom();
}

void rocket_separate() {
    theta = theta - 30;
    glPushMatrix();
    glRotated(theta, 1, -1, 0);
    rocket_top();
    glPopMatrix();

    glPushMatrix();
    glRotated(theta, -1, 0, 1);
    rocket_body();
    glPopMatrix();

    glPushMatrix();
    glRotated(theta, 1, 0, 0);
    rocket_bottom();
    glPopMatrix();

    glPushMatrix();
    glRotated(theta / 3, 1, 0, 0);
    rocket_plates();
    glPopMatrix();

    theta = theta + 30; // Resetting theta
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    switch (mainKey) {
    case 'r': {
        // The existing code for rocket animation
        tx = 0, ty = 0, z_dis = 100, z_viewPoint = -120;

        break;
    }
    case 's': {
        glPushMatrix();
        //glTranslated(0, 0, 0);
        glTranslated(6.5, 0, z_viewPoint);
        glRotated(theta, 1, 1, 1);
        glTranslated(-6, 0, z_viewPoint * (-1));
        rocket_separate();
        glPopMatrix();
        break;
    }
    case 'd': {
        glPushMatrix();
        //glTranslated(0, 0, 0);
        glTranslated(6.5, 0, z_viewPoint);
        glRotated(-40, 1, 0, 0);
        glTranslated(-6, 0, z_viewPoint * (-1));
        rocket_assembly();
        glPopMatrix();
        break;
    }
    }

    glPopMatrix();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'r' || key == 's' || key == 'd') {
        mainKey = key;
        if (key == 'd')
            theta = 45;
        else if (key == 's') {
            theta = 30;
        }
    } else {
        mainKey = 'r';
    }
    glutPostRedisplay();
}

void idle(void) {
    theta = (theta < 360) ? theta + dt : dt;
    tx = tx - 0.02;
    ty = ty + 0.02;
    glutPostRedisplay();
}

void output_menu() {
    fprintf(stdout, "Rocket 3D Manual:");
    cout << "\n\nThe program is run by default." << endl;
    cout << "Press 'r' to run the program again." << endl;
    cout << "Press 'd' to display the rocket." << endl;
    cout << "Press 's' to separate the parts of the rocket." << endl;
}

int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Rocket 3D");
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.0, 10, 10000);

    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glEnable(GL_NORMALIZE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    output_menu();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
