/*
* copy from google's opensource project :mathfu
* for learning
*/

#ifndef _DAMONS_MATH_H_
#define _DAMONS_MATH_H_

#include "utilities.h"
#include <assert.h>
#include <string>

namespace DMath {
	///class DVector
	///@breif Vector of d dimension with Type T
	///
	/// Vector stores <b>d</b> elements of type <b>T</b> and provides a set
	/// functions to perform operations on the set of elements.
	///
	/// @tparam T type of Vector elements.
	/// @tparam d dimensions (number of elements) in the Vector structure.
	template<class T, int d>
	class DVector {
	public:

		//////////////////////////////////////////////////////////////////////////
		////construct functions
		/// @brief Create an uninitialized Vector.
		inline DVector() {}

		/// @brief Create a vector from another vector copying each element.
		///
		/// @param v Vector that the data will be copied from.
		inline DVector(const DVector<T, d>& v) {
			DAMONSMATH_VECTOR_OPERATION(data_[i] = v.data_[i]);
		}

		/// @brief Create a vector from another vector of a different type.
		///
		/// This copies each element of a Vector which makes it possible to between
		/// vectors of different types, for example
		/// <code>float/double/int</code> vectors.
		/// @param v Vector that the data will be copied from.
		/// @tparam U type of Vector elements to copy.
		template <typename U>
		explicit inline DVector(const DVector<U, d>& v) {
			DAMONSMATH_VECTOR_OPERATION(data_[i] = static_cast<T>(v[i]));
		}

		/// @brief Create a vector from a single float.
		///
		/// Each elements is set to be equal to the value given.
		/// @param s Scalar value that the vector will be initialized to.
		explicit inline DVector(const T& s) { DAMONSMATH_VECTOR_OPERATION(data_[i] = s); }

		/// @brief Create a vector form the first d elements of an array.
		///
		/// @param a Array of values that the vector will be iniitlized to.
		explicit inline DVector(const T* a) {
			DAMONSMATH_VECTOR_OPERATION(data_[i] = a[i]);
		}

		/// @brief Create a vector from two values.
		///
		/// @note This method only works when the vector is of size two.
		///
		/// @param s1 Scalar value for the first element of the vector.
		/// @param s2 Scalar value for the second element of the vector.
		inline DVector(const T& s1, const T& s2) {
			assert(d == 2);
			data_[0] = s1;
			data_[1] = s2;
		}

		/// @brief Create a vector from three values.
		///
		/// @note This method only works when the vector is of size three.
		///
		/// @param s1 Scalar value for the first element of the vector.
		/// @param s2 Scalar value for the second element of the vector.
		/// @param s3 Scalar value for the third element of the vector.
		inline DVector(const T& s1, const T& s2, const T& s3) {
			assert(d == 3);
			data_[0] = s1;
			data_[1] = s2;
			data_[2] = s3;
		}

		/// @brief Create a vector from three diffent type values.
		///
		/// @note This method only works when the point is of dimension three.
		///
		/// @param s1 of Type T1 Scalar value for the first element of the vector.
		/// @param s2 of Type T2 Scalar value for the second element of the vector.
		/// @param s3 of Type T3 Scalar value for the third element of the vector.
		template<typename T1, typename T2, typename T3>
		inline DVector(const T1& s1, const T2& s2, const T3& s3)  {
			assert(d == 3);
			data_[0] = static_cast<T>(s1);
			data_[1] = static_cast<T>(s2);
			data_[2] = static_cast<T>(s3);
		}

		/// @brief Create a vector from a 2 component vector and a third value.
		///
		/// @note This method only works when the vector is of size three.
		///
		/// @param v12 Vector containing the first 2 values.
		/// @param s3 Scalar value for the third element of the vector.
		inline DVector(const DVector<T, 2>& v12, const T& s3) {
			assert(d == 3);
			data_[0] = v12[0];
			data_[1] = v12[1];
			data_[2] = s3;
		}

		/// @brief Create a vector from four values.
		///
		/// @note This method only works when the vector is of size four.
		///
		/// @param s1 Scalar value for the first element of the vector.
		/// @param s2 Scalar value for the second element of the vector.
		/// @param s3 Scalar value for the third element of the vector.
		/// @param s4 Scalar value for the forth element of the vector.
		inline DVector(const T& s1, const T& s2, const T& s3, const T& s4) {
			assert(d == 4);
			data_[0] = s1;
			data_[1] = s2;
			data_[2] = s3;
			data_[3] = s4;
		}

