#include "Double.h"

int sign(double x) {
  if (x == 0 || x != x) return x;
  if (x < 0) return -1;
  return 1;
}

double mantissa(double x) {
  if (x == 0 || x != x || x == INF) return x;
  if (x < 0) return mantissa(-x);
  while (x >= 2) x /= 2;
  while (x < 1) x *= 2;
  return x;
}

int exponent(double x) {
  if (x == 0 || x != x || x == INF) return 1;
  if (x < 0) return exponent(-x);
  int n = 0;
  while (x >= 2) { x /= 2; n += 1; }
  while (x < 1) { x *= 2; n -= 1; }
  return n;
}

bool is_integer(double x) {
  if (x > 1LL << 52 || x < -(1LL << 52)) return true;
  return (long long)x == x;
}

bool is_even(double x) {
  if (x > 1LL << 53 || x < -(1LL << 53)) return true;
  if (!is_integer(x)) return false;
  return (long long)x % 2 == 0;
}

bool is_odd(double x) {
  if (x > 1LL << 53 || x < -(1LL << 53)) return false;
  if (!is_integer(x)) return false;
  return (long long)x % 2 == 1;
}

double min(double x, double y) { return x <= y ? x : y; }

double max(double x, double y) { return x >= y ? x : y; }

double abs(double x) { return max(x, -x); }

double trunc(double x) { return abs(x) >= 1 << 52 ? x : (long long)x; }

double floor(double x) {
  double t = trunc(x);
  if (x < 0 && x != t) return t - 1;
  return t;
}

double ceil(double x) {
  double t = trunc(x);
  if (x > 0 && x != t) return t + 1;
  return t;
}

double round(double x) { return trunc(x >= 0 ? x + 0.5 : x - 0.5); }

double dist(double x, double y) { return abs(x - y); }

double mod(double x, double y) {
  if (x < 0) return -mod(-x, y);
  if (y < 0) return mod(x, -y);
  if (x != x || x == INF || y != y || y == 0) return NAN;
  if (x < y) return x;
  double modulus = x;
  double power = y;
  while (power * 2 <= modulus) power *= 2;
  while (power >= y) {
    if (power <= modulus) modulus -= power;
    power /= 2;
  }
  return modulus;
}

double remainder(double x, double y) {
  if (y == 0) return NAN;
  if (x < 0) return -remainder(-x, y);
  if (y < 0) return remainder(x, -y);
  double q = x / y;
  long long n = (long long)(q + 0.5);
  double d = n - q;
  if (d == 0.5 && n % 2 != 0) n--;
  return x - (n * y);
}

double ipow(double x, long long n) {
  if (n < 0) return 1 / ipow(x, -n);
  double power = 1;
  while (n > 0) {
    if (n % 2 == 1) power *= x;
    x *= x;
    n /= 2;
  }
  return power;
}

double exp(double x) {
  if (x != x) return x;
  if (x < 0) return 1 / exp(-x);
  if (x >= INV_LN_2) {
    long long k = x / LN_2;
    return exp(x - k * LN_2_HI - k * LN_2_LO) * ipow(2, k);
  }
  double last;
  double i = 0;
  double term = 1;
  double sum = 1;
  do {
    last = sum;
    i++;
    term *= x / i;
    sum += term;
  } while (sum != last);
  return sum;
}

double expm1(double x) {
  if (x != x) return NAN;
  if (x < -0.5 || x > 0.5) return exp(x) - 1;
  double last;
  double i = 1;
  double term = x;
  double sum = x;
  do {
    last = sum;
    i += 1;
    term *= x / i;
    sum += term;
  } while (sum != last);
  return sum;
}

double pow(double x, double y) {
  if (y == 0) return 1;
  if (y == 1) return x;
  if (y == 0.5) return sqrt(x);
  if (is_integer(y)) return ipow(x, y);
  if (x == 1) return 1;
  if (x == 0 && y >= 0) return 0;
  if (x == 0 && y < 0) return INF;
  if (x < 0 && !is_integer(y)) return NAN;
  return exp(y * ln(x));
}

double sqrt(double x) {
  if (x == 0 || x == 1 || x != x || x == INF) return x;
  if (x < 0) return NAN;
  double last;
  double root = 1;
  double temp = x;
  while (temp > 4) { temp /= 4; root *= 2; }
  while (temp < 0.25) { temp *= 4; root /= 2; }
  do {
    last = root;
    double square = root * root;
    root = root * (square + 3 * x) / (3 * square + x);
  } while (dist(root, last) > root * 1e-15);
  return root;
}

double cbrt(const double x) {
  if (x == 0 || x == 1 || x != x || x == INF) return x;
  if (x < 0) return -cbrt(-x);
  double last;
  double root = 1;
  double temp = x;
  while (temp > 8) { temp /= 8; root *= 2; }
  while (temp < 0.125) { temp *= 8; root /= 2; }
  do {
    last = root;
    double cube = root * root * root;
    root = root * (cube + 2 * x) / (2 * cube + x);
  } while (dist(root, last) > root * 1e-15);
  return root;
}

double hypot(double x, double y) {
  if (x < 0) return hypot(-x, y);
  if (y < 0) return hypot(x, -y);
  if (x < y) return hypot(y, x);
  if (x == 0) return y;
  if (y == 0) return x;
  double r = y / x;
  return x * sqrt(1 + r * r);
}

double ln(double x) {
  if (x < 0) return NAN;
  if (x == 0) return -INF;
  if (x == 1) return 0;
  int exponent = 0;
  while (x > SQRT_2) { x /= 2; exponent++; }
  while (x < INV_SQRT_2) { x *= 2; exponent--; }
  double last;
  double z = (x - 1) / (x + 1);
  double term = z;
  double i = 1;
  double sum = z;
  do {
    last = sum;
    term *= z * z;
    i += 2;
    sum += term / i;
  } while (sum != last);
  return 2 * sum + exponent * LN_2;
}

