// Local Headers
// System Headers
#include <iostream>
#include <glad/glad.h>
#include "math/math.hpp"
#include <GLFW/glfw3.h>
#include "graphics/graphics.hpp"
#include "util/container/handlvec.hpp"
#include "ctti/type_id.hpp"
#include <window/window.hpp>
#include <thread>
#include <future>
#include <string>
#include <chrono>
#include <mutex>
#include <async++.h>
#include <asio.hpp>

using namespace breeze::graphics;
using namespace breeze::math;
using namespace breeze::util::container;

int
main()
{
  auto window = breeze::window::window(1080, 720, "OpenGL");
  window.make_current();

  ressource res{};
  res.add_tex("char", "/home/maik/Downloads/png/Idle__000.png");
  res.add_tex("wall", "/home/maik/Downloads/wall.jpg");
  auto char_wp = res.get("char");
  auto wall_wp = res.get("wall");

  gl_renderer2d render{ 1080, 720 };
  // glfwMakeContextCurrent(nullptr);
  async::task<void> t = async::spawn([] {});
  float r = 1.0f;
  window.loop([&](float dt) {
    render.clear();
    for (int i = 0; i < 2; ++i) {
      render.submit(char_wp, 1.0f, vec2(i * 500, 0.), r, 600.0f);
      render.submit(wall_wp, 0.0f, vec2(i * 500, 0.), r, 600.0f);
    }
    render.render();
    window.swap_buffer();
    window.poll_events();
    std::cout << 1 / dt << std::endl;
    // glfwMakeContextCurrent(nullptr);
    // if (t.ready()) {
    //  t = async::spawn([&] {
    //    glfwMakeContextCurrent(window.glfw_window);
    //    render.clear();
    //    for (int i = 0; i < 100; ++i) {
    //      render.submit(char_wp, 0.0f, vec2(i * 10, 0.), i * 100, 500.0f);
    //      //   render.draw(char_wp, -.5f, vec2(0, 0.), 10 * 2 + pi<float>() /
    //      //   2.0f,
    //      //               500.0f);
    //    }
    //    render.render();
    //    window.swap_buffer();
    //    window.poll_events();
    //    glfwMakeContextCurrent(nullptr);
    //  });
    //}
    r += dt;
    // std::cout << 1 / dt << std::endl;
    if (glfwGetKey(window.glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window.glfw_window, true);
    }

  });
}
