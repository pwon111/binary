#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

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
#include <string.h>

#include "util.hpp"
#include "terrain.hpp"


using namespace std;

// causes seg fault at the moment
bool fexists(string filename);

void saveTerrain(const Terrain& trn);
Terrain* loadTerrain();

#endif