		/// @brief Create a 4-dimensional vector from a Vector<T, 3> .
		///
		/// The last element is initialized to the specified value.
		/// @note This method only works with 4 element vectors.
		///
		/// @param vector3 Vector used to initialize the first 3 elements.
		/// @param value Value used to set the last element of the vector.
		inline DVector(const DVector<T, 3>& vector3, const T& value) {
			assert(d == 4);
			data_[0] = vector3[0];
			data_[1] = vector3[1];
			data_[2] = vector3[2];
			data_[3] = value;
		}

		/// @brief Create a vector from two 2 component vectors.
		///
		/// @note This method only works when the vector is of size four.
		///
		/// @param v12 Vector containing the first 2 values.
		/// @param v34 Vector containing the last 2 values.
		inline DVector(const DVector<T, 2>& v12, const DVector<T, 2>& v34) {
			assert(d == 4);
			data_[0] = v12[0];
			data_[1] = v12[1];
			data_[2] = v34[0];
			data_[3] = v34[1];
		}

		////construct functions end
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		////operator overload

		/// @brief Access an element of the vector.
		///
		/// @param i Index of the element to access.
		/// @return A reference to the accessed data that can be modified by the
		/// caller.
		inline T& operator()(const int i) { return data_[i]; }

		/// @brief Access an element of the vector.
		///
		/// @param i Index of the element to access.
		/// @return A reference to the accessed data.
		inline const T& operator()(const int i) const { return data_[i]; }

		/// @brief Access an element of the vector.
		///
		/// @param i Index of the element to access.
		/// @return A reference to the accessed data that can be modified by the
		/// caller.
		inline T& operator[](const int i) { return data_[i]; }

		/// @brief Access an element of the vector.
		///
		/// @param i Index of the element to access.
		/// @return A const reference to the accessed.
		inline const T& operator[](const int i) const { return data_[i]; }

		////operator overload end
		//////////////////////////////////////////////////////////////////////////
		//// data access

		/// @return A reference to the first data that can be modified by the caller.

		inline  T& x() {
			assert(d >= 1);
			return data_[0];
		}
		/// @brief get the second value of point.
		///
		/// @return A reference to the second data that can be modified by the caller..
		inline  T& y() {
			assert(d >= 2);
			return data_[1];
		}
		/// @brief get the third value of point.
		///
		/// @return A reference to the third data that can be modified by the caller.
		inline  T& z() {
			assert(d >= 3);
			return data_[2];
		}

		/// @brief get the first value of point.
		///
		/// @return the first value of point.

		inline const T& x() const {
			assert(d >= 1);
			return data_[0];
		}
		/// @brief get the second value of point.
		///
		/// @return the second value of point.
		inline const T& y() const {
			assert(d >= 2);
			return data_[1];
		}
		/// @brief get the third value of point.
		///
		/// @return the third value of point.
		inline const T& z() const {
			assert(d >= 3);
			return data_[2];
		}

		/// @brief GLSL style 3 element accessor.
		///
		/// This only works with vectors that contain more than 3 elements.
		/// @returns A 3-dimensional Vector containing the first 3 elements of
		/// this Vector.
		inline DVector<T, 3> xyz() {
			assert(d >= 3);
			return DVector<T, 3>(data_[0], data_[1], data_[2]);
		}

		/// @brief GLSL style 3 element accessor.
		///
		/// This only works with vectors that contain more than 3 elements.
		/// @returns A 3-dimensional Vector containing the first 3 elements of
		/// this Vector.
		inline const DVector<T, 3> xyz() const {
			assert(d >= 3);
			return DVector<T, 3>(data_[0], data_[1], data_[2]);
		}
		/// @brief GLSL style 2 element accessor.
		///
		/// This only works with vectors that contain more than 2 elements.
		/// @returns A 2-dimensional Vector with the first 2 elements of this Vector.
		inline DVector<T, 2> xy() {
			assert(d >= 2);
			return DVector<T, 2>(data_[0], data_[1]);
		}

