#ifndef _DAMONS_SEGMENT_H_
#define _DAMONS_SEGMENT_H_

#include "DamonsObject.h"
#include "DamonsPoint.h"


namespace DGraphic {

	template<class T>
	class DDirection;


	///class DSegment
	///@breif line with 2 point of Type T as line segment
	///
	/// DSegment stores <b>2</b> elements of points
	/// @tparam T type of Point.

	template< class T = double>
	class DSegment: public DObject
	{
	public:
		/// @brief Create an uninitialized Line.
		inline DSegment() {
		}

		/// @brief create a line with two point
		///
		/// @param p1 first point of line
		/// @param p2 second point of line

		DSegment(const DPoint< T > &p1, const DPoint< T > &p2) {
			lpoints_[0] = p1;
			lpoints_[1] = p2;
		}

		/// @brief create a line with six values

		/// @param p11 first   value of first point
		/// @param p12 second  value of first point
		/// @param p13 third   value of first point
		/// @param p21 first   value of second point
		/// @param p22 second  value of second point
		/// @param p23 third   value of second point

		explicit DSegment(T &p11, T &p12, T &p13, T &p21, T &p22, T &p23) {
			lpoints_[0] = DPoint<T>(p11, p12, p13);
			lpoints_[1] = DPoint<T>(p21, p22, p23);
		}

		~DSegment(){
		}
	public:
		/// @brief get the direction of this line, normlized
		///
		/// @return the direction of this line
		inline DDirection<T> Direction() const {
			DPoint< T > dir = (lpoints_[1] - lpoints_[0]);
			dir.Normalize();

			return DDirection<T>(dir[0],dir[1],dir[2]);
		}
		/// @brief get the first point of line
		///
		/// @return the start point of the line.
		inline  DPoint< T > GetStartPoint() const {
			return lpoints_[0];
		}
		/// @brief get the second point of line
		///
		/// @return the end point of the line.
		inline  DPoint< T > GetEndPoint() const {
			return lpoints_[1];
		}

		/// @brief get point of line by index
		///
		/// @param i Index of the element to access.
		/// @return the point accessed.
		inline  DPoint< T > Vertex(const int i) const {
			assert(i < 2);
			return lpoints_[i];
		}

		/// @brief Access an point of the line.
		///
		/// @param i Index of the element to access.
		/// @return A const reference to the accessed.
		inline const DPoint< T >& operator[](const int i) const {
			assert(i < 2);
			return lpoints_[i];
		}
		/// @brief Access an point of the line.
		///
		/// @param i Index of the element to access.
		/// @return A reference to the accessed data that can be modified by the
		/// caller.
		inline  DPoint< T >& operator[](const int i){
			assert(i < 2);
			return lpoints_[i];
		}
		///@breif compare if two line is equal
		/// two line is equal if the source and target point is equal
		///
		///@param the line need to compare with
		///@return true if they are equal otherwise false

		inline bool operator ==(const DSegment<T> & l) {
			return ((lpoints_[0] == l[0]) && (lpoints_[1] == l[1]));
		}

		///@breif compare if two line is not equal
		/// two line is equal if the source and target point is equal
		///
		///@param the line need to compare with
		///@return true if they are not equal otherwise false

		inline bool operator !=(const DSegment<T> & l) {
			return !(*this == l);
		}

		/// @brief Calculate the length of the line.
		///
		/// @return the length of the line.

		inline T Length() const {
			return (lpoints_[0] - lpoints_[1]).Length();
		}

		/// @brief Calculate the squared ength of the line.
		///
		/// @return the squared length of the line.

		inline T SquaredLength() const {
			return (lpoints_[0] - lpoints_[1]).LengthSquared();
		}

		/// @brief convert the line to vector.
		///
		/// @return the vector equal with line.

		inline DVector<T, 3> to_vector() const {
			return (lpoints_[1] - lpoints_[0]);
		}
		/// @brief opposite the start and source point of this line .
		///
		/// @returns a line with source and target interchanged..
		inline DSegment<T> Opposite() const{

			return DSegment<T>(lpoints_[1], lpoints_[0]);
		}
		/// @brief calculate is the given point on the line
		///
		/// @param the point need to test
		/// @returns a bool value that is point on line
		inline bool Has_on(const DPoint< T > &p) const {
			//first check if they are colinear
			DPoint< T > s = (p - lpoints_[0]).CrossProduct(lpoints_[1] - lpoints_[0]);
			if (std::abs(s[0]) > DEplision || std::abs(s[1]) > DEplision || std::abs(s[2]) > DEplision) {
				return false;
			}
			//secont check if the point is between two points
			T t = (p - lpoints_[0]).DotProduct(lpoints_[1] - lpoints_[0]);
			  t = t / SquaredLength();

			return (t < static_cast<T>(1.000001) && t > static_cast<T>(-0.000001));
		}
		/// @brief line is degenerate, if source and target fall together.
		///
		/// @returns a bool value that is line degenerate
		inline bool Is_degenerate() const {
			return Length() < DEplision;
		}
		
		/// @brief get string of line data seperate by space.
		///  line data must be basic type like :float int double
		///
		/// @return string of line data.
		inline std::string ToString() const {
			std::string str;
			str = "first point data: \n";
			str += lpoints_[0].ToString();

			str += "\nsecond point data: \n";
			str += lpoints_[1].ToString();

			return str;
		}

		/// @brief get wstring of line data seperate by space.
		///  line data must be basic type like :float int double
		///
		/// @return wstring of line data.
		inline std::wstring ToWString() const {
			std::wstring str;
			str = L"first point data: \n";
			str += lpoints_[0].ToWString();

			str += L"\nsecond point data: \n";
			str += lpoints_[1].ToWString();

			return str;
		}
	public:
		///point data 
		///lpoints[0] means the source (start )point
		///lpoints[1] means the target (end   )point
		DPoint< T > lpoints_[2];
	};
};

#endif
