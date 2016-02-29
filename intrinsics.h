#ifndef INTRINSICS
#define INTRINSICS

#include <xmmintrin.h>
#include <smmintrin.h>

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
};


bool inTriangle_i(const Point3 &x, const Point3 &a, const Point3 &b, const Point3 &c)
{
	__m128 _x = _mm_load_ps(x.point);
	__m128 _a = _mm_load_ps(a.point);
	__m128 _b = _mm_load_ps(b.point);
	__m128 _c = _mm_load_ps(c.point);

}

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

inline __m128 crossProduct(__m128 a, __m128 b)
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

#endif // INTRINSICS