		/// @brief GLSL style 2 element accessor.
		///
		/// This only works with vectors that contain more than 2 elements.
		/// @returns A 2-dimensional Vector with the first 2 elements of this Vector.
		inline const DVector<T, 2> xy() const {
			assert(d >= 2);
			return DVector<T, 2>(data_[0], data_[1]);
		}

		/// @brief GLSL style 2 element accessor.
		///
		/// This only works with vectors that contain 4 elements.
		/// @returns A 2-dimensional Vector with the last 2 elements of this Vector.
		inline DVector<T, 2> zw() {
			assert(d == 4);
			return DVector<T, 2>(data_[2], data_[3]);
		}

		/// @brief GLSL style 2 element accessor.
		///
		/// This only works with vectors that contain 4 elements.
		/// @returns A 2-dimensional Vector with the last 2 elements of this Vector.
		inline const DVector<T, 2> zw() const {
			assert(d == 4);
			return DVector<T, 2>(data_[2], data_[3]);
		}
		//// data access end
		//////////////////////////////////////////////////////////////////////////
		//// data operation
		/// @brief Calculate the squared length of this vector.
		///
		/// @return The length of this vector squared.
		inline T LengthSquared() const { return LengthSquaredHelper(*this); }

		/// @brief Calculate the length of this vector.
		///
		/// @return The length of this vector.
		inline T Length() const { return LengthHelper(*this); }

		/// @brief Normalize this vector in-place.
		///
		/// @return The length of this vector.
		inline T Normalize() { return NormalizeHelper(*this); }

		/// @brief Calculate the normalized version of this vector.
		///
		/// @return The normalized vector.
		inline DVector<T, d> Normalized() const { return NormalizedHelper(*this); }

		/// @brief Calculate the dot product of two vectors.
		///
		/// @param v1 First vector.
		/// @param v2 Second vector.
		/// @return The dot product of v1 and v2.
		static inline T DotProduct(const DVector<T, d>& v1, const DVector<T, d>& v2) {
			return DotProductHelper(v1, v2);
		}

		/// @brief Calculate the hadamard or componentwise product of two vectors.
		///
		/// @param v1 First vector.
		/// @param v2 Second vector.
		/// @return The hadamard product of v1 and v2.
		static inline DVector<T, d> HadamardProduct(const DVector<T, d>& v1,
			const DVector<T, d>& v2) {
			return HadamardProductHelper(v1, v2);
		}

		/// @brief Calculate the cross product of two vectors.
		///
		/// Note that this function is only defined for 3-dimensional Vectors.
		/// @param v1 First vector.
		/// @param v2 Second vector.
		/// @return The cross product of v1 and v2.
		static inline DVector<T, 3> CrossProduct(const DVector<T, 3>& v1,
			const DVector<T, 3>& v2) {
			return CrossProductHelper(v1, v2);
		}

		/// @brief Linearly interpolate two vectors.
		///
		/// @param v1 First vector.
		/// @param v2 Second vector.
		/// @param percent Percentage from v1 to v2 in range 0.0...1.0.
		/// @return The interpolated vector of v1 and v2.
		static inline DVector<T, d> Lerp(const DVector<T, d>& v1,
			const DVector<T, d>& v2, const T percent) {
			return LerpHelper(v1, v2, percent);
		}

		/// @brief Generates a random vector.
		///
		/// The range of each component is bounded by min and max.
		/// @param min Minimum value of the vector.
		/// @param max Maximum value of the vector.
		static inline DVector<T, d> RandomInRange(const DVector<T, d>& min,
			const DVector<T, d>& max) {
			return RandomInRangeHelper(min, max);
		}

		/// @brief Compare each component and returns max values.
		///
		/// @param v1 First vector.
		/// @param v2 Second vector.
		/// @return Max value of v1 and v2.
		static inline DVector<T, d> Max(const DVector<T, d>& v1,
			const DVector<T, d>& v2) {
			return MaxHelper(v1, v2);
		}

		/// @brief Compare each component and returns min values.
		///
		/// @param v1 First vector.
		/// @param v2 Second vector.
		/// @return Min value of v1 and v2.
		static inline DVector<T, d> DMin(const DVector<T, d>& v1,
			const DVector<T, d>& v2) {
			return MinHelper(v1, v2);
		}

