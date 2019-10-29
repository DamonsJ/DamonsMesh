#ifndef _DAMONS_LINE_H_
#define _DAMONS_LINE_H_

#include "DamonsObject.h"
#include "DamonsPoint.h"
#include "DamonsSegment.h"
#include "DamonsRay.h"

namespace DGraphic {

	template<class T>
	class DDirection;


	///class DLine
	///@breif a directed straight line in the three-dimensional Euclidean space 
	///
	/// DLine stores a direction and a point
	/// @tparam T type of Line.

	template< class T = double>
	class DLine : public DObject
	{
	public:
		/// @brief Create an uninitialized Line.
		inline DLine() {
		}

		/// @brief create a line with two point
		///
		/// @param p1 first point of line
		/// @param p2 second point of line

		DLine(const DPoint< T > &p1, const DPoint< T > &p2) {
			spoint_ = p1;
			direction_ = DDirection<T >(p1, p2);
		}

		/// @brief create a line with a point and a direction
		///
		/// @param p point of the line passed
		/// @param dir direction of line

		DLine(const DPoint< T > &p, const DDirection< T > &dir) {
			spoint_ = p;
			direction_ = dir;
		}
	
		/// @brief create a line with a segment
		///
		/// @param sg the segment

		DLine(const DSegment<T> &sg) {
			spoint_ = sg.GetStartPoint();
			direction_ = sg.Direction();
		}

		/// @brief create a line with a ray
		///
		/// @param r the ray

		DLine(const DRay<T> &r) {
			spoint_ = r.GetSourcePoint();
			direction_ = r.Direction();
		}

		~DLine() {
		}
	public:
		/// @brief get the direction of this line, normlized
		///
		/// @return the direction of this line
		inline DDirection<T> Direction() const {
			return direction_;
		}
		/// @brief get the source point of line
		///
		/// @return the source point of the line.
		inline  DPoint< T > GetSourcePoint() const {
			return spoint_;
		}

		/// @brief get a point on line with a paramter t
		/// @param t distance away from origin point 
		/// @return the result point of the line.
		inline  DPoint< T > GetPointOnLine(T t) const {
			DPoint<T > res = spoint_ + DPoint<T>(t*direction_.x(), t*direction_.y(), t*direction_.z());
			return res;
		}

		///@breif compare if two line is equal
		/// two line is equal  if they have a non empty intersection and the same direction. 
		///
		///@param the line need to compare with
		///@return true if they are equal otherwise false

		inline bool operator ==(const DLine<T> & l) {
			return (Has_on(l.GetSourcePoint())) && (direction_ == l.Direction());
		}

		///@breif compare if two line is not equal
		/// two line is equal if they have a non empty intersection and the same direction. 
		///
		///@param the line need to compare with
		///@return true if they are not equal otherwise false

		inline bool operator !=(const DLine<T> & l) {
			return !(*this == l);
		}

		/// @brief convert the line direction to vector.
		///
		/// @return the vector equal with line.

		inline DVector<T, 3> to_vector() const {
			return DVector<T, 3>(direction_.x(), direction_.y(), direction_.z());
		}
		/// @brief opposite the direction of this line .
		///
		/// @returns a line with direction converted
		inline DLine<T> Opposite() const {

			return DLine<T>(spoint_, -direction_);
		}
		/// @brief calculate is the given point on the line
		///
		/// @param the point need to test
		/// @returns a bool value that is point on line
		inline bool Has_on(const DPoint< T > &p) const {
			//check if they are colinear
			DPoint<T> dir(direction_.x(), direction_.y(), direction_.z());
			DPoint< T > s = (p - spoint_).CrossProduct(dir);
			if (std::abs(s[0]) > DEplision || std::abs(s[1]) > DEplision || std::abs(s[2]) > DEplision) {
				return false;
			}
			
			return true;
		}

		/// @breif project p on line
		///
		/// @param p the point need to project
		/// @return  projection point

		inline  DPoint<T> Projection(const DPoint<T> &p) const {

			DPoint<T> norm(direction_.x(), direction_.y(), direction_.z());
			T s1 = norm.DotProduct(p);
			T s2 = norm.DotProduct(spoint_);
			T s = s1 - s2;
			return (spoint_ + s*norm);
		}

		/// @brief get string of line data seperate by space.
		///  line data must be basic type like :float int double
		///
		/// @return string of line data.
		inline std::string ToString() const {
			std::string str;
			str = "first point data: \n";
			str += spoint_.ToString();

			str += "\nsecond point data: \n";
			str += direction_.ToString();

			return str;
		}

		/// @brief get wstring of line data seperate by space.
		///  line data must be basic type like :float int double
		///
		/// @return wstring of line data.
		inline std::wstring ToWString() const {
			std::wstring str;
			str = L" point data: \n";
			str += spoint_.ToWString();

			str += L"\ndirection data: \n";
			str += direction_.ToWString();

			return str;
		}
	public:
		///source point of line
		DPoint<T >  spoint_;
		///direction of line
		DDirection<T >  direction_;
	};
};

#endif
