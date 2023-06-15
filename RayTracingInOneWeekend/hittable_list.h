#pragma once

#include <memory>
#include <vector>

#include "hittable.h"

class HittableList : public Hittable {
  public:
    HittableList() = default;
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    virtual bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override;

  private:
    std::vector<std::shared_ptr<Hittable>> objects;
};

inline bool HittableList::hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const {
    HitRecord tempRec;
    bool hitAnything = false;

    for (auto p : objects) {
        if (p->hit(r, tmin, tmax, tempRec)) {
            hitAnything = true;
            tmax = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}