#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "hittable.h"

class HittableList : public Hittable {
  public:
    HittableList() = default;
    HittableList(const std::shared_ptr<Hittable>& object) { add(object); }

    void clear() { objects.clear(); }
    void add(const std::shared_ptr<Hittable>& object) { objects.push_back(object); }

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override;

  private:
    std::vector<std::shared_ptr<Hittable>> objects;
};

inline bool HittableList::hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const {
    HitRecord tempRec;
    bool hitAnything = false;

    for (const auto& p : objects) {
        if (p->hit(r, tmin, tmax, tempRec)) {
            hitAnything = true;
            tmax = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}