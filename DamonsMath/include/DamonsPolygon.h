/*
* learning from google's opensource project :mathfu
*/

#ifndef _DAMONSPOLYGON_HEADER_
#define _DAMONSPOLYGON_HEADER_

#include "DamonsObject.h"
#include "DamonsVector.h"
#include <vector>

using namespace DMath;
namespace DGraphic {
	///class DPolygon
	///@breif polygon with plane coordinates with Type T
	/// 
	/// polygon stores arbitrary number of 2d points of type <b>T</b> and provides a set
	/// functions to perform operations on polygon.
	///
	/// @tparam T type of polygon point.
	/// @note note that polygon is default closed, if input polygon has different
	/// first and last point,then the first point will be pushed .
	template< class T = double>
	class DPolygon : public DObject
	{
	public:
		/// @brief Create an uninitialized polygon.
		inline DPolygon() {}

		/// @brief create a polygon with points vector
		///
		/// @param points points of polygon

		inline DPolygon(std::vector< DVector<T, 2> > &points) {
			std::copy(points.begin(), points.end(), std::back_inserter(m_polygonPoints));
		}
		/// @brief deallocate memory
		~DPolygon() {
			std::vector<PolygonPoint >().swap(m_polygonPoints);
			m_polygonPoints.clear();
		}
	public:
		/// @brief add a point to polygon
		///
		/// @return void
		void AddPoint(const DVector<T, 2> &_point) { m_polygonPoints.push_back(_point); }
		void AddPoint(const T &x, const T &y) { PolygonPoint _pp(x, y); m_polygonPoints.push_back(_pp); }
        /// @brief read a polygon from string
        /// @note string contains coordinate x and y split by comma","
        /// @return void
        void ReadFromString(std::string polystr){
            m_polygonPoints.clear();
            std::vector<double > values;
            std::size_t beg = 0;
            std::size_t end = 0;
            while((end = polystr.find(",")) != std::string::npos){
                std::string cv = polystr.substr(beg,end-beg);
                polystr = polystr.substr(end+1);
                double d = std::stod(cv);
                values.push_back(d);
            }
            double d = std::stod(polystr);
            values.push_back(d);
            for(int i = 0 ;i<values.size();i=i+2){
                double x = values[i];
                double y = values[i+1];
                DVector<T, 2> p;
                p[0] = x;
                p[1] = y;
                m_polygonPoints.push_back(p);
            }
            std::vector<double >().swap(values);
        }
        /// @brief  get all polygon points string
        /// @return string
        std::string PrintPolygonPoints(){
            std::string pstr;
            int i = 1;
            for (auto &p : m_polygonPoints) {
                pstr = pstr + std::to_string(i++) +"  "+ p.ToString();
            }
            return pstr;
        }
	public:
		/// @brief get point size of polygon
		/// @note only count point size,if first point same with last point
		/// only one counted
		/// @return unsigned int : point size 
		unsigned int GetPolygonPointSize() const{
			return IsClosed()? m_polygonPoints.size() - 1:m_polygonPoints.size();
		}
		/// @brief close polygon 
		/// @return void
		void ClosePolygon() {
			if(!IsClosed()){
				auto p = m_polygonPoints.front();
				m_polygonPoints.push_back(p);
			}
		}
	public:
		/// @brief tell whether polygon is simple
		/// @note a polygon is simple when there are two edges intersected
		/// which means the intersected point is not end point.
		/// @return  true if simple otherwise false 
		bool IsSimple() { 
			ClosePolygon();
			unsigned int sz = GetPolygonPointSize();
			for(unsigned int i = 0;i < sz - 1;++i){
                for(unsigned int j = i+1;j < sz ;++j){	if(IsSegmentIntersect(m_polygonPoints[i],m_polygonPoints[i+1],m_polygonPoints[j],m_polygonPoints[j+1]) > 1)
					return false;
                }
			}
			return true;
		}
		/// @brief tell whether polygon is convex
        /// @note perform this function makes polygon counterclockwise
		/// @return  true if convex otherwise false 
		bool IsConvex() { 
			if (!IsCounterClockWise()) {
				ReversePolygon();
			}
			unsigned int sz = GetPolygonPointSize();
			for (unsigned int i = 1; i <= sz; ++i) {
				auto &p0 = m_polygonPoints[i - 1];
				auto &p1 = m_polygonPoints[i ];
				auto &p2 = ((i + 1) == m_polygonPoints.size()) ? m_polygonPoints[0] : m_polygonPoints[i + 1];

				const T Ax = p1[0] - p0[0];
				const T Ay = p1[1] - p0[1];
				const T Bx = p2[0] - p1[0];
				const T By = p2[1] - p1[1];
				if (Ax*By - Bx * Ay < 0)
					return false;
			}
 			return true;
		}

