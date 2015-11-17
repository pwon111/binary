#include "util.hpp"

// homogeneous coordinate

h_Vector3f::h_Vector3f() : coord() {

}
h_Vector3f::h_Vector3f(const Vector3f& c_coord) : coord() {
    this->coord << c_coord[0], c_coord[1], c_coord[2], 1.0;
}
h_Vector3f::~h_Vector3f() {

}

Vector3f cartesian(const h_Vector3f& h_coord) {
    Vector3f c_coord;
    c_coord <<  h_coord.coord[0] / h_coord.coord[3],
                h_coord.coord[1] / h_coord.coord[3],
                h_coord.coord[2] / h_coord.coord[3];
    return c_coord;
}
void h_Vector3f::applyTransformation(const Matrix4f& transformation) {
    this->coord = transformation * this->coord;
}

bool h_Vector3f::operator==(const h_Vector3f& rhs) const {
    return this->coord == rhs.coord;
}
bool h_Vector3f::operator!=(const h_Vector3f& rhs) const {
    return this->coord != rhs.coord;
}
const h_Vector3f& h_Vector3f::operator+=(const h_Vector3f& rhs) {
    h_Vector3f cartesian_sum = h_Vector3f(cartesian(*this) + cartesian(rhs));
    this->coord = cartesian_sum.coord;
    return *this;
}
const h_Vector3f& h_Vector3f::operator-=(const h_Vector3f& rhs) {
    h_Vector3f cartesian_sum = h_Vector3f(cartesian(*this) - cartesian(rhs));
    this->coord = cartesian_sum.coord;
    return *this;
}

h_Vector3f operator+(const h_Vector3f& lhs, const h_Vector3f& rhs) {
    return h_Vector3f(cartesian(lhs) + cartesian(rhs));
}
h_Vector3f operator-(const h_Vector3f& lhs, const h_Vector3f& rhs) {
    return h_Vector3f(cartesian(lhs) - cartesian(rhs));
}
float dot(const h_Vector3f& lhs, const h_Vector3f& rhs) {
    return cartesian(lhs).dot(cartesian(rhs));
}
h_Vector3f cross(const h_Vector3f& lhs, const h_Vector3f& rhs) {
    return h_Vector3f(cartesian(lhs).cross(cartesian(rhs)));
}

// etc graphics util functions
float degrees(float radians) {
    return radians / M_PI * 180;
}

float radians(float degrees) {
    return degrees / 180 * M_PI;
}
