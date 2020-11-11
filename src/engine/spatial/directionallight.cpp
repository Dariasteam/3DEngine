#include "directionallight.h"

DirectionalLight::DirectionalLight(const Vector3& orientation,
                                   const Color& c,
                                   const double i) :
  ParallelCamera(orientation, Point3{0, 0, 0}),
  color (c),
  intensity (i)
{}
