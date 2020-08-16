#ifndef _DAMONS_QUATERNION_H
#define _DAMONS_QUATERNION_H

#include "DamonsVector.h"
#include "DamonsMatrix.h"
namespace DGraphic {

	/// @class Quaternion
	///
	/// @brief Stores a Quaternion of type T and provides a set of utility
	/// operations on each Quaternion.
	/// @tparam T Type of each element in the Quaternion.
	template <class T>
	class DQuaternion {
	public:
		/// @brief Construct an uninitialized Quaternion.
		inline DQuaternion() {}

		/// @brief Construct a Quaternion from a copy.
		/// @param q Quaternion to copy.
		inline DQuaternion(const DQuaternion<T>& q) {
			s_ = q.s_;
			v_ = q.v_;
		}
		/// @brief Construct a Quaternion using scalar values to initialize each
		/// element.
		///
		/// @param s1 Scalar component.
		/// @param s2 First element of the Vector component.
		/// @param s3 Second element of the Vector component.
		/// @param s4 Third element of the Vector component.
		inline DQuaternion(const T& s1, const T& s2, const T& s3, const T& s4) {
			s_ = s1;
			v_ = DVector<T, 3>(s2, s3, s4);
		}

		/// @brief Construct a quaternion from a scalar and 3-dimensional Vector.
		///
		/// @param s1 Scalar component.
		/// @param v1 Vector component.
		inline DQuaternion(const T& s1, const DVector<T, 3>& v1) {
			s_ = s1;
			v_ = v1;
		}

		/// @brief Return the scalar component of the quaternion.
		///
		/// @return The scalar component
		inline T& scalar() { return s_; }

		/// @brief Return the scalar component of the quaternion.
		///
		/// @return The scalar component
		inline const T& scalar() const { return s_; }

		/// @brief Set the scalar component of the quaternion.
		///
		/// @param s Scalar component.
		inline void set_scalar(const T& s) { s_ = s; }

		/// @brief Return the vector component of the quaternion.
		///
		/// @return The scalar component
		inline DVector<T, 3>& vector() { return v_; }
		/// @brief Return the vector component of the quaternion.
		///
		/// @return The scalar component
		inline const DVector<T, 3>& vector() const { return v_; }

		/// @brief Set the vector component of the quaternion.
		///
		/// @param v Vector component.
		inline void set_vector(const DVector<T, 3>& v) { v_ = v; }

		/// @brief Calculate the inverse Quaternion.
		///
		/// This calculates the inverse such that <code>(q * q).Inverse()</code>
		/// is the identity.
		///
		/// @return Quaternion containing the result.
		inline DQuaternion<T> Inverse() const { return DQuaternion<T>(s_, -v_); }
		
		/// @brief Multiply this Quaternion with another Quaternion.
		///
		/// @note This is equivalent to
		/// <code>FromMatrix(ToMatrix() * q.ToMatrix()).</code>
		/// @param q Quaternion to multiply with.
		/// @return Quaternion containing the result.
		inline DQuaternion<T> operator*(const DQuaternion<T>& q) const {
			return DQuaternion<T>(
				s_ * q.s_ - DVector<T, 3>::DotProduct(v_, q.v_),
				s_ * q.v_ + q.s_ * v_ + DVector<T, 3>::CrossProduct(v_, q.v_));
		}

		/// @brief Multiply this Quaternion by a scalar.
		///
		/// This multiplies the angle of the rotation by a scalar factor.
		/// @param s1 Scalar to multiply with.
		/// @return Quaternion containing the result.
		inline DQuaternion<T> operator*(const T& s1) const {
			T angle;
			DVector<T, 3> axis;
			ToAngleAxis(&angle, &axis);
			angle *= s1;
			return DQuaternion<T>(cos(0.5f * angle),
				axis.Normalized() * static_cast<T>(sin(0.5f * angle)));
		}

