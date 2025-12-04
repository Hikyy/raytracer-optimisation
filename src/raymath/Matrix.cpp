#include <iostream>
#include "Matrix.hpp"

Matrix::Matrix()
{
}

Matrix::~Matrix()
{
}

Matrix::Matrix(double (*mat)[4][4])
{
  for (int row = 0; row < 4; row++)
  {
    for (int col = 0; col < 4; col++)
    {
      this->matrix[row][col] = (*mat)[row][col];
    }
  }
}

const Matrix Matrix::operator*(Matrix const &right) const
{
  Matrix result;

  for (int row = 0; row < 4; row++)
  {
    for (int col = 0; col < 4; col++)
    {

      result.matrix[row][col] = 0;
      for (int m = 0; m < 4; m++)
      {
        result.matrix[row][col] += this->matrix[row][m] * right.matrix[m][col];
      }
    }
  }

  return result;
}

const Vector3 Matrix::operator*(Vector3 const &point) const
{
  double pt[4] = {point.x, point.y, point.z, 1};
  double ptM[4] = {0, 0, 0, 0};
  for (int row = 0; row < 4; row++)
  {
    for (int col = 0; col < 4; col++)
    {
      ptM[row] += this->matrix[row][col] * pt[col];
    }
  }

  Vector3 result(ptM[0], ptM[1], ptM[2]);

  return result;
}

Matrix &Matrix::operator=(Matrix const &mat)
{
  for (int row = 0; row < 4; row++)
  {
    for (int col = 0; col < 4; col++)
    {
      this->matrix[row][col] = mat.matrix[row][col];
    }
  }
  return *this;
}

std::ostream &operator<<(std::ostream &_stream, Matrix const &mat)
{
  _stream << "[";
  for (int row = 0; row < 4; row++)
  {
    for (int col = 0; col < 4; col++)
    {
      _stream << mat.matrix[row][col] << ",";
    }
    _stream << std::endl;
  }
  _stream << "]" << std::endl;

  return _stream;
}
