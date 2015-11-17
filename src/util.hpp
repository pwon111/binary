#ifndef UTIL_HPP
#define UTIL_HPP

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

#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

enum SingletonActionType {GET, DESTROY, EXISTS};

// homogeneous coordinate
struct h_Vector3f {
    Vector4f coord;

    h_Vector3f();
    h_Vector3f(const Vector3f& c_coord);
    ~h_Vector3f();

    friend Vector3f cartesian(const h_Vector3f& h_coord);
    void applyTransformation(const Matrix4f& transformation);

    bool operator==(const h_Vector3f& rhs) const;
    bool operator!=(const h_Vector3f& rhs) const;
    const h_Vector3f& operator+=(const h_Vector3f& rhs);
    const h_Vector3f& operator-=(const h_Vector3f& rhs);

    friend h_Vector3f operator+(const h_Vector3f& lhs, const h_Vector3f& rhs);
    friend h_Vector3f operator-(const h_Vector3f& lhs, const h_Vector3f& rhs);
    friend float dot(const h_Vector3f& lhs, const h_Vector3f& rhs);
    friend h_Vector3f cross(const h_Vector3f& lhs, const h_Vector3f& rhs);
};

// etc graphics util functions
float degrees(float radians);
float radians(float degrees);

// etc util functions and enums
enum SortType {ASCN, DSCN};
template<class T> bool push_backNFND(vector<T>& v, const T& element) {
    if (find(v.begin(), v.end(), element) == v.end()) {
        v.push_back(element);
        return true;
    }
    return false;
}
template<class T> bool removeFND(vector<T>& v, const T& element) {
    typename vector<T>::iterator it;
    it = find(v.begin(), v.end(), element);
    if (it != v.end()) {
        v.erase(it);
        return true;
    }
    return false;
}
template<class T> bool sortedInsert(
    vector<T>& v,
    const T& element,
    SortType type)
{
    typename vector<T>::iterator it;
    // make sure v is already sorted
    for (it = v.begin(); it != v.end() || it != v.end() - 1; it++)
    {
        switch (type) {
            case ASCN:
                if (*it > *(it + 1)) {
                    fprintf(stderr, "sortedInsert ERROR v is not sorted by SortType %d\n",
                        type);
                    return false;
                }
                break;
            case DSCN:
                if (*it < *(it + 1)) {
                    fprintf(stderr, "sortedInsert ERROR v is not sorted by SortType %d\n",
                        type);
                    return false;
                }
                break;
            default:
                fprintf(stderr, "sortedInsert ERROR invalid SortType %d\n",
                    type);
                return false;
        }
    }
    for (it = v.begin(); it != v.end(); it++) {
        switch (type) {
            case ASCN:
                if (*it >= element) {
                    v.insert(it, element);
                    return true;
                }
                break;
            case DSCN:
                if (*it <= element) {
                    v.insert(it, element);
                    return true;
                }
                break;
        }
    }
    v.push_back(element);
    return true;
}

#endif