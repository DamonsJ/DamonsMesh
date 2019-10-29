#ifndef _DAMONS_POINT_H_
#define _DAMONS_POINT_H_

#include "DamonsObject.h"
#include "DamonsVector.h"

using namespace DMath;
namespace DGraphic {

	///class DPoint
	///@breif Point of 3 dimension with Type T
	///
	/// Point stores <b>3</b> elements of type <b>T</b> and provides a set
	/// functions to perform operations on the set of elements.
	///
	/// @tparam T type of Point elements.
	template< class T = double>
	class DPoint: public DVector<T,3>,public DObject 
	{
		//add this for gcc compiler 
		//vs compiler not need this statement
		using DVector<T,3>::data_;
	public:
		/// @brief Create an uninitialized Point.
		inline DPoint():DVector<T,3>(){}

		/// @brief Create a Point from another Point copying each element.
		///
		/// @param p Point that the data will be copied from.
		inline DPoint(const DPoint<T>& p):DVector<T,3>(p) {
			
		}

		/// @brief Create a vector from a single float.
		///
		/// Each elements is set to be equal to the value given.
		/// @param s Scalar value that the vector will be initialized to.
		explicit inline DPoint(const T& s) :DVector<T, 3>(s) {
		}

		/// @brief Create a point form the first 3 elements of an array.
		///
		/// @param a Array of values that the vector will be iniitlized to.
		explicit inline DPoint(const T* a) :DVector<T, 3>(a) {
			
		}

		/// @brief Create a point from a vector of 3 dimension.
		///
		/// Each elements is set to be equal to the vector given.
		/// @param v   the vector will be copyed from.
		explicit inline DPoint(const DVector<T, 3> &v) {
			data_[0] = v[0];
			data_[1] = v[1];
			data_[2] = v[2];
		}

		/// @brief Create a point from another point of a different type.
		///
		/// This copies each element of a point which makes it possible to between
		/// point of different types, for example
		/// <code>float/double/int</code> vectors.
		/// @param v point that the data will be copied from.
		/// @tparam U type of point  to copy.
		template <typename U>
		explicit inline DPoint(const DPoint<U>& v):DVector<T,3>(v) {
		}

		/// @brief Create a point from two values.
		///
		/// @note This method only works when the point is of dimension two.
		///
		/// @param s1 Scalar value for the first element of the point.
		/// @param s2 Scalar value for the second element of the point.
		inline DPoint(const T& s1, const T& s2):DVector<T,3>(s1,s2) {
		}

		/// @brief Create a point from three values.
		///
		/// @note This method only works when the point is of dimension three.
		///
		/// @param s1 Scalar value for the first element of the point.
		/// @param s2 Scalar value for the second element of the point.
		/// @param s3 Scalar value for the third element of the point.
		inline DPoint(const T& s1, const T& s2, const T& s3):DVector<T, 3>(s1, s2,s3) {

		}
		/// @brief Create a point from three diffent type values.
		///
		/// @note This method only works when the point is of dimension three.
		///
		/// @param s1 of Type T1 Scalar value for the first element of the point.
		/// @param s2 of Type T2 Scalar value for the second element of the point.
		/// @param s3 of Type T3 Scalar value for the third element of the point.
		template<typename T1, typename T2, typename T3>
		inline DPoint(const T1& s1, const T2& s2, const T3& s3) :DVector<T, 3>(s1,s2,s3) {

		}
		/// @brief get the first value of point.
		///
		/// @return the first value of point.

		inline const T& x() const {
			return data_[0];
		}
		/// @brief get the second value of point.
		///
		/// @return the second value of point.
		inline const T& y() const {
			return data_[1];
		}
		/// @brief get the third value of point.
		///
		/// @return the third value of point.
		inline const T& z() const{
			return data_[2];
		}

		/// @brief get the first value of point.
		///
		/// @return A reference to the first data that can be modified by the caller.

		inline  T& x() {
			return data_[0];
		}
		/// @brief get the second value of point.
		///
		/// @return A reference to the second data that can be modified by the caller..
		inline  T& y() {
			return data_[1];
		}
		/// @brief get the third value of point.
		///
		/// @return A reference to the third data that can be modified by the caller.
		inline  T& z() {
			return data_[2];
		}

		/// @brief Calculate the normalized version of this point.
		///
		/// @return The normalized point.
		inline DPoint<T> Normalized() const { 
			
			DVector<T, 3> cv = NormalizedHelper(*this);
			DPoint<T> p(cv);

			return p;
		}

		/// @brief Calculate the dot product of two points,in-place.
		///
		/// @param v1 First point.
		/// @return The dot product of this and v1.

		inline T DotProduct(const DPoint<T>& v1) {
			return DotProductHelper(*this, v1);
		}
		/// @brief Calculate the cross product of two points.
		///
		/// @param v second point.
		/// @return The cross product of this and v.
		inline  DPoint<T> CrossProduct(const DPoint<T>& v) {

			return DPoint<T>(data_[1] * v[2] - data_[2] * v[1],
							 data_[2] * v[0] - data_[0] * v[2],
							 data_[0] * v[1] - data_[1] * v[0]);
		}

