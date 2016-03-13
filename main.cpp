#include <QCoreApplication>
#include <stdio.h>
#include <iostream>
#include <QTime>
#include <QVector>

#include "intrinsics.h"
#include "non_intrinsics.h"

using namespace std;

/// принадлежность точек отрезку/треугольнику
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	Facet polyg(6);
	polyg.vertices[0]=Point3(0, 0, 0);
	polyg.vertices[1]=(Point3(1, -1, 0));
	polyg.vertices[2]=(Point3(2, -1, 0));
	polyg.vertices[3]=(Point3(3, 0, 0));
	polyg.vertices[4]=(Point3(2, 1, 0));
	polyg.vertices[5]=(Point3(1, 1, 0));
	polyg.normal = Point3(0, 0, 1);


//	int res0 = inFacet_i(Point3(-1, 0, 0), a, b, polyg);
//	int res1 = inFacet_i(Point3(2, -1.0001, a, b, 0), polyg);
//	int res2 = inFacet_i(Point3(1, 0, 0), a, b, polyg);

//	cout << res0 << " " << res1 << " " << res2 << endl;

//	int res3 = inFacet_i(Point3(1.5, -1, 0), polyg, s1, s2);
//	cout << res3 << " " << s1 << " " << s2 << endl;

	PointPosition res3;
	inFacet_i(Point3(2, 1, 0), polyg, res3);
	cout << res3.position << " " << res3.facet_side_index_1
		 << " " << res3.facet_side_index_2 << endl;

//	return a.exec();

	float count = 0;

	QTime time;
	cout << "Begin" << endl;
	time.start();

	for (int i = 0; i < 10000000; ++i)
	{
		PointPosition res0;
		inFacet_i(Point3(2, 0.0001, 0.01), polyg, res0);

		PointPosition res1;
		inFacet_i(Point3(2, 0.0001, 0.01), polyg, res1);

		PointPosition res2;
		inFacet_i(Point3(2, 0.0001, 0.01), polyg, res2);

		count = res0.position + res1.position + res2.position;
	}
	cout << time.elapsed() << " msec" << endl;

	cout << "End" << endl;
	cout << count << endl;


	return a.exec();
}

