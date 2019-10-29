//
//  DamonsDelaunay.h
//  DPolygonTest
//
//  Created by ydb on 2018/10/21.
//  Copyright © 2018 Damons. All rights reserved.
//

#ifndef DamonsDelaunay_h
#define DamonsDelaunay_h

#include "DamonsVector.h"
#include <random>
#include <map>
#include <numeric>
#include <fstream>
#include <functional>

using namespace DMath;

template < class T = double>
class DDelaunay{
public:
    using DPoint2 = DVector<T,2>;
    class Edge
    {
    public:
        Edge( DPoint2 *p1, DPoint2 *p2) : p1(p1), p2(p2), isBad(false) {};
        Edge(const Edge &e) : p1(e.p1), p2(e.p2), isBad(false) {};
        DPoint2 *p1;
        DPoint2 *p2;
        bool isBad;
    public:
        bool isEqual(Edge &ed){
            bool e1 =  (*p1 - *ed.p1).LengthSquared() < DEplision;
            bool e2 =  (*p1 - *ed.p2).LengthSquared() < DEplision;
            bool e3 =  (*p2 - *ed.p1).LengthSquared() < DEplision;
            bool e4 =  (*p2 - *ed.p2).LengthSquared() < DEplision;
            
            return (e1&&e4) || (e2&&e3);
        }
        void print(){
            std::cout<<"pt1 : "<<p1->x()<<","<<p1->y()<<std::endl;
            std::cout<<"pt2 : "<<p2->x()<<","<<p2->y()<<std::endl;
           
        }
    };
    class Triangle
    {
    public:
        Triangle( DPoint2 *_p1,  DPoint2 *_p2,  DPoint2 *_p3)
        :    p1(_p1), p2(_p2), p3(_p3),
        e1(_p1, _p2), e2(_p2, _p3), e3(_p3, _p1), isBad(false)
        {}
        
        bool containsVertex(const DPoint2 *v) const
        {
            // return p1 == v || p2 == v || p3 == v;
            T l1 =  (*p1 - *v).LengthSquared();
            T l2 =  (*p2 - *v).LengthSquared();
            T l3 =  (*p3 - *v).LengthSquared();
            
            return l1< DEplision*DEplision || l2< DEplision*DEplision || l3 < DEplision*DEplision;
        }
        
