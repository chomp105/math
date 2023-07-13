#include <vector>
#include <iostream>
#include <cmath>

class Point {
public:
  double x;
  double y;
  Point(double x, double y) : x{x}, y{y} {}
};

class Slope {
public:
  double m;
  double b;
  Slope() = default;
  Slope(double m, double b) : m{m}, b{b} {}
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
  double xMean = 0, yMean = 0; // the means of all x and y values
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

  double numerator = 0;  // the numerator of the r coefficient
  double denomSum1 = 0;  // the sum of all (x - x̄)²
  double denomSum2 = 0;  // the sum of all (y - ȳ)²

  for (auto& point : points) {
    double xSubMean = point.x - xMean;
    double ySubMean = point.y - yMean;

    numerator += xSubMean * ySubMean;
    denomSum1 += xSubMean * xSubMean;
    denomSum2 += ySubMean * ySubMean;
  }

  double r = numerator / sqrt(denomSum1 * denomSum2); // the correlation coefficient or "r"

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

  double nSubOne = pointsSize - 1;
  double yStdDev = sqrt(denomSum2 / nSubOne);
  double xStdDev = sqrt(denomSum1 / nSubOne);
  
  double b = r * (yStdDev / xStdDev);
  double a = yMean - b * xMean;

  return Slope {b, a};
}