double ln1p(double x) {
  if (x < -1) return NAN;
  if (x == -1) return -INF;
  if (x < -0.25 || x > 0.25) return ln(1 + x);
  double last;
  double term = x;
  double i = 1;
  double sum = x;
  do {
    last = sum;
    term *= -x;
    i += 1;
    sum += term / i;
  } while (sum != last);
  return sum;
}

double log2(double x) { return ln(x) / LN_2; }

double log10(double x) { return ln(x) / LN_10; }

double sin(double x) {
  if (x < 0) return -sin(-x);
  if (x > 2 * PI) return sin(mod(x, 2 * PI));
  if (x > PI) return -sin(x - PI);
  if (x > PI / 2) return sin(PI - x);
  if (x > PI / 4) return cos(PI / 2 - x);
  double last;
  double i = 1;
  double term = x;
  double sum = term;
  do {
    last = sum;
    i += 2;
    term *= -x * x / ((i - 1) * i);
    sum += term;
  } while (sum != last);
  return sum;
}

double cos(double x) {
  if (x < 0) return cos(-x);
  if (x > 2 * PI) return cos(mod(x, 2 * PI));
  if (x > PI) return -cos(x - PI);
  if (x > PI / 2) return -cos(PI - x);
  if (x > PI / 4) return sin(PI / 2 - x);
  double last;
  double i = 0;
  double term = 1;
  double sum = term;
  do {
    last = sum;
    i += 2;
    term *= -x * x / ((i - 1) * i);
    sum += term;
  } while (sum != last);
  return sum;
}

double tan(const double x) { return sin(x) / cos(x); }

double asin(const double x) {
  if (x < 0) return -asin(-x);
  if (x > 1) return NAN;
  if (x > INV_SQRT_2) return PI / 2 - 2 * asin(sqrt((1 - x) / 2));
  double last;
  double n = 1;
  double term = x;
  double sum = term;
  do {
    last = sum;
    n += 2;
    term *= x * x * (n - 2) * (n - 2) / (n - 1) / n;
    sum += term;
  } while (sum != last);
  return sum;
}

double acos(const double x) { return PI / 2 - asin(x); }

double atan(const double x) {
  if (x < 0) return -atan(-x);
  if (x > 1) return PI / 2 - atan(1 / x);
  if (x > SQRT_2 - 1) return PI / 4 + atan((x - 1) / (x + 1));
  double last;
  double n = 1;
  double term = x;
  double sum = term;
  do {
    last = sum;
    n += 2;
    term *= -x * x;
    sum += term / n;
  } while (sum != last);
  return sum;
}

double atan2(double y, double x) {
  if (x > 0) {
    if (y >= -x && y <= x) return atan(y / x);
    else if (y > x) return PI / 2 - atan(x / y);
    else return -PI / 2 - atan(x / y);
  } else if (x < 0) {
    if (y >= x && y <= -x) return (y >= 0 ? PI : -PI) + atan(y / x);
    else if (y > -x) return PI / 2 - atan(x / y);
    else return -PI / 2 - atan(x / y);
  } else {
    if (y > 0) return PI / 2;
    if (y < 0) return -PI / 2;
    return NAN;
  }
}

double sinh(const double x) {
  if (x < 0) return -sinh(-x);
  if (x == 0) return 0;
  if (x >= 1) {
    const double e = exp(x);
    return (e - 1 / e) / 2;
  }
  double last;
  double i = 1;
  double term = x;
  double sum = term;
  do {
    last = sum;
    i += 2;
    term *= x * x / ((i - 1) * i);
    sum += term;
  } while (sum != last);
  return sum;
}

double cosh(const double x) {
  if (x < 0) return cosh(-x);
  if (x == 0) return 1;
  if (x >= 1) {
    const double e = exp(x);
    return (e + 1 / e) / 2;
  }
  double last;
  double i = 0;
  double term = 1;
  double sum = term;
  do {
    last = sum;
    i += 2;
    term *= x * x / ((i - 1) * i);
    sum += term;
  } while (sum != last);
  return sum;
}

double tanh(const double x) {
  if (x < 0) return -tanh(-x);
  if (x > 20) return 1;
  const double e = expm1(2 * x);
  return e / (e + 2);
}

double asinh(const double x) {
  if (x < 0) return -asinh(-x);
  if (x > 1e8) return ln(x) + LN_2;
  return ln(x + sqrt(x * x + 1));
}

double acosh(const double x) {
  if (x < 1) return NAN;
  if (x == 1) return 0;
  if (x > 1e8) return ln(x) + LN_2;
  return ln(x + sqrt(x * x - 1));
}

double atanh(const double x) {
  if (x < -1 || x > 1) return NAN;
  if (x == -1) return -INF;
  if (x == 1) return INF;
  return ln1p(2 * x / (1 - x)) / 2;
}

double tgamma(double x) {
  if (x < 10) return tgamma(x + 1) / x;
  return exp((x - 0.5) * ln(x) - x + ln(2 * PI) / 2 + 1 / (12 * x) - 1 / (360 * x * x * x));
}

double lgamma(double x) {
  if (x <= 0) return NAN;
  return (x - 0.5) * ln(x) - x + LN_2PI / 2;
}

double erf(double x) {
  if (x < 0) return -erf(-x);
  if (x > 4) return 1;
  double last;
  double i = 0;
  double term = x;
  double sum = x;
  do {
    last = sum;
    i++;
    term *= -x * x / i;
    sum += term / (2 * i + 1);
  } while (sum != last);
  return 2 * sum / SQRT_PI;
}

double erfc(double x) { return 1 - erf(x); }
