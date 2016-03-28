#include <QCoreApplication>
#include <stdio.h>
#include <iostream>
#include <QTime>
#include <QVector>

#include "intrinsics.h"
#include "non_intrinsics.h"
#include "Intersection.h"

using namespace std;

void speed_test();

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

//	speed_test();
//	return a.exec();

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
	intersection_concave_i(face, beam, out);

	for (int i = 0; i < out.size; ++i){
		cout << out.points[i].point[0] << " " << out.points[i].point[1] << endl;
	}

	return a.exec();
}

void speed_test()
{
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

	float count = 0;

	QTime time;
	cout << "Begin" << endl;
	time.start();

	for (int i = 0; i < 100000; ++i)
	{
		Polygon out;
		intersection_concave(face, beam, out);
	}
	cout << time.elapsed() << " msec" << endl;

	cout << "End" << endl;
	cout << count << endl;
}
