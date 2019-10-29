#ifndef _DAMONS_INTERSECT_H_
#define _DAMONS_INTERSECT_H_

#include "DamonsPoint.h"
#include "DamonsSegment.h"
#include "DamonsDirection.h"
#include "DamonsBox.h"
#include "DamonsTriangle.h"
#include "DamonsPlane.h"
#include "DamonsLine.h"

namespace DGraphic {

	//! Edge to edge test based on Franlin Antonio's gem: "Faster Line Segment Intersection", in Graphics Gems III, pp. 199-202
#define EDGE_EDGE_TEST(V0, U0, U1)						\
	Bx = U0[i0] - U1[i0];								\
	By = U0[i1] - U1[i1];								\
	Cx = V0[i0] - U0[i0];								\
	Cy = V0[i1] - U0[i1];								\
	f  = Ay*Bx - Ax*By;									\
	d  = By*Cx - Bx*Cy;									\
	if((f>0.0f && d>=0.0f && d<=f) || (f<0.0f && d<=0.0f && d>=f))	\
	{													\
		const float e=Ax*Cy - Ay*Cx;					\
		if(f>0.0f)										\
		{												\
			if(e>=0.0f && e<=f) return true;			\
		}												\
		else											\
		{												\
			if(e<=0.0f && e>=f) return true;			\
		}												\
	}

	//! TO BE DOCUMENTED
#define EDGE_AGAINST_TRI_EDGES(V0, V1, U0, U1, U2)		\
{														\
	float Bx,By,Cx,Cy,d,f;								\
	const float Ax = V1[i0] - V0[i0];					\
	const float Ay = V1[i1] - V0[i1];					\
	/* test edge U0,U1 against V0,V1 */					\
	EDGE_EDGE_TEST(V0, U0, U1);							\
	/* test edge U1,U2 against V0,V1 */					\
	EDGE_EDGE_TEST(V0, U1, U2);							\
	/* test edge U2,U1 against V0,V1 */					\
	EDGE_EDGE_TEST(V0, U2, U0);							\
}

	//! TO BE DOCUMENTED
#define POINT_IN_TRI(V0, U0, U1, U2)					\
{														\
	/* is T1 completly inside T2? */					\
	/* check if V0 is inside tri(U0,U1,U2) */			\
	float a  = U1[i1] - U0[i1];							\
	float b  = -(U1[i0] - U0[i0]);						\
	float c  = -a*U0[i0] - b*U0[i1];					\
	float d0 = a*V0[i0] + b*V0[i1] + c;					\
														\
	a  = U2[i1] - U1[i1];								\
	b  = -(U2[i0] - U1[i0]);							\
	c  = -a*U1[i0] - b*U1[i1];							\
	const float d1 = a*V0[i0] + b*V0[i1] + c;			\
														\
	a  = U0[i1] - U2[i1];								\
	b  = -(U0[i0] - U2[i0]);							\
	c  = -a*U2[i0] - b*U2[i1];							\
	const float d2 = a*V0[i0] + b*V0[i1] + c;			\
	if(d0*d1>0.0f)										\
	{													\
		if(d0*d2>0.0f) return true;						\
	}													\
}

	/// class  DIntersection
	/// @breif DIntersection compute 3 dimension entity intersection between another
	///        it is an operation class that contains no data

	class DIntersection {

		public:
			///@breif define a uninitialize constructor
			DIntersection() {

			}
			///@breif define a uninitialize deallocator
			~DIntersection() {

			}

		public:
			/// @brief Calculate the intersection between line and plane
			///
			/// @param p DPlane of type T
			/// @param l DLine  of type T.
			/// @param result is intersect point as return
			/// @return a bool value whether  plane  and line is intersected
			template<class T>
			static bool LineWithPlane(DLine<T> &l,DPlane<T> &p, DPoint<T > &result) {
				DDirection<T> ld = l.Direction();
				DDirection<T> pn = p.GetDirection();

				T det = ld.DotProduct(pn);
				/// line is parallel with plane
				if (std::abs(det) < DEplision) {
				///check whether the line is lie on plane
					if (p.Has_on(l.GetSourcePoint())) {
						result = l.GetSourcePoint();
						return true;
					}
					else {
						return false;
					}
				}

				T t = -(pn.DotProduct(l.GetSourcePoint())- pn.DotProduct(p.GetOrigin())) / det;

				result = l.GetPointOnLine(t);

				return true;
			}

