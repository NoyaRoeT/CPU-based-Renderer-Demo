#pragma once
#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<int> Mesh::SplitVertexIndices(const std::string& str, char delim)
{
	std::stringstream ss(str);
	std::vector<int> indices;
	std::string idx;
	while (std::getline(ss, idx, delim))
	{
		if (idx == "") indices.push_back(-1);
		else indices.push_back(std::stoi(idx) - 1);
	}
	return indices;
}

void Mesh::LoadObj(const char* filename)
{
	std::ifstream file(filename);
	std::string line, key, x, y, z, w;
	char delim = '/';
	while (!file.eof())
	{
		std::getline(file, line);
		std::istringstream is(line);
		is >> key;
	
		if (key == "v")
		{
			Vector3f v;
			is >> v.x >> v.y >> v.z;
			/*std::cout << v.x << ' ' << v.y << ' ' << v.z << std::endl;
			std::cin.get();*/
 			Vertices.push_back(v);
		}

		if (key == "vn")
		{
			Vector3f n;
			is >> n.x >> n.y >> n.z;
			VertexNormals.push_back(n);
		}

		if (key == "vt")
		{
			Vector3f uv;
			is >> uv.x >> uv.y;
			VertexUVs.push_back(uv);

		}

		if (key == "f")
		{
			//We're expecting 1 for each vertex. Hence, we first split them into these 3/4 portions.
			is >> x >> y >> z >> w;

			// For triangle geometry
			if (w == "")
			{
				//Then, subdivide them further into the 3 vertex attributes.
				std::vector<int> v0 = SplitVertexIndices(x, delim);
				std::vector<int> v1 = SplitVertexIndices(y, delim);
				std::vector<int> v2 = SplitVertexIndices(z, delim);

				std::vector<std::vector<int> > indices;
				indices.push_back(std::vector<int>{-1, -1, -1});
				indices.push_back(std::vector<int>{-1, -1, -1});
				indices.push_back(std::vector<int>{-1, -1, -1});

				for (int i = 0; i != v0.size(); ++i)
				{
					indices[i][0] = v0[i];
					indices[i][1] = v1[i];
					indices[i][2] = v2[i];

				}
				

				VertexIndices.push_back(indices[0]);
				TextureIndices.push_back(indices[1]);
				NormalIndices.push_back(indices[2]);
			}
			// For polygon mesh of 4 vertices
			else
			{
				//Total no. of triangles for a polygon with n vertices = n - 2.
				//Hence, we prepare 2 vectors for each triangle
				std::vector<int> f1Vertices;
				std::vector<int> f2Vertices;

				std::vector<int> f1Normals;
				std::vector<int> f2Normals;

				std::vector<int> f1Textures;
				std::vector<int> f2Textures;

				//Split into 4 vertices
				std::vector<int> v0 = SplitVertexIndices(x, delim);
				std::vector<int> v1 = SplitVertexIndices(y, delim);
				std::vector<int> v2 = SplitVertexIndices(z, delim);
				std::vector<int> v3 = SplitVertexIndices(w, delim);

				// Separate into vectors of each attribute
				std::vector<std::vector<int> > indices;
				indices.push_back(std::vector<int>{-1, -1, -1, -1});
				indices.push_back(std::vector<int>{-1, -1, -1, -1});
				indices.push_back(std::vector<int>{-1, -1, -1, -1});

				for (int i = 0; i != v0.size(); ++i)
				{
					indices[i][0] = v0[i];
					indices[i][1] = v1[i];
					indices[i][2] = v2[i];
					indices[i][3] = v3[i];
				}

				// Create ccw oriented triangles by connecting vertices in the form v[0], v[n+1], v[n+2]
				f1Vertices.push_back(indices[0][0]);
				f1Vertices.push_back(indices[0][1]);
				f1Vertices.push_back(indices[0][2]);

				f1Normals.push_back(indices[2][0]);
				f1Normals.push_back(indices[2][1]);
				f1Normals.push_back(indices[2][2]);

				f1Textures.push_back(indices[1][0]);
				f1Textures.push_back(indices[1][1]);
				f1Textures.push_back(indices[1][2]);

				f2Vertices.push_back(indices[0][0]);
				f2Vertices.push_back(indices[0][2]);
				f2Vertices.push_back(indices[0][3]);

				f2Normals.push_back(indices[2][0]);
				f2Normals.push_back(indices[2][2]);
				f2Normals.push_back(indices[2][3]);

				f2Textures.push_back(indices[1][0]);
				f2Textures.push_back(indices[1][2]);
				f2Textures.push_back(indices[1][3]);

				VertexIndices.push_back(f1Vertices);
				VertexIndices.push_back(f2Vertices);
				TextureIndices.push_back(f1Textures);
				TextureIndices.push_back(f2Textures);
				NormalIndices.push_back(f1Normals);
				NormalIndices.push_back(f2Normals);
			}
		}
	}
	nFaces = VertexIndices.size();
	ComputeFaceNormals();
	if (!VertexNormals.size()) ComputeVertexNormals();
	ComputeTangentSpace();
}

