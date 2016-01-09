#ifndef GRAPHICS_H
#define GRAPHICS_H

// Local Headers
// System Headers
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

// Standard Headers
#include <algorithm>
#include <memory>
#include <vector>
#include <cstdio>
#include <array>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

#include <math/math.hpp>
namespace breeze {
namespace graphics {

struct rectangle
{
  std::array<breeze::math::vec4, 4> verts;
  rectangle() = delete;
  explicit rectangle(std::array<breeze::math::vec4, 4>& verts)
    : verts(verts)
  {
  }
  explicit rectangle(math::vec2 min, math::vec2 max, float depth)
  {
    verts[0] = math::vec4(min, depth, 1);
    verts[1] = math::vec4(math::vec2(max.x, min.y), depth, 1);
    verts[2] = math::vec4(max, depth, 1);
    verts[3] = math::vec4(math::vec2(min.x, max.y), depth, 1);
  }
  // explicit rectangle(math::vec2 pivot, float horizonal_size,
  //                   float vertical_size)
  //{
  //  verts[0] = math::vec2(0.0f, 0.0f) + pivot;
  //  verts[1] = math::vec2(horizonal_size, 0.0f) + pivot;
  //  verts[2] = math::vec2(horizonal_size, vertical_size) + pivot;
  //  verts[3] = math::vec2(0.0f, vertical_size) + pivot;
  //}
  rectangle operator*(const math::mat4& m)
  {
    std::array<math::vec4, 4> verts;
    for (std::size_t i = 0; i <= 4; ++i) {
      verts[i] = m * this->verts[i];
    }
    return rectangle(verts);
  }
};

struct gl_texture_array
{
  GLuint texture_id;
  int width;
  int height;
  std::size_t size;
  std::vector<std::size_t> freelist;
  void add_texture(std::string filename) {}

  void grow()
  {
    //    GLuint fbo = 0;
    //    glGenFramebuffers(1, &fbo);
    //    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    //
    //    glBindTexture(GL_TEXTURE_2D_ARRAY, new_tex_id);
    //
    //    for (int layer = 0; layer < 100; ++layer) {
    //      glFramebufferTextureLayer(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
    //                                texture_id, 0, layer);
    //      glCopyTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, 0, 0,
    //      width,
    //                          height);
    //    }
  }

  void test()
  {

    GLuint new_tex_id;
    glGenTextures(1, &new_tex_id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, new_tex_id);
    glTexImage3D(GL_TEXTURE_2D_ARRAY,
                 0,                // No mipmaps
                 GL_RGBA8,         // Internal format
                 width, height, 2, // width,height,layer count
                 0,                // border?
                 GL_RGBA,          // format
                 GL_UNSIGNED_BYTE, // type
                 0);               // pointer to data
    GLuint fbo = 0;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

    for (int layer = 0; layer < 2; ++layer) {
      glFramebufferTextureLayer(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                texture_id, 0, layer);
      glCopyTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, 0, 0, width,
                          height);
    }
    texture_id = new_tex_id;
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
  }
  explicit gl_texture_array(std::string filename)
  {
    int comp;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image =
      stbi_load(filename.c_str(), &width, &height, &comp, 0);

    if (image == nullptr)
      throw(std::string("Failed to load texture"));

    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);

    glTexImage3D(GL_TEXTURE_2D_ARRAY,
                 0,                // No mipmaps
                 GL_RGBA8,         // Internal format
                 width, height, 2, // width,height,layer count
                 0,                // border?
                 GL_RGBA,          // format
                 GL_UNSIGNED_BYTE, // type
                 0);               // pointer to data
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                    0,                // Mipmap number
                    0, 0, 0,          // xoffset, yoffset, zoffset
                    width, height, 1, // width, height, depth
                    GL_RGBA,          // format
                    GL_UNSIGNED_BYTE, // type
                    image);           // pointer to data

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                    0,                // Mipmap number
                    0, 0, 0,          // xoffset, yoffset, zoffset
                    width, height, 2, // width, height, depth
                    GL_RGBA,          // format
                    GL_UNSIGNED_BYTE, // type
                    image);           // pointer to data
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    stbi_image_free(image);
  }
  void bind()
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
};

struct gl_texture
{
  GLuint texture_id;
  int width;
  int height;
  gl_texture(const gl_texture&) = delete;
  gl_texture(gl_texture&& tex)
    : texture_id(tex.texture_id)
  {
    tex.texture_id = 0;
  }
  gl_texture() = delete;
  void bind()
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  gl_texture(std::string filename)
  {
    int comp;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image =
      stbi_load(filename.c_str(), &width, &height, &comp, 0);

    if (image == nullptr)
      throw(std::string("Failed to load texture"));

    glGenTextures(1, &texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);

    if (comp == 3)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, image);
    else if (comp == 4)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, image);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);
  }
  ~gl_texture() { glDeleteTextures(1, &texture_id); }
};
struct gl_quad
{
  rectangle rect;
  GLuint vbo;
  GLuint uvs;
  GLuint ibo;
  GLuint vao;