			/// @brief Calculate the intersection between ray and plane
			///
			/// @param p DPlane of type T
			/// @param l DRay  of type T.
			/// @param result is intersect point as return
			/// @return a bool value whether  plane  and ray is intersected
			template<class T>
			static bool RayWithPlane(DRay<T> &l, DPlane<T> &p, DPoint<T > &result) {
				DDirection<T> ld = l.Direction();
				DDirection<T> pn = p.GetDirection();

				T det = ld.DotProduct(pn);
				/// line is parallel with plane
				if (std::abs(det) < DEplision) {
					///check whether the line is lie on plane
					if (p.Has_on(l.GetSourcePoint())) {
						result = l.GetSourcePoint();
						return true;
					}
					else {
						return false;
					}
				}

				T t = -(pn.DotProduct(l.GetSourcePoint()) - pn.DotProduct(p.GetOrigin())) / det;
				if (t > 0) {
					result = l.GetSourcePoint()+DPoint<T>(t*ld.x(), t*ld.y(), t*ld.z());
					return true;
				}
				else
					return false;

			}

			/// @brief Calculate the intersection between segment and plane
			///
			/// @param p DPlane of type T
			/// @param l DSegment  of type T.
			/// @param result is intersect point as return
			/// @return a bool value whether  plane  and segment is intersected
			template<class T>
			static bool SegmentWithPlane(DSegment<T> &l, DPlane<T> &p, DPoint<T > &result) {
				DDirection<T> ld = l.Direction();
				DDirection<T> pn = p.GetDirection();

				T det = ld.DotProduct(pn);
				/// line is parallel with plane
				if (std::abs(det) < DEplision) {
					///check whether the line is lie on plane
					if (p.Has_on(l.GetStartPoint())) {
						result = l.GetStartPoint();
						return true;
					}
					else {
						return false;
					}
				}

				T t = -(pn.DotProduct(l.GetStartPoint()) - pn.DotProduct(p.GetOrigin())) / det;
				if (t > 0 && t < l.Length()) {
					result = l.GetStartPoint() + DPoint<T>(t*ld.x(), t*ld.y(), t*ld.z());
					return true;
				}
				else
					return false;
			}

			/// @brief Calculate the intersection between line and triangle
			///
			/// @param tri DTriangle of type T
			/// @param l DLine  of type T.
			/// @param result is intersect point as return
			/// @return a bool value whether triangle and line is intersected
			template<class T>
			static bool LineWithTriangle(DLine<T> &l, DTriangle<T> &tri, DPoint<T > &result) {
				DDirection<T> tn = tri.Normal();
				DDirection<T> ld = l.Direction();

				DPoint<T> e1 = tri[1] - tri[0];
				DPoint<T> e2 = tri[2] - tri[0];

				DPoint<T> p = ld.CrossProduct(e2);
				T det = e1.DotProduct(p);

				if (det < DEplision && det > -DEplision)
					return false;

				T OneOverDet = 1.0 / det;
	
				DPoint<T> s = l.GetSourcePoint() - tri[0];
				T u = OneOverDet*(s.DotProduct(p));
				if (u < 0.0 || u > 1.0)
					return false;

				DPoint<T> q = s.CrossProduct(e1);

				T v = OneOverDet*(ld.DotProduct(q));
				if (v < 0.0 || (v+u) > 1.0)
					return false;

				T t = (e2.DotProduct(q))* OneOverDet;
				
				result = l.GetPointOnLine(t);

				return true;
			}

			/// @brief Calculate the intersection between ray and triangle
			///
			/// @param tri DTriangle of type T
			/// @param l DRay  of type T.
			/// @param result is intersect point as return
			/// @return a bool value whether triangle and ray is intersected
			template<class T>
			static bool RayWithTriangle(DRay<T> &l, DTriangle<T> &tri, DPoint<T > &result) {
				DDirection<T> tn = tri.Normal();
				DDirection<T> ld = l.Direction();

				DPoint<T> e1 = tri[1] - tri[0];
				DPoint<T> e2 = tri[2] - tri[0];

				DPoint<T> p = ld.CrossProduct(e2);
				T det = e1.DotProduct(p);

				if (det < DEplision && det > -DEplision)
					return false;

				T OneOverDet = 1.0 / det;

				DPoint<T> s = l.GetSourcePoint() - tri[0];
				T u = OneOverDet*(s.DotProduct(p));
				if (u < 0.0 || u > 1.0)
					return false;

				DPoint<T> q = s.CrossProduct(e1);

				T v = OneOverDet*(ld.DotProduct(q));
				if (v < 0.0 || v > 1.0)
					return false;

				T t = (e2.DotProduct(q))* OneOverDet;

				if (t > 0) {
					result = l.GetSourcePoint() + DPoint<T>(t*ld.x(), t*ld.y(), t*ld.z());
					return true;
				}
				else
					return false;

			}

