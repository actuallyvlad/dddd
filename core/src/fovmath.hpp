#ifndef FOVMATH_H
#define FOVMATH_H

namespace fovMath {
    extern const double sinTable[];;
    extern const double cosTable[];
    
    double getSin(unsigned int x);
    double getCos(unsigned int x);
}

#endif
