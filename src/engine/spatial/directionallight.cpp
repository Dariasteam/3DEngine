#include "directionallight.h"

DirectionalLight::DirectionalLight(const Vector3& v_plane,
                                   const Color& c,
                                   const double i) :
  PerspectiveCamera(v_plane, RectF{-1, -1, 1, 1}),
  color (c),
  intensity (i)
{}
