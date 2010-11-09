#ifndef VECTOR4_H_
#define VECTOR4_H_

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

template<typename T> class vector3;

template<typename T> class vector4 {
	public:
		T x, y, z, w;

		vector4() : x(0), y(0), z(0), w(0) { }
		vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { }
		explicit vector4(vector3<T> const & v) : x(v.x), y(v.y), z(v.z), w(1) { }
		explicit vector4(vector3<T> const & v, T w) : x(v.x), y(v.y), z(v.z), w(w) { }

		// possibly UB, but might be useful
		T & operator[](size_t index) {
                        MATHLIB_ASSERT(index < 4);
			return reinterpret_cast<T *>(this)[index];
		}

		T operator[](size_t index) const {
                        MATHLIB_ASSERT(index < 4);
			return reinterpret_cast<T const *>(this)[index];
		}

		// vector4 @ vector4
		vector4 & operator+=(vector4 const & other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		vector4 & operator-=(vector4 const & other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		vector4 & operator*=(vector4 const & other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}
		vector4 operator+(vector4 const & other) const {
			vector4 ret(*this);
			ret += other;
			return ret;
		}
		vector4 operator-(vector4 const & other) const {
			vector4 ret(*this);
			ret -= other;
			return ret;
		}
		vector4 operator*(vector4 const & other) const {
			vector4 ret(*this);
			ret *= other;
			return ret;
		}

		// vector4*scalar
		vector4 & operator*=(T scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}
		vector4 operator*(T scalar) const {
			vector4 ret(*this);
			ret *= scalar;
			return ret;
		}

		// vector4/scalar
		vector4 & operator/=(T scalar) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;
			return *this;
		}
		vector4 operator/(T scalar) const {
			vector4 ret(*this);
			ret /= scalar;
			return ret;
		}

		vector4 operator-() const {
			return vector4(-x, -y, -z, -w);
		}

		// dot/cross product
		T dot(vector4 const & other) const {
			return x * other.x + y * other.y + z * other.z + w * other.w;
		}

		// length and normalization
		T length2() const {
			return x*x + y*y + z*z + w*w;
		}

		T length() const {
			return sqrt(length2());
		}

		void normalize() {
			T len = length();
			*this /= len;
		}

		vector4 normalized() const {
			vector4 ret(*this);
			ret.normalize();
			return ret;
		}

#ifndef MATHLIB_NO_IOSTREAM
		friend std::ostream & operator<<(std::ostream & os, vector4<T> const & v) {
			return os << "vector4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		}
#endif
};

typedef vector4<double> vec4;

#endif // VECTOR4_H_
