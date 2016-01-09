#ifndef MATH_H
#define MATH_H

#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>
#include <array>
#include <cmath>
#include <iostream>

namespace breeze {
namespace math = glm;
//template <class T>
//struct tvec2
//{
//  T x;
//  T y;
//  tvec2() = default;
//  tvec2(T x, T y)
//    : x(x)
//    , y(y)
//  {
//  }
//
//  tvec2<T> operator+(const tvec2<T>& v) { return tvec2{ x + v.x, y + v.y }; }
//  tvec2<T> operator*(const T& v) { return tvec2{ x * v, y * v }; }
//
//  T dot(const tvec2<T>& v) { return x * v.x + y * v.y; }
//
//  void operator+=(const tvec2<T>& v) { *this = *this + v; }
//};
//template <class T>
//std::ostream& operator<<(std::ostream& os, const tvec2<T>& v)
//{
//  return os << "vec2(" << v.x << ", " << v.y << ")";
//}
//template <class T>
//struct tmat2
//{
//  tvec2<T> row1;
//  tvec2<T> row2;
//  tvec2<T> operator*(const tvec2<T>& v)
//  {
//    return tvec2<T> { row1.x *v.x + row1.y *v.y, row2.x *v.x + row2.y *v.y };
//  }
//
//  tmat2<T> operator*(const tmat2<T>& m)
//  {
//    return tmat2<T>{ tvec2<T>{ row1.x * m.row1.x + row1.y * m.row2.x,
//                               row1.x * m.row1.y + row1.y * m.row2.y },
//                     tvec2<T>{ row2.x * m.row1.x + row2.y * m.row2.x,
//                               row2.x * m.row1.y + row2.y * m.row2.y } };
//  }
//  static tmat2<T> scale(T width, T height)
//  {
//    return tmat2{ tvec2<T>{ width, 0 }, tvec2<T>{ 0, height } };
//  }
//  static tmat2<T> rotation(float alpha)
//  {
//    return tmat2{ tvec2<T>{ std::cos(alpha), -std::sin(alpha) },
//                  tvec2<T>{ std::sin(alpha),  std::cos(alpha) } };
//  }
//};
//template <class T>
//std::ostream& operator<<(std::ostream& os, const tmat2<T>& m)
//{
//  os << "mat2(" << m.row1 << std::endl;
//  os << "     " << m.row2 << ")";
//  return os;
//}
//using vec2 = tvec2<float>;
//using mat2 = tmat2<float>;
}
#endif // MATH_H
