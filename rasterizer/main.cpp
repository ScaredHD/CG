#include "geometry.h"
#include "rasterizer.h"
#include <Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace Eigen;

using std::cout;

int main() {
    namedWindow("lineDrawing", WINDOW_NORMAL);
    resizeWindow("lineDrawing", 720, 720);

    Camera camera;
    Frame frame(100, 100);
    Rasterizer ras(camera, frame);
    ras.drawLessPositiveSlope(0, 0, 50, 25, {255, 255, 255});
    ras.drawLessPositiveSlope(0, 0, 50, 50, {255, 0, 255});
    imshow("lineDrawing", ras.frame.buffer);
    waitKey();
}

// int main() {
//     namedWindow("testWindow", WINDOW_NORMAL);
//     resizeWindow("testWindow", 720, 720);

//     Camera camera({0, 0, 5}, {0, 0, -1}, {0, 1, 0}, 70, 1, 1, 50);
//     size_t framesize = 100;
//     size_t msaa = 1;
//     Frame frame(framesize, framesize, msaa);
//     Rasterizer ras(camera, frame);
//     // Triangle t1, t2, t3;
//     // t1.v0 = { 1, 0, -1 };
//     // t1.v1 = { 0, 1, -1 };
//     // t1.v2 = { -1, 0, -1 };
//     // t1.setColor({128, 0, 0}, {0, 128, 0}, {0, 0, 128});

//     Triangle t1, t2, t3;
//     t1.v0 = {1, 0, 0};
//     t1.v1 = {0, 1, 0};
//     t1.v2 = {0, 0, 0};
//     t2.v0 = {0, 0, 0};
//     t2.v1 = {0, 1, 0};
//     t2.v2 = {0, 0, 1};
//     t3.v0 = {1, 0, 0};
//     t3.v1 = {0, 0, 0};
//     t3.v2 = {0, 0, 1};
//     t1.setColor({128, 0, 0}, {0, 128, 0}, {0, 0, 128});
//     t2.setColor({0, 128, 0});
//     t3.setColor({0, 0, 128});

//     int key;
//     bool isRunning = true;
//     const double stepDist = 0.1;
//     const double stepAngle = 2;
//     while (isRunning) {
//         Camera& cam = ras.camera;
//         // std::cout << "cam pos = \n" << cam.pos << "\n\n";
//         // std::cout << "gaze = \n" << cam.gaze << "\n\n";
//         // std::cout << "top = \n" << cam.top << "\n\n";
//         // std::cout << "dot(g, t) = \n" << cam.gaze.dot(cam.top) << "\n\n";
//         std::cout << "msaa = " << ras.frame.getMsaa() << "\n\n";
//         ras.clear();
//         ras.draw(t1);
//         ras.draw(t2);
//         ras.draw(t3);
//         imshow("testWindow", ras.frame.buffer);

//         while ((key = waitKey()) == -1) {}
//         switch (key) {
//             case 'a':
//                 cam.moveRight(-stepDist);
//                 break;
//             case 'd':
//                 cam.moveRight(stepDist);
//                 break;
//             case 'w':
//                 cam.moveForward(stepDist);
//                 break;
//             case 's':
//                 cam.moveForward(-stepDist);
//                 break;
//             case 'i':
//                 cam.lookup(stepAngle);
//                 break;
//             case 'j':
//                 cam.lookleft(stepAngle);
//                 break;
//             case 'k':
//                 cam.lookup(-stepAngle);
//                 break;
//             case 'l':
//                 cam.lookleft(-stepAngle);
//                 break;
//             case 'h':
//                 msaa *= 2;
//                 ras = Rasterizer(cam, Frame(framesize, framesize, msaa));
//                 break;
//             case 'f':
//                 msaa = std::max(1ULL, msaa / 2);
//                 ras = Rasterizer(cam, Frame(framesize, framesize, msaa));
//                 break;
//             default:
//                 isRunning = false;
//                 break;
//         }
//     }

//     return 0;
// }