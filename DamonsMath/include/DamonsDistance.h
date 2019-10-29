#ifndef _DAMONS_DISTANCE_H_
#define _DAMONS_DISTANCE_H_

#include "DamonsPoint.h"
#include "DamonsSegment.h"
#include "DamonsDirection.h"
#include "DamonsBox.h"
#include "DamonsTriangle.h"
#include "DamonsPlane.h"
#include "DamonsLine.h"

namespace DGraphic {

	/// class  DDistance
	/// @breif DDistance compute 3 dimension entity distance between another
	///        it is an operation class that contains no data

	class DDistance
	{
	public:
		///@breif define a uninitialize constructor
		DDistance() {

		}
		///@breif define a uninitialize deallocator
		~DDistance() {

		}

	public:
		/// @brief Calculate the distance between point and line
		///
		/// @param p DPoint of type T
		/// @param l DLine  of type T.
		/// @param The distance between point  and line

		template <class T>
		static inline T PointToLine(const DPoint<T> &p, const DLine<T> &l) {
			/// assume the line is Ps + dir*t
			/// we can calculate t,and the nearest point ,so we can calculate distance
			DPoint<T>  pjt = l.Projection(p);

			return pjt.Distance(p);

		}

		/// @brief Calculate the distance between point and segment
		///
		/// @param p DPoint of type T
		/// @param l DSegment  of type T.
		/// @param The distance between point  and line

		template <class T>
		static inline T PointToSegment(const DPoint<T> &p, const DSegment<T> &l) {
			/// assume the line is Ps + (Pe-Ps)*t,where t >= 0 and t <= 1
			/// if the nearest point of p lie between the start and end point of line
			/// then we can calculate t,and the nearest point ,so we can calculate distance
			/// otherwise the distance is distance between p and the two end points of line
			DPoint<T> vec = p - l.GetStartPoint();
			DPoint<T> v = DPoint<T>(l.to_vector());

			T t = vec.DotProduct(v) / l.SquaredLength();
			if (t < static_cast<T>(DEplision)) {
				return vec.Length();
			}
			else if ((t + DEplision) > static_cast<T>(1)) {
				return (p - l.GetEndPoint()).Length();
			}
			else {
				DPoint<T> r = (p - (l.GetStartPoint() + t*v));
				return r.Length();
			}

		}

		/// @brief Calculate the distance between point and ray
		///
		/// @param p DPoint of type T
		/// @param l DRay  of type T.
		/// @return The distance between point and ray

		template <class T>
		static inline T PointToRay(const DPoint<T> &p, const DRay<T> &r) {
			///same method with PointToLine but different t value
			DPoint<T> vec = p - r.GetSourcePoint();
			const DDirection<T> dir = r.Direction();
			DPoint<T> v = DPoint<T>(dir.x(), dir.y(), dir.z());

			T t = vec.DotProduct(v);
			if (t < static_cast<T>(DEplision)) {
				return vec.Length();
			}
			else {
				DPoint<T> res = (p - (r.GetSourcePoint() + t*v));
				return res.Length();
			}
		}

		/// @brief Calculate the distance between point and plane
		///
		/// @param p DPoint of type T
		/// @param pl DPlane  of type T.
		/// @return The distance between point and plane

		template <class T>
		static inline T PointToPlane(const DPoint<T> &p, const DPlane<T> &pl) {

			DDirection<T> dir = pl.GetDirection();
			DPoint<T> norm = DPoint<T>(dir.x(), dir.y(), dir.z());
			DPoint<T> orgpt = pl.GetOrigin();

			T s1 = norm.DotProduct(p);
			T s2 = norm.DotProduct(orgpt);

			return (s1 - s2);
		}

		/// @brief Calculate the distance between point and triangle
		///
		/// @param p DPoint of type T
		/// @param tri DTriangle  of type T.
		/// @return The distance between point and triangle

		template <class T>
		static inline T PointToTriangle(const DPoint<T> &p, const DTriangle<T> &tri) {

			///1. calculate the nearest point lie on triangle
			DDirection<T> dir = tri.Normal();
			DPoint<T> norm = DPoint<T>(dir.x(), dir.y(), dir.z());
			DPoint<T> orgpt = tri[0];

			T s1 = norm.DotProduct(p);
			T s2 = norm.DotProduct(orgpt);
			DPoint<T> npt = (p - (s1-s2)*norm);
			
			///2. calculate whether the nearest point is lie in triangle
			/// if lie in triangle,the distance is p and the nearest point
			/// otherwise calculate the nearest distance between p and three lines of triangle
			if (tri.Has_on(npt)) {
				return p.Distance(npt);
			}
			else {
				DSegment<T> l1 = DSegment<T>(tri[0], tri[1]);
				DSegment<T> l2 = DSegment<T>(tri[1], tri[2]);
				DSegment<T> l3 = DSegment<T>(tri[2], tri[0]);

				T d1 = PointToSegment(p, l1);
				T d2 = PointToSegment(p, l2);
				T d3 = PointToSegment(p, l3);

				return std::min(d1, std::min(d2,d3));
			}
		}

