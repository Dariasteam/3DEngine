#include "renderengine.h"

RenderEngine::RenderEngine() :
    world(World::get()),
    fps_render("RENDER")
  {
  CommonBuffers::get().set_dimension(1000, 1000);

  //fragmentShader.push_operation(new FlatNormals());
  //fragmentShader.push_operation(new TexturePainter());
  fragmentShader.push_operation(new Lightness());
}

void RenderEngine::render_loop () {
  while (1) {
    world.calculate_next_frame();

    projector.project_camera(world.get_light());
    rasteriser.rasterise(world.get_light());

    Texture<double, 1> z = CommonBuffers::get().z_buffer;
    Texture<unsigned char, 3> lightmap (z.get_height(), z.get_width());

    for (int i = 0; i < z.get_height() * z.get_width(); i++) {
      if (z.get(i) < INFINITY_DISTANCE) {
        lightmap.set(i * 3 + 0, 255);
        lightmap.set(i * 3 + 1, 255);
        lightmap.set(i * 3 + 2, 255);
      } else {
        lightmap.set(i * 3 + 0, 0);
        lightmap.set(i * 3 + 1, 0);
        lightmap.set(i * 3 + 2, 0);
      }
    }

    CommonBuffers::get().l_triangle_index_buffer = CommonBuffers::get().triangle_index_buffer;
    CommonBuffers::get().lightmap_buffer = lightmap;
    CommonBuffers::get().light_triangles = CommonBuffers::get().triangles;
    CommonBuffers::get().l_triangle_indices = CommonBuffers::get().triangle_indices;
    CommonBuffers::get().n_l_renderable_triangles = CommonBuffers::get().n_renderable_triangles;

    projector.project_camera(world.get_camera());
    rasteriser.rasterise(world.get_camera());
    fragmentShader();
    fps_render.update();

    canvas.paint(CommonBuffers::get().screen_buffer);
  }
}