		/// @brief Multiply a Vector by this Quaternion.
		///
		/// This will rotate the specified vector by the rotation specified by this
		/// Quaternion.
		///
		/// @param v1 Vector to multiply by this Quaternion.
		/// @return Rotated Vector.
		inline DVector<T, 3> operator*(const DVector<T, 3>& v1) const {
			T ss = s_ + s_;
			return ss * DVector<T, 3>::CrossProduct(v_, v1) + (ss * s_ - 1) * v1 +
				2 * DVector<T, 3>::DotProduct(v_, v1) * v_;
		}

		/// @brief Normalize this quaterion (in-place).
		///
		/// @return Length of the quaternion.
		inline T Normalize() {
			T length = sqrt(s_ * s_ + DVector<T, 3>::DotProduct(v_, v_));
			T scale = (1 / length);
			s_ *= scale;
			v_ *= scale;
			return length;
		}

		/// @brief Calculate the normalized version of this quaternion.
		///
		/// @return The normalized quaternion.
		inline DQuaternion<T> Normalized() const {
			DQuaternion<T> q(*this);
			q.Normalize();
			return q;
		}

		/// @brief Convert this Quaternion to an Angle and axis.
		///
		/// The returned  angle is the size of the rotation in radians about the
		/// axis represented by this Quaternion.
		///
		/// @param angle Receives the angle.
		/// @param axis Receives the normalized axis.
		inline void ToAngleAxis(T* angle, DVector<T, 3>* axis) const {
			*axis = s_ > 0 ? v_ : -v_;
			*angle = 2 * atan2(axis->Normalize(), s_ > 0 ? s_ : -s_);
		}

		/// @brief Convert this Quaternion to 3 Euler Angles.
		///
		/// @return 3-dimensional Vector where each element is a angle of rotation
		/// (in radians) around the x, y, and z axes.
		inline DVector<T, 3> ToEulerAngles() const {
			DMatrix<T, 3> m(ToMatrix());
			T cos2 = m[0] * m[0] + m[1] * m[1];
			if (cos2 < 1e-6f) {
				return DVector<T, 3>(
					0,
					m[2] < 0 ? static_cast<T>(0.5 * M_PI) : static_cast<T>(-0.5 * M_PI),
					-std::atan2(m[3], m[4]));
			}
			else {
				return DVector<T, 3>(std::atan2(m[5], m[8]),
					std::atan2(-m[2], std::sqrt(cos2)),
					std::atan2(m[1], m[0]));
			}
		}

		/// @brief Convert to a 3x3 Matrix.
		///
		/// @return 3x3 rotation Matrix.
		inline DMatrix<T, 3> ToMatrix() const {
			const T x2 = v_[0] * v_[0], y2 = v_[1] * v_[1], z2 = v_[2] * v_[2];
			const T sx = s_ * v_[0], sy = s_ * v_[1], sz = s_ * v_[2];
			const T xz = v_[0] * v_[2], yz = v_[1] * v_[2], xy = v_[0] * v_[1];
			return DMatrix<T, 3>(1 - 2 * (y2 + z2), 2 * (xy + sz), 2 * (xz - sy),
				2 * (xy - sz), 1 - 2 * (x2 + z2), 2 * (sx + yz),
				2 * (sy + xz), 2 * (yz - sx), 1 - 2 * (x2 + y2));
		}

