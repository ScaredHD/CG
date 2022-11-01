#pragma once
#include "camera.h"
#include "geometry.h"
#include "math.h"
#include "transformation.h"
#include <Eigen/Core>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

// screen space coordinates:
// origin at upper-left
//       y=0    y=1 
// x=0  (0,0)  (0,1)
// x=1  (1,0)  (1,1)
// width: distance along y-axis
// height: distance along x-axis
struct Frame {
    size_t width;
    size_t height;
    cv::Mat buffer;
    std::vector<std::vector<double>> zBuffer;

    Frame(size_t w, size_t h, size_t msaaLevel = 1) : width(w), height(h) {
        msaa = msaaLevel;
        zBuffer = decltype(zBuffer)(width * height,
                                    std::vector<double>(msaa * msaa, inf));
        clear();
    }

    // clear frame buffer and z-buffer
    void clear();

    // set rgb color of screen space point (x, y)
    // (r, g, b) in [0, 255] x [0, 255] x [0, 255]
    void setPixel(size_t x, size_t y, cv::Vec3b color) {
        buffer.at<cv::Vec3b>(x, y) = color;
    }

    size_t getMsaa() { return msaa; }

private:
    size_t msaa;
};

struct DepthAndColor {
    double depth;
    cv::Vec3b color;
};

struct Rasterizer {
    Rasterizer(const Camera& camera_, const Frame frame_)
        : camera(camera_), frame(frame_) {}

    // calculate and apply view transformation (camera -> projection ->
    // viewport) rasterize result after view transformation
    void draw(const Line& l);
    void draw(const Triangle& t);

    // calculating matrices
    Eigen::Matrix4d cameraTransformation() const;
    Eigen::Matrix4d projectionTransformation() const;
    Eigen::Matrix4d viewportTransformation() const;

    void clear() { frame.clear(); }

    Camera camera;
    Frame frame;

    void drawInScreenSpace(double x1, double y1, double x2, double y2, cv::Vec3b color);
    
private:
    void drawLessPositiveSlope(double x1, double y1, double x2, double y2, cv::Vec3b color);
    void drawGreaterPositiveSlope(double x1, double y1, double x2, double y2, cv::Vec3b color);
    void drawGreaterNegativeSlope(double x1, double y1, double x2, double y2, cv::Vec3b color);
    void drawLessNegativeSlope(double x1, double y1, double x2, double y2, cv::Vec3b color);

    // determine if (x, y) in screen space is inside t projected on the screen
    bool insideTriangle(double x, double y, const Triangle& t) const;
    // interpolation to get depth and color of (x, y) in screen space
    DepthAndColor interpolate(double x, double y, const Triangle& t) const;
};