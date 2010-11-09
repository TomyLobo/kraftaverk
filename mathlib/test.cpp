#include "quaternion.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3.h"
#include "matrix4.h"

#include <iostream>

using namespace std;

int main() {
	cout << "--- Testing quaternions ---" << endl;
	cout << "These should return -1:" << endl;
	cout << quat::i*quat::i << endl;
	cout << quat::j*quat::j << endl;
	cout << quat::k*quat::k << endl;
	cout << quat::i*quat::j*quat::k << endl;
	cout << endl;
	cout << "These should return k, i, j:" << endl;
	cout << quat::i*quat::j << endl;
	cout << quat::j*quat::k << endl;
	cout << quat::k*quat::i << endl;
	cout << endl;
	cout << "These should return -k, -i, -j:" << endl;
	cout << quat::j*quat::i << endl;
	cout << quat::k*quat::j << endl;
	cout << quat::i*quat::k << endl;
	cout << endl;
	cout << 1 - 2*quat::i + 3*quat::j + 4*quat::k << endl;
	cout << quat(1,2,3,4)/2 << endl;
	cout << endl;
	cout << "This should return (about) i:" << endl;
	cout << quat::rotation_quaternion(180,vec3(1,0,0)) << endl;

	// this make sure all the templated friend functions and operators are instantiated
	quat q;
	double s = 0.0;
	exp(q);
	log(q);
	pow(q,q);
	pow(q,s);
	slerp(s,q,q);
	q*s; s*q;
	q/s; s/q;
	q+s; s+q;
	q-s; s-q;

	cout << "--- Testing vectors ---" << endl;
	cout << "This should return vector3(1, 5, 3):" << endl;
	vec3 v3(1, 2, 3);
	v3[1] = 5;
	cout << v3 << endl;
	vec4 v4;
	v4.x = 3;
	v4.y = 2;
	v4.z = 5;
	v4.w = 4;
	cout << v4 << endl;

	cout << endl << endl;

	cout << "--- Testing matrices ---" << endl;
	mat4 A(3,9,4,7,2,9,3,4,7,2,9,8,4,7,2,5);
	mat4 B(2,8,5,9,6,2,8,9,3,5,6,2,8,5,7,9);
	mat4 C = A*B;
	A.print() << endl;
	B.print() << endl;
	C.print() << endl;
	cout << endl;
	vec4 Av4 = A*v4;
	cout << Av4 << endl;
	vec4 v4A = v4*A;
	cout << v4A << endl;
	cout << B.det() << endl;
	cout << B*B.inverse() << endl;

	mat3 D(1,2,1,4,5,9,7,8,7);
	cout << D.det() << endl;
	cout << D*D.inverse() << endl;
}