		/// @brief Calculate the min distance between two lines
		///
		/// @param l1 DLine of type T
		/// @param l2 DLine  of type T.
		/// @return The min distance between two lines

		template<class T>
		static inline T LineToLine(const DLine<T> &l1, const DLine<T> &l2) {
			/// assume P and Q is the nearest point
			/// P = p1+s*d1 Q = p2+t*d2
			/// (P-Q)*d1 = 0 
			/// (P-Q)*d2 = 0
			DPoint<T> u =( l1.GetSourcePoint() - l2.GetSourcePoint());

			DDirection<T> d0 = l1.Direction();
			DDirection<T> d1 = l2.Direction();

			T a = d0.DotProduct(d0);
			T b = d0.DotProduct(d1);
			T c = d1.DotProduct(d1);
			T d = d0.DotProduct(u);
			T e = d1.DotProduct(u);

			T s = 0.0f;
			T t = 0.0f;
			if (std::abs(a*c - b*b) < DEplision) {
				s = 0.0f;
				t = (b > c ? d / b : e / c);
			}
			else {
				T inv = 1.0 / (a*c - b*b);
				s = (b*e - d*c)*inv;
				t = (a*e - d*b)*inv;
			}

			DPoint<T> r = u + DPoint<T>((s*d0[0] - t*d1[0]), (s*d0[1] - t*d1[1]), (s*d0[2] - t*d1[2]));

			return r.Length();
		}

		/// @brief Calculate the min distance between two segments
		///
		/// @param l1 DSegment of type T
		/// @param l2 DSegment  of type T.
		/// @return The min distance between two segments

		template<class T>
		static inline T SegmentToSegment(const DSegment<T> &l1, const DSegment<T> &l2) {
			///same with LineToLine but need to check range
			DPoint<T> u = (l1.GetStartPoint() - l2.GetStartPoint());

			DPoint<T> d0 = l1.GetEndPoint() - l1.GetStartPoint();
			DPoint<T> d1 = l2.GetEndPoint() - l2.GetStartPoint();

			T a = d0.DotProduct(d0);
			T b = d0.DotProduct(d1);
			T c = d1.DotProduct(d1);
			T d = d0.DotProduct(u);
			T e = d1.DotProduct(u);

			T s = 0.0f;
			T t = 0.0f;
			T det = (a*c - b*b);

			T sD, tD;

			if (std::abs(a*c - b*b) < DEplision) {
				s = 0.0f;
				t = e;
				tD = c;
				sD = det;
			}
			else {
				s = (b*e - d*c);
				t = (a*e - d*b);
			}

			sD = det;
			if (s < DEplision) {
				s = 0.0;
				t = e;
				tD = c;
			}
			else if (s > (det )) {
				s = det;
				t = e + b;
				tD = c;
			}
			else {
				tD = det;
			}

			if (t < DEplision) {
				t = 0.0;
				if (-d < DEplision) {
					s = 0.0;
				 }
				else if (-d > a) {
					s = sD;
				}
				else {
					s = -d;
					sD = a;
				}
			}
			else if (t > (det )) {
				t = tD;
				if ((-d+b) < DEplision) {
					s = 0.0;
				}
				else if ((-d + b) > a) {
					s = sD;
				}
				else {
					s = -d+b;
					sD = a;
				}
			}
	
			s = s / sD;
			t = t / tD;

			DPoint<T> r = u + DPoint<T>((s*d0[0] - t*d1[0]), (s*d0[1] - t*d1[1]), (s*d0[2] - t*d1[2]));

			return r.Length();
		}

		/// @brief Calculate the min distance between line and ray
		///
		/// @param l1 DLine of type T
		/// @param l2 DRay  of type T.
		/// @return The min distance between line and ray

		template<class T>
		static inline T LineToRay(const DLine<T> &l1, const DRay<T> &l2) {

			DPoint<T> u = (l1.GetSourcePoint() - l2.GetSourcePoint());

			DDirection<T> d0 = l1.Direction();
			DDirection<T> d1 = l2.Direction();

			T a = d0.DotProduct(d0);
			T b = d0.DotProduct(d1);
			T c = d1.DotProduct(d1);
			T d = d0.DotProduct(u);
			T e = d1.DotProduct(u);

			T s = 0.0f;
			T t = 0.0f;
			T det = (a*c - b*b);

			T sD, tD;
			sD = det;
			tD = det;
			if (std::abs(a*c - b*b) < DEplision) {
				s = 0.0f;
				t = e;
				tD = c;
				sD = det;
			}
			else {
				s = (b*e - d*c);
				t = (a*e - d*b);
			}

			if (t < DEplision) {
				t = 0.0;
				s = -d;
				sD = a;
			}

			s = s / sD;
			t = t / tD;

			DPoint<T> r = u + DPoint<T>((s*d0[0] - t*d1[0]), (s*d0[1] - t*d1[1]), (s*d0[2] - t*d1[2]));

			return r.Length();
		}

		/// @brief Calculate the min distance between line and Segment
		///
		/// @param l1 DLine of type T
		/// @param l2 DSegment  of type T.
		/// @return The min distance between line and Segment

