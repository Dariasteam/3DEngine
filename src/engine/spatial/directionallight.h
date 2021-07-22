#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "parallelcamera.h"

class DirectionalLight : public ParallelCamera {
private:
  Color color;
  double intensity;
public:
  DirectionalLight(const Vector3& orientation,
                   const Color& color,
                   const double instensity);
  inline const Color& get_color () const { return color; }
  inline double get_intensity () const { return intensity; }
};

#endif // DIRECTIONALLIGHT_H