        bool circumCircleContains(const DPoint2 *v) const
        {
            const T ab = p1->LengthSquared();
            const T cd = p2->LengthSquared();
            const T ef = p3->LengthSquared();
            
            const T circum_x = (ab * (p3->y() - p2->y()) + cd * (p1->y() - p3->y()) + ef * (p2->y() - p1->y())) / (p1->x() * (p3->y() - p2->y()) + p2->x() * (p1->y() - p3->y()) + p3->x() * (p2->y() - p1->y()));
            
            const T circum_y = (ab * (p3->x() - p2->x()) + cd * (p1->x() - p3->x()) + ef * (p2->x() - p1->x())) / (p1->y()* (p3->x() - p2->x()) + p2->y() * (p1->x() - p3->x()) + p3->y() * (p2->x() - p1->x()));
            
            const DPoint2 circum(circum_x*0.5, circum_y*0.5);
            const T circum_radius = (*p1 - circum).Length();
            const T dist = (*v - circum).Length();
            return dist <= circum_radius;
        }
        void print(){
            std::cout<<"pt1 : "<<p1->x()<<","<<p1->y()<<std::endl;
            std::cout<<"pt2 : "<<p2->x()<<","<<p2->y()<<std::endl;
            std::cout<<"pt3 : "<<p3->x()<<","<<p3->y()<<std::endl;
        }
        DPoint2 *p1;
        DPoint2 *p2;
        DPoint2 *p3;
        Edge e1;
        Edge e2;
        Edge e3;
        bool isBad;
    };

    
public:
    DDelaunay(){}
    DDelaunay(std::vector<DPoint2 > &points){
        m_points.resize(points.size()+3);
        m_points[0] = points[0];
        m_points[1] = points[1];
        m_points[2] = points[2];
        std::copy(points.begin(),points.end(),m_points.begin()+3);
    }
    ~DDelaunay(){
        std::vector<DPoint2 >().swap(m_points);
        std::vector<Triangle >().swap(m_triangles);
        std::vector<Edge >().swap(m_edges);
        m_pairs.clear();
    }
public:
    void test(){
        m_points.push_back(DPoint2(0,0));
        m_points.push_back(DPoint2(5,5*std::sqrt(5.0)));
        m_points.push_back(DPoint2(10,0));
        Triangle t(&m_points[0],&m_points[1],&m_points[2]);
        DPoint2 p(11,1);
        t.circumCircleContains(&p);
    }
    void RandomlizePoints(unsigned int _ptnumber){
        std::uniform_real_distribution<double > generator(1,1000.0);
        std::random_device rd;
        std::mt19937 seed(rd());
        auto dice = std::bind(generator,seed);
        _ptnumber+=3;
        m_points.resize(_ptnumber);
        for(int i = 0; i < _ptnumber;++i){
            T x = dice();
            T y = dice();
            m_points[i] = DPoint2(x,y);
        }
    }
    
protected:
    void CalculateSuperTriangle(){
        T min_x =  std::numeric_limits<T>::max();
        T min_y =  std::numeric_limits<T>::max();
        T max_x =  std::numeric_limits<T>::lowest();
        T max_y =  std::numeric_limits<T>::lowest();
        
        for (auto &p:m_points) {
            if( p.x() < min_x)
                min_x = p.x();
            if( p.y() < min_y)
                min_y = p.y();
            if( p.x() > max_x)
                max_x = p.x();
            if( p.y() > max_y)
                max_y = p.y();
        }

		const T dx = max_x - min_x;
		const T dy = max_y - min_y;
		const T deltaMax = std::max(dx, dy);
		const T midx = 0.5*(max_x + min_x);
		const T midy = 0.5*(max_y + min_y);

        m_points[0] = DPoint2(midx - 20 * deltaMax, midy - deltaMax);
        m_points[1] = DPoint2(midx, midy + 20 * deltaMax);
        m_points[2] = DPoint2(midx + 20 * deltaMax, midy - deltaMax);
    }
public:
    void DelaunayTriangulate(){
        CalculateSuperTriangle();
        m_triangles.push_back(Triangle(&m_points[0],&m_points[1],&m_points[2]));
        for(int i = 3;i < m_points.size();++i){
            m_pairs[&m_points[i]] = i-3;
            std::vector<Edge > _polygon;
            for(auto &tri : m_triangles){
                if(tri.circumCircleContains(&m_points[i])){
                    tri.isBad = true;
                    _polygon.push_back(tri.e1);
                    _polygon.push_back(tri.e2);
                    _polygon.push_back(tri.e3);
                }
            }
            m_triangles.erase(std::remove_if(m_triangles.begin(),m_triangles.end(), [](Triangle &t){return t.isBad;}), end(m_triangles));

            for(int  e1 = 0; e1 < _polygon.size(); ++e1)
            {
                for(int  e2 = e1+1; e2 < _polygon.size(); ++e2)
                {
                    auto &edge1 = _polygon[e1];
                    auto &edge2 = _polygon[e2];

                    if(edge1.isEqual(edge2))
                    {
                        edge1.isBad = true;
                        edge2.isBad = true;
                    }
                }
            }
            
            _polygon.erase(std::remove_if(_polygon.begin(), _polygon.end(), [](Edge &e){
                return e.isBad;
            }), end(_polygon));

            for(const auto &e : _polygon){
                m_triangles.push_back(Triangle(e.p1, e.p2, &m_points[i]));
            }
        }
        
        m_triangles.erase(std::remove_if(m_triangles.begin(), m_triangles.end(), [&](Triangle &t){
            return t.containsVertex(&m_points[0]) || t.containsVertex(&m_points[1]) || t.containsVertex(&m_points[2]);
        }), end(m_triangles));
        
        for(const auto &t : m_triangles)
        {
            m_edges.push_back(t.e1);
            m_edges.push_back(t.e2);
            m_edges.push_back(t.e3);
        }
    }

public:
    void PrintPoints(std::string _filename){
        std::ofstream m_out_strm(_filename);
        m_out_strm.setf(std::ios::fixed);
        m_out_strm.width(10);
        T z = 0;
		for (int i = 3; i < m_points.size(); ++i) {
			DPoint2 &pt = m_points[i];
            m_out_strm<<pt.x()<<"  "<<pt.y()<<"  "<<z<<std::endl;
        }
        m_out_strm.close();
    }
    
    void PrintTriangleToOff(std::string _filename){
        std::ofstream m_out_strm(_filename);
        m_out_strm<<"OFF"<<std::endl;
		m_out_strm << m_points.size() - 3<< "  " << m_triangles.size() << " " << 0 << std::endl;

        m_out_strm.setf(std::ios::fixed);
        m_out_strm.width(10);
        
        T z = 0;
		for (int i = 3; i < m_points.size(); ++i) {
			DPoint2 &pt = m_points[i];
            m_out_strm<<pt.x()<<"  "<<pt.y()<<"  "<<z<<std::endl;
        }
        int num = 3;
        for (auto &tri:m_triangles) {
            unsigned int ind1 = m_pairs[ tri.p1 ];
            unsigned int ind2 = m_pairs[ tri.p2 ];
            unsigned int ind3 = m_pairs[ tri.p3 ];
            m_out_strm<<num<<" "<<ind1<<" "<<ind2<<"  "<<ind3<<std::endl;
        }
        m_out_strm.close();
    }
protected:
    std::vector<DPoint2 > m_points;
    std::vector<Triangle > m_triangles;
    std::vector<Edge > m_edges;
    std::map<DPoint2*,unsigned int > m_pairs;
};
#endif /* DamonsDelaunay_h */