		/// @brief tell whether polygon is counter clockwise
        /// @note use green formula
		/// @return  true if counter clockwise otherwise false 
		bool IsCounterClockWise()  { 
			ClosePolygon(); 
			double area = 0.0;
			unsigned int sz = GetPolygonPointSize();
			for (unsigned int i = 0; i < sz; i++)
			{
				auto &p0 = m_polygonPoints[i];
				auto &p1 = m_polygonPoints[i+1];

				area +=-0.5*(p1[1] + p0[1])*(p1[0] - p0[0]);
			}

			return area > 0.0;
		}

		/// @brief tell whether a point lie in polygon
		/// @return  2 if in polygon
        ///			 1 if on Polygon edge
		///			 0 if out polygon

		int IsPointInPolygon(const T &x,const T &y) { 
			DVector<T, 2> p(x,y);
			return IsPointInPolygon(p);
		}
		int IsPointInPolygon(const DVector<T,2> &p) {
			ClosePolygon();
			unsigned int sz = GetPolygonPointSize();
			int num_intersect = 0;
			T x = p[0];
			T y = p[1];

			for (unsigned int i = 0; i < sz; ++i) {
				auto &p0 = m_polygonPoints[i];
				auto &p1 = m_polygonPoints[i + 1];

				if ((p0 - p).LengthSquared() < DEplision*DEplision)
					return 1;

				if (std::abs(p0[1] - y) < DEplision && std::abs(p1[1] - y) < DEplision) //点在线段的方向上
				{
					if (std::abs(p0[0] - x) < 0.00001)
						return 1;
					if (std::abs(p1[0] - x) < 0.00001)
						return 1;
					if ((p0[0] - x)*(p1[0] - x) < 0)
						return 1;

					continue;
				}

				if (std::min(p0[1], p1[1]) - y > -DEplision) //去掉误差 判断线段在射线之上或者在射线上方
					continue;

				if (std::abs(std::max(p0[1], p1[1]) - y) < DEplision)//线段最高点在射线上					
				{
					num_intersect++;
					continue;
				}

				if (std::max(p0[1], p1[1]) < y)//线段在射线下
					continue;

				double xseg = p1[0] - (p1[0] - p0[0]) * (p1[1] - y) / (p1[1] - p0[1]); //求交
				if (xseg < x)  //交点在射线起点的左侧
					continue;

				num_intersect++;
			}

			return num_intersect % 2 ? 2 : 0;
		}
	public:
		/// @brief reverse polygon
		/// @return  void
		void ReversePolygon() {
			std::reverse(m_polygonPoints.begin(), m_polygonPoints.end());
		}