		/// @brief get string of vector data seperate by space.
		///  vector data must be basic type like :float int double
		///
		/// @return string of vector data.
		inline std::string ToString() const {
			std::string str = "";
			DAMONSMATH_VECTOR_OPERATION(str += (std::to_string(data_[i]) + std::string(" ")));
			str += "\n";

			return str;
		}

		/// @brief get wstring of vector data seperate by space.
		///  vector data must be basic type like :float int double
		///
		/// @return wstring of vector data.
		inline std::wstring ToWString() const {
			std::wstring str = L"";
			DAMONSMATH_VECTOR_OPERATION(str += (std::to_wstring(data_[i]) + std::wstring(L" ")));
			str += L"\n";

			return str;
		}

		DAMONS_DEFINE_CLASS_SIMD_AWARE_NEW_DELETE
		/// Elements of the vector.
		T data_[d];
	};

	//////////////////////////////////////////////////////////////////////////
	///helper function

	/// @brief Compare 2 Vectors of the same size for equality.
	///
	/// @note: The likelyhood of two float values being the same is very small.
	/// Instead consider comparing the difference between two float vectors using
	/// LengthSquared() with an epsilon value.
	/// For example, v1.LengthSquared(v2) < epsilon.
	///
	/// @return true if the 2 vectors contains the same value, false otherwise.
	template <class T, int d>
	inline bool operator==(const DVector<T, d>& lhs, const DVector<T, d>& rhs) {
		for (int i = 0; i < d; ++i) {
			if (lhs[i] != rhs[i]) return false;
		}
		return true;
	}

	/// @brief Compare 2 Vectors of the same size for inequality.
	///
	/// @return true if the elements of two vectors differ, false otherwise.
	template <class T, int d>
	inline bool operator!=(const DVector<T, d>& lhs, const DVector<T, d>& rhs) {
		return !(lhs == rhs);
	}

	/// @brief Negate all elements of the Vector.
	///
	/// @return A new Vector containing the result.
	template <class T, int d>
	inline DVector<T, d> operator-(const DVector<T, d>& v) {
		DAMONSMATH_VECTOR_OPERATOR(-v.data_[i]);
	}

	/// @brief Multiply a Vector by a scalar.
	///
	/// Multiplies each component of the specified Vector with a scalar.
	///
	/// @param s scalar to multiply.
	/// @param v Vector to multiply.
	/// @return Vector containing the result.
	/// @related Vector
	template <class T, int d>
	inline DVector<T, d> operator*(const T& s, const DVector<T, d>& v) {
		DAMONSMATH_VECTOR_OPERATOR(v.data_[i] * s);
	}

	/// @brief Divide a Vector by a scalar.
	///
	/// Divides each component of the specified Vector by a scalar.
	///
	/// @param v Vector to be divided.
	/// @param s scalar to divide the vector by.
	/// @return Vector containing the result.
	/// @related Vector
	template <class T, int d>
	inline DVector<T, d> operator/(const DVector<T, d>& v, const T& s) {
		DAMONSMATH_VECTOR_OPERATOR(v.data_[i] / s);
	}

	/// @brief Add a scalar to each element of a Vector.
	///
	/// @param s scalar to add to each element of a Vector.
	/// @param v Vector to add the scalar to.
	/// @return Vector containing the result.
	/// @related Vector
	template <class T, int d>
	inline DVector<T, d> operator+(const T& s, const DVector<T, d>& v) {
		DAMONSMATH_VECTOR_OPERATOR(v.data_[i] + s);
	}

	/// @brief Add a scalar to all elements of a vector.
	///
	/// @param v Vector for the operation.
	/// @param s A scalar to add to the vector.
	/// @return A new vector containing the result.
	template <class T, int d>
	inline DVector<T, d> operator+(const DVector<T, d>& v, const T& s) {
		DAMONSMATH_VECTOR_OPERATOR(v.data_[i] + s);
	}


	/// @brief Subtract a scalar from each element of a Vector.
	///
	/// @param s scalar to subtract from each element of a Vector.
	/// @param v Vector to subtract the scalar from.
	/// @return Vector containing the result.
	/// @related Vector
	template <class T, int d>
	inline DVector<T, d> operator-(const T& s, const DVector<T, d>& v) {
		DAMONSMATH_VECTOR_OPERATOR(s - v.data_[i]);
	}

