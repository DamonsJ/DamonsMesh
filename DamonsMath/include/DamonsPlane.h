#ifndef _DAMONS_PLANE_H_
#define _DAMONS_PLANE_H_

#include "DamonsObject.h"

namespace DGraphic {

	template<class T>
	class DDirection;

	///class DPlane
	///@breif plane with a point on it and a normal(orthogonal to plane)
	///       plane is presented as n*(P-O) = 0
	///@tparam T type of Point.
	template<class T = double>
	class DPlane : public DObject 
	{
	public:
		/// @brief Create an uninitialized Plane.
		inline DPlane() {

		}

		/// @brief Create a Plane with another.
		///
		/// @param plane plane need to copy from
 		inline DPlane(const DPlane &plane) {
			origin_ = plane.origin_;
			normal_ = plane.normal_;
		}

		/// @brief Create a Plane defined by ax + by + cz + d = 0.
		///
		/// @param a : the a in equation ax + by +cz +d = 0
		/// @param b : the b in equation ax + by +cz +d = 0
		/// @param c : the c in equation ax + by +cz +d = 0
		/// @param d : the d in equation ax + by +cz +d = 0

		inline DPlane(const T &a, const T &b, const T &c, const T &d) {
			DPoint<T> t(a,b,c);
			normal_ = t.Normalize();
			origin_ = (-d)*normal_;
		}

		/// @brief Create a Plane defined by ax + by + cz + d = 0.
		///		   different type of a,b,c,d
		///
		/// @param a : the a in equation ax + by +cz +d = 0
		/// @param b : the b in equation ax + by +cz +d = 0
		/// @param c : the c in equation ax + by +cz +d = 0
		/// @param d : the d in equation ax + by +cz +d = 0

		template<typename T1, typename T2, typename T3, typename T4>
		inline DPlane(const T1 &a, const T2 &b, const T3 &c, const T4 &d) {
			DPoint<T> t(a, b, c);
			normal_ = t.Normalize();
			T df = static_cast<T>(-d);
			origin_ = df*normal_;
		}

		/// @brief Create a Plane with a point and a normal.
		///
		/// @note the normal is orthogonal to plane
		///
		/// @param p the  point on plane
		/// @param n the  normal of plane
		inline DPlane(const DPoint<T> &p, const DDirection<T> &n) {
			origin_ = p;
			normal_ = DPoint<T>(n.x(), n.y(), n.z());
		}

		/// @brief Create a Plane with a point and a vector.
		///
		/// @note the vector is orthogonal to plane
		///
		/// @param p the  point on plane
		/// @param v the  vector that is orthogonal to plane
		inline DPlane(const DPoint<T> &p, const DVector<T,3> &v) {
			origin_ = p;
			normal_ = DPoint<T>(v.Normalized());
		}

		~DPlane() {

		}
	public:

		/// @breif compare if two plane is equal
		/// two plane is equal if they have no intersection
		///
		/// @param the plane need to compare with
		/// @return true if they are equal otherwise false

		inline bool operator == (const DPlane &plane) {
			DPoint<T> cp = normal_.CrossProduct(plane.normal_);
			if (std::abs(cp[0]) > DEplision || std::abs(cp[1]) > DEplision || std::abs(cp[2]) > DEplision) {
				return false;
			}
			///warning:
			/*
			对象引用前加const报错:不能将“this”指针从“const a”转换为“a &”
			const a &aa相当于一个const对象，由于const对象在调用成员函数的时候，会将this指针强行转换为const this，
			所以它将无法找到相应的函数，并且编译器也无法将一个const的对象转化为一个普通对象来调用这个普通方法，所以就会产生如题的编译错误。
			*/
			T d1 = normal_.DotProduct(origin_);
			DPoint<T> normp = plane.normal_;
			DPoint<T> origp = plane.origin_;
			T d2 = normp.DotProduct(origp);
			if (std::abs(d1 - d2) > DEplision) {
				return false;
			}

			return true;
		}

		/// @breif compare if two plane is equal
		/// two plane is equal if they have no intersection
		///
		/// @param the plane need to compare with
		/// @return true if they are not equal otherwise false
		inline bool operator != (const DPlane &plane) {
			return !((*this) == plane);
		}

		/// @breif project p on plane
		///
		/// @detail assume the projection point is Q
		/// P = Q + r*normal->P*normal = (Q + r*normal)*normal
		/// r = P*normal - Q*normal -> Q*normal = normal*origin
		/// Q = P - r*normal->Q = P-(P*normal - normal*origin)*normal
		///
		/// @param p the point need to project
		/// @return  projection point

		inline DPoint<T> Projection(const DPoint<T> &p) {

			T s1 = normal_.DotProduct(p);
			T s2 = normal_.DotProduct(origin_);
			T s =  s1 - s2;
			return (p - s*normal_);
		}

		/// @brief opposite the normal of this plane .
		///
		/// @returns a plane with normal opposite.

		inline DPlane<T> Opposite() const {
			return DPlane<T>(origin_, -normal_);
		}

		/// @brief get the normal of this plane .
		///
		/// @returns the normal of plane .

		inline DDirection<T> GetDirection() const {
			return DDirection<T>(normal_[0], normal_[1], normal_[2]);
		}

		/// @brief get the origin point of this plane .
		///
		/// @returns the origin point of plane .
		inline DPoint<T> GetOrigin() const {
			return origin_;
		}
		/// @brief calculate is the given point on the plane
		///
		/// @param the point need to test
		/// @returns a bool value that is point on plane
		inline bool Has_on(const DPoint< T > &p) {
			
			T s = normal_.DotProduct(p - origin_);
			
			return (std::abs(static_cast<T>(s)) < DEplision);
		}

		/// @brief calculate if the given point lie on the same side with plane normal
		///
		/// @param the point need to test
		/// @returns a bool value 
		inline bool IsPositiveSide(const DPoint< T > &p) {

			T s = normal_.DotProduct(p - origin_);

			return (static_cast<T>(s) > -DEplision);
		}


		///  @brief calculate if the given point lie on the opposite side with plane normal
		///
		/// @param the point need to test
		/// @returns a bool value
		inline bool IsNegativeSide(const DPoint< T > &p) {

			T s = normal_.DotProduct(p - origin_);

			return (static_cast<T>(s) < DEplision);
		}


	public:
		///origin point of plane
		DPoint<T> origin_;
		///normal of plane
		DPoint<T> normal_;
	};

};
#endif // !_DAMONS_PLANE_H_

