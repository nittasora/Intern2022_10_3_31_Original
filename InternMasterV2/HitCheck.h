#pragma once

namespace MySystem::HitCheck
{
	inline Vec2 CalcuNormal(const Vec2& vec)
	{
		return Vec2{ -vec.y, vec.x }.normalized();
	}

	inline Vec2 calcWallScratchVector(const Vec2& front, const Vec2& normal) {
		Vec2 normal_n = normal.normalized();

		return (front - front.dot(normal_n) * normal_n);
	}

	inline Vec2 calcReflectVector(const Vec2& front, const Vec2& normal) {
		Vec2 normal_n = normal.normalized();

		return (front - 2.0f * front.dot(normal_n) * normal_n).normalized();
	}

	inline bool HitCheckSegmentToSegment(
		const Vec2& segment1Start, const Vec2& segment1End,
		const Vec2& segment2Start, const Vec2& segment2End,
		double& t,
		Vec2& fixHitPos
	)
	{
		Vec2 v = segment2Start - segment1Start;
		Vec2 dir1 = segment1End - segment1Start;
		Vec2 dir2 = segment2End - segment2Start;
		double Crs_v1_v2 = dir1.cross(dir2);
		if (Crs_v1_v2 == 0.0f) {
			// 平行状態
			return false;
		}

		double Crs_v_v1 = v.cross(dir1);
		double Crs_v_v2 = v.cross(dir2);

		double t1 = Crs_v_v2 / Crs_v1_v2;
		double t2 = Crs_v_v1 / Crs_v1_v2;

		const double eps = 0.001;
		if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1) {
			// 交差していない
			return false;
		}

		t = t1;

		fixHitPos = (segment1End - segment1Start) * t1 + segment1Start;

		return true;
	}
}
