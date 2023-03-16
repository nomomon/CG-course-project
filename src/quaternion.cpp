#include "quaternion.h"

#include <cmath> // fmin
#include <exception>
#include <iostream>

using namespace std;

// --- Constructors ------------------------------------------------------------

Quaternion::Quaternion(double RE, double I, double J, double K) : re(RE), i(I), j(J), k(K) {}

// --- Operators ---------------------------------------------------------------
Quaternion Quaternion::operator+(Quaternion const &t) const
{
    return Quaternion(re + t.re, i + t.i, j + t.j, k + t.k);
}

Quaternion Quaternion::operator+(double f) const
{
    return Quaternion(re + f, i, j, k);
}

Quaternion Quaternion::operator-() const
{
    return Quaternion(-re, -i, -j, -k);
}

Quaternion Quaternion::operator-(Quaternion const &t) const
{
    return Quaternion(re - t.re, i - t.i, j - t.j, k - t.k);
}

Quaternion Quaternion::operator-(double f) const
{
    return Quaternion(re - f, i, j, k);
}

Quaternion Quaternion::operator*(Quaternion const &t) const
{
    return Quaternion(re * t.re - i * t.i - j * t.j - k * t.k,
                      re * t.i + i * t.re + j * t.k - k * t.j,
                      re * t.j - i * t.k + j * t.re + k * t.i,
                      re * t.k + i * t.j - j * t.i + k * t.re);
}

Quaternion Quaternion::operator*(double f) const
{
    return Quaternion(re * f, i * f, j * f, k * f);
}

Quaternion Quaternion::operator/(double f) const
{
    double invf = 1.0 / f;
    return Quaternion(re * invf, i * invf, j * invf, k * invf);
}

// --- Compound operators ------------------------------------------------------

Quaternion &Quaternion::operator+=(Quaternion const &t)
{
    re += t.re;
    i += t.i;
    j += t.j;
    k += t.k;
    return *this;
}

Quaternion &Quaternion::operator-=(Quaternion const &t)
{
    re -= t.re;
    i -= t.i;
    j -= t.j;
    k -= t.k;
    return *this;
}

Quaternion &Quaternion::operator*=(Quaternion const &t)
{
    double re0 = re * t.re - i * t.i - j * t.j - k * t.k;
    double i0 = re * t.i + i * t.re + j * t.k - k * t.j;
    double j0 = re * t.j - i * t.k + j * t.re + k * t.i;
    double k0 = re * t.k + i * t.j - j * t.i + k * t.re;
    re = re0;
    i = i0;
    j = j0;
    k = k0;
    return *this;
}

Quaternion &Quaternion::operator*=(double f)
{
    re *= f;
    i *= f;
    j *= f;
    k *= f;
    return *this;
}

Quaternion &Quaternion::operator/=(double f)
{
    double invf = 1.0 / f;
    re *= invf;
    i *= invf;
    j *= invf;
    k *= invf;
    return *this;
}

// --- Free Operators ----------------------------------------------------------

Quaternion operator+(double f, Quaternion const &t)
{
    return Quaternion(f + t.re, t.i, t.j, t.k);
}

Quaternion operator-(double f, Quaternion const &t)
{
    return Quaternion(f - t.re, -t.i, -t.j, -t.k);
}

Quaternion operator*(double f, Quaternion const &t)
{
    return Quaternion(f * t.re, f * t.i, f * t.j, f * t.k);
}

// --- IO Operators ------------------------------------------------------------

istream &operator>>(istream &is, Quaternion &t)
{
    // format: [re, i, j, k]
    char c;
    is >> c >> t.re >> c >> t.i >> c >> t.j >> c >> t.k >> c;
    return is;
}

ostream &operator<<(ostream &os, Quaternion const &t)
{
    os << "[" << t.re << ", " << t.i << ", " << t.j << ", " << t.k << "]";
    return os;
}