			/// @brief Calculate the intersection between segment and triangle
			///
			/// @param tri DTriangle of type T
			/// @param l DSegment  of type T.
			/// @param result is intersect point as return
			/// @return a bool value whether triangle and segment is intersected
			template<class T>
			static bool SegmentWithTriangle(DSegment<T> &l, DTriangle<T> &tri, DPoint<T > &result) {
				DDirection<T> tn = tri.Normal();
				DDirection<T> ld = l.Direction();

				DPoint<T> e1 = tri[1] - tri[0];
				DPoint<T> e2 = tri[2] - tri[0];

				DPoint<T> p = ld.CrossProduct(e2);
				T det = e1.DotProduct(p);

				if (det < DEplision && det > -DEplision)
					return false;

				T OneOverDet = 1.0 / det;

				DPoint<T> s = l.GetStartPoint() - tri[0];
				T u = OneOverDet*(s.DotProduct(p));
				if (u < 0.0 || u > 1.0)
					return false;

				DPoint<T> q = s.CrossProduct(e1);

				T v = OneOverDet*(ld.DotProduct(q));
				if (v < 0.0 || v > 1.0)
					return false;

				T t = (e2.DotProduct(q))* OneOverDet;

				if (t > 0 && t < l.Length()) {
					result = l.GetStartPoint() + DPoint<T>(t*ld.x(), t*ld.y(), t*ld.z());
					return true;
				}
				else
					return false;

			}

			/// @breif Calculate the intersection between two plane
			/// @detail the direction of intersection line is n1.crossproduct(n2)
			/// just need to calcalate the point lie on this line
			/// assume that p = s*n1 + t*n2,then we get p is lie on p1 and p2 
			/// so n1*(p - O1) = 0 n2*(p - O2) = 0
			///
			/// @param p1,p2 plane need to calculate intersection
			/// @param rl result line of two intersection plane as return
			/// @return bool value whether two plane is intersected

			template<class T>
			static bool PlaneWithPlane(DPlane<T> &p1,DPlane<T> &p2,DLine<T> &rl){
				DDirection<T> n1 = p1.GetDirection();
				DDirection<T> n2 = p2.GetDirection();

				DDirection<T> n = n1.CrossProduct(n2);
				if ((n.x()*n.x() + n.y()*n.y() + n.z()*n.z()) < DEplision){
					return false;
				}
				T a = n1.DotProduct(n1);
				T b = n2.DotProduct(n2);
				T c = n1.DotProduct(n2);
				T d = n1.DotProduct(p1.GetOrigin());
				T e = n2.DotProduct(p2.GetOrigin());

				T t = (d*c - a*e) / (c*c - a*b);
				T s = (e*c - d*b) / (c*c - a*b);

				DPoint<T > po((s*n1.x() + t*n2.x()), (s*n1.y() + t*n2.y()), (s*n1.z() + t*n2.z()));
				rl = DLine<T>(po,n);
				return true;
			}

			enum TrianglePlaneIntersectType{
				TPIT_NONE,// no intersection
				TPIT_ONEPOINT,//a point of triangle lie on plane
				TPIT_ON,//triangle lie on plane
				TPIT_TPIntersect//triangle intersect with plane
			};

