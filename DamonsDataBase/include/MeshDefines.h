#ifndef _MESHDEFINES_H_
#define _MESHDEFINES_H_

#include <iostream>
#include <vector>
#include <map>

namespace DMeshLib {

#define __BUILD_HALFEDGE_TOPO__

	using data_type = double;
	using index_type = unsigned int;


	class DamonsHalfEdge;
	class DamonsFace;

	/*!
	 * \class DamonsVertex
	 *
	 * \brief basic defines of vertex
	 *
	 * \author Damons
	 * \date ʮ�� 2019
	 */
	class DamonsVertex 
	{
	public:
		unsigned int id;
		data_type x;
		data_type y;
		data_type z;
		int edge_out;// �Ӵ˵㿪ʼ�����еİ���е�����һ��(������һ������Ա������еı�)
	public:
		DamonsVertex() :x(0.0), y(0.0), z(0.0) { edge_out = -1; }
		DamonsVertex(data_type _x, data_type _y, data_type _z = 0.0) :x(_x), y(_y), z(_z) { edge_out = -1; }
		~DamonsVertex() { edge_out = -1; }

	};

	/*!
	 * \class DamonsHalfEdge
	 *
	 * \brief halfedge class
	 *
	 * \author Damons
	 * \date ʮ�� 2019
	 */
	class DamonsHalfEdge
	{
	public:
		int start_vert;  // ��ߵĿ�ʼ�Ķ���
		int pair;		 // ��ߵĶԱߣ�oppositely oriented adjacent half-edge �� 
		int face;		 // λ�ڰ����ߵ��棨face the half-edge borders)
		int next;		 // ��ߵ���һ���ߣ�next half-edge around the face��
		int prev;		 // ��ߵ�ǰһ���ߣ�prev half-edge around the face��
	public:
		DamonsHalfEdge() :start_vert(-1), pair(-1), face(-1), next(-1), prev(-1){}
		~DamonsHalfEdge() {
			start_vert = -1; pair = -1; face = -1; next = -1; next = -1; prev = -1;
		}
	};

	/*!
	 * \class DamonsFace
	 *
	 * \brief 
	 *
	 * \author Damons
	 * \date ʮ�� 2019
	 */
	class DamonsFace {
	public:
		int edge;// one of the half-edges bordering the face
		int id;

		std::vector<index_type > point_ids;// all the face's vertex index
	public:
		DamonsFace() :edge(-1) {
			//number_vertex = 0;
		}
		~DamonsFace() {
			edge = -1;
			std::vector<index_type >().swap(point_ids);
			point_ids.shrink_to_fit();
		}
	};
}

#endif