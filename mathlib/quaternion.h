/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "mathlib.h"
#include "vector3.h"
#include "matrix4.h"
#include "angle.h"

#ifndef MATHLIB_NO_IOSTREAM
#include <iostream>
#endif

#include <cmath>

template<typename T> class quaternion {
	private:
		T real;
		vector3<T> imag;

	public:

		/******************************** constructors ********************************/

		quaternion() : real(0), imag(0, 0, 0) { }

		explicit quaternion(T real) : real(real), imag(0, 0, 0) { }

		quaternion(T real, T i, T j, T k) : real(real), imag(i, j, k) { }

		quaternion(T real, vector3<T> const & imag) : real(real), imag(imag) { }

		explicit quaternion(vector3<T> const & imag) : real(0), imag(imag) { }
		explicit quaternion(angle<T> const & ang) : real(1), imag(0, 0, 0) {
			// TODO: fix axes
                        quaternion qy = rotation_quaternion(deg2rad(ang.y), vec3(0,1,0));
                        quaternion qp = rotation_quaternion(deg2rad(ang.p), vec3(1,0,0));
			quaternion qr = rotation_quaternion(deg2rad(ang.r), vec3(0,0,1));
			*this = qy*(qp*qr);
		}

		static quaternion rotation_quaternion(T radians, vector3<T> const & axis) {
			return quaternion(
				std::cos(radians/2),
				axis.normalized()*std::sin(radians/2)
			);
		}

		static quaternion position_quaternion(vector3<T> const & position) {
			return quaternion(position);
		}

		/********************************** constants *********************************/

		static quaternion const i; // (0,1,0,0)
		static quaternion const j; // (0,0,1,0)
		static quaternion const k; // (0,0,0,1)

		/****************************** utility functions *****************************/

		/*matrix3<T> toMatrix3() const {
			T imaglen2 = imag.length2();
			T x = imag.x, y = imag.y, z = imag.z;

			return matrix3<T>(
				real*real+2*x*x-imaglen2, 2*(x*y - real*z)        , 2*(x*z + real*y),
				2*(x*y + real*z)        , real*real+2*y*y-imaglen2, 2*(y*z - real*x),
				2*(x*z - real*y)        , 2*(y*z + real*x)        , real*real+2*z*z-imaglen2
			);
		}*/

		matrix4<T> toMatrix4() const {
			T imaglen2 = imag.length2();
			T x = imag.x, y = imag.y, z = imag.z;

			return matrix4<T>(
				real*real+2*x*x-imaglen2, 2*(x*y - real*z)        , 2*(x*z + real*y)        , 0,
				2*(x*y + real*z)        , real*real+2*y*y-imaglen2, 2*(y*z - real*x)        , 0,
				2*(x*z - real*y)        , 2*(y*z + real*x)        , real*real+2*z*z-imaglen2, 0,
				0                       , 0                       , 0                       , 1
			);
		}

		vector3<T> toVector3() const {
			assert(real == 0.0);
			return imag;
		}

		// returns ||q||²
		T length2() const {
			return real*real+imag.dot(imag);
		}

		T length() const {
			return sqrt(length2());
		}

		void invert() {
			// conjugate / abs²
			T invlen2 = 1/length2();
			real *= invlen2;
			imag *= -invlen2;
		}

		// returns q^-1
		quaternion inverse() const {
			quaternion ret(*this);
			ret.invert();
			return ret;
		}

		friend quaternion exp(quaternion const & q) {
			T imaglen = q.imag.length();
			return std::exp(q.real)*quaternion(cos(imaglen), q.imag*(sin(imaglen)/imaglen));
		}

		friend quaternion log(quaternion const & q) {
			T len = q.length();
			return quaternion(
				log(len),
				q.imag/q.imag.length() * acos(q.real/len)
			);
		}

		friend quaternion pow(quaternion const & base, quaternion const & exponent) {
			return exp(log(base)*exponent);
		}

		friend quaternion pow(quaternion const & base, T exponent) {
			return exp(log(base)*exponent);
		}

		// returns q1*(q1^-1*q2)^t
		friend quaternion slerp(T t, quaternion q1, quaternion q2) {
			return q1*pow(q1.inverse()*q2, t);
		}

		/********************* operations between two quaternions *********************/

		quaternion & operator=(T scalar) {
			real = scalar;
			imag = vector3<T>();
		}

		quaternion & operator*=(quaternion const & other) {
			#if 0
			T newreal = real*other.real-imag.dot(other.imag);

			imag = other.imag*real+imag*other.real+imag.cross(other.imag);
			real = newreal;
			#else
			vector3<T> newimag(other.imag);
			newimag *= real;
			real = real*other.real-imag.dot(other.imag);
			newimag += imag.cross(other.imag);
			(imag *= other.real) += newimag;
			#endif

			return *this;
		}

		quaternion & operator/=(quaternion const & other) {

			return (*this) *= other.inverse();
		}

		quaternion & operator+=(quaternion const & other) {
			real += other.real;
			imag += other.imag;
			return *this;
		}

		quaternion & operator-=(quaternion const & other) {
			real -= other.real;
			real -= other.real;
			imag -= other.imag;
			return *this;
		}

		quaternion operator*(quaternion const & other) const {
			return quaternion(*this) *= other;
		}

		quaternion operator/(quaternion const & other) const {
			return quaternion(*this) /= other;
		}

		quaternion operator+(quaternion const & other) const {
			return quaternion(*this) += other;
		}

		quaternion operator-(quaternion const & other) const {
			return quaternion(*this) -= other;
		}

		// -quaternion
		quaternion operator-() const {
			return quaternion(-real, -imag);
		}

		/*************************** operations with scalars **************************/

		quaternion & operator*=(T scalar) {
			real *= scalar;
			imag *= scalar;
			return *this;
		}

		quaternion & operator/=(T scalar) {
			real /= scalar;
			imag /= scalar;
			return *this;
		}

		quaternion & operator+=(T scalar) {
			real += scalar;
			return *this;
		}

		quaternion & operator-=(T scalar) {
			real -= scalar;
			return *this;
		}

		// quaternion*scalar
		friend quaternion operator*(quaternion const & q, T scalar) {
			return quaternion(q) *= scalar;
		}

		// scalar*quaternion
		friend quaternion operator*(T scalar, quaternion const & q) {
			return quaternion(q) *= scalar;
		}

		// quaternion/scalar
		friend quaternion operator/(quaternion const & q, T scalar) {
			return quaternion(q) /= scalar;
		}

		// quaternion/scalar
		friend quaternion operator/(T scalar, quaternion const & q) {
			return quaternion(scalar) /= q;
		}

		// quaternion+scalar
		friend quaternion operator+(quaternion const & q, T scalar) {
			return quaternion(q) += scalar;
		}

		// scalar+quaternion
		friend quaternion operator+(T scalar, quaternion const & q) {
			return quaternion(q) += scalar;
		}

		// quaternion-scalar
		friend quaternion operator-(quaternion const & q, T scalar) {
			return quaternion(q) -= scalar;
		}

		// scalar-quaternion
		friend quaternion operator-(T scalar, quaternion const & q) {
			return (-q) += scalar;
		}

#ifndef MATHLIB_NO_IOSTREAM
                /*********************************** output ***********************************/

	private:
		static void output_element(std::ostream & os, T element, bool flag) {
			// i -i 2i -2i
			if (element > 0) {
				// i 2i
				if (flag) {
					// foo + x
					os << " + ";
				}
				if (element != 1.0) {
					// 2i
					os << element;
				}
			}
			else {
				// -i -2i
				if (flag) {
					// foo - x
					os << " - ";
					if (element != -1.0) {
						// foo - 2i
						os << -element;
					}
				}
				else if (element == -1.0) {
					// -i
					os << "-";
				}
				else {
					// -2i
					os << element;
				}
			}
		}

	public:
		friend std::ostream & operator<<(std::ostream & os, quaternion<T> const & q) {
			bool flag = false;

			if (q.real) {
				os << q.real;
				flag = true;
			}

			if (q.imag.x) {
				output_element(os, q.imag.x, flag);
				os << "i";
				flag = true;
			}

			if (q.imag.y) {
				output_element(os, q.imag.y, flag);
				os << "j";
				flag = true;
			}

			if (q.imag.z) {
				output_element(os, q.imag.z, flag);
				os << "k";
				flag = true;
			}

			if (!flag) os << "0";

			return os;
			//return os << "quat(" << q.real << ", " << q.imag.x << ", " << q.imag.y << ", " << q.imag.z << ")";
		}
#endif
};

/****************************** define constants ******************************/

template<typename T> quaternion<T> const quaternion<T>::i(0,1,0,0);
template<typename T> quaternion<T> const quaternion<T>::j(0,0,1,0);
template<typename T> quaternion<T> const quaternion<T>::k(0,0,0,1);

#endif // QUATERNION_H_
