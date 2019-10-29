#ifndef _DAMONS_TRIANGLE_H_
#define _DAMONS_TRIANGLE_H_

#include "DamonsObject.h"

namespace DGraphic {

	template<class T>
	class DDirection;

	template<class T>
	class DBox;

	template<class T>
	class DPlane;
	/// class DTriangle
	/// @breif creat a DTriangle with three points
	///
	/// @tparam T type of triangle data.

	template<class T = double>
	class DTriangle:public DObject
	{
	public:
		/// @brief Create an uninitialized Triangle.
		DTriangle() {

		}
		/// @brief create a triangle with 3 points
		///
		/// @param p1 first point of triangle
		/// @param p2 second point of triangle
		/// @param p3 third point of triangle
		DTriangle(DPoint<T> &p1, DPoint<T> &p2, DPoint<T> &p3) {
			triangle_points_[0] = p1;
			triangle_points_[1] = p2;
			triangle_points_[2] = p3;
		}
		~DTriangle() {

		}
	public:
		///@breif compare if two triangles is equal
		/// two triangles is equal if 3 points of triangle are equal
		///
		///@param the triangle need to compare with
		///@return true if they are equal otherwise false

		inline bool operator ==(const  DTriangle<T>  &r) {

			return ((triangle_points_[0] == r.triangle_points_[0]) && 
					(triangle_points_[1] == r.triangle_points_[1]) &&
					(triangle_points_[2] == r.triangle_points_[2]));
		}

		///@breif compare if two triangles is equal
		/// two triangles is equal if 3 points of triangle are equal
		///
		///@param the triangle need to compare with
		///@return true if they are not equal otherwise false
		inline bool operator !=(const  DTriangle<T>  &r) {
			return !(*this == r);
		}
		/// @breif get the indexth point of triangle
		///
		/// @param index the point index : 0,1,2
		/// @return the point need to access
		inline const DPoint<T> operator[](int index) const {
			assert(index < 3 && index > -1);
			return triangle_points_[index];
		}
	public:
		/// @breif calculate the normal of triangle
		/// @return the triangle normal
		DDirection<T> Normal() const{
			DPoint<T> v1 = triangle_points_[1] - triangle_points_[0];
			DPoint<T> v2 = triangle_points_[2] - triangle_points_[0];

			DPoint< T > s = v1.CrossProduct(v2);

			return DDirection<T>(s.x(), s.y(), s.z());
		}

		/// @breif calculate the aabb box of triangle
		/// @return the aabb box
		DBox<T> Box() {
			DBox<T> box;
			box.ExtendBox(triangle_points_[0]);
			box.ExtendBox(triangle_points_[1]);
			box.ExtendBox(triangle_points_[2]);

			return box;
		}
		/// @breif get the plane of triangle
		/// @return the plane
		DPlane<T> Plane(){
			DDirection<T> dir = Normal();
			return DPlane<T>(triangle_points_[0],dir);
		}
		/// @breif calculate the area of triangle
		/// @return the area
		T Area() {
			DPoint<T> v1 = triangle_points_[1] - triangle_points_[0];
			DPoint<T> v2 = triangle_points_[2] - triangle_points_[0];

			DPoint<T> v = v1.CrossProduct(v2);

			return static_cast<T>(0.5*v.Length());
		}

		/// @brief calculate is the given point on the triangle
		///        on triangle means on vertex, on edge or in the facet
		/// @param the point need to test
		/// @returns a bool value that is point on triangle
		inline bool Has_on(const DPoint< T > &p) const {
			//has two methos:
			/*
			1. whether point p is lie on same side with all the three line
			2. binary center :P = (1-u-v)*p1+u*p2+v*p3 where u >= 0 v >= 0 u+v <= 1 
			*/
			//using method 2:

			DPoint< T > v0 = triangle_points_[2] - triangle_points_[0];
			DPoint< T > v1 = triangle_points_[1] - triangle_points_[0];
			DPoint< T > v2 = p - triangle_points_[0];

			T dot00 = v0.DotProduct(v0);
			T dot01 = v0.DotProduct(v1);
			T dot02 = v0.DotProduct(v2);
			T dot11 = v1.DotProduct(v1);
			T dot12 = v1.DotProduct(v2);

			double inverZero = 1.0 / (dot00 * dot11 - dot01 * dot01);
			double u = (dot11 * dot02 - dot01 * dot12) * inverZero;

			if (u < 0.0 || u > 1.0) // if u out of range, return directly
			{
				return false;
			}

			double v = (dot00 * dot12 - dot01 * dot02) * inverZero;
			if (v < 0.0 || v > 1.0) // if v out of range, return directly
			{
				return false;
			}

			return (u + v <= static_cast<T>(1.0));
		}
		/// @brief triangle is degenerate, if points are colinear.
		///
		/// @returns a bool value that is triangle degenerate
		inline bool Is_degenerate() const {
			DPoint<T> v1 = triangle_points_[1] - triangle_points_[0];
			DPoint<T> v2 = triangle_points_[2] - triangle_points_[0];

			DPoint< T > s = v1.CrossProduct(v2);
			if (std::abs(s[0]) > DEplision || std::abs(s[1]) > DEplision || std::abs(s[2]) > DEplision) {
				return false;
			}

			return true;
		}

	public:
		/// triangle points
		DPoint<T> triangle_points_[3];
	};
};

#endif