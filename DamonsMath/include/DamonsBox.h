#ifndef _DAMONS_BOX_H_
#define _DAMONS_BOX_H_

#include "DamonsObject.h"
#include <numeric>

namespace DGraphic {

	template<class T>
	class DPoint;
	/// class DBox
	/// @breif creat a AABB box
	///
	/// DBox contain a min point and a max point of AABB Box
	/// @tparam T type of box data.

	template<class T = double>
	class DBox : public DObject
	{
	public:
		/// @brief Create an uninitialized Box.
		/// make min point infinite large as FLT_MAX
		/// and max point infinite small as -(FLT_MAX-1)
		DBox() {
			minPoint_ = DPoint<T>(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
			maxPoint_ = DPoint<T>(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest());
		}

		/// @brief create an aabb box using min and max point of box
		///
		/// @param minPt min point of aabb box
		/// @param maxPt max point of aabb box
		inline DBox(const  DPoint<T> &minPt, const  DPoint<T> &maxPt){
			SetMinMax(minPt, maxPt);
		}

		/// @brief create an aabb box using another box
		///
		/// @param box the box need to copy
		inline DBox(const DBox<T> &box) {
			minPoint_ = box.minPoint_;
			maxPoint_ = box.maxPoint_;
		}


		~DBox() {

		}
	public:
		/// @brief set the min and max point of an aabb box
		///
		/// @param minPt min point of aabb box
		/// @param maxPt max point of aabb box
		void SetMinMax(const DPoint<T> &minPt, const DPoint<T> &maxPt) {
			minPoint_ = minPt;
			maxPoint_ = maxPt;
		}
		/// @brief set up an AABB from center & extents 
		///		   center = (min + max)*0.5f extents = (max - min)*0.5f
		/// @param e extent of aabb box
		/// @param c center of aabb box
		void SetExtentCenter(const DPoint<T> &e, const DPoint<T> &c) {
			minPoint_ = c - e;
			maxPoint_ = c + e;
		}
		/// @brief extend aabb box using point  
		///		   
		/// @param p point need to extend
		void ExtendBox(const  DPoint<T>& p)
		{
			if (p.x() > maxPoint_.x())	maxPoint_.x() = p.x();
			if (p.x() < minPoint_.x())	minPoint_.x() = p.x();

			if (p.y() > maxPoint_.y())	maxPoint_.y() = p.y();
			if (p.y() < minPoint_.y())	minPoint_.y() = p.y();

			if (p.z() > maxPoint_.z())	maxPoint_.z() = p.z();
			if (p.z() < minPoint_.z())	minPoint_.z() = p.z();
		}
		/// @brief test if box intersect  
		///		   
		/// @return true if box intersect otherwise false
		bool IsIntersect(const DBox<T> &box) const {

			if (       maxPoint_.x() < box.minPoint_.x()
				|| box.maxPoint_.x() < minPoint_.x()
				||	   maxPoint_.y() < box.minPoint_.y()
				|| box.maxPoint_.y() < minPoint_.y()
				||	   maxPoint_.z() < box.minPoint_.z()
				|| box.maxPoint_.z() < minPoint_.z())	
				
				return false;

			return true;
		}
		/// @brief test if box intersect  in axis(0,1,2)
		///		   
		/// @return true if box intersect otherwise false
		bool IsIntersect(const DBox<T> &box,int axis) const {
			if (maxPoint_[axis] < box.minPoint_[axis] || box.maxPoint_[axis] < minPoint_[axis])
				return false;
			return true;
		}

		///@breif compare if two box is equal
		/// two boxs is equal if the min point and max point is equal
		///
		///@param the box need to compare with
		///@return true if they are equal otherwise false

		inline bool operator ==(const  DBox<T>  &r) {

			return (maxPoint_ == r.maxPoint_) && (minPoint_ == r.minPoint_);
		}

		///@breif compare if two box is not equal
		/// two boxs is equal if the min point and max point is equal
		///
		///@param the box need to compare with
		///@return true if they are not equal otherwise false
		inline bool operator !=(const  DBox<T>  &r) {
			return !(*this == r);
		}

		///@breif add box with another box
		///
		///@param r the box need to add 
		///@return  a new box containing result
		inline  typename DBox<T>  operator +(const DBox<T> &r) {
			DBox<T> box;
			box += *this;
			box += r;

			return box;
		}
		///@breif add box in place
		///
		///@param r the box need to add 
		///@return this box 
		inline typename  DBox<T> & operator +=(const DBox<T> &r) {
			this->ExtendBox(r.maxPoint_);
			this->ExtendBox(r.minPoint_);

			return *this;
		}
		///@breif move this box 
		///
		///@param p move distance 
		///@return this box 
		inline typename  DBox<T> & operator +=(const DPoint<T> &p) {
			maxPoint_ += p;
			minPoint_ += p;

			return *this;
		}
	public:
		/// @brief Get min point of the box
		/// @return the min point of the box
		inline		typename 	DPoint<T>		GetMin()						const { return minPoint_; }
		/// @brief Get max point of the box
		/// @return the max point of the box
		inline		typename 	DPoint<T>		GetMax()						const { return maxPoint_; }

		/// @brief Get component of the box's min point along a given axis
		/// axis is one of  0,1,2
		/// @return the min value of given axis
		inline			T		GetMin(int axis)						const { return minPoint_[axis]; }
		/// @brief Get component of the box's max point along a given axis
		/// axis is one of  0,1,2
		/// @return the max value of given axis
		inline			T		GetMax(int axis)						const { return maxPoint_[axis]; }
		/// @brief Get box center
		/// @return the box center
		inline		typename 	DPoint<T>		GetCenter()						const { return (maxPoint_ + minPoint_)*0.5f; }
		/// @brief Get box extents
		/// @return the box extents
		inline		typename 	DPoint<T>		GetExtents()					const { return (maxPoint_ - minPoint_)*0.5f; }
		/// @brief Get box width
		/// @return the box width
		inline			T 		GetWidth()								const { return maxPoint_.x() - minPoint_.x(); }
		/// @brief Get box Height
		/// @return the box Height
		inline			T 		GetHeight()								const { return maxPoint_.y() - minPoint_.y(); }
		/// @brief Get box Depth
		/// @return the box Depth
		inline			T 		GetDepth()								const { return maxPoint_.z() - minPoint_.z(); }
						
		/// @brief Get box volume
		/// @return the box volume	 
		inline			T 		GetVolume()								const { return GetWidth() * GetHeight() * GetDepth(); }
	public:
		///min point of aabb box
		DPoint<T> minPoint_;
		///max point of aabb box
		DPoint<T> maxPoint_;
	};

};
#endif
