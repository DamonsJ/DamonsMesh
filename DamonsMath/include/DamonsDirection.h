#ifndef _DAMONS_DIRECTION_H_
#define _DAMONS_DIRECTION_H_

#include "DamonsObject.h"
#include "DamonsPoint.h"
#include "DamonsSegment.h"
#include "DamonsRay.h"
#include "DamonsLine.h"

namespace DGraphic {

	/// class DDirection
	/// @breif direction with 3 normalized elements with type T

	/// DDirection stores <b>3</b> elements of data of type T
	/// @tparam T type of data.

	template<class T = double>
	class DDirection :public DObject
	{
	public:
		/// @brief Create an uninitialized DDirection.

		DDirection() {

		}

		/// @brief create a direction with a DSegment

		/// @param l DSegment 
		DDirection(DSegment<T> &l) {
			DVector<T, 3> d = l.to_vector();
			dir_ = DPoint<T>(d.Normalized());
		}

		/// @brief create a direction with a DLine

		/// @param l DLine 
		DDirection(DLine<T> &l) {
			DVector<T, 3> d = l.to_vector();
			dir_ = DPoint<T>(d.Normalized());
		}

		/// @brief create a direction with a Ray

		/// @param l DRay 
		DDirection(DRay<T> &l) {
			DDirection<T> d = l.Direction();
			dir_.x() = d.x();
			dir_.y() = d.y();
			dir_.z() = d.z();
		}

		/// @brief create a direction with another direction

		/// @param l direction 
		DDirection(const DDirection<T> &l) {
			dir_ = l.dir_;
		}

		/// @brief create a direction from two point

		/// @param p1 start point of direction
		/// @param p2 end   point of direction
		DDirection(const DPoint<T> &p1, const DPoint<T> &p2) {
			dir_ = p2 - p1;
			dir_.Normalize();
		}

		/// @brief create a direction from a vector

		/// @param v  vector need to create direction
		DDirection(DVector<T, 3> &v) {
			DVector<T, 3> d = v.Normalized();
			dir_ = DPoint<T>(d);
		}

		/// @brief Create a direction from three values.
		///
		/// @param s1 Scalar value for the first element of the direction.
		/// @param s2 Scalar value for the second element of the direction.
		/// @param s3 Scalar value for the third element of the direction.
		inline DDirection(const T& s1, const T& s2, const T& s3) {
			dir_ = DPoint<T>(s1, s2, s3);
			dir_.Normalize();
		}

		/// @brief Create a direction from three diffent type values.
		///
		/// @note This method convert T1,T2,T3 to Type T,using static_cast
		///
		/// @param s1 of Type T1 Scalar value for the first element of the point.
		/// @param s2 of Type T2 Scalar value for the second element of the point.
		/// @param s3 of Type T3 Scalar value for the third element of the point.
		template<typename T1, typename T2, typename T3>
		inline DDirection(const T1& s1, const T2& s2, const T3& s3) {
			dir_ = DPoint<T>(s1, s2, s3);
			dir_.Normalize();
		}

		~DDirection() {

		}

	public:

		/// @brief get the first value of direction.
		///
		/// @return A reference to the first data that can be modified by the caller.

		inline  T& x() {
			return dir_.x();
		}
		/// @brief get the second value of direction.
		///
		/// @return A reference to the second data that can be modified by the caller..
		inline  T& y() {
			return dir_.y();
		}
		/// @brief get the third value of direction.
		///
		/// @return A reference to the third data that can be modified by the caller.
		inline  T& z() {
			return dir_.z();
		}

		/// @brief get the first value of direction.
		///
		/// @return the first value of direction.

		inline const T& x() const {
			return dir_.x();
		}
		/// @brief get the second value of direction.
		///
		/// @return the second value of direction.
		inline const T& y() const {
			return dir_.y();
		}
		/// @brief get the third value of direction.
		///
		/// @return the third value of direction.
		inline const T& z() const {
			return dir_.z();
		}

		/// @brief Calculate the dot product of point and direction
		///
		/// @param v1  point .
		/// @return The dot product of this and v1.

		inline T DotProduct(const DPoint<T>& v1) {
			return (v1[0] * dir_[0] + v1[1] * dir_[1] + v1[2] * dir_[2]);
		}

		/// @brief Calculate the dot product of two direction,in-place.
		///
		/// @param v1 First direction.
		/// @return The dot product of this and v1.

		inline T DotProduct(const DDirection<T>& v1) {
			return (v1[0] * dir_[0] + v1[1] * dir_[1] + v1[2] * dir_[2]);
		}
		/// @brief Calculate the cross product of two directions.
		///
		/// @param v second direction.
		/// @return The cross product of this and v.
		inline  DDirection<T> CrossProduct(const DDirection<T>& v) {

			return DDirection<T>(dir_[1] * v[2] - dir_[2] * v[1],
								 dir_[2] * v[0] - dir_[0] * v[2],
								 dir_[0] * v[1] - dir_[1] * v[0]);
		}

		/// @brief Calculate the cross product of direction and vector.
		///
		/// @param v vector.
		/// @return The cross product of this and v.
		inline  DPoint<T> CrossProduct(const DPoint<T>& v) {

			return DPoint<T>(dir_[1] * v[2] - dir_[2] * v[1],
				dir_[2] * v[0] - dir_[0] * v[2],
				dir_[0] * v[1] - dir_[1] * v[0]);
		}

		///@breif compare if two direction is equal
		/// direction is equal if their elements is equal
		///
		///@param the direction need to compare with
		///@return true if they are equal otherwise false

		inline bool operator ==(const DDirection<T> & l) const {
			return (l.dir_ == dir_);
		}

		///@breif compare if two direction is not equal
		/// direction is equal if their elements is equal
		///
		///@param the direction need to compare with
		///@return true if they are not equal otherwise false

		inline bool operator !=(const DDirection<T> & l) const {
			return !(*this == l);
		}

		/// @brief Access values of a direction.
		///
		/// @param i Index of the element to access.
		/// @return  the accessed data 
		inline const T& operator[](const int i) const {
			assert(i < 3);
			return dir_[i];
		}

		/// @brief Negate all elements of the Direction.
		///
		/// @return A new Direction containing the result.
		inline DDirection<T> operator-() const {
			return DDirection<T>(-dir_[0], -dir_[1], -dir_[2]);
		}

		/// @brief convert the direction to vector.
		///
		/// @return the vector equal with direction.

		inline DVector<T, 3> to_vector() const {
			return DVector<T, 3>(dir_[0], dir_[1], dir_[2]);
		}

		/// @brief get string of direction data seperate by space.
		///  direction data must be basic type like :float int double
		///
		/// @return string of direction data.
		inline std::string ToString() const {
			return dir_.ToString();
		}

		/// @brief get wstring of direction data seperate by space.
		///  direction data must be basic type like :float int double
		///
		/// @return wstring of direction data.
		inline std::wstring ToWString() const {
			return dir_.ToWString();
		}
	public:
		///direction data
		DPoint<T> dir_;
	};

};

#endif