	protected:
		/// @brief tell whether polygon is close
		/// @note a polygon is close when the first and last point is same
		/// @return  true if close otherwise false 
	    bool IsClosed() const{
			auto &p1 = m_polygonPoints.front();
			auto &p2 = m_polygonPoints.back();
			auto pd = p1 - p2;
			return pd.LengthSquared() < DEplision*DEplision;
		}
		/// @brief tell whether two segment is intersect
		/// @note https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		/// @return  0 if not intersect
		///			 1 intersect at endpoint
		///			 2 overlap
		///			 3 intersect at a point which is not endpoint
		int IsSegmentIntersect(DVector<T, 2> &V0,DVector<T, 2> &V1,DVector<T, 2> &U0,DVector<T, 2> &U1) {
				 							
				const T Ax = V1[0] - V0[0];
				const T Ay = V1[1] - V0[1];
				const T Bx = U0[0] - U1[0];
				const T By = U0[1] - U1[1];
				const T Cx = V0[0] - U0[0];
				const T Cy = V0[1] - U0[1];
				const T f = Ay * Bx - Ax * By;
				const T d = By * Cx - Bx * Cy;
				const T e = Ax * Cy - Ay * Cx;

				// the two lines are collinear.
				if (std::abs(f) < DEplision && std::abs(e) < DEplision) {
					//intersect at endpoint
					if ((V0 - U0).LengthSquared() < DEplision*DEplision || (V0 - U1).LengthSquared() < DEplision*DEplision)
						return 1;
					else if ((V1 - U0).LengthSquared() < DEplision*DEplision || (V1 - U1).LengthSquared() < DEplision*DEplision)
						return 1;

					double  l = Ax * Ax + Ay * Ay;
					double t0 = -(Cx*Ax + Cy*Ay);
					const T sx = U1[0] - V0[0];
					const T sy = U1[1] - V0[1];
					double t1 = (sx * Ax + Ay * sy) / l;
					//overlap
					if (std::min(t0, t1) < (1+DEplision) && std::min(t0, t1) > -DEplision)
						return 2;
					//disjoint
					return 0;
				}
				//then the two lines are parallel and non-intersecting.
				else if (std::abs(f) < DEplision && std::abs(e) > DEplision)
					return 0;
				//If 0 ≤ t ≤ 1 and 0 ≤ u ≤ 1, the two line segments meet at the point p + t r = q + u s
				else if (std::abs(f) > DEplision) {
                    T u = d/f;
                    T v = e/f;
                    
                    if( std::abs(u) < DEplision || std::abs(v) < DEplision ||std::abs(u-1.0) < DEplision||std::abs(v-1.0) < DEplision)
                        return 1;
                    else if (u < 1.0+DEplision && u > 0.0-DEplision && v>0.0-DEplision && v<1.0+DEplision)
                        return 3;
                    else
                        return 0;
					/*// 0 < t < 1 and 0 < u < 1
                    if ((f > 0.0 && d > 0.0 && d < f) || (f < 0.0 && d < 0.0 && d > f)){
                        if(( e > 0.0 && e < f) ||(e < 0.0 && e>f))
                            return 3;
                        return 0;
                    }
					// t==0 && (u ==0||u==1)
					else if (std::abs(d) < DEplision && (std::abs(e) < DEplision || std::abs(e - f) < DEplision))
						return 1;
					// t==1 && (u ==0||u==1)
					else if (std::abs(d - f) < DEplision && (std::abs(e) < DEplision || std::abs(e - f) < DEplision))
						return 1;
					// u==0 && (t ==0||t==1)
					else if (std::abs(e) < DEplision && (std::abs(d) < DEplision || std::abs(d - f) < DEplision))
						return 1;
					// u==1 && (t ==0||t==1)
					else if (std::abs(e - f) < DEplision && (std::abs(d) < DEplision || std::abs(d - f) < DEplision))
						return 1;
					//end point intersect with another point which is not end point
					else
						return 3;*/
				}
				else//Otherwise, the two line segments are not parallel but do not intersect
					return 0;
				return 0;
		}
	protected:
		typedef DVector<T, 2> PolygonPoint;
		std::vector<PolygonPoint > m_polygonPoints;
	};

};

#endif// 2018/09/17
