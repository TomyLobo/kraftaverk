#ifndef MATRIX4_H_
#define MATRIX4_H_

#include "vector4.h"
#include "mathlib.h"

#ifndef MATHLIB_NO_IOSTREAM
#include <iostream>
#include <iomanip>
#endif

#include <cmath>

template<typename T> class matrix4 {
	private:
		T data[16];

	public:

		/******************************** constructors ********************************/

		matrix4() {
			for (size_t n = 0; n < 16; ++n) data[n] = 0;
		}

		matrix4(
			T a00, T a01, T a02, T a03,
			T a10, T a11, T a12, T a13,
			T a20, T a21, T a22, T a23,
			T a30, T a31, T a32, T a33
		) {
			data[ 0] = a00; data[ 1] = a01; data[ 2] = a02; data[ 3] = a03;
			data[ 4] = a10; data[ 5] = a11; data[ 6] = a12; data[ 7] = a13;
			data[ 8] = a20; data[ 9] = a21; data[10] = a22; data[11] = a23;
			data[12] = a30; data[13] = a31; data[14] = a32; data[15] = a33;
		}

		/********************************** accessors *********************************/

		T * operator[](size_t index) {
                        MATHLIB_ASSERT(index < 4);
			return &data[index*4];
		}

		T const * operator[](size_t index) const {
                        MATHLIB_ASSERT(index < 4);
			return &data[index*4];
		}

		T & operator()(size_t row, size_t col) {
                        MATHLIB_ASSERT(row < 4);
                        MATHLIB_ASSERT(col < 4);
			return data[row*4+col];
		}

		T operator()(size_t row, size_t col) const {
                        MATHLIB_ASSERT(row < 4);
                        MATHLIB_ASSERT(col < 4);
			return data[row*4+col];
		}

		/****************************** utility functions *****************************/

		// Bareiss algorithm for determinants, as described on http://cs.nyu.edu/~yap/book/alge/ftpSite/l10.ps.gz
		// All elements of the prinicipal minor (Hauptdiagonale) must be non-zero.
		T det() const {
			static size_t const n = 4;
			matrix4 M(*this);
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

		static void Invert2(double const *mat, double *dst) {
			double tmp[12]; /* temp array for pairs */
			double src[16]; /* array of transpose source matrix */
			double det; /* determinant */

			/* transpose matrix */
			for (int i = 0; i < 4; i++) {
				src[i] = mat[i*4];
				src[i + 4] = mat[i*4 + 1];
				src[i + 8] = mat[i*4 + 2];
				src[i + 12] = mat[i*4 + 3];
			}

			/* calculate pairs for first 8 elements (cofactors) */
			tmp[0] = src[10] * src[15];
			tmp[1] = src[11] * src[14];
			tmp[2] = src[9] * src[15];
			tmp[3] = src[11] * src[13];
			tmp[4] = src[9] * src[14];
			tmp[5] = src[10] * src[13];
			tmp[6] = src[8] * src[15];
			tmp[7] = src[11] * src[12];
			tmp[8] = src[8] * src[14];
			tmp[9] = src[10] * src[12];
			tmp[10] = src[8] * src[13];
			tmp[11] = src[9] * src[12];

			/* calculate first 8 elements (cofactors) */
			dst[0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
			dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
			dst[1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
			dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
			dst[2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
			dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
			dst[3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
			dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
			dst[4] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
			dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
			dst[5] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
			dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
			dst[6] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
			dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
			dst[7] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
			dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

			/* calculate pairs for second 8 elements (cofactors) */
			tmp[0] = src[2]*src[7];
			tmp[1] = src[3]*src[6];
			tmp[2] = src[1]*src[7];
			tmp[3] = src[3]*src[5];
			tmp[4] = src[1]*src[6];
			tmp[5] = src[2]*src[5];
			tmp[6] = src[0]*src[7];
			tmp[7] = src[3]*src[4];
			tmp[8] = src[0]*src[6];
			tmp[9] = src[2]*src[4];
			tmp[10] = src[0]*src[5];
			tmp[11] = src[1]*src[4];

			/* calculate second 8 elements (cofactors) */
			dst[8] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
			dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
			dst[9] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
			dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
			dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
			dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
			dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
			dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
			dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
			dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
			dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
			dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
			dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
			dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
			dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
			dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];

			/* calculate determinant */
			det=src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];

#ifndef MATHLIB_NO_EXCEPTION
			if (det == 0.0) throw singular_matrix_exception();
#endif

			/* calculate matrix inverse */
			det = 1/det;
			for (int j = 0; j < 16; j++)
				dst[j] *= det;
		}

		matrix4 inverse() const {
			matrix4 ret;
			Invert2((*this)[0], ret[0]);
			return ret;
		}

		matrix4 transpose() {
			matrix4 ret;
			for (size_t i = 0; i < 4; ++i) {
				for (size_t j = 0; j < 4; ++j) {
					ret[i][j] = (*this)[j][i];
				}
			}
			return ret;
		}

		/*********************** operations between two matrices **********************/
		matrix4 & operator*=(matrix4 const & other) {
			matrix4 A(*this);
			for (size_t i = 0; i < 4; ++i) {
				for (size_t j = 0; j < 4; ++j) {
					(*this)[i][j] = 0;
					for (size_t r = 0; r < 4; ++r) {
						(*this)[i][j] += A[i][r] * other[r][j];
					}
				}
			}
			return *this;
		}

		matrix4 operator*(matrix4 const & other) const {
			return matrix4(*this) *= other;
		}

		matrix4 & operator+=(matrix4 const & other) {
			for (size_t n = 0; n < 16; ++n) data[n] += other.data[n];
			return *this;
		}

		matrix4 operator+(matrix4 const & other) const {
			return matrix4(*this) += other;
		}

		matrix4 & operator-=(matrix4 const & other) {
			for (size_t n = 0; n < 16; ++n) data[n] -= other.data[n];
			return *this;
		}

		matrix4 operator-(matrix4 const & other) const {
			return matrix4(*this) -= other;
		}

		/*************************** operations with vectors **************************/

		// transpose(v)*m (probably not very useful)
		friend vector4<T> operator*(vector4<T> v, matrix4 m) {
			vector4<T> ret;
			for (size_t j = 0; j < 4; ++j) {
				ret[j] = 0;
				for (size_t r = 0; r < 4; ++r) {
					ret[j] += v[r] * m[r][j];
				}
			}
			return ret;
		}

		friend vector4<T> operator*(matrix4 m, vector4<T> v) {
			vector4<T> ret;
			for (size_t i = 0; i < 4; ++i) {
				ret[i] = 0;
				for (size_t r = 0; r < 4; ++r) {
					ret[i] += m[i][r] * v[r];
				}
			}
			return ret;
		}

		/*************************** operations with scalars **************************/

		matrix4 & operator*=(T scalar) {
			for (size_t n = 0; n < 16; ++n) data[n] *= scalar;
			return *this;
		}

		matrix4 operator*(T scalar) const {
			return matrix4(*this) *= scalar;
		}

		matrix4 & operator/=(T scalar) {
			for (size_t n = 0; n < 16; ++n) data[n] /= scalar;
			return *this;
		}

		matrix4 operator/(T scalar) const {
			return matrix4(*this) /= scalar;
		}

		matrix4 operator-() const {
			matrix4 ret;
			for (size_t n = 0; n < 16; ++n) ret.data[n] = -data[n];
			return ret;
		}

#ifndef MATHLIB_NO_IOSTREAM
                /*********************************** output ***********************************/

		friend std::ostream & operator<<(std::ostream & os, matrix4<T> const & m) {
			os << "matrix4(" << m.data[0];
			for (size_t n = 1; n < 16; ++n) {
				os << ", " << m.data[n];
			}
			return os << ")";
		}

		std::ostream & print(std::ostream & os = std::cout) const {
			T const * ptr = data;
			for (size_t i = 0; i < 4; ++i) {
				for (size_t j = 0; j < 4; ++j) {
					os << std::setw(5) << " " << std::floor(*ptr+0.5);
					++ptr;
				}
				os << std::endl;
			}
			return os;
		}
#endif
};

#endif // MATRIX4_H_
