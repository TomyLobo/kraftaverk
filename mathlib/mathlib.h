#ifndef MATHLIB_H_
#define MATHLIB_H_

#include <cmath>
#ifndef MATHLIB_NO_EXCEPTION
#include <stdexcept>
#endif
//double pi = 3.14159;
//double deg2rad = pi/180;
//double rad2deg = 180/pi;

// ein paar Winkelfunktionen

template<typename T> T deg2rad(T degrees) {
	static T pi = 4*std::atan(1);
	return degrees/180*pi;
}

template<typename T> T rad2deg(T degrees) {
	static T pi = 4*std::atan(1);
	return degrees/pi*180;
}

template<typename T> T asin(T x) {
	return 2*std::atan( x / (1+std::sqrt(1-x*x)) );
}

template<typename T> T acos(T x) {
	return 2*std::atan( std::sqrt(1-x*x) / (1+x) );
}

#ifndef MATHLIB_NO_EXCEPTION
struct mathlib_exception : public std::logic_error { explicit mathlib_exception(std::string const & s) : std::logic_error(s) {} };
struct singular_matrix_exception : public mathlib_exception { explicit singular_matrix_exception() : mathlib_exception("Cannot invert singular matrix!") {} };
#endif

#endif // MATHLIB_H_
