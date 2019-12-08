
#include "..\include\3DSFilter.h"

//System
#include <string.h>

namespace DamonsIO {

	bool TDSFilter::canLoadExtension(const std::string& upperCaseExt) const
	{
		return (upperCaseExt == "STL");
	}

	DAMONS_FILE_ERROR TDSFilter::saveToFile(DMeshLib::ModelObject* entity, const std::string& filename, const SaveParameters& parameters)
	{
		if (!entity)
			return CC_FERR_BAD_ARGUMENT;

		if (!entity->isA(DMeshLib::DB_TYPES::MESH))
			return CC_FERR_BAD_ENTITY_TYPE;

		DMeshLib::MeshModel* mesh = static_cast<DMeshLib::MeshModel*>(entity);
		if (!mesh || mesh->getTriangleNumber() == 0 || mesh->getPointsNumber() == 0)
		{
			return CC_FERR_NO_SAVE;
		}

		//check max polygons
		if (mesh->getPointsNumber() > MAX_POLYGON_3DS)
			return CC_FERR_BAD_ARGUMENT;

		Lib3dsFile *file = lib3ds_file_new();//creates new file
		Lib3dsMesh *_dmesh = lib3ds_mesh_new(mesh->getName().c_str());//creates a new mesh with mesh's name "mesh"		

		unsigned vertCount = mesh->getPointsNumber();
		unsigned triCount = mesh->getTriangleNumber();

		{
			lib3ds_mesh_new_point_list(_dmesh, vertCount);// set number of vertexs

			DMeshLib::data_type x, y, z;
			for (unsigned i = 0; i < vertCount; ++i)
			{
				mesh->getPoint(i, x, y, z);
				Lib3dsPoint point;
				point.pos[0] = x;
				point.pos[1] = y;
				point.pos[2] = z;

				_dmesh->pointL[i] = point;
			}
		}
		{
			lib3ds_mesh_new_face_list(_dmesh, triCount);//set number of faces
			DMeshLib::index_type id1, id2, id3;
			for (unsigned i = 0; i < triCount; ++i)
			{
				mesh->getTriangleIndex(i, id1, id2, id3);
				Lib3dsFace face;
				face.points[0] = (Lib3dsWord)id1;
				face.points[1] = (Lib3dsWord)id2;
				face.points[2] = (Lib3dsWord)id3;

				face.flags = 65535;
				face.smoothing = 10;
				face.user.p = (void*)malloc(1);
				strcpy(face.material,"");

				_dmesh->faceL[i] = face;
			}
		}
		
		lib3ds_file_insert_mesh(file, _dmesh);//inserts the Mesh into file

		Lib3dsNode *node = lib3ds_node_new_object();//creates a new node
		strcpy(node->name, mesh->getName().c_str());
		node->parent_id = LIB3DS_NO_PARENT;
		lib3ds_file_insert_node(file, node);//inserts the node into file

		bool result = lib3ds_file_save(file, filename.c_str()); //saves the file
		if (!result)
			return CC_FERR_WRITING;

		return CC_FERR_NO_ERROR;
	}

  
	DAMONS_FILE_ERROR TDSFilter::loadFile(const std::string& filename, DMeshLib::ModelObject *&container, LoadParameters& parameters)
	{

		DMeshLib::MeshModel* mesh = new DMeshLib::MeshModel("3ds_mesh");
		DAMONS_FILE_ERROR error = CC_FERR_NO_ERROR;
		//////////////////////////////////////////////////////////////////////////
		Lib3dsFile *file = lib3ds_file_load(filename.c_str());
		if (!file)
		{
			return CC_FERR_READING;
		}

		if (!file->nodes && file->meshes)
		{
			Lib3dsMesh *mesh;
			Lib3dsNode *node;

			for (mesh = file->meshes; mesh != NULL; mesh = mesh->next)
			{
				node = lib3ds_node_new_object();
				strcpy(node->name, mesh->name);
				node->parent_id = LIB3DS_NO_PARENT;
				lib3ds_file_insert_node(file, node);
			}
		}

		if (!file->nodes) {
			lib3ds_file_free(file);
			return CC_FERR_BAD_ENTITY_TYPE;
		}

		lib3ds_file_eval(file, 0);

		bool singleLayer = true;
		if (file->nodes->next)
			singleLayer = false;

		if (!singleLayer)
		{
			for (Lib3dsNode *p = file->nodes; p != 0; p = p->next)
			{
				error = loadFileImpl(file, p, mesh, parameters);
			}
		}
		else
			error = loadFileImpl(file,nullptr,mesh, parameters);

		if (error != CC_FERR_NO_ERROR)
		{
			return CC_FERR_MALFORMED_FILE;
		}

		mesh->refreshBoundBox();
		mesh->build();
		container = mesh;

		return CC_FERR_NO_ERROR;
	}


