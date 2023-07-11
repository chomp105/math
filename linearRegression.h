#include <vector>
#include <iostream>
#include <cmath>

class Point {
public:
  float x;
  float y;
  Point(float x, float y) : x{x}, y{y} {}
};

class Slope {
public:
  float m;
  float b;
  Slope() = default;
  Slope(float m, float b) : m{m}, b{b} {}
};

Slope calculateLinearRegression(std::vector<Point> points) {
  
  if (points.size() < 2)
    return Slope {};
  
  /// Calculate Pearon's Correlation Coefficient (r)
  //
  //        Σ((x-x̄)(y-ȳ))
  // r = ───────────────────
  //     √(Σ(x-ȳ)² Σ(y-ȳ)²))
  //
  
  int pointsSize = (int)points.size();
  float xMean = 0.f, yMean = 0.f; // the means of all x and y values
  bool notZero = false, notInfinite = false; // slope edge cases

  for (auto& point : points) {
    xMean += point.x;
    yMean += point.y;

    notZero += point.y != points[0].y;
    notInfinite += point.x != points[0].x;
  }

  if (!notZero) return Slope {0, points[0].y};
  if (!notInfinite) return Slope {INFINITY, 0};

  xMean /= pointsSize;
  yMean /= pointsSize;

  float numerator = 0.f;  // the numerator of the r coefficient
  float denomSum1 = 0.f;  // the sum of all (x - x̄)²
  float denomSum2 = 0.f;  // the sum of all (y - ȳ)²

  for (auto& point : points) {
    float xSubMean = point.x - xMean;
    float ySubMean = point.y - yMean;

    numerator += xSubMean * ySubMean;
    denomSum1 += xSubMean * xSubMean;
    denomSum2 += ySubMean * ySubMean;
  }

  float r = numerator / sqrt(denomSum1 * denomSum2); // the correlation coefficient or "r"

  /// Calculate Linear Regression
  //
  // y = a + bx
  //
  //       sᵧ
  // b = r ── 
  //       sₓ
  //
  // a = ȳ - bx̄
  //
  //     √(Σ(y-ȳ)²)      √(Σ(x-x̄)²)
  // sᵧ = ─────────  sₓ = ─────────
  //         n-1            n-1
  //

  float nSubOne = pointsSize - 1;
  float yStdDev = sqrt(denomSum2 / nSubOne);
  float xStdDev = sqrt(denomSum1 / nSubOne);
  
  float b = r * (yStdDev / xStdDev);
  float a = yMean - b * xMean;

  return Slope {b, a};
}