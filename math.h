#pragma once

class Vector2 {
public:
  Vector2() : _x(0), _y(0) {}
  Vector2(int x, int y) : _x(x), _y(y) {}
  Vector2(const Vector2& v) { this->x(v.x()); this->y(v.y()); }
  int x() const { return _x; }
  int y() const { return _y; }
  void x(int x) { _x = x; }
  void y(int y) { _y = y; }

private:
  int _x;
  int _y;
};