  gl_quad(const gl_quad& gl_rect) = delete;
  gl_quad(gl_quad&& gl_rect)
    : rect(std::move(gl_rect.rect))
    , vbo(gl_rect.vbo)
    , uvs(gl_rect.uvs)
    , ibo(gl_rect.ibo)
    , vao(gl_rect.vao)

  {
    gl_rect.vbo = 0;
    gl_rect.ibo = 0;
    gl_rect.uvs = 0;
    gl_rect.vao = 0;
  }

  gl_quad() = delete;
  explicit gl_quad(math::vec2 min, math::vec2 max, float depth)
    : rect(rectangle(min, max, depth))
  {
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    glGenBuffers(1, &uvs);
    glGenVertexArrays(1, &vao);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s)
    // and attribute pointer(s).
    // glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect.verts), &rect.verts,
                 GL_STATIC_DRAW);
    std::array<math::vec2, 4> uv{ math::vec2(0.0f, 0.0f),
                                  math::vec2(1.0f, 0.0f),
                                  math::vec2(1.0f, 1.0f),
                                  math::vec2(0.0f, 1.0f) };
    glBindBuffer(GL_ARRAY_BUFFER, uvs);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), &uv, GL_STATIC_DRAW);
    GLuint indices[] = { 0, 1, 3, 1, 2, 3 };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(math::vec3),
                          (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvs);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(math::vec2),
                          (GLvoid*)0);
    glBindVertexArray(0);
  }
  void bind() { glBindVertexArray(vao); }
  ~gl_quad()
  {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &uvs);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
  }
};
struct sprite_batch
{
  std::weak_ptr<gl_texture> texture;
  std::vector<gl_quad> quads;
  explicit sprite_batch(std::weak_ptr<gl_texture> tex)
    : texture(tex)
  {
  }
};

struct ressource
{
  std::unordered_map<std::string, std::shared_ptr<gl_texture>> textures;

  bool add_tex(std::string name, std::string path)
  {
    if (textures.count(name)) {
      return false;
    }
    textures.insert({ name, std::make_shared<gl_texture>(path) });
    return true;
  }
  std::weak_ptr<gl_texture> get(std::string name)
  {
    if (textures.count(name)) {
      return textures[name];
    }
    return std::weak_ptr<gl_texture>{};
  }
};

struct sprite
{
  math::vec2 position;
  math::vec2 size;
  std::weak_ptr<gl_texture> tex_ptr;

  sprite(math::vec2 pos, math::vec2 size, std::weak_ptr<gl_texture> tex_ptr)
    : position(pos)
    , size(size)
    , tex_ptr(tex_ptr)
  {
  }
};
class vertex_data
{
public:
  GLuint vbo, ibo, vao, ubo;
  std::vector<math::vec4> vertices;
  std::vector<math::vec2> uvs;
  std::vector<GLuint> indices;

  vertex_data()
  {
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    glGenBuffers(1, &ubo);
    glGenVertexArrays(1, &vao);
  }
  void submit_data()
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(math::vec4),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, ubo);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(math::vec2), &uvs[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, ubo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glBindVertexArray(0);
  }

  void bind_vao() { glBindVertexArray(vao); }

  ~vertex_data()
  {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ubo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
  }
};

