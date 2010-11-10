#ifndef MATRIX3_H_
#define MATRIX3_H_

#include "vector3.h"
#include "mathlib.h"

#ifndef MATHLIB_NO_IOSTREAM
#include <iostream>
#include <iomanip>
#endif

#include <cmath>

template<typename T> class matrix3 {
	private:
		T data[9];

	public:

		/******************************** constructors ********************************/

		matrix3() {
			for (size_t n = 0; n < 9; ++n) data[n] = 0;
		}

		matrix3(vector3<T> const & a0, vector3<T> const & a1, vector3<T> const & a2) {
			data[0] = a0.x; data[1] = a1.x; data[2] = a2.x;
			data[3] = a0.y; data[4] = a1.y; data[5] = a2.y;
			data[6] = a0.z; data[7] = a1.z; data[8] = a2.z;
		}

		matrix3(
			T a00, T a01, T a02,
			T a10, T a11, T a12,
			T a20, T a21, T a22
		) {
			data[0] = a00; data[1] = a01; data[2] = a02;
			data[3] = a10; data[4] = a11; data[5] = a12;
			data[6] = a20; data[7] = a21; data[8] = a22;
		}

		/********************************** accessors *********************************/

		T * operator[](size_t index) {
                        MATHLIB_ASSERT(index < 3);
			return &data[index*3];
		}

		T const * operator[](size_t index) const {
                        MATHLIB_ASSERT(index < 3);
			return &data[index*3];
		}

		T & operator()(size_t row, size_t col) {
                        MATHLIB_ASSERT(row < 3);
                        MATHLIB_ASSERT(col < 3);
			return data[row*3+col];
		}

		T operator()(size_t row, size_t col) const {
                        MATHLIB_ASSERT(row < 3);
                        MATHLIB_ASSERT(col < 3);
			return data[row*3+col];
		}

		/****************************** utility functions *****************************/

		#if 0
		// Bareiss algorithm for determinants, as described on http://cs.nyu.edu/~yap/book/alge/ftpSite/l10.ps.gz
		// All elements of the prinicipal minor (Hauptdiagonale) must be non-zero.
		T det() const {
			static size_t const n = 3;
			matrix3 M(*this);
			for (size_t k = 0; k < n-1; ++k) {
                                MATHLIB_ASSERT(k == 0 || M[k-1][k-1] != 0.0);
				for (size_t i = k+1; i < n; ++i) {
					for (size_t j = k+1; j < n; ++j) {
						M[i][j] = M[i][j] * M[k][k] - M[i][k] * M[k][j];

						// M[-1][-1] = 1:
						if (k != 0) M[i][j] /= M[k-1][k-1];
					}
				}
			}

			return M[n-1][n-1];
		}
		#else
		T det() const {
			matrix3 const & m = *this;
			T ret = m(0,0)* m(1,1)* m(2,2)+m(0,1)* m(1,2)* m(2,0)+m(0,2)* m(1,0)* m(2,1)-m(2,0)* m(1,1)* m(0,2)-m(2,1)* m(1,2)* m(0,0)-m(2,2)* m(1,0)* m(0,1);
			return ret;
		}
		#endif

		matrix3 inverse() const {
			matrix3 const & A = *this;
			double determinant = det();

#ifndef MATHLIB_NO_EXCEPTION
			if (determinant == 0.0) throw singular_matrix_exception();
#endif

			double invdet = 1/determinant;
			matrix3 ret(
				+(A(1,1)*A(2,2)-A(2,1)*A(1,2))*invdet,
				-(A(0,1)*A(2,2)-A(0,2)*A(2,1))*invdet,
				+(A(0,1)*A(1,2)-A(0,2)*A(1,1))*invdet,
				-(A(1,0)*A(2,2)-A(1,2)*A(2,0))*invdet,
				+(A(0,0)*A(2,2)-A(0,2)*A(2,0))*invdet,
				-(A(0,0)*A(1,2)-A(1,0)*A(0,2))*invdet,
				+(A(1,0)*A(2,1)-A(2,0)*A(1,1))*invdet,
				-(A(0,0)*A(2,1)-A(2,0)*A(0,1))*invdet,
				+(A(0,0)*A(1,1)-A(1,0)*A(0,1))*invdet
			);
			return ret;
		}

		matrix3 transpose() {
			matrix3 ret;
			for (size_t i = 0; i < 3; ++i) {
				for (size_t j = 0; j < 3; ++j) {
					ret[i][j] = (*this)[j][i];
				}
			}
			return ret;
		}

		/*********************** operations between two matrices **********************/
		matrix3 & operator*=(matrix3 const & other) {
			matrix3 A(*this);
			for (size_t i = 0; i < 3; ++i) {
				for (size_t j = 0; j < 3; ++j) {
					(*this)[i][j] = 0;
					for (size_t r = 0; r < 3; ++r) {
						(*this)[i][j] += A[i][r] * other[r][j];
					}
				}
			}
			return *this;
		}

		matrix3 operator*(matrix3 const & other) const {
			return matrix3(*this) *= other;
		}

		matrix3 & operator+=(matrix3 const & other) {
			for (size_t n = 0; n < 9; ++n) data[n] += other.data[n];
			return *this;
		}

		matrix3 operator+(matrix3 const & other) const {
			return matrix3(*this) += other;
		}

		matrix3 & operator-=(matrix3 const & other) {
			for (size_t n = 0; n < 9; ++n) data[n] -= other.data[n];
			return *this;
		}

		matrix3 operator-(matrix3 const & other) const {
			return matrix3(*this) -= other;
		}

		/*************************** operations with vectors **************************/

		// transpose(v)*m (probably not very useful)
		friend vector3<T> operator*(vector3<T> v, matrix3 m) {
			vector3<T> ret;
			for (size_t j = 0; j < 3; ++j) {
				ret[j] = 0;
				for (size_t r = 0; r < 3; ++r) {
					ret[j] += v[r] * m[r][j];
				}
			}
			return ret;
		}

		friend vector3<T> operator*(matrix3 m, vector3<T> v) {
			vector3<T> ret;
			for (size_t i = 0; i < 3; ++i) {
				ret[i] = 0;
				for (size_t r = 0; r < 3; ++r) {
					ret[i] += m[i][r] * v[r];
				}
			}
			return ret;
		}

		/*************************** operations with scalars **************************/

		matrix3 & operator*=(T scalar) {
			for (size_t n = 0; n < 9; ++n) data[n] *= scalar;
			return *this;
		}

		matrix3 operator*(T scalar) const {
			return matrix3(*this) *= scalar;
		}

		matrix3 & operator/=(T scalar) {
			for (size_t n = 0; n < 9; ++n) data[n] /= scalar;
			return *this;
		}

		matrix3 operator/(T scalar) const {
			return matrix3(*this) /= scalar;
		}

		matrix3 operator-() const {
			matrix3 ret;
			for (size_t n = 0; n < 9; ++n) ret.data[n] = -data[n];
			return ret;
		}

#ifndef MATHLIB_NO_IOSTREAM
                /*********************************** output ***********************************/

                friend std::ostream & operator<<(std::ostream & os, matrix3<T> const & m) {
			os << "matrix3(" << m.data[0];
			for (size_t n = 1; n < 9; ++n) {
				os << ", " << m.data[n];
			}
			return os << ")";
		}

		std::ostream & print(std::ostream & os = std::cout) const {
			T const * ptr = data;
			for (size_t i = 0; i < 3; ++i) {
				for (size_t j = 0; j < 3; ++j) {
					os << std::setw(5) << " " << std::floor(*ptr+0.5);
					++ptr;
				}
				os << std::endl;
			}
			return os;
                }
#endif
};

#endif // MATRIX3_H_
