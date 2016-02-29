#include <QCoreApplication>
#include <stdio.h>
#include <iostream>
#include <QTime>
#include <QVector>

#include "math.h"
#include "intrinsics.h"

using namespace std;

float dotProduct(const Point3 &p1, const Point3 &p2) {
	return p1.point[0]*p2.point[0] + p1.point[1]*p2.point[1] + p1.point[2]*p2.point[2];
}

double angle(const Point3 &a, const Point3 &b)
{
	return acos(dotProduct(a, b)*sqrt(dotProduct(b, b))/sqrt(dotProduct(a, a)));
}

bool inTriangle(const Point3 &x, const Point3 &a, const Point3 &b, const Point3 &c)
{
	Point3 ab(b.point[0] - a.point[0], b.point[1] - a.point[1], b.point[2] - a.point[2]);
	Point3 ac(c.point[0] - a.point[0], c.point[1] - a.point[1], c.point[2] - a.point[2]);
	Point3 ax(x.point[0] - a.point[0], x.point[1] - a.point[1], x.point[2] - a.point[2]);

	double sqrt_dp_ab = sqrt(dotProduct(ab, ab));
	double sqrt_dp_ac = sqrt(dotProduct(ac, ac));
	double sqrt_dp_ax = sqrt(dotProduct(ax, ax));

	double ab_ac = acos(dotProduct(ab, ac)*sqrt_dp_ac/sqrt_dp_ab);
	double ab_ax = acos(dotProduct(ab, ax)*sqrt_dp_ax/sqrt_dp_ab);
	double ac_ax = acos(dotProduct(ac, ax)*sqrt_dp_ax/sqrt_dp_ac);

	if (ab_ax >= ab_ac && ac_ax >= ab_ac) {
		return false;
	}

	Point3 ba(a.point[0] - b.point[0], a.point[1] - b.point[1], a.point[2] - b.point[2]);
	Point3 bc(c.point[0] - b.point[0], c.point[1] - b.point[1], c.point[2] - b.point[2]);
	Point3 bx(x.point[0] - b.point[0], x.point[1] - b.point[1], x.point[2] - b.point[2]);

	double sqrt_dp_ba = sqrt(dotProduct(ba, ba));
	double sqrt_dp_bc = sqrt(dotProduct(bc, bc));
	double sqrt_dp_bx = sqrt(dotProduct(bx, bx));

	double ba_bc = acos(dotProduct(ba, bc)*sqrt_dp_bc/sqrt_dp_ba);
	double ba_bx = acos(dotProduct(ba, bx)*sqrt_dp_bx/sqrt_dp_ba);
	double bc_bx = acos(dotProduct(bc, bx)*sqrt_dp_bx/sqrt_dp_bc);

	if (ba_bx >= ba_bc && bc_bx >= ba_bc) {
		return false;
	}

	return true;
}

bool inLine(const Point3 &x, const Point3 &a, const Point3 &b)
{
	Point3 ab(b.point[0] - a.point[0], b.point[1] - a.point[1], b.point[2] - a.point[2]);
	Point3 ax(a.point[0] - x.point[0], a.point[1] - x.point[1], a.point[2] - x.point[2]);
	Point3 bx(b.point[0] - x.point[0], b.point[1] - x.point[1], b.point[2] - x.point[2]);

	float len_ab = sqrt(ab.point[0]*ab.point[0] + ab.point[1]*ab.point[1] + ab.point[2]*ab.point[2]);
	float len_ax = sqrt(ax.point[0]*ax.point[0] + ax.point[1]*ax.point[1] + ax.point[2]*ax.point[2]);
	float len_bx = sqrt(bx.point[0]*bx.point[0] + bx.point[1]*bx.point[1] + bx.point[2]*bx.point[2]);

	return (len_ax <= len_ab) && (len_bx <= len_ab);
}

void intersection(const Point3 &source_point, const Point3 &source_vector,
				  float d_param, const Point3 &normal_vector, Point3 &res)
{
	float dp0 = source_vector.point[0]*normal_vector.point[0]
			+ source_vector.point[1]*normal_vector.point[1]
			+ source_vector.point[2]*normal_vector.point[2];

	float dp1 = source_point.point[0]*normal_vector.point[0]
			+ source_point.point[1]*normal_vector.point[1]
			+ source_point.point[2]*normal_vector.point[2];

	float t = (dp1 + d_param)/dp0;

	res.point[0] = source_point.point[0] - t*source_vector.point[0];
	res.point[1] = source_point.point[1] - t*source_vector.point[1];
	res.point[2] = source_point.point[2] - t*source_vector.point[2];
}

void cp(const Point3 &p1, const Point3 &p2, float *res) {
	res[0] = p1.point[1]*p2.point[2] - p1.point[2]*p2.point[1];
	res[1] = p1.point[2]*p2.point[0] - p1.point[0]*p2.point[2];
	res[2] = p1.point[0]*p2.point[1] - p1.point[1]*p2.point[0];
}

/// принадлежность точек отрезку/треугольнику
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

//	Point3 p1(0.1, 0.2, 0.3);
//	Point3 p1(1.0, 0.0, 0.0);

//	Point3 p2(1.0, 1.0, 1.0);
//	Point3 p2(0.0, 1.0, 0.0);

//	__m128 p = intersection_i(Point3(0.1, 0.2, 0.3), Point3(0.9, 0.1, 0.2), -0.8, Point3(1, 0.2, 0.3));
//	printf_s("Result res: %f\t%f\t%f\t%f\n", p[0], p[1], p[2], p[3]);

//	Point3 p = intersection(Point3(0.1, 0.2, 0.3), Point3(0.9, 0.1, 0.2), -0.8, Point3(1, 0.2, 0.3));
//	printf_s("Result res: %f\t%f\t%f\t%f\n", p.point[0], p.point[1], p.point[2], p.point[3]);

//	__m128 res = crossProduct(_mm_load_ps(p1.point), _mm_load_ps(p2.point));
//	printf_s("Result res: %f\t%f\t%f\t%f\n", res[0], res[1], res[2], res[3]);

//	__m128 a1 = _mm_load_ps(p1.point);
//	__m128 a2 = _mm_load_ps(p2.point);
//	__m128 res = _mm_dp_ps(a1, a2, 0x71);

//	printf_s("Result res: %f\t%f\t%f\t%f\n", res[0], res[1], res[2], res[3]);

//	printf_s("Result res: %f\t%f\t%f\t%f\n", res);

	Point3 s_p(0.1, 0.2, 0.3);
	Point3 s_v(0.9, 0.1, 0.2);
	float d_param = -0.8;
	Point3 n_v(1, 0.2, 0.3);

	float count = 0;

	QTime time;
	cout << "Begin" << endl;
	time.start();

	for (int i = 0; i < 100000000; ++i)
	{
		bool ok = inLine_i(Point3(1, 2, 0.1), Point3(0, 3, 0.1), Point3(3, 0, 0.1));

		//__m128 p = intersection_i(s_p, s_v, d_param, n_v);
//		count += p[0];
	//	count+=i;
	}

	cout << time.elapsed() << " msec" << endl;
	cout << "End";
	cout << count;

//	qDebug() << ;
	return a.exec();
}

