#include "directionallight.h"

DirectionalLight::DirectionalLight(const Vector3& orientation,
                                   const Color& c,
                                   const double i) :
  ParallelCamera(orientation, RectF{-1, -1, 1, 1}),
  color (c),
  intensity (i)
{}