			/// @brief Calculate the intersection between plane and triangle
			///
			/// @param tri DTriangle of type T
			/// @param p plane  of type T.
			/// @param result is intersect result as return
			/// @return a enum value that which type is the intersection
			template<class T>
			static TrianglePlaneIntersectType PlaneWithTriangle(DPlane<T> &p, DTriangle<T> &tri, DObject &result){
				///just need to calculate which side is the three point of triangle lie on 
				float dis1, dis2, dis3;
				DDirection<T > n = p.GetDirection();
				dis1 = n.DotProduct((tri[0] - p.GetOrigin()));
				dis2 = n.DotProduct((tri[1] - p.GetOrigin()));
				dis3 = n.DotProduct((tri[2] - p.GetOrigin()));

				if (std::abs(dis1) < DEplision)
					dis1 = 0.0f;
				if (std::abs(dis2) < DEplision)
					dis2 = 0.0f;
				if (std::abs(dis3) < DEplision)
					dis3 = 0.0f;

				float d1d2 = dis1*dis2;
				float d1d3 = dis1*dis3;

				if (d1d2 > 0 && d1d3 > 0){
					//all point above or below plane
					return TPIT_NONE;
				}
				
				if ((std::abs(dis1) + std::abs(dis2) + std::abs(dis3)) < DEplision){
					///coplaner
					result = tri;
					return TPIT_ON;
				}

				//////////////////////////////////////////////////////////////////////////
				///one point on plane
				if (std::abs(dis1) < DEplision && ( dis2*dis3 > 0.0f)){
					result = tri[0];
					return TPIT_ONEPOINT;
				}
				
				if (std::abs(dis2) < DEplision && (dis1*dis3 > 0.0f)){
					result = tri[1];
					return TPIT_ONEPOINT;
				}

				if (std::abs(dis3) < DEplision && (dis2*dis1 > 0.0f)){
					result = tri[2];
					return TPIT_ONEPOINT;
				}

				if (std::abs(dis1) < DEplision && (dis2*dis3 < 0.0f)){
					DSegment<T > sl(tri[1],tri[2]);
					DPoint<T> rp;
					SegmentWithPlane(sl, p, rp);
					result = DSegment<T>(tri[0], rp);
					return TPIT_TPIntersect;
				}

				if (std::abs(dis2) < DEplision && (dis1*dis3 < 0.0f)){
					DSegment<T > sl(tri[2], tri[0]);
					DPoint<T> rp;
					SegmentWithPlane(sl, p, rp);
					result = DSegment<T>(tri[1], rp);
					return TPIT_TPIntersect;
				}

				if (std::abs(dis3) < DEplision && (dis2*dis1 < 0.0f)){
					DSegment<T > sl(tri[0], tri[1]);
					DPoint<T> rp;
					SegmentWithPlane(sl, p, rp);
					result = DSegment<T>(tri[2], rp);
					return TPIT_TPIntersect;
				}
				//////////////////////////////////////////////////////////////////////////
				///two point on plane
				if (std::abs(dis1) < DEplision && std::abs(dis2) < DEplision && std::abs(dis3) > DEplision){
					result = DSegment<T>(tri[0],tri[1]);
					return TPIT_TPIntersect;
				}

				if (std::abs(dis2) < DEplision && std::abs(dis3) < DEplision && std::abs(dis1) > DEplision){
					result = DSegment<T>(tri[1], tri[2]);
					return TPIT_TPIntersect;
				}

				if (std::abs(dis3) < DEplision && std::abs(dis1) < DEplision && std::abs(dis2) > DEplision){
					result = DSegment<T>(tri[2], tri[0]);
					return TPIT_TPIntersect;
				}
				//////////////////////////////////////////////////////////////////////////
				/// intersect
				if (dis1 > 0 && (dis2 < 0&& dis3 < 0)){
					DSegment<T > sl1(tri[0], tri[1]);
					DPoint<T> rp1;
					SegmentWithPlane(sl1, p, rp1);

					DSegment<T > sl2(tri[0], tri[2]);
					DPoint<T> rp2;
					SegmentWithPlane(sl2, p, rp2);

					result = DSegment<T>(rp1, rp2);
					return TPIT_TPIntersect;
				}
				if (dis2 > 0 && (dis3 < 0&& dis1 < 0)){
					DSegment<T > sl1(tri[1], tri[2]);
					DPoint<T> rp1;
					SegmentWithPlane(sl1, p, rp1);

					DSegment<T > sl2(tri[1], tri[0]);
					DPoint<T> rp2;
					SegmentWithPlane(sl2, p, rp2);

					result = DSegment<T>(rp1, rp2);
					return TPIT_TPIntersect;
				}
				if (dis3 > 0 && (dis2 < 0&& dis1 < 0)){
					DSegment<T > sl1(tri[2], tri[0]);
					DPoint<T> rp1;
					SegmentWithPlane(sl1, p, rp1);

					DSegment<T > sl2(tri[2], tri[1]);
					DPoint<T> rp2;
					SegmentWithPlane(sl2, p, rp2);

					result = DSegment<T>(rp1, rp2);
					return TPIT_TPIntersect;
				}

				if (dis1 < 0 && (dis2 > 0 && dis3 > 0)){
					DSegment<T > sl1(tri[0], tri[2]);
					DPoint<T> rp1;
					SegmentWithPlane(sl1, p, rp1);

					DSegment<T > sl2(tri[0], tri[1]);
					DPoint<T> rp2;
					SegmentWithPlane(sl2, p, rp2);

					result = DSegment<T>(rp1, rp2);
					return TPIT_TPIntersect;
				}
				if (dis2 < 0 && (dis3 > 0 && dis1> 0)){
					DSegment<T > sl1(tri[1], tri[0]);
					DPoint<T> rp1;
					SegmentWithPlane(sl1, p, rp1);

					DSegment<T > sl2(tri[1], tri[2]);
					DPoint<T> rp2;
					SegmentWithPlane(sl2, p, rp2);

					result = DSegment<T>(rp1, rp2);
					return TPIT_TPIntersect;
				}
				if (dis3 < 0 && (dis2 > 0 && dis1 > 0)){
					DSegment<T > sl1(tri[2], tri[1]);
					DPoint<T> rp1;
					SegmentWithPlane(sl1, p, rp1);

					DSegment<T > sl2(tri[2], tri[0]);
					DPoint<T> rp2;
					SegmentWithPlane(sl2, p, rp2);

					result = DSegment<T>(rp1, rp2);
					return TPIT_TPIntersect;
				}
			}