	DAMONS_FILE_ERROR TDSFilter::loadFileImpl(Lib3dsFile *file,
											Lib3dsNode *node,
											DMeshLib::MeshModel* mesh,
											LoadParameters& parameters) {

		DAMONS_FILE_ERROR error = CC_FERR_NO_ERROR;

		if (!node)
		{
			for (Lib3dsNode *p = file->nodes; p != 0; p = p->next) {
				error = ReadNode(file, p, mesh);
				if (error != CC_FERR_NO_ERROR)
					return error;
			}
		}
		else
		{
			error = ReadNode(file, node, mesh);
			if (error != CC_FERR_NO_ERROR)
				return error;
		}

		return error;

	}

	DAMONS_FILE_ERROR TDSFilter::ReadNode(Lib3dsFile *file, Lib3dsNode *node, DMeshLib::MeshModel* _dmesh) {
		DAMONS_FILE_ERROR error = CC_FERR_NO_ERROR;
		assert(file);
		{
			Lib3dsNode *p;
			for (p = node->childs; p != 0; p = p->next) {

				error = ReadNode(file, p, _dmesh);
				if (error != CC_FERR_NO_ERROR)
					return error;
			}
		}

		if (node->type == LIB3DS_OBJECT_NODE)
		{
			if (strcmp(node->name, "$$$DUMMY") == 0)
				return CC_FERR_NO_ERROR;

			Lib3dsMesh * mesh = lib3ds_file_mesh_by_name(file, node->data.object.morph);
			if (mesh == NULL)
				mesh = lib3ds_file_mesh_by_name(file, node->name);

			if (!mesh)
				return CC_FERR_NO_ERROR;

			if (!mesh->user.d)
			{
				//Lib3dsVector *normalL = (Lib3dsVector*)malloc(3 * sizeof(Lib3dsVector)*mesh->faces);

				// Obtain current transformation matrix
				// ------------------------------------
				Lib3dsMatrix matrix;
				//Lib3dsMatrix translatedMatrix;
				Lib3dsMatrix inverseMatrix;

				Lib3dsObjectData *d;
				d = &node->data.object;
				
				// Version for lib3ds-1.3.0
				lib3ds_matrix_copy(matrix, mesh->matrix);
				lib3ds_matrix_copy(inverseMatrix, mesh->matrix);
				lib3ds_matrix_inv(inverseMatrix);
				lib3ds_matrix_translate_xyz(matrix, -d->pivot[0], -d->pivot[1], -d->pivot[2]);
				lib3ds_matrix_mult(matrix, inverseMatrix);

				//lib3ds_mesh_calculate_normals(mesh, normalL);

				for (unsigned v = 0; v < mesh->points; ++v) {
					Lib3dsVector	*p = &mesh->pointL[v].pos;

					Lib3dsVector	transformedP;
					lib3ds_vector_transform(transformedP, matrix, *p);

					DMeshLib::data_type  x =  static_cast<DMeshLib::data_type>(transformedP[0]);
					DMeshLib::data_type  y =  static_cast<DMeshLib::data_type>(transformedP[1]);
					DMeshLib::data_type  z =  static_cast<DMeshLib::data_type>(transformedP[2]);
					
					_dmesh->addPoint(x,y,z);
					
				}

				for (unsigned p = 0; p < mesh->faces; ++p) {
					Lib3dsFace			*f = &mesh->faceL[p];

					DMeshLib::index_type  id1 = static_cast<DMeshLib::index_type>(numVertices + f->points[0]);
					DMeshLib::index_type  id2 = static_cast<DMeshLib::index_type>(numVertices + f->points[1]);
					DMeshLib::index_type  id3 = static_cast<DMeshLib::index_type>(numVertices + f->points[2]);

					_dmesh->addTriangle(id1,id2,id3);

					DMeshLib::data_type  nx = static_cast<DMeshLib::data_type>(f->normal[0]);
					DMeshLib::data_type  ny = static_cast<DMeshLib::data_type>(f->normal[1]);
					DMeshLib::data_type  nz = static_cast<DMeshLib::data_type>(f->normal[2]);

					_dmesh->addFaceNormal(nx, ny, nz);

					++numFaces;
				}

				numVertices += mesh->points;

				//free(normalL);
			}
		}

		return error;
	}
}