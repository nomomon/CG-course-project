#ifndef QUATERNION_H_
#define QUATERNION_H_

#include <iosfwd>

// Color, Point and Vector are all Quaternions (name them so)
class Quaternion;
typedef Quaternion Quaternion;

class Quaternion
{
public:
    // --- data members ------------------------------------------------------------

    // union to acces the same elements by
    // re, i, j, k
    union
    {
        struct
        {
            double re;
            double i;
            double j;
            double k;
        };
    };

    // --- Constructors ------------------------------------------------------------

    explicit Quaternion(double RE = 0, double I = 0, double J = 0, double K = 0);

    // --- Operators ---------------------------------------------------------------

    Quaternion operator+(Quaternion const &t) const; // add two quaternions
    Quaternion operator+(double f) const;            // add a value to each member
                                                     // of a quaternion
    Quaternion operator-() const;                    // negate
    Quaternion operator-(Quaternion const &t) const; // subtract two quaternions
    Quaternion operator-(double f) const;            // subtract a value from each
                                                     // member

    Quaternion operator*(Quaternion const &t) const; // memberwise multiplication
    Quaternion operator*(double f) const;            // multiply each member with a
                                                     // value
    Quaternion operator/(double f) const;            // divide each member by a value

    // --- Compound operators ------------------------------------------------------

    Quaternion &operator+=(Quaternion const &t);
    Quaternion &operator+=(double f);

    Quaternion &operator-=(Quaternion const &t);
    Quaternion &operator-=(double f);

    Quaternion &operator*=(double f);
    Quaternion &operator/=(double f);

    Quaternion &operator*=(Quaternion const &t);
};

// --- Free Operators ----------------------------------------------------------

Quaternion operator+(double f, Quaternion const &t);
Quaternion operator-(double f, Quaternion const &t);
Quaternion operator*(double f, Quaternion const &t);

// --- IO Operators ------------------------------------------------------------

std::istream &operator>>(std::istream &is, Quaternion &t);
std::ostream &operator<<(std::ostream &os, Quaternion const &t);

#endif
