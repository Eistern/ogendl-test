#include <iostream>
#include <cmath>
#include <GL/glut.h>

#define PI 3.14159265

bool g_debug = true;
float g_x = 0.0f;
float g_y = 0.0f;
float g_z = 0.0f;

float g_radius = 1.0f;
int g_angle = 0;

float g_up[3] = {0.0f, 0.0f, 1.0f};
float g_center[3] = {0.0f, 0.0f, 0.0f};

void changeSize(GLint width, GLint height) {
    if (height == 0) {
        height++;
    }
    if (g_debug) {
        std::cout << "Change size got: " << width << " " << height << std::endl;
    }

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(65, (float) width / height,1, 1000);

    glMatrixMode(GL_MODELVIEW);
}

void cameraBack() {
    g_radius += 1.0f;
}

void cameraForward() {
    if (g_radius > 1.0f) {
        g_radius -= 1.0f;
    }
}

void cameraLeft() {
    g_angle = (g_angle + 1 + 360) % 360;
}

void cameraRight() {
    g_angle = (g_angle - 1) % 360;
}

void keyboardFunc(int pressed, int x, int y) {
    switch (pressed) {
        case GLUT_KEY_UP:
            cameraForward();
            break;
        case GLUT_KEY_DOWN:
            cameraBack();
            break;
        case GLUT_KEY_LEFT:
            cameraLeft();
            break;
        case GLUT_KEY_RIGHT:
            cameraRight();
            break;
        default:
            break;
    }

    g_x = g_radius * std::sin(g_angle * PI / 180);
    g_y = g_radius * std::cos(g_angle * PI / 180);

    if (g_debug) {
        std::cout << "Camera angles: radius=" << g_radius << " angle=" << g_angle << std::endl;
        std::cout << "Current camera position: x=" << g_x << " y=" << g_y << " z=" << g_z << std::endl;
        std::cout << "Key pressed: " << pressed << std::endl;
    }

    glutPostRedisplay();
}

void renderScene() {
    if (g_debug) {
        std::cout << "Render Scene called..." << std::endl;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(
            g_x, g_y, g_z,
            g_center[0], g_center[1], g_center[2],
            g_up[0], g_up[1], g_up[2]
    );

    glColor3f(0.0,1.0,1.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.0,0.0,1.0);
        glVertex3f(1.0,0.0,0.0);
        glVertex3f(0.0,1.0,0.0);
    glEnd();

    glColor3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.0,0.0,1.0);
        glVertex3f(1.0,0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
    glEnd();

    glColor3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.0,1.0,0.0);
        glVertex3f(1.0,0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
    glEnd();

    glColor3f(0.0,1.0,0.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.0,0.0,1.0);
        glVertex3f(0.0,1.0,0.0);
        glVertex3f(0.0,0.0,0.0);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    std::cout << "Launching GLUT application" << std::endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world!");

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutSpecialFunc(keyboardFunc);

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