struct gl_renderer2d
{
  std::unordered_map<std::shared_ptr<gl_texture>, vertex_data> batch;
  GLuint shader_program;
  //  gl_quad gl_rect{ math::vec2(-0.5f, -0.5f), 1.0f, 1.0f };
  //  rectangle rect{ math::vec2(-0.5f, -0.5f), 1.0f, 1.0f };
  std::vector<sprite_batch> batches;
  float width, height;
  void clear()
  {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  // void draw(std::weak_ptr<gl_texture> tex, float depth, math::vec2 pos,
  //          float angle, float scale)
  //{
  //  glUseProgram(shader_program);
  //  math::mat4x4 model;
  //  float s = (float)tex.lock()->width / (float)tex.lock()->height;
  //  model = math::translate(model, math::vec3(pos, 0.0f));
  //  model = math::rotate(model, angle, math::vec3(0, 0, 1.0f));
  //  model = math::scale(model, math::vec3(scale, scale, 1.0f));
  //  model = math::scale(model, math::vec3(s, 1.0f, 1.0f));
  //  math::mat4x4 ortho = math::ortho(-width, width, -height, height, -1.f,
  //  1.f);
  //  model = ortho * model;
  //  // model = math::rotate(model, angle);

  //  tex.lock()->bind();
  //  gl_rect.bind();
  //  GLint sampler = glGetUniformLocation(shader_program, "sampler");
  //  GLint matrix = glGetUniformLocation(shader_program, "matrix");
  //  GLint depth_uniform = glGetUniformLocation(shader_program, "depth");

  //  glUniform1i(sampler, 0);
  //  glUniform1f(depth_uniform, depth);
  //  glUniformMatrix4fv(matrix, 1, GL_FALSE, glm::value_ptr(model));
  //  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  //}
  void submit(std::weak_ptr<gl_texture> tex, float depth, math::vec2 pos,
              float angle, float scale)
  {
    rectangle r{ math::vec2(-1, -1), math::vec2(1, 1), depth };
    math::mat4 model;
    float s = (float)tex.lock()->width / (float)tex.lock()->height;
    model = math::translate(model, math::vec3(pos.x, pos.y, 0));
    model = math::rotate(model, angle, math::vec3(0, 0, 1));
    model = math::scale(model, math::vec3(scale, scale, 1));
    model = math::scale(model, math::vec3(s, 1.0f,1.0f));

    auto new_rect = r * model;
    auto& data = batch[tex.lock()];
    std::array<math::vec2, 4> uv{ math::vec2(0.0f, 0.0f),
                                  math::vec2(1.0f, 0.0f),
                                  math::vec2(1.0f, 1.0f),
                                  math::vec2(0.0f, 1.0f) };
    std::array<GLuint, 6> indices = { 0, 1, 3, 1, 2, 3 };
    std::array<GLuint, 6> indices_out;
    std::size_t size = data.vertices.size();
    std::transform(indices.begin(), indices.end(), indices_out.begin(),
                   [size](GLuint index) { return index + size; });
    data.vertices.insert(data.vertices.end(), new_rect.verts.begin(),
                         new_rect.verts.end());
    data.uvs.insert(data.uvs.end(), uv.begin(), uv.end());
    data.indices.insert(data.indices.end(), indices_out.begin(),
                        indices_out.end());
  }
  void render()
  {
    glUseProgram(shader_program);
    for (auto& p : batch) {
      auto& data = std::get<1>(p);
      if (data.vertices.size() == 0) {
        break;
      }
      auto& tex = std::get<0>(p);

      math::mat4x4 ortho =
        math::ortho(-width, width, -height, height, -1.f, 1.f);
      tex->bind();
      data.submit_data();
      data.bind_vao();
      GLint sampler = glGetUniformLocation(shader_program, "sampler");
      GLint matrix = glGetUniformLocation(shader_program, "matrix");

      glUniform1i(sampler, 0);
      glUniformMatrix4fv(matrix, 1, GL_FALSE, glm::value_ptr(ortho));
      glDrawElements(GL_TRIANGLES, data.indices.size(), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    }
    batch.clear();
  }

  // void draw()
  //{
  //  GLint sampler = glGetUniformLocation(shader_program, "sampler");
  //  glUseProgram(shader_program);

  //  // for (auto& s : sprites) {
  //  //  //s.tex_ptr.lock()->bind();
  //  //  gl_text.bind();
  //  //  glUniform1i(sampler, 0);
  //  //  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  //  //}
  //  for (auto& batch : batches) {
  //    batch.texture.lock()->bind();
  //    for (auto& quad : batch.quads) {
  //      quad.bind();
  //      glUniform1i(sampler, 0);
  //      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  //    }
  //  }
  //}
  gl_renderer2d(float width, float height)
    : width(width)
    , height(height)
  {
    const GLchar* vertexShaderSource =
      "#version 430 core\n"
      "layout (location = 0) in vec4 position;\n"
      "layout (location = 1) in vec2 uv;\n"
      "uniform mat4x4 matrix;\n"
      "out vec2 o_uv;\n"
      "void main()\n"
      "{\n"
      "vec4 new_pos = vec4(position.xyz, 1); "
      "gl_Position = matrix * new_pos; \n"
      "o_uv = uv;\n"
      "}\0";
    const GLchar* fragmentShaderSource =
      "#version 430 core\n"
      "uniform sampler2D sampler;\n"
      "in vec2 o_uv;\n"
      "out vec4 color;\n"
      "void main()\n"
      "{\n"
      "//color = vec4(1,0,0,1);\n"
      "color = texture(sampler, o_uv);\n"
      "if(color.a == 0) discard;\n"
      "//color = vec4(o_uv.x, o_uv.y, 0.0f, 1.0f);\n"
      "}\n\0";
    glEnable(GL_STENCIL_TEST);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog
                << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog
                << std::endl;
    }
    // Link shaders
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexShader);
    glAttachShader(shader_program, fragmentShader);
    glLinkProgram(shader_program);
    // Check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog
                << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s)
    // and attribute pointer(s).
  }
  ~gl_renderer2d() {}
};
}
}

#endif // GRAPHICS_H
