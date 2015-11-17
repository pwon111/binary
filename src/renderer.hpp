#ifndef RENDERER_HPP
#define RENDERER_HPP

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

#include "model.hpp"
#include "terrain.hpp"
#include "util.hpp"

// rendering functions using unoptimized VBO implementation
void renderBRUTE(Model* model);
void renderWireBRUTE(Model* model);

#endif