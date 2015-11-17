#ifndef DISPLAY_HPP
#define DISPLAY_HPP

// This includes more headers than you will probably need.
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <utility> // pair
#include <list>
#include <vector>
#include <array>
#include <math.h>
#include <limits>
#include <fstream>
#include <algorithm> //swap
#include <cassert>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "game.hpp"
#include "world.hpp"
#include "renderer.hpp"
#include "model.hpp"
#include "terrain.hpp"
#include "util.hpp"
#include "timer.hpp"

using namespace std;


// struct for rgb values
// uses floats
struct RGBf {
    // all between 0.0 and 1.0
    float r;
    float g;
    float b;
};

// struct for camera object
// holds camera location and shape information
struct Camera {
    // camera location and orientation
    Vector3f position;
    Vector3f axis;
    float angle;    // in radians

    // view port dimensions (in screen space (float))
    float left_param;   
    float right_param;
    float bottom_param;
    float top_param;
    float near_param;
    float far_param;

    // resolution (in pixels)
    int x_res;
    int y_res;

    // returns screen Width and Height
    // i.e. right_param - left_param and top_param - bottom_param
    const float screenWidth() const;
    const float screenHeight() const;

    // returns rotation vector that transforms the default axis to this->axis
    const Vector3f rotateToAxisVec() const;
};

// default camera
const Camera default_cam = {
    {0.0, 0.0, 1.001},  // position
    {0.0, 0.0, -1.0},   // axis
    0.0,                // angle
    -1.0,               // left_param
    1.0,                // right_param
    -1.0,               // bottom_param
    1.0,                // top_param
    1.0,                // near_param
    100.0,               // far_param
    1000,               // x_res
    1000                // y_res
};

// struct for mouse object
// holds mouse location and response behavior
struct Mouse {
    int pixel_x;
    int pixel_y;
    float screen_x;
    float screen_y;
    bool is_pressed = false;
};

// function for syncing mouse's pixel coordinate and screen coordinate
void syncMouseCoords(Mouse& mouse, const Camera& cam);

/********************************** View Class ********************************/

class View {
public:
    explicit View();
    ~View();

    virtual void render(World* world) = 0;
    virtual void update(int msecs) = 0;

    // virtual function for setting up opengl display
    virtual void setupFunc(int argc, char** argv);
    // virtual functions called in the callback functions
    virtual void reshapeFunc(int width, int height);
    virtual void displayFunc(World* world);
    virtual void mousePressedFunc(int button, int state, int x, int y);
    virtual void mouseMovedFunc(int x, int y);
    virtual void keyPressedFunc(unsigned char key, int x, int y);
    virtual void keyReleasedFunc(unsigned char key, int x, int y);
    virtual void specKeyPressedFunc(int key, int x, int y);
    virtual void specKeyReleasedFunc(int key, int x, int y);

protected:
    Camera cam;
    Mouse mouse;

    // default implementations for opengl functions
    void defaultSetupFunc(int argc, char** argv);
    void defaultReshapeFunc(int width, int height);
    void defaultDisplayFunc(World* world);
    void defaultMousePressedFunc(int button, int state, int x, int y);
    void defaultMouseMovedFunc(int x, int y);
    void defaultKeyPressedFunc(unsigned char key, int x, int y);
    void defaultKeyReleasedFunc(unsigned char key, int x, int y);
    void defaultSpecKeyPressedFunc(int key, int x, int y);
    void defaultSpecKeyReleasedFunc(int key, int x, int y);
};

static const Coordinate init_focus(10, 10);
class Map3DView : public View {
private:
    // camera control
    Coordinate focus;
    Coordinate prev_focus;
    bool focus_cam_match;

    // update camera position
    // will only be called if focus_cam_match is false
    void updateCamPosition();
    void updateCamAxis();

public:
    explicit Map3DView();
    ~Map3DView();

    // derived functionality
    void render(World* world);
    void update(int msecs);

    // virtual function for setting up opengl display
    void setupFunc(int argc, char** argv);
    // derived (Display) virtual functions called in the callback functions
    void displayFunc(World* world);
    void keyPressedFunc(unsigned char key, int x, int y);
};


/******************************** Display Class *******************************/

// class for Display (OpenGL based displays) objects
class Display {
private:
    // interface class has access to everything in Display Class
    friend class Game;

    // is extra default implementations worth it?
    static Display* display;

    // access to world information and view through which to display it
    View* view;
    World* world;

    // enable glutTimerFunc?
    bool dynamic;
    Timer timer;
    static const int desired_fps = 60;

    explicit Display();
    virtual ~Display();

    void setView(View* view);
    void updateView(int msecs);

    void setWorld(World* world);

    // draw to OpenGL display
    void setup(int argc, char** argv);
    void drawFrame();

    // static callback functions passed to OpenGL
    static void reshapeCallback(int width, int height);
    static void displayCallback();
    static void mousePressedCallback(int button, int state, int x, int y);
    static void mouseMovedCallback(int x, int y);
    static void keyPressedCallback(unsigned char key, int x, int y);
    static void keyReleasedCallback(unsigned char key, int x, int y);
    static void specKeyPressedCallback(int key, int x, int y);
    static void specKeyReleasedCallback(int key, int x, int y);
    static void timerCallback(int msecs);

    // virtual void timerFunc(int msecs);
    void defaultTimerFunc(int msecs);
};

#endif