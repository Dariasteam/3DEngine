project('3D', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

thread_dep = dependency('threads')

add_project_arguments('-pg', language : 'cpp')

executable('engine',
		'src/engine/buffers/commonbuffers.cpp',
		'src/engine/math/matrix.cpp',
		'src/engine/planar/texture.cpp',
		'src/engine/spatial/mesh.cpp',
		'src/engine/fragment/texturepainter.cpp',
		'src/engine/fragment/fragmentshader.cpp',
   	   	'src/engine/fragment/flatnormals.cpp',
		'src/engine/fragment/smoothnormals.cpp',
		'src/engine/fragment/normalmapping.cpp',
	   	'src/engine/fragment/shadowlesslightning.cpp',
	   	'src/engine/fragment/lightness.cpp',
	   	'src/engine/planar/textureprojector.cpp',
	   	'src/engine/rasteriser/abstractrasteriser.cpp',
	   	'src/engine/rasteriser/abstractrasteriserCPU.cpp',
	   	'src/engine/rasteriser/rasteriserflat.cpp',
	   	'src/auxiliar/multithreadmanager.cpp',
	   	'src/auxiliar/objparser.cpp',
	   	'src/frontends/framebufferhandler.cpp',
	   	'src/engine/spatial/spatial.cpp',
	   	'src/engine/spatial/directionallight.cpp',
	   	'src/engine/spatial/camera.cpp',
		'src/engine/spatial/parallelcamera.cpp',
	   	'src/engine/spatial/perspectivecamera.cpp',
	   	'src/engine/world.cpp',
	   	'src/engine/projector.cpp',
	   	'src/engine/renderengine.cpp',
	   	'src/main.cpp',
	   	dependencies : thread_dep,
		install : true)