void Mesh::ComputeFaceNormals()
{
	std::vector<Vector3f> fNormals;
	Vector3f Verts[3];
	for (int i = 0; i != nFaces; ++i)
	{
		Verts[0] = Vertices[VertexIndices[i][0]];
		Verts[1] = Vertices[VertexIndices[i][1]];
		Verts[2] = Vertices[VertexIndices[i][2]];
		
		Vector3f Normal = Normalize(Cross(Verts[1] - Verts[0], Verts[2] - Verts[0]));
		fNormals.push_back(Normal);
	}

	FaceNormals = fNormals;
}

void Mesh::ComputeTangentSpace()
{
	int NumVertices = Vertices.size();
	// Temporary storage for tangents and bitangents for ea vertex and initialise them to 0
	// Calculated tangent vectors will be added into a single vector to give the average tangent vector
	std::vector<Vector3f> Tangents(NumVertices, Vector3f(0.0f, 0.0f, 0.0f));
	std::vector<Vector3f > BiTangents(NumVertices, Vector3f(0.0f, 0.0f, 0.0f));

	// Calculate tangent and bitangent for ea triangle and add to all 3 vertices
	for (int i = 0; i != nFaces; ++i)
	{
		const std::vector<int>& vIndex = VertexIndices[i];
		const std::vector<int>& tIndex = TextureIndices[i];

		Vector3f e1 = Vertices[vIndex[1]] - Vertices[vIndex[0]];
		Vector3f e2 = Vertices[vIndex[2]] - Vertices[vIndex[0]];
		float x1 = VertexUVs[tIndex[1]].x - VertexUVs[tIndex[0]].x;
		float x2 = VertexUVs[tIndex[2]].x - VertexUVs[tIndex[0]].x;
		float y1 = VertexUVs[tIndex[1]].y - VertexUVs[tIndex[0]].y;
		float y2 = VertexUVs[tIndex[2]].y - VertexUVs[tIndex[0]].y;

		float InvDet = 1 / (x1 * y2 - x2 * y1);
		Vector3f t = (e1 * y2 - e2 * y1) * InvDet;
		Vector3f b = (-x2 * e1 + x1 * e2) * InvDet;

		Tangents[vIndex[0]] += t;
		Tangents[vIndex[1]] += t;
		Tangents[vIndex[2]] += t;
		BiTangents[vIndex[0]] += b;
		BiTangents[vIndex[1]] += b;
		BiTangents[vIndex[2]] += b;
	}

	// Orthonormalize each tangent and calculate handedness
	for (int i = 0; i != NumVertices; ++i)
	{
		Vector3f t = Tangents[i];
		Vector3f b = BiTangents[i];
		Vector3f n = VertexNormals[i];
		t = Normalize(Reject(t, n));
		if (Dot(Cross(t, b), n) < 0) VertexTangents.push_back(-t);
		else VertexTangents.push_back(t);
	}
}

void Mesh::ComputeVertexNormals()
{
	std::vector<Vector3f> Normals(Vertices.size(), Vector3f(0.0f, 0.0f, 0.0f));
	for (int i = 0; i != nFaces; ++i)
	{
		Vector3f fNormal = FaceNormals[i];
		std::vector<int> vIndices = VertexIndices[i];
		Normals[vIndices[0]] += fNormal;
		Normals[vIndices[1]] += fNormal;
		Normals[vIndices[2]] += fNormal;
		NormalIndices[i][0] = vIndices[0];
		NormalIndices[i][1] = vIndices[1];
		NormalIndices[i][2] = vIndices[2];
	}

	for (int i = 0; i != Vertices.size(); ++i)
	{
		VertexNormals.push_back(Normalize(Normals[i]));
	}
}