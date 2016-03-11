#include <QCoreApplication>
#include <stdio.h>
#include <iostream>
#include <QTime>
#include <QVector>

#include "math.h"
#include "intrinsics.h"


using namespace std;

void crossProduct(const Point3 &p1, const Point3 &p2, Point3 &res);

float dotProduct(const Point3 &p1, const Point3 &p2) {
	return p1.point[0]*p2.point[0] + p1.point[1]*p2.point[1] + p1.point[2]*p2.point[2];
}

double angle(const Point3 &a, const Point3 &b)
{
	return acos(dotProduct(a, b)*sqrt(dotProduct(b, b))/sqrt(dotProduct(a, a)));
}

/// 1 - in polygon, 0 - in line, -1 - out of polygon
int inPolygon(const Point3 &x, const std::vector<Point3> &polygon, const Point3 &normal)
{
	int res = -2;
	int size = polygon.size();
float dir;
	for (int i = 1; i <= size; ++i)
	{
		Point3 p1, p2;

		p1 = polygon.at(i-1);

		p2 = (i != size) ? polygon.at(i)
						 : polygon.at(0);

		Point3 p12;
		p12.point[0] = p2.point[0] - p1.point[0];
		p12.point[1] = p2.point[1] - p1.point[1];
		p12.point[2] = p2.point[2] - p1.point[2];

		Point3 p1x;
		p1x.point[0] = x.point[0] - p1.point[0];
		p1x.point[1] = x.point[1] - p1.point[1];
		p1x.point[2] = x.point[2] - p1.point[2];

		Point3 r;
		crossProduct(p12, p1x, r);

		dir = dotProduct(r, normal);

		if (dir < 0)
		{
			if (dir > -EPS_IN_POLYGON) {
				res = 0;
			}
			else {
				return -1;
			}
		}
		else if (dir < EPS_IN_POLYGON) {
			res = 0;
		}
		else {
			res = (res == 0) ? 0
							 : 1;
		}
	}

	return res;
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

void crossProduct(const Point3 &p1, const Point3 &p2, Point3 &res) {
	res.point[0] = p1.point[1]*p2.point[2] - p1.point[2]*p2.point[1];
	res.point[1] = p1.point[2]*p2.point[0] - p1.point[0]*p2.point[2];
	res.point[2] = p1.point[0]*p2.point[1] - p1.point[1]*p2.point[0];
}

/// принадлежность точек отрезку/треугольнику
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	int size = 6;
	Polygon polyg(6);
	polyg.vertices[0]=Point3(0, 0, 0);
	polyg.vertices[1]=(Point3(1, -1, 0));
	polyg.vertices[2]=(Point3(2, -1, 0));
	polyg.vertices[3]=(Point3(3, 0, 0));
	polyg.vertices[4]=(Point3(2, 1, 0));
	polyg.vertices[5]=(Point3(1, 1, 0));
	polyg.normal = Point3(0, 0, 1);

//	Point3 polyg[6];
//	polyg[0]=Point3(0, 0, 0);
//	polyg[1]=(Point3(1, -1, 0));
//	polyg[2]=(Point3(2, -1, 0));
//	polyg[3]=(Point3(3, 0, 0));
//	polyg[4]=(Point3(2, 1, 0));
//	polyg[5]=(Point3(1, 1, 0));
	int res0 = inPolygon_i2(Point3(-1, 0, 0), polyg);
	int res1 = inPolygon_i2(Point3(2, -1.0001, 0), polyg);
	int res2 = inPolygon_i2(Point3(1, 0, 0), polyg);
	cout << res0<< " "<<res1<<" "<<res2<<endl;

	float count = 0;

	QTime time;
	cout << "Begin" << endl;
	time.start();

	for (int i = 0; i < 10000000; ++i)
	{
		//int res0 = inPolygon_i(Point3(-1, 0, 0), Point3(0, 0, 1), polygon);
	//	int res1 = inPolygon_i(Point3(2, -1.0001, 0), polygon, Point3(0, 0, 1));
	//	int res2 = inPolygon_i(Point3(1, 0, 0), polygon, Point3(0, 0, 1));

		int res0 = inPolygon_i2(Point3(2, 0.0001, 0.01), polyg);
		//int res1 = inPolygon_i2(Point3(2, -1.001, 0), polyg, Point3(0, 0, 1));
		//int res2 = inPolygon_i2(Point3(1, 0, 0), polyg, Point3(0, 0, 1));
		int res1 = inPolygon_i2(Point3(2, 0.0001, 0.01), polyg);
		int res2 = inPolygon_i2(Point3(2, 0.0001, 0.01), polyg);
//		int res0 = inPolygon(Point3(-1, 0, 0), polygon, Point3(0, 0, 1));
//		int res1 = inPolygon(Point3(2, -1.0001, 0), polygon, Point3(0, 0, 1));
//		int res2 = inPolygon(Point3(1, 0, 0), polygon, Point3(0, 0, 1));

		count = res0 + res1 + res2;
		//__m128 p = intersection_i(s_p, s_v, d_param, n_v);
//		count += p[0];
	//	count+=i;
	}
	cout << time.elapsed() << " msec" << endl;

	cout << "End" << endl;
	cout << count << endl;


//	qDebug() << ;
	return a.exec();
}

