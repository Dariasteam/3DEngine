#ifndef VERTEX_H
#define VERTEX_H

#include "../math/point3.h"
#include "../math/vector3.h"
#include "../math/vector3unitary.h"

struct Vertex {
    Point3 point_local;
    Normal3 normal_local;

    Point3 point_global;
    Normal3 normal_global;
};

#endif // VERTEX_H