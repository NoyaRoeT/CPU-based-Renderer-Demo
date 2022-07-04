#pragma once
#include <vector>
#include "Vector.h"
#include <string>



struct Mesh
{
public:
	Mesh() = default;
	void LoadObj(const char* Filename);

public:
	int nFaces = 0;

	std::vector<Vector3f> Vertices;
	std::vector<Vector3f> VertexNormals;
	std::vector<Vector3f> VertexUVs;
	std::vector<Vector3f> VertexTangents;

	std::vector<Vector3f> FaceNormals;
	std::vector<std::vector<int> > VertexIndices;
	std::vector<std::vector<int> > NormalIndices;
	std::vector<std::vector<int> > TextureIndices;

	void ComputeFaceNormals();
	void ComputeTangentSpace();
	void ComputeVertexNormals();

private:
	std::vector<int> SplitVertexIndices(const std::string& str, char delim);
};