		/// @brief Calculate the distance of two points.
		///
		/// @param v another point.
		/// @return The distance of this and v.
		inline  T Distance(const DPoint<T>& v) const{

			return (*this - v).Length();
		}

		/// @brief Linearly interpolate two points.
		///
		/// @param v1 First point.
		/// @param v2 Second point.
		/// @param percent Percentage from v1 to v2 in range 0.0...1.0.
		/// @return The interpolated point of v1 and v2.
		static inline DPoint<T> Lerp(const DPoint<T>& v1,
			const DPoint<T>& v2, const T percent) {

			DVector<T, 3> cv = LerpHelper(v1, v2, percent);
			DPoint<T> p(cv);

			return p;
		}

		/// @brief Compare each component and returns max values.
		///
		/// @param v1 First point.
		/// @param v2 Second point.
		/// @return Max value of v1 and v2.
		static inline DPoint<T> Max(const DPoint<T>& v1,
			const DPoint<T>& v2) {

			DVector<T, 3> cv = MaxHelper(v1, v2);
			DPoint<T> p(cv);

			return p;
		}

		/// @brief Compare each component and returns min values.
		///
		/// @param v1 First point.
		/// @param v2 Second point.
		/// @return min value of v1 and v2.
		static inline DPoint<T> Min(const DPoint<T>& v1,
			const DPoint<T>& v2) {

			DVector<T, 3> cv = MinHelper(v1, v2);
			DPoint<T> p(cv);

			return p;
		}

		virtual ~DPoint() {}

	protected:

	};

	/// @brief Negate all elements of the Point.
	///
	/// @return A new Point containing the result.
	template <class T>
	inline DPoint<T> operator-(const DPoint<T>& v) {
		return DPoint<T>(-v[0],-v[1],-v[2]);
	}

	/// @brief Multiply a Point by a scalar.
	///
	/// Multiplies each component of the specified Point with a scalar.
	///
	/// @param s scalar to multiply.
	/// @param v Point to multiply.
	/// @return Point containing the result.
	template <class T>
	inline DPoint<T> operator*(const T& s, const DPoint<T>& v) {
		return DPoint<T>(s*v[0], s*v[1], s*v[2]);
	}

	/// @brief Divide a Point by a scalar.
	///
	/// Divides each component of the specified Point by a scalar.
	///
	/// @param v Point to be divided.
	/// @param s scalar to divide the vector by.
	/// @return Point containing the result.
	template <class T>
	inline DPoint<T> operator/(const DPoint<T>& v, const T& s) {
		return DPoint<T>(v[0]/s, v[1] / s, v[2] / s);
	}

	/// @brief Add a scalar to each element of a Point.
	///
	/// @param s scalar to add to each element of a Point.
	/// @param v Vector to add the scalar to.
	/// @return Point containing the result.
	template <class T>
	inline DPoint<T> operator+(const T& s, const DPoint<T>& v) {
		return DPoint<T>(s+v[0], s+v[1], s+v[2]);
	}

	/// @brief Add a scalar to all elements of a Point.
	///
	/// @param v Point for the operation.
	/// @param s A scalar to add to the Point.
	/// @return A new vector containing the result.
	template <class T>
	inline DPoint<T> operator+(const DPoint<T>& v, const T& s) {
		return DPoint<T>(s + v[0], s + v[1], s + v[2]);
	}


	/// @brief Subtract a scalar from each element of a Point.
	///
	/// @param s scalar to subtract from each element of a Point.
	/// @param v Point to subtract the scalar from.
	/// @return Point containing the result.
	 
	template <class T>
	inline DPoint<T> operator-(const T& s, const DPoint<T>& v) {
		return DPoint<T>(s - v[0], s - v[1], s - v[2]);
	}

	/// @brief Subtract a scalar from all elements of a Point.
	///
	/// @param v Point for the operation.
	/// @param s A scalar to subtract from a Point.
	/// @return A new Point that stores the result.
	template <class T>
	inline DPoint<T> operator-(const DPoint<T>& v, const T& s) {
		return DPoint<T>( v[0] - s, v[1] - s, v[2] - s);
	}