		template<class T>
		static inline T LineToSegment(const DLine<T> &l1, const DSegment<T> &l2) {

			DPoint<T> u = (l1.GetSourcePoint() - l2.GetStartPoint());

			DDirection<T> dd = l1.Direction();
			DPoint<T>     d0 = DPoint<T>(dd.x(), dd.y(), dd.z());
			DPoint<T>     d1 = l2.GetEndPoint() - l2.GetStartPoint();

			T a = d0.DotProduct(d0);
			T b = d0.DotProduct(d1);
			T c = d1.DotProduct(d1);
			T d = d0.DotProduct(u);
			T e = d1.DotProduct(u);

			T s = 0.0f;
			T t = 0.0f;
			T det = (a*c - b*b);

			T sD, tD;
			sD = det;
			tD = det;

			if (std::abs(a*c - b*b) < DEplision) {
				s = 0.0f;
				t = e;
				tD = c;
				sD = det;
			}
			else {
				s = (b*e - d*c);
				t = (a*e - d*b);
			}

			if (t < DEplision) {
				t = 0.0;
				s = -d;
				sD = a;
			}
			else if (t > (det)) {
				t = tD;
				s = -d + b;
				sD = a;
			}

			s = s / sD;
			t = t / tD;

			DPoint<T> r = u + DPoint<T>((s*d0[0] - t*d1[0]), (s*d0[1] - t*d1[1]), (s*d0[2] - t*d1[2]));

			return r.Length();
		}

		/// @brief Calculate the min distance between ray and ray
		///
		/// @param l1 DRay of type T
		/// @param l2 DRay  of type T.
		/// @return The min distance between ray and ray

		template<class T>
		static inline T RayToRay(const DRay<T> &l1, const DRay<T> &l2) {

			DPoint<T> u = (l1.GetSourcePoint() - l2.GetSourcePoint());

			DDirection<T> d0 = l1.Direction();
			DDirection<T> d1 = l2.Direction();

			T a = d0.DotProduct(d0);
			T b = d0.DotProduct(d1);
			T c = d1.DotProduct(d1);
			T d = d0.DotProduct(u);
			T e = d1.DotProduct(u);

			T s = 0.0f;
			T t = 0.0f;
			T det = (a*c - b*b);

			T sD, tD;
			sD = det;
			tD = det;
			if (std::abs(a*c - b*b) < DEplision) {
				s = 0.0f;
				t = e;
				tD = c;
				sD = det;
			}
			else {
				s = (b*e - d*c);
				t = (a*e - d*b);
			}

			if (s < DEplision) {
				s = 0.0;
				t = e;
				tD = c;
			}

			
			if (t < DEplision) {
				t = 0.0;
				if (-d < DEplision) {
					s = 0.0;
				}
				else {
					s = -d;
					sD = a;
				}
			}

			s = s / sD;
			t = t / tD;

			DPoint<T> r = u + DPoint<T>((s*d0[0] - t*d1[0]), (s*d0[1] - t*d1[1]), (s*d0[2] - t*d1[2]));

			return r.Length();
		}

		/// @brief Calculate the min distance between ray and Segment
		///
		/// @param l1 DRay of type T
		/// @param l2 DSegment  of type T.
		/// @return The min distance between ray and Segment

		template<class T>
		static inline T RayToSegment(const DRay<T> &l1, const DSegment<T> &l2) {

			DPoint<T> u = (l1.GetSourcePoint() - l2.GetStartPoint());

			DDirection<T> dd = l1.Direction();
			DPoint<T>     d0 = DPoint<T>(dd.x(), dd.y(), dd.z());
			DPoint<T>     d1 = l2.GetEndPoint() - l2.GetStartPoint();

			T a = d0.DotProduct(d0);
			T b = d0.DotProduct(d1);
			T c = d1.DotProduct(d1);
			T d = d0.DotProduct(u);
			T e = d1.DotProduct(u);

			T s = 0.0f;
			T t = 0.0f;
			T det = (a*c - b*b);

			T sD, tD;
			sD = det;
			
			if (std::abs(a*c - b*b) < DEplision) {
				s = 0.0f;
				t = e;
				tD = c;
				sD = det;
			}
			else {
				s = (b*e - d*c);
				t = (a*e - d*b);
			}

			if (s < DEplision) {
				s = 0.0;
				t = e;
				tD = c;
			}
			else  {
				tD = det;
			}

			if (t < DEplision) {
				t = 0.0;
				if (-d < DEplision) {
					s = 0.0;
				}
				else {
					s = -d;
					sD = a;
				}
			}
			else if(t > tD){
				t = tD;
				if ((-d + b) < DEplision) {
					s = 0.0;
				}
				else {
					s = -d + b;
					sD = a;
				}
			}

			s = s / sD;
			t = t / tD;

			DPoint<T> r = u + DPoint<T>((s*d0[0] - t*d1[0]), (s*d0[1] - t*d1[1]), (s*d0[2] - t*d1[2]));

			return r.Length();
		}

	protected:
	};
};
#endif
