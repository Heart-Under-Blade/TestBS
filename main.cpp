#include <QCoreApplication>
#include <stdio.h>
#include <iostream>
#include <QTime>
#include <QVector>

#include "intrinsics.h"
#include "non_intrinsics.h"
#include "Intersection.h"

using namespace std;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

//	speed_test();

	Facet face(4);
	face.normal = Point3(0,0,1);
	face.vertices[0] = Point3(3,5,0);
	face.vertices[1] = Point3(1,1,0);
	face.vertices[2] = Point3(8,3,0);
	face.vertices[3] = Point3(6,5,0);

	Polygon beam(10);
	beam.points[0] = Point3(1,4,0);
	beam.points[1] = Point3(4,1,0);
	beam.points[2] = Point3(5,3,0);
	beam.points[3] = Point3(6,1,0);
	beam.points[4] = Point3(8,5,0);
	beam.points[5] = Point3(7,5,0);
	beam.points[6] = Point3(6,3,0);
	beam.points[7] = Point3(5,4,0);
	beam.points[8] = Point3(4,3,0);
	beam.points[9] = Point3(2,4,0);

//	bool f = inside(Point3(1,4,0), Point3(3,5,0), Point3(1,1,0), Point3(0,0,1));

	Polygon out;
	intersection_concave(face, beam, out);

	for (int i = 0; i < out.size; ++i){
		cout << out.points[i].point[0] << " " << out.points[i].point[1] << endl;
	}

	return a.exec();
}

void speed_test()
{
	Facet polyg(6);
	polyg.vertices[0]=Point3(0, 0, 0);
	polyg.vertices[1]=(Point3(1, -1, 0));
	polyg.vertices[2]=(Point3(2, -1, 0));
	polyg.vertices[3]=(Point3(3, 0, 0));
	polyg.vertices[4]=(Point3(2, 1, 0));
	polyg.vertices[5]=(Point3(1, 1, 0));
	polyg.normal = Point3(0, 0, 1);

	__m128 b = intersection_line_facet_i(Point3(1.5, 0, 0), Point3(1.5, -2, 0), polyg);
	__m128 c = intersection_line_facet_i(Point3(1.5, 0, 0), Point3(1.5, 2, 0), polyg);
	__m128 d = intersection_line_facet_i(Point3(1.5, 0, 0), Point3(4, 0, 0), polyg);

	cout << b[0] << " " << b[1] << " " << b[2] << endl;
	cout << c[0] << " " << c[1] << " " << c[2] << endl;
	cout << d[0] << " " << d[1] << " " << d[2] << endl;

//	PointPosition res3;
//	inFacet_i(Point3(2, 1, 0), polyg, res3);
//	cout << res3.position << " " << res3.facet_side_index_1
//		 << " " << res3.facet_side_index_2 << endl;

//	return a.exec();

	float count = 0;

	QTime time;
	cout << "Begin" << endl;
	time.start();

	for (int i = 0; i < 10000000; ++i)
	{
		__m128 b = intersection_line_facet_i(Point3(1.5, 0, 0), Point3(1.5, -2, 0), polyg);
		count += b[0];
	}
	cout << time.elapsed() << " msec" << endl;

	cout << "End" << endl;
	cout << count << endl;
}