			/// @brief test if plane and triangle is intersected
			///
			/// @param tri1 DTriangle  of type T
			/// @param tri2 DTriangle  of type T.
			/// @return a bool value that whether two triangle is intersected
			template<class T>
			static bool TriangleWithTriangle(DTriangle<T> &tri1, DTriangle<T> &tri2){

				///1. triangle is degenerated , return
				if (tri1.Is_degenerate() || tri2.Is_degenerate())
					return false;
				///2. triangle is on the one side of another,return
				DPlane<T> plane1 = tri1.Plane();
				DDirection<T > n1 = plane1.GetDirection();
				float dis1 = n1.DotProduct((tri2[0] - plane1.GetOrigin()));
				float dis2 = n1.DotProduct((tri2[1] - plane1.GetOrigin()));
				float dis3 = n1.DotProduct((tri2[2] - plane1.GetOrigin()));

				if (std::abs(dis1) < DEplision)
					dis1 = 0.0f;
				if (std::abs(dis2) < DEplision)
					dis2 = 0.0f;
				if (std::abs(dis3) < DEplision)
					dis3 = 0.0f;

				float d1d2 = dis1*dis2;
				float d1d3 = dis1*dis3;

				if (d1d2 > 0 && d1d3 > 0){
					//all point above or below plane
					return false;
				}
				///3.
				DPlane<T> plane2 = tri2.Plane();
				DDirection<T> n2 = plane2.GetDirection();
				//two triangle plane is parallel
				DDirection<T> n12 = n1.CrossProduct(n2);
				if ((n12.x()*n12.x() + n12.y()*n12.y() + n12.z()*n12.z()) < DEplision){
					//with same plane
					if (std::abs(dis1) < DEplision || std::abs(dis2) < DEplision || std::abs(dis3) < DEplision)
					{
						float A[3];
						int i0, i1;
						/* first project onto an axis-aligned plane, that maximizes the area */
						/* of the triangles, compute indices: i0,i1. */
						A[0] = fabsf(n1[0]);
						A[1] = fabsf(n1[1]);
						A[2] = fabsf(n1[2]);
						if (A[0] > A[1])
						{
							if (A[0] > A[2])
							{
								i0 = 1;      /* A[0] is greatest */
								i1 = 2;
							}
							else
							{
								i0 = 0;      /* A[2] is greatest */
								i1 = 1;
							}
						}
						else   /* A[0]<=A[1] */
						{
							if (A[2] > A[1])
							{
								i0 = 0;      /* A[2] is greatest */
								i1 = 1;
							}
							else
							{
								i0 = 0;      /* A[1] is greatest */
								i1 = 2;
							}
						}
						//////////////////////////////////////////////////////////////////////////
						/* test all edges of triangle 1 against the edges of triangle 2 */
						EDGE_AGAINST_TRI_EDGES(tri1[0], tri1[1], tri2[0], tri2[1], tri2[2]);
						EDGE_AGAINST_TRI_EDGES(tri1[1], tri1[2], tri2[0], tri2[1], tri2[2]);
						EDGE_AGAINST_TRI_EDGES(tri1[2], tri1[0], tri2[0], tri2[1], tri2[2]);

						/* finally, test if tri1 is totally contained in tri2 or vice versa */
						POINT_IN_TRI(tri1[0], tri2[0], tri2[1], tri2[2]);
						POINT_IN_TRI(tri2[0], tri1[0], tri1[1], tri1[2]);

						return false;
					}
					// plane is parallel and seperate
					else{
						return false;
					}
				}
				///4.
				float dis4 = n2.DotProduct((tri1[0] - plane2.GetOrigin()));
				float dis5 = n2.DotProduct((tri1[1] - plane2.GetOrigin()));
				float dis6 = n2.DotProduct((tri1[2] - plane2.GetOrigin()));

				float d4d5 = dis4*dis5;
				float d4d6 = dis4*dis6;
				if (d4d5 > 0 && d4d6 > 0){
					//all point above or below plane
					return false;
				}
				///5.get the intersect line of two plane
				DLine<T> l;
				bool isinter = PlaneWithPlane(plane1, plane2, l);
				assert(isinter);
				///6.project triangle to intersect line
				DDirection<T> d = l.Direction();
				DPoint<T> pl = l.GetSourcePoint();

				T t1 = d.DotProduct(tri1[0] - pl);
				T t2 = d.DotProduct(tri1[1] - pl);
				T t3 = d.DotProduct(tri1[2] - pl);

				T s1 = d.DotProduct(tri2[0] - pl);
				T s2 = d.DotProduct(tri2[1] - pl);
				T s3 = d.DotProduct(tri2[2] - pl);

				T tmin = std::min(t1, std::min(t2, t3));
				T tmax = std::max(t1, std::max(t2, t3));

				T smin = std::min(s1, std::min(s2, s3));
				T smax = std::max(s1, std::max(s2, s3));

				if (tmax < smin || smax < tmin){
					return false;
				}

				return true;
			}