	/// @brief Subtract a scalar from all elements of a vector.
	///
	/// @param v Vector for the operation.
	/// @param s A scalar to subtract from a vector.
	/// @return A new vector that stores the result.
	template <class T, int d>
	inline DVector<T, d> operator-(const DVector<T, d>& v, const T& s) {
		DAMONSMATH_VECTOR_OPERATOR(v.data_[i] - s);
	}

	/// @brief Multiply a vector by another Vector.
	///
	/// In line with GLSL, this performs component-wise multiplication.
	/// @param lhs First vector to use as a starting point.
	/// @param rhs Second vector to multiply by.
	/// @return A new Vector containing the result.
	template <class T, int d>
	inline DVector<T, d> operator*(const DVector<T, d>& lhs,
		const DVector<T, d>& rhs) {
		return HadamardProductHelper(lhs, rhs);
	}

	/// @brief Divide a vector by another Vector.
	///
	/// In line with GLSL, this performs component-wise division.
	/// @param lhs First vector to use as a starting point.
	/// @param rhs Second vector to divide by.
	/// @return A new Vector containing the result.
	template <class T, int d>
	inline DVector<T, d> operator/(const DVector<T, d>& lhs,
		const DVector<T, d>& rhs) {
		DAMONSMATH_VECTOR_OPERATOR(lhs.data_[i] / rhs[i]);
	}

	/// @brief Add a vector with another Vector.
	///
	/// @param lhs First vector to use as a starting point.
	/// @param rhs Second vector to add by.
	/// @return A new vector containing the result.
	template <class T, int d>
	inline DVector<T, d> operator+(const DVector<T, d>& lhs,
		const DVector<T, d>& rhs) {
		DAMONSMATH_VECTOR_OPERATOR(lhs.data_[i] + rhs[i]);
	}

	/// @brief subtract a vector with another Vector.
	///
	/// @param lhs First vector to use as a starting point.
	/// @param rhs Second vector to subtract by.
	/// @return A new vector containing the result.
	template <class T, int d>
	inline DVector<T, d> operator-(const DVector<T, d>& lhs,
		const DVector<T, d>& rhs) {
		DAMONSMATH_VECTOR_OPERATOR(lhs.data_[i] - rhs[i]);
	}

	/// @brief Multiply a vector with a scalar.
	///
	/// @param v Vector for the operation.
	/// @param s A scalar to multiply the vector with.
	/// @return A new vector containing the result.
	template <class T, int d>
	inline DVector<T, d> operator*(const DVector<T, d>& v, const T& s) {
		DAMONSMATH_VECTOR_OPERATOR(v.data_[i] * s);
	}

	/// @brief Multiply (in-place) a vector with another Vector.
	///
	/// In line with GLSL, this performs component-wise multiplication.
	/// @param lhs First vector to use as a starting point.
	/// @param rhs Second vector to multiply by.
	/// @return A reference to the input <b>v</b> vector.
	template <class T, int d>
	inline DVector<T, d>& operator*=(DVector<T, d>& lhs, const DVector<T, d>& rhs) {
		DAMONSMATH_VECTOR_OPERATION(lhs.data_[i] *= rhs[i]);
		return lhs;
	}

	/// @brief Divide (in-place) a vector by another Vector.
	///
	/// In line with GLSL, this performs component-wise division.
	/// @param lhs First vector to use as a starting point.
	/// @param rhs Second vector to divide by.
	/// @return A reference to the input <b>v</b> vector.
	template <class T, int d>
	inline DVector<T, d>& operator/=(DVector<T, d>& lhs, const DVector<T, d>& rhs) {
		DAMONSMATH_VECTOR_OPERATION(lhs.data_[i] /= rhs[i]);
		return lhs;
	}

	/// @brief Add (in-place) a vector with another Vector.
	///
	/// @param lhs First vector to use as a starting point.
	/// @param rhs Second vector to add.
	/// @return A reference to the input <b>v</b> vector.
	template <class T, int d>
	inline DVector<T, d>& operator+=(DVector<T, d>& lhs, const DVector<T, d>& rhs) {
		DAMONSMATH_VECTOR_OPERATION(lhs.data_[i] += rhs[i]);
		return lhs;
	}

