#ifndef PATHFINDER_TEST_HPP
#define PATHFINDER_TEST_HPP

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

#include "terrain.hpp"
#include "file_manager.hpp"

#include "pathfinder.hpp"

using namespace std;
// temp struct for colors
struct RGBf
{
    float r;
    float g;
    float b;
};

const int x_dim = 21;
const int y_dim = 21;

const int min_alt = -5;
const int max_alt = 10;

const float x_bound = 1.0;
const float y_bound = 1.0;

const float grid_scale = 0.9;

/************************ OPENGL PROTOTYPE FUNCTIONS **************************/

void init(void);
void reshape(int width, int height);
void display(void);

void draw_objects();

void mouse_pressed(int button, int state, int x, int y);
void mouse_moved(int x, int y);

void key_pressed(unsigned char key, int x, int y);
void key_up(unsigned char key, int x, int y);
void key_special_pressed(int key, int x, int y);
void key_special_up(int key, int x, int y);

void draw_block(int x, int y);

float pixel_to_screen_coord(int pixel_coord, char type);

RGBf alt_to_RGBf(int alt);

float grid_coord_to_gl_coord(const float coord, const char type);

/************************** OPENGL GLOBAL VARIABLES ***************************/

int XRES = 1000, YRES = 1000;

int mouse_x, mouse_y;
float mouse_scale_x, mouse_scale_y;

Coordinate select_coord(-1, -1);

char src_dst = 's';
Coordinate select_src(-1, -1);
Coordinate select_dst(-1, -1);
Pathfinder pf = Pathfinder();

bool is_pressed = false;

float cam_position[] = {0, 0, 1};

float cam_orientation_axis[] = {0, 0, -1};

float cam_orientation_angle = 0; // degrees

float near_param = 1,
      far_param = 10,
      left_param = -x_bound,
      right_param = x_bound,
      top_param = y_bound,
      bottom_param = -y_bound;

#endif