		/// @brief Convert to a 4x4 Matrix.
		///
		/// @return 4x4 transform Matrix.
		inline DMatrix<T, 4> ToMatrix4() const {
			const T x2 = v_[0] * v_[0], y2 = v_[1] * v_[1], z2 = v_[2] * v_[2];
			const T sx = s_ * v_[0], sy = s_ * v_[1], sz = s_ * v_[2];
			const T xz = v_[0] * v_[2], yz = v_[1] * v_[2], xy = v_[0] * v_[1];
			return DMatrix<T, 4>(1 - 2 * (y2 + z2), 2 * (xy + sz), 2 * (xz - sy), 0.0f,
				2 * (xy - sz), 1 - 2 * (x2 + z2), 2 * (sx + yz), 0.0f,
				2 * (sy + xz), 2 * (yz - sx), 1 - 2 * (x2 + y2), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
		}

		/// @brief Create a Quaternion from an angle and axis.
		///
		/// @param angle Angle in radians to rotate by.
		/// @param axis Axis in 3D space to rotate around.
		/// @return Quaternion containing the result.
		static DQuaternion<T> FromAngleAxis(const T& angle, const DVector<T, 3>& axis) {
			const T halfAngle = static_cast<T>(0.5) * angle;
			DVector<T, 3> localAxis(axis);
			return DQuaternion<T>(
				cos(halfAngle),
				localAxis.Normalized() * static_cast<T>(sin(halfAngle)));
		}

		/// @brief Create a quaternion from 3 euler angles.
		///
		/// @param angles 3-dimensional Vector where each element contains an
		/// angle in radius to rotate by about the x, y and z axes.
		/// @return Quaternion containing the result.
		static DQuaternion<T> FromEulerAngles(const DVector<T, 3>& angles) {
			const DVector<T, 3> halfAngles(
				static_cast<T>(0.5) * angles[0],
				static_cast<T>(0.5) * angles[1],
				static_cast<T>(0.5) * angles[2]);

			const T sinx = std::sin(halfAngles[0]);
			const T cosx = std::cos(halfAngles[0]);
			const T siny = std::sin(halfAngles[1]);
			const T cosy = std::cos(halfAngles[1]);
			const T sinz = std::sin(halfAngles[2]);
			const T cosz = std::cos(halfAngles[2]);
			return DQuaternion<T>(cosx * cosy * cosz + sinx * siny * sinz,
				sinx * cosy * cosz - cosx * siny * sinz,
				cosx * siny * cosz + sinx * cosy * sinz,
				cosx * cosy * sinz - sinx * siny * cosz);
		}

		/// @brief Create a quaternion from a rotation Matrix.
		///
		/// @param m 3x3 rotation Matrix.
		/// @return Quaternion containing the result.
		static DQuaternion<T> FromMatrix(const DMatrix<T, 3>& m) {
			const T trace = m(0, 0) + m(1, 1) + m(2, 2);
			if (trace > 0) {
				const T s = sqrt(trace + 1) * 2;
				const T oneOverS = 1 / s;
				return DQuaternion<T>(static_cast<T>(0.25) * s, (m[5] - m[7]) * oneOverS,
					(m[6] - m[2]) * oneOverS, (m[1] - m[3]) * oneOverS);
			}
			else if (m[0] > m[4] && m[0] > m[8]) {
				const T s = sqrt(m[0] - m[4] - m[8] + 1) * 2;
				const T oneOverS = 1 / s;
				return DQuaternion<T>((m[5] - m[7]) * oneOverS, static_cast<T>(0.25) * s,
					(m[3] + m[1]) * oneOverS, (m[6] + m[2]) * oneOverS);
			}
			else if (m[4] > m[8]) {
				const T s = sqrt(m[4] - m[0] - m[8] + 1) * 2;
				const T oneOverS = 1 / s;
				return DQuaternion<T>((m[6] - m[2]) * oneOverS, (m[3] + m[1]) * oneOverS,
					static_cast<T>(0.25) * s, (m[5] + m[7]) * oneOverS);
			}
			else {
				const T s = sqrt(m[8] - m[0] - m[4] + 1) * 2;
				const T oneOverS = 1 / s;
				return DQuaternion<T>((m[1] - m[3]) * oneOverS, (m[6] + m[2]) * oneOverS,
					(m[5] + m[7]) * oneOverS, static_cast<T>(0.25) * s);
			}
		}

		/// @brief Create a quaternion from the upper-left 3x3 roation Matrix of a 4x4
		/// Matrix.
		///
		/// @param m 4x4 Matrix.
		/// @return Quaternion containing the result.
		static DQuaternion<T> FromMatrix(const DMatrix<T, 4>& m) {
			const T trace = m(0, 0) + m(1, 1) + m(2, 2);
			if (trace > 0) {
				const T s = sqrt(trace + 1) * 2;
				const T oneOverS = 1 / s;
				return DQuaternion<T>(static_cast<T>(0.25) * s, (m[6] - m[9]) * oneOverS,
					(m[8] - m[2]) * oneOverS, (m[1] - m[4]) * oneOverS);
			}
			else if (m[0] > m[5] && m[0] > m[10]) {
				const T s = sqrt(m[0] - m[5] - m[10] + 1) * 2;
				const T oneOverS = 1 / s;
				return DQuaternion<T>((m[6] - m[9]) * oneOverS, static_cast<T>(0.25) * s,
					(m[4] + m[1]) * oneOverS, (m[8] + m[2]) * oneOverS);
			}
			else if (m[5] > m[10]) {
				const T s = sqrt(m[5] - m[0] - m[10] + 1) * 2;
				const T oneOverS = 1 / s;
				return DQuaternion<T>((m[8] - m[2]) * oneOverS, (m[4] + m[1]) * oneOverS,
					static_cast<T>(0.25) * s, (m[6] + m[9]) * oneOverS);
			}
			else {
				const T s = sqrt(m[10] - m[0] - m[5] + 1) * 2;
				const T oneOverS = 1 / s;
				return DQuaternion<T>((m[1] - m[4]) * oneOverS, (m[8] + m[2]) * oneOverS,
					(m[6] + m[9]) * oneOverS, static_cast<T>(0.25) * s);
			}
		}

		/// @brief Calculate the dot product of two Quaternions.
		///
		/// @param q1 First quaternion.
		/// @param q2 Second quaternion
		/// @return The scalar dot product of both Quaternions.
		static inline T DotProduct(const DQuaternion<T>& q1, const DQuaternion<T>& q2) {
			return q1.s_ * q2.s_ + DVector<T, 3>::DotProduct(q1.v_, q2.v_);
		}

		/// @brief Calculate the spherical linear interpolation between two
		/// Quaternions.
		///
		/// @param q1 Start Quaternion.
		/// @param q2 End Quaternion.
		/// @param s1 The scalar value determining how far from q1 and q2 the
		/// resulting quaternion should be.  A value of 0 corresponds to q1 and a
		/// value of 1 corresponds to q2.
		/// @result Quaternion containing the result.
		static inline DQuaternion<T> Slerp(const DQuaternion<T>& q1,
										   const DQuaternion<T>& q2, 
										   const T& s1) {
			if (q1.s_ * q2.s_ + DVector<T, 3>::DotProduct(q1.v_, q2.v_) > 0.999999f)
				return DQuaternion<T>(q1.s_ * (1 - s1) + q2.s_ * s1,
					q1.v_ * (1 - s1) + q2.v_ * s1);

			return q1 * ((q1.Inverse() * q2) * s1);
		}

		/// @brief Access an element of the quaternion.
		///
		/// @param i Index of the element to access.
		/// @return A reference to the accessed data that can be modified by the
		/// caller.
		inline T& operator[](const int i) {
			if (i == 0) return s_;
			return v_[i - 1];
		}

		/// @brief Access an element of the quaternion.
		///
		/// @param i Index of the element to access.
		/// @return A const reference to the accessed.
		inline const T& operator[](const int i) const {
			return const_cast<DQuaternion<T>*>(this)->operator[](i);
		}

		/// @brief Returns a vector that is perpendicular to the supplied vector.
		///
		/// @param v1 An arbitrary vector
		/// @return A vector perpendicular to v1.  Normally this will just be
		/// the cross product of v1, v2.  If they are parallel or opposite though,
		/// the routine will attempt to pick a vector.
		static inline DVector<T, 3> PerpendicularVector(const DVector<T, 3>& v) {
			// We start out by taking the cross product of the vector and the x-axis to
			// find something parallel to the input vectors.  If that cross product
			// turns out to be length 0 (i. e. the vectors already lie along the x axis)
			// then we use the y-axis instead.
			DVector<T, 3> axis = DVector<T, 3>::CrossProduct(
				DVector<T, 3>(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)),
				v);
			// We use a fairly high epsilon here because we know that if this number
			// is too small, the axis we'll get from a cross product with the y axis
			// will be much better and more numerically stable.
			if (axis.LengthSquared() < static_cast<T>(0.05)) {
				axis = DVector<T, 3>::CrossProduct(
					DVector<T, 3>(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0)),
					v);
			}
			return axis;
		}

