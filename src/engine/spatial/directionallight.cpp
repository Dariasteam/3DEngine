#include "directionallight.h"

DirectionalLight::DirectionalLight(const Vector3& v_plane,
                                   const Color& c,
                                   const double i) :
  ParallelCamera(v_plane, RectF{-10, -10, 10, 10}),
  color (c),
  intensity (i)
{}
