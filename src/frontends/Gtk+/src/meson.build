engine3d_sources = [
  'main.cpp',
  'engine3d-window.cpp',
  'canvas.cpp',
  'renderengine.cpp',
  '../../../engine/matrix.cpp',
  '../../../engine/camera.cpp',
  '../../../engine/point3d.cpp',
  '../../../engine/point2d.cpp',
  '../../../engine/rasteriser.cpp',
  '../../../engine/triangle.cpp',
  '../../../engine/world.cpp'  
]

engine3d_deps = [
  dependency('gtkmm-3.0', version: '>= 3.18'),
]

gnome = import('gnome')

engine3d_sources += gnome.compile_resources('engine3d-resources',
  'engine3d.gresource.xml',
  c_name: 'engine3d'
)

executable('engine3d', engine3d_sources,
  dependencies: engine3d_deps,
  install: true,
)