			/// @brief test if ray and aabb box is intersected
			///
			/// @param ray DRay  of type T
			/// @param box DBox  of type T.
			/// @return a bool value that whether ray and aabb is intersected
			template<class T>
			static bool RayWithAABB(DRay<T > &ray,DBox<T> &box){
				///origin point of ray
				DPoint<T > mOrigin = ray.GetSourcePoint();
				/// direction of ray
				DDirection<T> mDir = ray.Direction();
				/// make abs of direction
				DDirection<T> mFDir = DDirection<T>(std::abs(mDir.x()), std::abs(mDir.y()), std::abs(mDir.z()));
				///box center and extent
				DPoint<T > center  = box.GetCenter();
				DPoint<T > extents = box.GetExtents();

				T Dx = mOrigin.x() - center.x();	if (std::abs(Dx) > extents.x() && Dx*mDir.x() >= static_cast<T>(0))	return false;
				T Dy = mOrigin.y() - center.y();	if (std::abs(Dy) > extents.y() && Dy*mDir.y() >= static_cast<T>(0))	return false;
				T Dz = mOrigin.z() - center.z();	if (std::abs(Dz) > extents.z() && Dz*mDir.z() >= static_cast<T>(0))	return false;

				T f;
				f = mDir.y() * Dz - mDir.z() * Dy;		if (std::abs(f) > extents.y()*mFDir.z() + extents.z()*mFDir.y())	return false;
				f = mDir.z() * Dx - mDir.x() * Dz;		if (std::abs(f) > extents.x()*mFDir.z() + extents.z()*mFDir.x())	return false;
				f = mDir.x() * Dy - mDir.y() * Dx;		if (std::abs(f) > extents.x()*mFDir.y() + extents.y()*mFDir.x())	return false;

				return true;
			}
	};
};

#endif