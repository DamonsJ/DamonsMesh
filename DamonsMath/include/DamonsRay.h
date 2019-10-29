#ifndef _DAMONS_RAY_H_
#define _DAMONS_RAY_H_

#include "DamonsObject.h"
#include "DamonsPoint.h"

namespace DGraphic {

	template<class T>
	class DDirection;

	/// class DRay
	/// @breif ray with a source point and direction
	///
	/// DRay stores a source point and a direction
	/// @tparam T type of ray data.

	template<class T = double>
	class DRay : public DObject
	{
	public:
		/// @brief Create an uninitialized Ray.
		DRay() {}

		/// @brief create a ray with source point and direction
		///
		/// @param p source point of ray
		/// @param dir direction of ray
		inline DRay(DPoint<T> &p, DDirection<T> &dir) {
			spoint_    = p;
			direction_ = dir;
		}
		/// @brief create a ray with source point and pass though q
		///
		/// @param p source point of ray
		/// @param q the point that ray passing though
		inline DRay(DPoint<T> &p, DPoint<T> &q) {
			spoint_ = p;
			direction_ = DDirection<T >(p,q);
		}

		~DRay() {

		}

	public:
		/// @brief get the direction of this ray
		///
		/// @return the direction of this ray
		inline DDirection<T> Direction() const {
			return direction_;
		}
		/// @brief get the source point of ray
		///
		/// @return the source point of the ray.
		inline  DPoint< T > GetSourcePoint() const {
			return spoint_;
		}

		/// @brief opposite the direction of ray .
		///
		/// @returns a ray with direction opposite..
		inline DRay<T> Opposite() const {
			return DRay<T>(spoint_, -direction_);
		}
		/// @brief calculate is the given point on the ray
		///
		/// @param the point need to test
		/// @returns a bool value that is point on ray
		inline bool Has_on(const DPoint< T > &p) const {

			//first check if they are colinear
			DPoint< T > s = (p - spoint_).CrossProduct(direction_);
			if (std::fabs(s[0]) > DEplision || std::fabs(s[1]) > DEplision || std::fabs(s[2]) > DEplision) {
				return false;
			}
			//secont check if the point on the ray
			T t = (p - spoint_).DotProduct(direction_);

			return ( t > static_cast<T>(-0.000001));
		}

		///@breif compare if two ray is equal
		/// two rays is equal if the source point and direction is equal
		///
		///@param the ray need to compare with
		///@return true if they are equal otherwise false

		inline bool operator ==(const DRay<T> &r) {

			return (spoint_ == r.spoint_) && (direction_ == r.direction_);
		}

		///@breif compare if two ray is not equal
		/// two rays is equal if the source point and direction is equal
		///
		///@param the ray need to compare with
		///@return true if they are not equal otherwise false
		inline bool operator !=(const DRay<T> &r) {
			return !(*this == r);
		}

	public:
		///source point of ray
		DPoint<T >  spoint_;
		///direction of ray
		DDirection<T >  direction_;
	};
 
};

#endif