		/// @brief Returns the a Quaternion that rotates from start to end.
		///
		/// @param v1 The starting vector
		/// @param v2 The vector to rotate to
		/// @param preferred_axis the axis to use, if v1 and v2 are parallel.
		/// @return A Quaternion describing the rotation from v1 to v2
		/// See the comment on RotateFromToWithAxis for an explanation of the math.
		static inline DQuaternion<T> RotateFromToWithAxis(
			const DVector<T, 3>& v1, const DVector<T, 3>& v2,
			const DVector<T, 3>& preferred_axis) {
			DVector<T, 3> start = v1.Normalized();
			DVector<T, 3> end = v2.Normalized();

			T dot_product = DVector<T, 3>::DotProduct(start, end);
			// Any rotation < 0.1 degrees is treated as no rotation
			// in order to avoid division by zero errors.
			// So we early-out in cases where it's less then 0.1 degrees.
			// cos( 0.1 degrees) = 0.99999847691
			if (dot_product >= static_cast<T>(0.99999847691)) {
				return DQuaternion<T>::identity;
			}
			// If the vectors point in opposite directions, return a 180 degree
			// rotation, on the axis that they asked for.
			if (dot_product <= static_cast<T>(-0.99999847691)) {
				return DQuaternion<T>(static_cast<T>(0), preferred_axis);
			}
			// Degenerate cases have been handled, so if we're here, we have to
			// actually compute the angle we want:
			DVector<T, 3> cross_product = DVector<T, 3>::CrossProduct(start, end);

			return DQuaternion<T>(static_cast<T>(1.0) + dot_product, cross_product)
				.Normalized();
		}

