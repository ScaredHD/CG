#include "camera.h"

void Camera::moveForward(double dist) {
    pos += dist * gaze.normalized();
}

void Camera::moveRight(double dist) {
    pos += dist * cross(gaze.normalized(), top.normalized());
}

void Camera::moveUp(double dist) {
    pos += dist * top.normalized();
}

void Camera::lookup(double deg) {
    auto axis = cross(gaze.normalized(), top.normalized());
    // auto M = matrix::rotation(axis, deg);
    // gaze = (M * gaze.homogeneous()).head<3>();
    // top = (M * top.homogeneous()).head<3>();
    gaze = matrix::rotation(gaze, axis, deg);
    top = matrix::rotation(top, axis, deg);
}

void Camera::lookleft(double deg) {
    // auto M = matrix::rotation(top.normalized(), deg);
    // gaze = (M * gaze.homogeneous()).head<3>();
    // top = (M * top.homogeneous()).head<3>();
    gaze = matrix::rotation(gaze, top.normalized(), deg);
}
