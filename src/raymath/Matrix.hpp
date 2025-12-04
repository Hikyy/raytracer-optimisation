#pragma once

#include <iostream>
#include "Vector3.hpp"

class  Matrix
{
private:
  double matrix[4][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  };

public:
  Matrix();
  Matrix(double (*)[4][4]);
  ~ Matrix();

  const Matrix operator*(Matrix const& right) const;
  const Vector3 operator*(Vector3 const& point) const;
  Matrix& operator=(Matrix const& mat);

  friend std::ostream & operator<<(std::ostream & _stream, Matrix const& mat);
};