		/// @brief Returns the a Quaternion that rotates from start to end.
		///
		/// @param v1 The starting vector
		/// @param v2 The vector to rotate to
		/// @return A Quaternion describing the rotation from v1 to v2.  In the case
		/// where the vectors are parallel, it returns the identity.  In the case
		/// where
		/// they point in opposite directions, it picks an arbitrary axis.  (Since
		/// there
		/// are technically infinite possible quaternions to represent a 180 degree
		/// rotation.)
		///
		/// The final equation used here is fairly elegant, but its derivation is
		/// not obvious:  We want to find the quaternion that represents the angle
		/// between Start and End.
		///
		/// The angle can be expressed as a quaternion with the values:
		///  angle: ArcCos(dotproduct(start, end) / (|start|*|end|)
		///  axis: crossproduct(start, end).normalized * sin(angle/2)
		///
		/// or written as:
		///  quaternion(cos(angle/2), axis * sin(angle/2))
		///
		/// Using the trig identity:
		///  sin(angle * 2) = 2 * sin(angle) * cos*angle)
		/// Via substitution, we can turn this into:
		///  sin(angle/2) = 0.5 * sin(angle)/cos(angle/2)
		///
		/// Using this substitution, we get:
		///  quaternion( cos(angle/2),
		///             0.5 * crossproduct(start, end).normalized
		///                 * sin(angle) / cos(angle/2))
		///
		/// If we scale the whole thing up by 2 * cos(angle/2) then we get:
		///  quaternion(2 * cos(angle/2) * cos(angle/2),
		///             crossproduct(start, end).normalized * sin(angle))
		///
		/// (Note that the quaternion is no longer normalized after this scaling)
		///
		/// Another trig identity:
		///   cos(angle/2) = sqrt((1 + cos(angle) / 2)
		///
		/// Substituting this in, we can simplify the quaternion scalar:
		///
		///  quaternion(1 + cos(angle),
		///             crossproduct(start, end).normalized * sin(angle))
		///
		/// Because cross(start, end) has a magnitude of |start|*|end|*sin(angle),
		///  crossproduct(start,end).normalized
		/// is equivalent to
		///  crossproduct(start,end) / |start| * |end| * sin(angle)
		/// So after that substitution:
		///
		///  quaternion(1 + cos(angle),
		///             crossproduct(start, end) / (|start| * |end|))
		///
		/// dotproduct(start, end) has the value of |start| * |end| * cos(angle),
		/// so by algebra,
		///  cos(angle) = dotproduct(start, end) / (|start| * |end|)
		/// we can replace our quaternion scalar here also:
		///
		///  quaternion(1 + dotproduct(start, end) / (|start| * |end|),
		///             crossproduct(start, end) / (|start| * |end|))
		///
		/// If start and end are normalized, then |start| * |end| = 1, giving us a
		/// final quaternion of:
		///
		/// quaternion(1 + dotproduct(start, end), crossproduct(start, end))
		static inline DQuaternion<T> RotateFromTo(const DVector<T, 3>& v1,
												  const DVector<T, 3>& v2) {
			DVector<T, 3> start = v1.Normalized();
			DVector<T, 3> end   = v2.Normalized();

			T dot_product = DVector<T, 3>::DotProduct(start, end);
			// Any rotation < 0.1 degrees is treated as no rotation
			// in order to avoid division by zero errors.
			// So we early-out in cases where it's less then 0.1 degrees.
			// cos( 0.1 degrees) = 0.99999847691
			if (dot_product >= static_cast<T>(0.99999847691)) {
				return DQuaternion<T>::identity;
			}
			// If the vectors point in opposite directions, return a 180 degree
			// rotation, on an arbitrary axis.
			if (dot_product <= static_cast<T>(-0.99999847691)) {
				return DQuaternion<T>(0, PerpendicularVector(start));
			}
			// Degenerate cases have been handled, so if we're here, we have to
			// actually compute the angle we want:
			DVector<T, 3> cross_product = DVector<T, 3>::CrossProduct(start, end);

			return DQuaternion<T>(static_cast<T>(1.0) + dot_product, cross_product)
				.Normalized();
		}

