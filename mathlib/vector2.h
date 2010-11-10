#ifndef VECTOR2_H_
#define VECTOR2_H_

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

template<typename T> class vector2 {
	public:
		T x, y;

		vector2() : x(0), y(0) { }
		vector2(T x, T y) : x(x), y(y) { }

		// possibly UB, but might be useful
		T & operator[](size_t index) {
			assert(index < 2);
			return reinterpret_cast<T *>(this)[index];
		}

		T operator[](size_t index) const {
			assert(index < 2);
			return reinterpret_cast<T const *>(this)[index];
		}

		// vector2 @ vector2
		vector2 & operator+=(vector2 const & other) {
			x += other.x;
			y += other.y;
			return *this;
		}
		vector2 & operator-=(vector2 const & other) {
			x -= other.x;
			y -= other.y;
			return *this;
		}
		friend vector2 operator+(vector2 const & lhs, vector2 const & rhs) {
			vector2 ret(lhs);
			ret += rhs;
			return ret;
		}
		friend vector2 operator-(vector2 const & lhs, vector2 const & rhs) {
			vector2 ret(lhs);
			ret -= rhs;
			return ret;
		}

		// vector2*scalar
		vector2 & operator*=(T scalar) {
			x *= scalar;
			y *= scalar;
			return *this;
		}
		friend vector2 operator*(vector2 const & v, T scalar) {
			vector2 ret(v);
			ret *= scalar;
			return ret;
		}
		friend vector2 operator*(T scalar, vector2 const & v) {
			vector2 ret(v);
			ret *= scalar;
			return ret;
		}

		// vector2/scalar
		vector2 & operator/=(T scalar) {
			x /= scalar;
			y /= scalar;
			return *this;
		}
		vector2 operator/(T scalar) const {
			vector2 ret(*this);
			ret /= scalar;
			return ret;
		}

		vector2 operator-() const {
			return vector2(-x, -y);
		}

		bool operator<(vector2 const & other) const {
			if (y == other.y) return x < other.x;
			return y < other.y;
		}

		// dot/cross product
		T dot(vector2 const & other) const {
			return x * other.x + y * other.y;
		}

		// length, distance and normalization
		T length2() const {
			return x*x + y*y;
		}

		T length() const {
			return std::sqrt(length2());
		}

		T distance2(vector2 const & other) const {
			return (*this-other).length2();
		}

		T distance(vector2 const & other) const {
			return std::sqrt(distance2(other));
		}

		void normalize() {
			T len = length();
			*this /= len;
		}

		vector2 normalized() const {
			vector2 ret(*this);
			ret.normalize();
			return ret;
		}

#ifndef MATHLIB_NO_IOSTREAM
                friend std::ostream & operator<<(std::ostream & os, vector2<T> const & v) {
			return os << "vector2(" << v.x << ", " << v.y << ")";
		}
#endif
};

#endif // VECTOR2_H_
