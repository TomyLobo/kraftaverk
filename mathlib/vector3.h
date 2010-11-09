#ifndef VECTOR3_H_
#define VECTOR3_H_

#ifndef MATHLIB_NO_IOSTREAM
#include <iostream>
#endif

#ifdef QT_CORE_LIB
#include <QtGlobal>
#define MATHLIB_ASSERT Q_ASSERT
#else
#include <cassert>
#define MATHLIB_ASSERT assert
#endif

#include <cmath>

template<typename T> class vector4;

template<typename T> class vector3 {
	public:
		T x, y, z;

		vector3() : x(0), y(0), z(0) { }
		vector3(T x, T y, T z) : x(x), y(y), z(z) { }
		explicit vector3(vector4<T> const & v) : x(v.x/v.w), y(v.y/v.w), z(v.z/v.w) { }

		// possibly UB, but might be useful
		T & operator[](size_t index) {
                        MATHLIB_ASSERT(index < 3);
			return reinterpret_cast<T *>(this)[index];
		}

		T operator[](size_t index) const {
                        MATHLIB_ASSERT(index < 3);
			return reinterpret_cast<T const *>(this)[index];
		}

		// vector3 @ vector3
		vector3 & operator+=(vector3 const & other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		vector3 & operator-=(vector3 const & other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		vector3 & operator*=(vector3 const & other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}
		friend vector3 operator+(vector3 const & lhs, vector3 const & rhs) {
			vector3 ret(lhs);
			ret += rhs;
			return ret;
		}
		friend vector3 operator-(vector3 const & lhs, vector3 const & rhs) {
			vector3 ret(lhs);
			ret -= rhs;
			return ret;
		}
		friend vector3 operator*(vector3 const & lhs, vector3 const & rhs) {
			vector3 ret(lhs);
			ret *= rhs;
			return ret;
		}

		// vector3*scalar
		vector3 & operator*=(T scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}
		friend vector3 operator*(vector3 const & v, T scalar) {
			vector3 ret(v);
			ret *= scalar;
			return ret;
		}
		friend vector3 operator*(T scalar, vector3 const & v) {
			vector3 ret(v);
			ret *= scalar;
			return ret;
		}

		// vector3/scalar
		vector3 & operator/=(T scalar) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}
		vector3 operator/(T scalar) const {
			vector3 ret(*this);
			ret /= scalar;
			return ret;
		}

		vector3 operator-() const {
			return vector3(-x, -y, -z);
		}

		// dot/cross product
		T dot(vector3 const & other) const {
			return x * other.x + y * other.y + z * other.z;
		}

		vector3 cross(vector3 const & other) const {
			return vector3(
				y*other.z-z*other.y,
				z*other.x-x*other.z,
				x*other.y-y*other.x
			);
		}

		// length, distance and normalization
		T length2() const {
			return x*x + y*y + z*z;
		}

		T length() const {
			return std::sqrt(length2());
		}

		T distance2(vector3 const & other) const {
			return (*this-other).length2();
		}

		T distance(vector3 const & other) const {
			return std::sqrt(distance2(other));
		}

		void normalize() {
			T len = length();
			*this /= len;
		}

		vector3 normalized() const {
			vector3 ret(*this);
			ret.normalize();
			return ret;
		}

#ifndef MATHLIB_NO_IOSTREAM
                friend std::ostream & operator<<(std::ostream & os, vector3<T> const & v) {
			return os << "vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
		}
#endif
                bool operator<(vector3 const & other) const {
                    if (x != other.x)
                        return x < other.x;

                    if (y != other.y)
                        return y < other.y;

                    return z < other.z;
                }
};

typedef vector3<double> vec3;

#endif // VECTOR3_H_
