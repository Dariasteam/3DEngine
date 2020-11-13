#include "directionallight.h"

DirectionalLight::DirectionalLight(const Vector3& orientation,
                                   const Color& c,
                                   const double i) :
  ParallelCamera(orientation, RectF{-12, -12, 12, 12}),
  color (c),
  intensity (i)
{}
