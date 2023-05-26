#include "geometry.h"
#include "transformation.h"

Mat4 Model::modelTransformation() const {
    auto m1 = translateTransform(location);
    auto m2 = rotateTransform(rotation[0], rotation[1], rotation[2]);
    auto m3 = scaleTransform(scale[0], scale[1], scale[2]);
    return m1 * m2 * m3;
}
