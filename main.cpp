#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <GL/glut.h>

#define PI 3.14159265

const bool g_debug = true;
const float g_radius = 1.0f;
int g_max_framerate = 60;

int g_mouse_x = 0;
int g_mouse_y = 0;

float g_angle_x = 270.0f;
float g_angle_z = 90.0f;

float g_camera_position[3] = {0.0f, 1.0f, 0.0f};
float g_camera_direction[3] = {0.0f, 0.0f, 0.0f};
float g_up[3] = {0.0f, 0.0f, 1.0f};

bool g_mouse_pressed = false;

struct {
    bool forward = false;
    bool backward = false;
    bool left = false;
    bool right = false;
} g_directions;

long g_last_camera_update = 0;

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
    glutWireSphere(2.0f, 16, 16);

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

void updateCameraVectors(float diff_x, float diff_y, float diff_z, float moment_multiplier) {
    float normalization_c = 1 / std::sqrt(diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
    moment_multiplier *= normalization_c;

    diff_x *= moment_multiplier;
    diff_y *= moment_multiplier;
    diff_z *= moment_multiplier;

    g_camera_position[0] += diff_x;
    g_camera_position[1] += diff_y;
    g_camera_position[2] += diff_z;

    g_camera_direction[0] += diff_x;
    g_camera_direction[1] += diff_y;
    g_camera_direction[2] += diff_z;
}

void updatePositionFunc() {
    long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    if (time - g_last_camera_update < 1000 / g_max_framerate) {
        return;
    }
    g_last_camera_update = time;

    float moment_multiplier = 0.25;

    float diff_x = (g_camera_direction[0] - g_camera_position[0]);
    float diff_y = (g_camera_direction[1] - g_camera_position[1]);
    float diff_z = (g_camera_direction[2] - g_camera_position[2]);

    float cross_x = diff_y * g_up[2] - diff_z * g_up[1];
    float cross_y = diff_z * g_up[0] - diff_x * g_up[2];
    float cross_z = diff_x * g_up[1] - diff_y * g_up[0];

    if (g_directions.forward) {
        moment_multiplier *= 1.0f;
        updateCameraVectors(diff_x, diff_y, diff_z, moment_multiplier);
    }

    if (g_directions.backward) {
        moment_multiplier *= -1.0f;
        updateCameraVectors(diff_x, diff_y, diff_z, moment_multiplier);
    }

    if (g_directions.right) {
        moment_multiplier *= 1.0f;
        diff_x = cross_x;
        diff_y = cross_y;
        diff_z = cross_z;
        updateCameraVectors(diff_x, diff_y, diff_z, moment_multiplier);
    }

    if (g_directions.left) {
        diff_x = cross_x;
        diff_y = cross_y;
        diff_z = cross_z;
        moment_multiplier *= -1.0f;
        updateCameraVectors(diff_x, diff_y, diff_z, moment_multiplier);
    }

    if (g_debug) {
        std::cout << "Camera angles:" << " angle_x=" << g_angle_x << " angle_z=" << g_angle_z << std::endl;
        std::cout << "Current camera position: x=" << g_camera_position[0] << " y=" << g_camera_position[1] << " z=" << g_camera_position[2] << std::endl;
        std::cout << "Keys pressed: f=" << g_directions.forward << " b=" << g_directions.backward << " r=" << g_directions.right << " l=" << g_directions.left << std::endl;
    }

    glutPostRedisplay();
}

void keyboardFunc(int pressed, int x, int y) {
    switch (pressed) {
        case GLUT_KEY_UP:
            g_directions.forward = true;
            break;
        case GLUT_KEY_DOWN:
            g_directions.backward = true;
            break;
        case GLUT_KEY_RIGHT:
            g_directions.right = true;
            break;
        case GLUT_KEY_LEFT:
            g_directions.left = true;
            break;
        default:
            return;
    }
}

void keyboardUpFunc(int released, int x, int y) {
    switch (released) {
        case GLUT_KEY_UP:
            g_directions.forward = false;
            break;
        case GLUT_KEY_DOWN:
            g_directions.backward = false;
            break;
        case GLUT_KEY_RIGHT:
            g_directions.right = false;
            break;
        case GLUT_KEY_LEFT:
            g_directions.left = false;
            break;
        default:
            return;
    }
}

void mouseValidator(int button, int state, int x, int y) {
    if (!g_mouse_pressed && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (g_debug) {
            std::cout << "Left key pressed on: x=" << x << " y=" << y << std::endl;
        }
        g_mouse_pressed = true;
    }
    if (g_mouse_pressed && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (g_debug) {
            std::cout << "Left key released on: x=" << x << " y" << y << std::endl;
        }
        g_mouse_pressed = false;
    }
    g_mouse_x = x;
    g_mouse_y = y;
}

void leftMouseListener(int x, int y) {
    if (!g_mouse_pressed) {
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

    glutIgnoreKeyRepeat(true);
    glutSpecialFunc(keyboardFunc);
    glutSpecialUpFunc(keyboardUpFunc);

    glutMouseFunc(mouseValidator);
    glutMotionFunc(leftMouseListener);

    glEnable(GL_DEPTH_TEST);

    glutIdleFunc(updatePositionFunc);
    glutMainLoop();
    return 0;
}
