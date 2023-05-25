#include "geometry.h"
#include "transformation.h"

Mat4 Model::modelTransformation() const {
    return translateTransform(location) * rotateTransform(rotation[0], rotation[1], rotation[2]) *
           scaleTransform(scale[0], scale[1], scale[2]);
}
