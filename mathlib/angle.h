#ifndef ANGLE_H_
#define ANGLE_H_

template<typename T> class angle {
	public:
		T p, y, r; // pitch, yaw, roll

		angle() : p(0), y(0), r(0) { }

		angle(T p, T y, T r) : p(p), y(y), r(r) { }
};

typedef angle<double> ang;

#endif // ANGLE_H_
