#include <stdbool.h>

const double INF = 1.0 / 0.0;
const double NAN = 0.0 / 0.0;
const double E = 2.71828182845904523536;
const double LN_2 = 0.69314718055994530941;
const double LN_2_HI = 6.93147180369123816490e-01;
const double LN_2_LO = 1.90821492927058770002e-10;
const double INV_LN_2 = 1.44269504088896340736;
const double LN_10 = 2.3025850929940456840;
const double LN_2PI = 1.83787706640934548356;
const double SQRT_2 = 1.41421356237309504880;
const double INV_SQRT_2 = 0.70710678118654752440;
const double PI = 3.14159265358979323846;
const double SQRT_PI = 1.77245385090551602730;

int sign(double x);
double mantissa(double x);
int exponent(double x);
bool is_integer(double x);
bool is_even(double x);
bool is_odd(double x);
double min(double x, double y);
double max(double x, double y);
double abs(double x);
double trunc(double x);
double floor(double x);
double ceil(double x);
double round(double x);
double dist(double x, double y);
double mod(double x, double y);
double remainder(double x, double y);

double exp(double x);
double expm1(double x);
double pow(double x, double y);
double ipow(double x, long long n);
double sqrt(double x);
double cbrt(double x);
double hypot(double x, double y);
double ln(double x);
double ln1p(double x);
double log2(double x);
double log10(double x);

double sin(double x);
double cos(double x);
double tan(double x);
double asin(double x);
double acos(double x);
double atan(double x);
double atan2(double y, double x);
double sinh(double x);
double cosh(double x);
double tanh(double x);
double asinh(double x);
double acosh(double x);
double atanh(double x);

double tgamma(double x);
double lgamma(double x);
double erf(double x);
double erfc(double x);