	/// @brief Multiply a Point by another Vector.
	///
	/// In line with GLSL, this performs component-wise multiplication.
	/// @param lhs First Point to use as a starting point.
	/// @param rhs Second Point to multiply by.
	/// @return A new Point containing the result.
	template <class T>
	inline DPoint<T> operator*(const DPoint<T>& lhs,const DPoint<T>& rhs) {
		return DPoint<T>(lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2]);
	}

	/// @brief Divide a Point by another Point.
	///
	/// In line with GLSL, this performs component-wise division.
	/// @param lhs First Point to use as a starting point.
	/// @param rhs Second Point to divide by.
	/// @return A new Point containing the result.
	template <class T>
	inline DPoint<T> operator/(const DPoint<T>& lhs,const DPoint<T>& rhs) {
		return DPoint<T>(lhs[0] / rhs[0], lhs[1] / rhs[1], lhs[2] / rhs[2]);
	}

	/// @brief Add a Point with another Point.
	///
	/// @param lhs First Point to use as a starting point.
	/// @param rhs Second Point to add by.
	/// @return A new Point containing the result.
	template <class T>
	inline  DPoint<T> operator+(const  DPoint<T>& lhs,const  DPoint<T>& rhs) {
		return DPoint<T>(lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]);
	}

	/// @brief subtract a Point with another Point.
	///
	/// @param lhs First Point to use as a starting point.
	/// @param rhs Second Point to subtract by.
	/// @return A new Point containing the result.
	template <class T>
	inline DPoint<T> operator-(const DPoint<T> & lhs,const DPoint<T> & rhs) {
		return DPoint<T>(lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]);
	}

	/// @brief Multiply a Point with a scalar.
	///
	/// @param v Point for the operation.
	/// @param s A scalar to multiply the Point with.
	/// @return A new Point containing the result.
	template <class T>
	inline DPoint<T> operator*(const DPoint<T>& v, const T& s) {
		return DPoint<T>(v[0]*s, v[1] * s, v[2] * s);

	}

	/// @brief Add (in-place) a Point with another Point.
	///
	/// @param lhs First Point to use as a starting point.
	/// @param rhs Second Point to add.
	/// @return A reference to the input <b>v</b> Point.
	template <class T>
	inline DPoint<T>& operator+=(DPoint<T>& lhs, const DPoint<T>& rhs) {
		int d = 3;
		DAMONSMATH_VECTOR_OPERATION(lhs.data_[i] += rhs[i]);
		return lhs;
	}

	/// @brief Subtract (in-place) another Point from a Point.
	///
	/// @param lhs First Point to use as a starting point.
	/// @param rhs Second Point to subtract by.
	/// @return A reference to the input <b>v</b> vector.
	template <class T>
	inline DPoint<T>& operator-=(DPoint<T>& lhs, const DPoint<T>& rhs) {
		int d = 3;
		DAMONSMATH_VECTOR_OPERATION(lhs.data_[i] -= rhs[i]);
		return lhs;
	}

	/// @brief Multiply (in-place) each element of a Point with a scalar.
	///
	/// @param v Point for the operation.
	/// @param s A scalar to multiply the Point with.
	/// @return A reference to the input <b>v</b> Point.
	template <class T>
	inline DPoint<T>& operator*=(DPoint<T>& v, const T& s) {
		int d = 3;
		DAMONSMATH_VECTOR_OPERATION(v.data_[i] *= s);
		return v;
	}

	/// @brief Divide (in-place) each element of a Point by a scalar.
	///
	/// @param v Point for the operation.
	/// @param s A scalar to divide the Point by.
	/// @return A reference to the input <b>v</b> Point.
	template <class T>
	inline DPoint<T>& operator/=(DPoint<T>& v, const T& s) {
		int d = 3;
		DAMONSMATH_VECTOR_OPERATION(v.data_[i] /= s);
		return v;
	}

	/// @brief Add (in-place) a scalar to each element of a Point.
	///
	/// @param v Point for the operation.
	/// @param s A scalar to add the vector to.
	/// @return A reference to the input <b>v</b> vector.
	template <class T>
	inline DPoint<T>& operator+=(DPoint<T>& v, const T& s) {
		int d = 3;
		DAMONSMATH_VECTOR_OPERATION(v.data_[i] += s);
		return v;
	}

	/// @brief Subtract (in-place) a scalar from each element of a Point.
	///
	/// @param v Point for the operation.
	/// @param s A scalar to subtract from the Point.
	/// @return A reference to the input <b>v</b> Point.
	template <class T>
	inline DPoint<T>& operator-=(DPoint<T>& v, const T& s) {
		int d = 3;
		DAMONSMATH_VECTOR_OPERATION(v.data_[i] -= s);
		return v;
	}

	/// @brief Compare 2 Vectors of the same size for equality.
	///
	/// Instead consider comparing the difference between two float vectors using
	/// LengthSquared() with an epsilon value.
	/// For example, v1.LengthSquared(v2) < epsilon.
	///
	/// @return true if the 2 vectors contains the same value, false otherwise.
	template <class T>
	inline bool operator == (const DPoint<T>& lhs, const DPoint<T>& rhs) {
		 
		T l = (lhs - rhs).LengthSquared();
		if (l < (DEplision*DEplision))
			return true;

		return false;
	}

	/// @brief Compare 2 Vectors of the same size for inequality.
	///
	/// @return true if the elements of two vectors differ, false otherwise.
	template <class T>
	inline bool operator!=(const DPoint<T>& lhs, const DPoint<T>& rhs) {
		return !(lhs == rhs);
	}
};
#endif // !_DAMONS_POINT_H_