	/// @brief Subtract (in-place) another Vector from a vector.
	///
	/// @param lhs First vector to use as a starting point.
	/// @param rhs Second vector to subtract by.
	/// @return A reference to the input <b>v</b> vector.
	template <class T, int d>
	inline DVector<T, d>& operator-=(DVector<T, d>& lhs, const DVector<T, d>& rhs) {
		DAMONSMATH_VECTOR_OPERATION(lhs.data_[i] -= rhs[i]);
		return lhs;
	}

	/// @brief Multiply (in-place) each element of a vector with a scalar.
	///
	/// @param v Vector for the operation.
	/// @param s A scalar to multiply the vector with.
	/// @return A reference to the input <b>v</b> vector.
	template <class T, int d>
	inline DVector<T, d>& operator*=(DVector<T, d>& v, const T& s) {
		DAMONSMATH_VECTOR_OPERATION(v.data_[i] *= s);
		return v;
	}

	/// @brief Divide (in-place) each element of a vector by a scalar.
	///
	/// @param v Vector for the operation.
	/// @param s A scalar to divide the vector by.
	/// @return A reference to the input <b>v</b> vector.
	template <class T, int d>
	inline DVector<T, d>& operator/=(DVector<T, d>& v, const T& s) {
		DAMONSMATH_VECTOR_OPERATION(v.data_[i] /= s);
		return v;
	}

	/// @brief Add (in-place) a scalar to each element of a vector.
	///
	/// @param v Vector for the operation.
	/// @param s A scalar to add the vector to.
	/// @return A reference to the input <b>v</b> vector.
	template <class T, int d>
	inline DVector<T, d>& operator+=(DVector<T, d>& v, const T& s) {
		DAMONSMATH_VECTOR_OPERATION(v.data_[i] += s);
		return v;
	}

	/// @brief Subtract (in-place) a scalar from each element of a vector.
	///
	/// @param v Vector for the operation.
	/// @param s A scalar to subtract from the vector.
	/// @return A reference to the input <b>v</b> vector.
	template <class T, int d>
	inline DVector<T, d>& operator-=(DVector<T, d>& v, const T& s) {
		DAMONSMATH_VECTOR_OPERATION(v.data_[i] -= s);
		return v;
	}

	/// @brief Calculate the hadamard or componentwise product of two vectors.
	///
	/// @param v1 First vector.
	/// @param v2 Second vector.
	/// @return The hadamard product of v1 and v2.
	template <class T, int d>
	inline DVector<T, d> HadamardProductHelper(const DVector<T, d>& v1,
											   const DVector<T, d>& v2) {
		DAMONSMATH_VECTOR_OPERATOR(v1[i] * v2[i]);
	}

	/// @brief Calculate the cross product of two vectors.
	///
	/// Note that this function is only defined for 3-dimensional Vectors.
	/// @param v1 First vector.
	/// @param v2 Second vector.
	/// @return The cross product of v1 and v2.
	template <class T>
	inline DVector<T, 3> CrossProductHelper(const DVector<T, 3>& v1,
										    const DVector<T, 3>& v2) {

		return DVector<T, 3>(v1[1] * v2[2] - v1[2] * v2[1],
			v1[2] * v2[0] - v1[0] * v2[2],
			v1[0] * v2[1] - v1[1] * v2[0]);
	}

	/// @brief Calculate the squared length of a vector.
	///
	/// @param v Vector to get the squared length of.
	/// @return The length of the vector squared.
	template <class T, int d>
	inline T LengthSquaredHelper(const DVector<T, d>& v) {
		return DotProductHelper(v, v);
	}

	/// @brief Calculate the length of a vector.
	///
	/// @param v Vector to get the squared length of.
	/// @return The length of the vector.
	template <class T, int d>
	inline T LengthHelper(const DVector<T, d>& v) {
		return sqrt(LengthSquaredHelper(v));
	}

	/// @brief Normalize a vector in-place.
	///
	/// @param v Vector to get the squared length of.
	/// @return The length of the vector.
	template <class T, int d>
	inline T NormalizeHelper(DVector<T, d>& v) {
		const T length = LengthHelper(v);
		v *= (T(1) / length);
		return length;
	}

	/// @brief Calculate the normalized version of a vector.
	///
	/// @param v Vector to get the squared length of.
	/// @return The normalized vector.
	template <class T, int d>
	inline DVector<T, d> NormalizedHelper(const DVector<T, d>& v) {
		return v * (T(1) / LengthHelper(v));
	}

