#ifndef INTRINSICS
#define INTRINSICS

#include <xmmintrin.h>
#include <smmintrin.h>
#include <vector>

#define EPS_IN_POLYGON 0.001

struct Point3
{
	float point[4];

	Point3() {}
	Point3(float x, float y, float z) {
		point[0] = x;
		point[1] = y;
		point[2] = z;
		point[3] = 0.0f;
	}
} __attribute__ ((aligned (16)))  ;

bool inLine_i(const Point3 &x, const Point3 &a, const Point3 &b)
{
	__m128 _x = _mm_load_ps(x.point);
	__m128 _a = _mm_load_ps(a.point);
	__m128 _b = _mm_load_ps(b.point);

	__m128 ab = _mm_sub_ps(_a, _b);
	__m128 ax = _mm_sub_ps(_a, _x);
	__m128 bx = _mm_sub_ps(_b, _x);

	__m128 sqr_len_ab = _mm_dp_ps(ab, ab, 0x71);
	__m128 sqr_len_ax = _mm_dp_ps(ax, ax, 0x71);
	__m128 sqr_len_bx = _mm_dp_ps(bx, bx, 0x71);

	return (sqr_len_ax[0] <= sqr_len_ab[0]) && (sqr_len_bx[0] <= sqr_len_ab[0]);
}

///////////////////////////////////////
//__m128 res = _mm_dp_ps(_mm_load_ps(p1.point), _mm_load_ps(p2.point), 0x71);
///////////////////////////////////////

inline __m128 crossProduct_i(__m128 a, __m128 b)
{
	return _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3,0,2,1)),
								 _mm_shuffle_ps(b, b, _MM_SHUFFLE(3,1,0,2))),
					  _mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3,1,0,2)),
								 _mm_shuffle_ps(b, b, _MM_SHUFFLE(3,0,2,1))));
}
//////////////////////////////////////////////////
__m128 intersection_i(const Point3 &source_point, const Point3 &source_vector,
					  float d_param, const Point3 &normal_vector)
{
	__m128 sp = _mm_load_ps(source_point.point);
	__m128 sv = _mm_load_ps(source_vector.point);
	__m128 nv = _mm_load_ps(normal_vector.point);
	__m128 d = _mm_load_ps1(&d_param);

	__m128 dp0 = _mm_dp_ps(sv, nv, 0x7F);
	__m128 dp1 = _mm_dp_ps(sp, nv, 0x7F);

	__m128 add = _mm_add_ps(dp1, d);
	__m128 t = _mm_div_ps(add, dp0);

	__m128 mul = _mm_mul_ps(t, sv);
	return _mm_sub_ps(sp, mul);
}

int inPolygon_i(const Point3 &x, const Point3 &normal, const std::vector<Point3> &polygon)
{
	int res = -2;
	int size = polygon.size();

	__m128 _x = _mm_load_ps(x.point);
	__m128 _n = _mm_load_ps(normal.point);

	__m128 dir;
	__m128 p1, p2;

	p2 = _mm_load_ps(polygon.at(0).point);

	for (int i = 1; i <= size; ++i)
	{
		p1 = p2;

		p2 = (i != size) ? _mm_load_ps(polygon.at(i).point)
						 : _mm_load_ps(polygon.at(0).point);

		dir = _mm_dp_ps(crossProduct_i(_mm_sub_ps(p2, p1), _mm_sub_ps(_x, p1)), _n, 0x71);

		if (dir[0] < 0)
		{
			if (dir[0] > -EPS_IN_POLYGON) {
				res = 0;
			}
			else {
				return -1;
			}
		}
		else if (dir[0] < EPS_IN_POLYGON) {
			res = 0;
		}
		else {
			res = (res == 0) ? 0
							 : 1;
		}
	}

	return res;
}

int inPolygon_i2(const Point3 &x, Point3 polygon[], const Point3 &normal)
{
	int res = -2;
	int size = 6;

	__m128 _x = _mm_load_ps(x.point);
	__m128 _n = _mm_load_ps(normal.point);

	__m128 dir;
	__m128 p1, p2;

	__m128 eps = _mm_set_ss(EPS_IN_POLYGON);
	__m128 m_eps = _mm_set_ss(-EPS_IN_POLYGON);
	__m128 zero = _mm_set_ss(0);

	p2 = _mm_load_ps(polygon[0].point);

	for (int i = 1; i <= size; ++i)
	{
		p1 = p2;

		p2 = (i != size) ? _mm_load_ps(polygon[i].point)
						 : _mm_load_ps(polygon[0].point);

		dir = _mm_dp_ps(crossProduct_i(_mm_sub_ps(p2, p1), _mm_sub_ps(_x, p1)), _n, 0x71);

//		if (dir[0]<-EPS_IN_POLYGON)
//			return -1;
//		else if (dir[0]<EPS_IN_POLYGON)
//			res=0;

//		if (_mm_ucomilt_ss(dir, m_eps))
//			return -1;
//		else if (_mm_ucomilt_ss(dir, eps))
//			res = 0;

		if (_mm_ucomilt_ss(dir, zero))
		{
			if (_mm_ucomigt_ss(dir, m_eps)) {
				res = 0;
			}
			else {
				res = -1;//return -1;
			}
		}
		else if (_mm_ucomilt_ss(dir, eps)) {
			res = 0;
		}
	}

	return res;
}

#endif // INTRINSICS

