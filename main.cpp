#include <iostream>
#include <cmath>
#include <algorithm>
#include <GL/glut.h>

#define PI 3.14159265

bool g_debug = true;

int g_mouse_x = 0;
int g_mouse_y = 0;

float g_radius = 1.0f;
float g_angle_x = 270.0f;
float g_angle_z = 90.0f;

float g_camera_position[3] = {0.0f, 1.0f, 0.0f};
float g_camera_direction[3] = {0.0f, 0.0f, 0.0f};
float g_up[3] = {0.0f, 0.0f, 1.0f};

bool g_pressed = false;

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

void renderScene() {
    if (g_debug) {
        std::cout << "Render Scene called..." << std::endl;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(
            g_camera_position[0], g_camera_position[1], g_camera_position[2],
            g_camera_direction[0], g_camera_direction[1], g_camera_direction[2],
            g_up[0], g_up[1], g_up[2]
    );

    if (g_debug) {
        std::cout << "Camera direction: x=" << g_camera_direction[0] << " y=" << g_camera_direction[1] << " z=" << g_camera_direction[2] << std::endl;
        std::cout << "Camera angles:" << " angle_x=" << g_angle_x << " angle_z=" << g_angle_z << std::endl;
        std::cout << "Current camera position: x=" << g_camera_position[0] << " y=" << g_camera_position[1] << " z=" << g_camera_position[2] << std::endl;
    }

    glTranslatef(3.0f, 3.0f, 3.0f);
    glColor3f(0.0,1.0,0.0);
    glutWireSphere(2.0f, 10, 10);

    glTranslatef(-3.0f, -3.0f, -3.0f);
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

void keyboardFunc(int pressed, int x, int y) {
    float moment_multiplier = 0.25;
    switch (pressed) {
        case GLUT_KEY_UP:
            moment_multiplier *= 1.0f;
            break;
        case GLUT_KEY_DOWN:
            moment_multiplier *= -1.0f;
            break;
        default:
            return;
    }
    float diff_x = moment_multiplier * (g_camera_direction[0] - g_camera_position[0]);
    float diff_y = moment_multiplier * (g_camera_direction[1] - g_camera_position[1]);
    float diff_z = moment_multiplier * (g_camera_direction[2] - g_camera_position[2]);

    g_camera_position[0] += diff_x;
    g_camera_position[1] += diff_y;
    g_camera_position[2] += diff_z;

    g_camera_direction[0] += diff_x;
    g_camera_direction[1] += diff_y;
    g_camera_direction[2] += diff_z;

    if (g_debug) {
        std::cout << "Camera angles:" << " angle_x=" << g_angle_x << " angle_z=" << g_angle_z << std::endl;
        std::cout << "Current camera position: x=" << g_camera_position[0] << " y=" << g_camera_position[1] << " z=" << g_camera_position[2] << std::endl;
        std::cout << "Key pressed: " << pressed << std::endl;
    }

    glutPostRedisplay();
}

void mouseValidator(int button, int state, int x, int y) {
    if (!g_pressed && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (g_debug) {
            std::cout << "Left key pressed on: x=" << x << " y=" << y << std::endl;
        }
        g_pressed = true;
    }
    if (g_pressed && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (g_debug) {
            std::cout << "Left key released on: x=" << x << " y" << y << std::endl;
        }
        g_pressed = false;
    }
    g_mouse_x = x;
    g_mouse_y = y;
}

void leftMouseListener(int x, int y) {
    if (!g_pressed) {
        return;
    }

    float sensitivity = 0.05f;

    float mouse_diff_x = (x - g_mouse_x) * sensitivity;
    float mouse_diff_y = (g_mouse_y - y) * sensitivity;

    g_angle_x = g_angle_x + mouse_diff_x;
    if (g_angle_x < 0) {
        g_angle_x += 360.0f;
    }
    if (g_angle_x > 360) {
        g_angle_x -= 360.0f;
    }

    g_angle_z = std::clamp(g_angle_z + mouse_diff_y, 0.0f, 180.0f);

    float direction_sphere_x = g_radius * std::sin(g_angle_z * PI / 180) * std::cos(g_angle_x * PI / 180);
    float direction_sphere_y = g_radius * std::sin(g_angle_z * PI / 180) * std::sin(g_angle_x * PI / 180);
    float direction_sphere_z = g_radius * std::cos(g_angle_z * PI / 180);

    g_camera_direction[0] = g_camera_position[0] + direction_sphere_x;
    g_camera_direction[1] = g_camera_position[1] + direction_sphere_y;
    g_camera_direction[2] = g_camera_position[2] + direction_sphere_z;

    g_mouse_x = x;
    g_mouse_y = y;

    glutPostRedisplay();
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

    glutMouseFunc(mouseValidator);
    glutMotionFunc(leftMouseListener);

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