	/// @brief Linearly interpolate two vectors.
	///
	/// @param v1 First vector.
	/// @param v2 Second vector.
	/// @param percent Percentage from v1 to v2 in range 0.0...1.0.
	/// @return The hadamard product of v1 and v2.
	template <class T, int d>
	inline DVector<T, d> LerpHelper(const DVector<T, d>& v1, const DVector<T, d>& v2,
								    const T percent) 
	{
		const T one_minus_percent = static_cast<T>(1.0) - percent;
		DAMONSMATH_VECTOR_OPERATOR(one_minus_percent * v1[i] + percent * v2[i]);
	}

	/// @brief Generates a random vector.
	///
	/// The range of each component is bounded by min and max.
	/// @param min Minimum value of the vector.
	/// @param max Maximum value of the vector.
	template <class T, int d>
	inline DVector<T, d> RandomInRangeHelper(const DVector<T, d>& min,
											 const DVector<T, d>& max) {
		DVector<T, d> result;
		DAMONSMATH_VECTOR_OPERATION(result[i] = RandomInRange<T>(min[i], max[i]));
		return result;
	}

	/// @brief Compare each component and returns max values.
	///
	/// @param v1 First vector.
	/// @param v2 Second vector.
	/// @return Max value of v1 and v2.
	template <class T, int d>
	inline DVector<T, d> MaxHelper(const DVector<T, d>& v1, const DVector<T, d>& v2) {
		DVector<T, d> result;
		DAMONSMATH_VECTOR_OPERATION(result[i] = std::max(v1[i], v2[i]));
		return result;
	}

	/// @brief Compare each component and returns min values.
	///
	/// @param v1 First vector.
	/// @param v2 Second vector.
	/// @return Min value of v1 and v2.
	template <class T, int d>
	inline DVector<T, d> MinHelper(const DVector<T, d>& v1, const DVector<T, d>& v2) {
		DVector<T, d> result;
		DAMONSMATH_VECTOR_OPERATION(result[i] = std::min(v1[i], v2[i]));
		return result;
	}

	/// @brief Check if val is within [range_start..range_end), denoting a
	/// rectangular area.
	///
	/// @param val 2D vector to be tested.
	/// @param range_start Starting point of the range (inclusive).
	/// @param range_end Ending point of the range (non-inclusive).
	/// @return Bool indicating success.
	///
	/// @tparam T Type of vector components to test.
	template <class T>
	bool InRange2D(const DMath::DVector<T, 2>& val,
		const DMath::DVector<T, 2>& range_start,
		const DMath::DVector<T, 2>& range_end) {
		return InRange(val[0], range_start[0], range_end[0]) &&
			   InRange(val[1], range_start[1], range_end[1]);
	}

	/// @cond MATHFU_INTERNAL
	/// @brief Calculate the dot product of two vectors.
	///
	/// @param v1 First vector.
	/// @param v2 Second vector.
	/// @return The dot product of v1 and v2.
	/// @related Vector
	template <class T, int d>
	static inline T DotProductHelper(const DVector<T, d>& v1,
		const DVector<T, d>& v2) {
		T result = 0;
		DAMONSMATH_VECTOR_OPERATION(result += v1[i] * v2[i]);
		return result;
	}
	/// @endcond

	/// @cond MATHFU_INTERNAL
	template <class T>
	static inline T DotProductHelper(const DVector<T, 2>& v1,
									 const DVector<T, 2>& v2) 
	{
		return v1[0] * v2[0] + v1[1] * v2[1];
	}
	/// @endcond

	/// @cond MATHFU_INTERNAL
	template <class T>
	static inline T DotProductHelper(const DVector<T, 3>& v1,
									 const DVector<T, 3>& v2) 
	{
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	}
	/// @endcond

	/// @cond MATHFU_INTERNAL
	template <class T>
	static inline T DotProductHelper(const DVector<T, 4>& v1,
									 const DVector<T, 4>& v2) 
	{
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3];
	}

	/// @brief Specialized version of RoundUpToPowerOf2 for vector.
	template <typename T, int d>
	inline DVector<T, d> RoundUpToPowerOf2(const DVector<T, d>& v) 
	{
		DVector<T, d> ret;
		DAMONSMATH_VECTOR_OPERATION(ret(i) = RoundUpToPowerOf2(v(i)));
		return ret;
	}
}/// namespace DMath end

#endif
