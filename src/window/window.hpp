#ifndef WINDOW_H
#define WINDOW_H
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
namespace breeze {
namespace window {
class window
{
public:
  GLFWwindow* glfw_window;
  int width;
  int height;
  std::string window_title;

  window(int width, int height, std::string title)
    : width(width)
    , height(height)
    , window_title(title)
  {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfw_window =
      glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);
  }
  void make_current()
  {
    glfwMakeContextCurrent(glfw_window);
    gladLoadGL();
  }
  template <class T>
  T time()
  {
    return static_cast<T>(glfwGetTime());
  }

  template <class F>
  void loop(F&& f)
  {
    float current_time = time<float>();
    float last_time = current_time;
    while (!glfwWindowShouldClose(glfw_window)) {
      last_time = current_time;
      current_time = time<float>();
      float delta_time = current_time - last_time;
      f(delta_time);
    }
  }
  void swap_buffer(){
    glfwSwapBuffers(glfw_window);
  }
  void poll_events(){
    glfwPollEvents();
  }

  ~window() { glfwTerminate(); }
};
}
}
#endif // WINDOW_H
