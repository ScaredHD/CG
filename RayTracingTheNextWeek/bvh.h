#pragma once

#include "hittable.h"
#include "hittable_list.h"
#include "aabb.h"

#include <algorithm>
#include <memory>

static bool compareBoxes(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b,
                         int axis) {
    AABB boxA;
    AABB boxB;
    if (!a->boundingBox(0, 0, boxA) || !b->boundingBox(0, 0, boxB)) {
        std::cerr << "No bounding box\n";
    }
    return boxA.a[axis] < boxB.a[axis];
}

class BvhNode : public Hittable {
  public:
    BvhNode() = default;

    BvhNode(const HittableList& list, double t0, double t1)
        : BvhNode(list.objects, 0, list.objects.size(), t0, t1) {}
    BvhNode(const std::vector<std::shared_ptr<Hittable>>& srcObjects, size_t start, size_t end,
            double t0, double t1);

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override;
    bool boundingBox(double t0, double t1, AABB& outBox) const override;

  private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB box;
};

// HitRecord rec is not used.
inline bool BvhNode::hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const {
    if (!box.hit(r, tmin, tmax)) return false;
    bool hitLeft = left->hit(r, tmin, tmax, rec);
    bool hitRight = right->hit(r, tmin, hitLeft ? rec.t : tmax, rec);
    return hitLeft || hitRight;
}

inline bool BvhNode::boundingBox(double t0, double t1, AABB& outBox) const {
    outBox = box;
    return true;
}

BvhNode::BvhNode(const std::vector<std::shared_ptr<Hittable>>& srcObjects, size_t start, size_t end,
                 double t0, double t1) {
    auto objects = srcObjects;

    auto axis = gen.randomInt(0, 2);
    const auto& objectSpan = end - start;
    if (objectSpan == 1) {
        left = right = objects[start];
    } else if (objectSpan == 2) {
        if (compareBoxes(objects[start], objects[start + 1], axis)) {
            left = objects[start];
            right = objects[start + 1];
        } else {
            left = objects[start + 1];
            right = objects[start];
        }
    } else {
        auto first = objects.begin() + start;
        auto last = objects.begin() + end;
        std::sort(first, last,
                  [axis](const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b) {
                      return compareBoxes(a, b, axis);
                  });
        auto mid = start + objectSpan / 2;
        left = std::make_shared<BvhNode>(objects, start, mid, t0, t1);
        right = std::make_shared<BvhNode>(objects, mid, end, t0, t1);
    }

    AABB boxL;
    AABB boxR;
    if (!left->boundingBox(t0, t1, boxL) || !right->boundingBox(t0, t1, boxR)) {
        std::cerr << "No bounding box in BvhNode constructor\n";
    }

    box = surroundingBox(boxL, boxR);
}