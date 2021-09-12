#include "rasterizer.h"

void Frame::clear() {
    cv::Mat init(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
    buffer = init.clone();
    for (auto& subpixels : zBuffer)
        for (auto& subpx : subpixels) subpx = inf;
}

void Rasterizer::draw(const Line& l) {
    auto u = l.u;
    auto v = l.v;
    if (u.x() > u.y()) { std::swap(u, v); }
    auto x1 = u.x();
    auto y1 = u.y();
    auto x2 = v.x();
    auto y2 = v.y();
    auto slope = (y1 - y2) / (x1 - x2);
    if (slope > 0) {
        if (slope < 1) {
        } else {
        }
    } else {
        if (slope > -1) {

        } else {
        }
    }
}

void Rasterizer::drawInScreenSpace(double x1, double y1, double x2, double y2,
                                   cv::Vec3b color) {
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    double slope = (y2 - y1) / (x2 - x1);
    if (slope > 0) {
        if (slope < 1)
            drawLessPositiveSlope(x1, y1, x2, y2, color);
        else
            drawGreaterPositiveSlope(x1, y1, x2, y2, color);
    } else {
        if (slope > -1)
            drawGreaterNegativeSlope(x1, y1, x2, y2, color);
        else
            drawLessNegativeSlope(x1, y1, x2, y2, color);
    }
}

void Rasterizer::drawGreaterPositiveSlope(double x1, double y1, double x2,
                                          double y2, cv::Vec3b color) {
    auto f = [=](double x, double y) {
        return (y1 - y2) * x + (x2 - x1) * y + (x1 * y2 - x2 * y1);
    };
    size_t bottom = static_cast<size_t>(y1);
    size_t top = static_cast<size_t>(y2);
    size_t x = static_cast<size_t>(x1);
    for (size_t y = bottom; y <= top; ++y) {
        frame.setPixel(x, y, color);
        if (f(x + 0.5, y + 1) > 0) { ++x; };
    }
}

void Rasterizer::drawLessPositiveSlope(double x1, double y1, double x2,
                                       double y2, cv::Vec3b color) {
    auto f = [=](double x, double y) {
        return (y1 - y2) * x + (x2 - x1) * y + (x1 * y2 - x2 * y1);
    };

    size_t left = static_cast<size_t>(x1);
    size_t right = static_cast<size_t>(x2);
    size_t y = static_cast<size_t>(y1);
    for (size_t x = left; x <= right; ++x) {
        frame.setPixel(x, y, color);
        if (f(x + 1, y + 0.5) < 0) { ++y; }
    }
}

void Rasterizer::draw(const Triangle& t) {
    std::vector<Eigen::Vector3d> vertices = {t.v0, t.v1, t.v2};
    auto MV = cameraTransformation();
    auto P = projectionTransformation();
    auto VP = viewportTransformation();
    auto viewTransformation = VP * P * MV;
    for (auto& v : vertices) {
        Eigen::Vector4d vec = viewTransformation * v.homogeneous();
        // normalization of w here is critical
        // v = (x, y, z, 1) after view transformation may become
        // T * v = (x', y', z', w) == (x'/w, y'/w, z'/w, 1)
        // (x'/w, y'/w, z'/w) is the correct coordinates
        vec = vec / vec.w();
        v = vec.head<3>();
    }

    // triangle to be rasterized in screen space
    Triangle tScreen(vertices[0], vertices[1], vertices[2], t.color0, t.color1,
                     t.color2);
    tScreen.print();

    // bounding box: note that now it's in screen space (x toward down)
    auto u = vertices[0];
    auto v = vertices[1];
    auto w = vertices[2];
    auto topf = std::min({u.x(), v.x(), w.x()});
    auto bottomf = std::max({u.x(), v.x(), w.x()});
    auto leftf = std::min({u.y(), v.y(), w.y()});
    auto rightf = std::max({u.y(), v.y(), w.y()});
    auto top = static_cast<size_t>(clamp(topf, 0.0, double(frame.height - 1)));
    auto bottom =
        static_cast<size_t>(clamp(bottomf, 0.0, double(frame.height - 1)));
    auto left = static_cast<size_t>(clamp(leftf, 0.0, double(frame.width - 1)));
    auto right =
        static_cast<size_t>(clamp(rightf, 0.0, double(frame.width - 1)));

    std::cout << "bounding box: [" << top << ", " << bottom << "] x [" << left
              << ", " << right << "]\n\n";

    // rasterization with z-buffering
    for (size_t r = top; r <= bottom; ++r) {
        for (size_t c = left; c <= right; ++c) {
            std::vector<double>& subpixels = frame.zBuffer[r * frame.width + c];
            // subpixel rasterization
            const size_t msaa = frame.getMsaa();
            double subpixelSize = 1.0 / msaa;
            cv::Vec3b originalColor = frame.buffer.at<cv::Vec3b>(r, c);
            cv::Vec3i color = cv::Vec3i(0, 0, 0);
            for (size_t i = 0; i < msaa; ++i) {
                for (size_t j = 0; j < msaa; ++j) {
                    double x = r + i * subpixelSize + subpixelSize / 2;
                    double y = c + j * subpixelSize + subpixelSize / 2;
                    cv::Vec3b subpixelColor = originalColor;
                    if (insideTriangle(x, y, tScreen)) {
                        DepthAndColor p = interpolate(x, y, tScreen);
                        double& depth = subpixels[i * msaa + j];
                        if (p.depth < depth) {
                            depth = p.depth;
                            subpixelColor = p.color;
                        }
                    }
                    color[0] += subpixelColor[0];
                    color[1] += subpixelColor[1];
                    color[2] += subpixelColor[2];
                }
            }
            color[0] /= (msaa * msaa);
            color[1] /= (msaa * msaa);
            color[2] /= (msaa * msaa);
            frame.setPixel(r, c, color);
        }
    }
}

Eigen::Matrix4d Rasterizer::cameraTransformation() const {
    auto x = camera.pos(0);
    auto y = camera.pos(1);
    auto z = camera.pos(2);
    Eigen::Matrix4d toOrigin;
    toOrigin << 1, 0, 0, -x, 0, 1, 0, -y, 0, 0, 1, -z, 0, 0, 0, 1;
    auto g = camera.gaze.normalized();
    auto t = camera.top.normalized();
    auto gxt = g.cross(t);
    Eigen::Matrix4d camRotation;
    camRotation << gxt(0), gxt(1), gxt(2), 0, t(0), t(1), t(2), 0, -g(0), -g(1),
        -g(2), 0, 0, 0, 0, 1;
    // std::cout << "camera = \n" << camRotation * toOrigin << "\n\n";
    return camRotation * toOrigin;
}

Eigen::Matrix4d Rasterizer::projectionTransformation() const {
    auto fov = camera.fovDeg / 180 * pi;
    auto n = -camera.zNear;
    auto f = -camera.zFar;
    auto t = abs(n) * tan(fov / 2);
    auto b = -t;
    auto r = t * camera.aspectRatio;
    auto l = -r;
    Eigen::Matrix4d orth;
    orth << 2 / (r - l), 0, 0, -(r + l) / (r - l), 0, 2 / (t - b), 0,
        -(t + b) / (t - b), 0, 0, 2 / (n - f), -(n + f) / (n - f), 0, 0, 0, 1;
    Eigen::Matrix4d perspToOrth;
    perspToOrth << n, 0, 0, 0, 0, n, 0, 0, 0, 0, n + f, -n * f, 0, 0, 1, 0;
    auto mat = orth * perspToOrth;
    // std::cout << "projection = \n" << mat << "\n\n";
    return mat;
}

Eigen::Matrix4d Rasterizer::viewportTransformation() const {
    // scale canonical square to screen size
    // then move to the fourth quadrant
    Eigen::Matrix4d mat;
    auto w = static_cast<double>(frame.width);
    auto h = static_cast<double>(frame.height);
    mat << w / 2, 0, 0, w / 2, 0, h / 2, 0, -h / 2, 0, 0, 1, 0, 0, 0, 0, 1;
    // axes 90deg clockwise or
    // points 90deg counterclockwise
    Eigen::Matrix4d rotation;
    rotation << 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1;
    // std::cout << "viewport = \n" << rotation * mat << "\n\n";
    return rotation * mat;
}

bool Rasterizer::insideTriangle(double x, double y, const Triangle& t) const {
    Eigen::Vector3d p(x, y, 0);
    auto u = t.v0;
    auto v = t.v1;
    auto w = t.v2;
    auto res0 = cross(p - u, v - u).z();
    auto res1 = cross(p - v, w - v).z();
    auto res2 = cross(p - w, u - w).z();
    bool isInside1 = res0 > 0 && res1 > 0 && res2 > 0;
    bool isInside2 = res0 < 0 && res1 < 0 && res2 < 0;
    return (isInside1 || isInside2 ? true : false);
}

DepthAndColor Rasterizer::interpolate(double x, double y,
                                      const Triangle& t) const {
    auto xa = t.v0.x();
    auto ya = t.v0.y();
    auto xb = t.v1.x();
    auto yb = t.v1.y();
    auto xc = t.v2.x();
    auto yc = t.v2.y();
    auto alpha = (-(x - xb) * (yc - yb) + (y - yb) * (xc - xb))
               / (-(xa - xb) * (yc - yb) + (ya - yb) * (xc - xb));
    auto beta = (-(x - xc) * (ya - yc) + (y - yc) * (xa - xc))
              / (-(xb - xc) * (ya - yc) + (yb - yc) * (xa - xc));
    auto gamma = 1 - alpha - beta;
    auto depth0 = -t.v0.z();
    auto depth1 = -t.v1.z();
    auto depth2 = -t.v2.z();
    auto depth = alpha * depth0 + beta * depth1 + gamma * depth2;
    unsigned r = alpha * t.color0[0] + beta * t.color1[0] + gamma * t.color2[0];
    unsigned g = alpha * t.color0[1] + beta * t.color1[1] + gamma * t.color2[1];
    unsigned b = alpha * t.color0[2] + beta * t.color1[2] + gamma * t.color2[2];
    auto color = cv::Vec3b(r, g, b);
    return {depth, color};
}