		/// @brief get string of vector data seperate by space.
		///  vector data must be basic type like :float int double
		///
		/// @return string of vector data.
		inline std::string ToString() const {
			std::string str = "quaternion :";
			
			str += (std::to_string(s_) + std::string(" "));
			str += (std::to_string(v_[0]) + std::string(" "));
			str += (std::to_string(v_[1]) + std::string(" "));
			str += (std::to_string(v_[2]) + std::string(" "));
			str += "\n";

			return str;
		}

		/// @brief get wstring of vector data seperate by space.
		///  vector data must be basic type like :float int double
		///
		/// @return wstring of vector data.
		inline std::wstring ToWString() const {
			std::wstring str = L"";
			str += (std::to_wstring(s_) + std::wstring(L" "));
			str += (std::to_wstring(v_[0]) + std::wstring(L" "));
			str += (std::to_wstring(v_[1]) + std::wstring(L" "));
			str += (std::to_wstring(v_[2]) + std::wstring(L" "));
			str += L"\n";

			return str;
		}

		DAMONS_DEFINE_CLASS_SIMD_AWARE_NEW_DELETE
		/// @brief Contains a quaternion doing the identity transform.
		static DQuaternion<T> identity;

	private:
		T s_;
		DVector<T, 3> v_;
	};

	template <typename T>
	DQuaternion<T> DQuaternion<T>::identity = DQuaternion<T>(1, 0, 0, 0);

	/// @brief Multiply a Quaternion by a scalar.
	///
	/// This multiplies the angle of the rotation of the specified Quaternion
	/// by a scalar factor.
	/// @param s Scalar to multiply with.
	/// @param q Quaternion to scale.
	/// @return Quaternion containing the result.
	///
	/// @related Quaternion
	template <class T>
	inline DQuaternion<T> operator*(const T& s, const DQuaternion<T>& q) {
		return q * s;
	}
};/// namespace DMath end

#endif // !_DAMONS_QUATERNION_H

