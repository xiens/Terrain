// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/MeshData.h"

MeshData::MeshData()
{
}


MeshData::~MeshData()
{
}

MeshData::MeshData(int mDivisions)
{
	triangleIndex = 0;
	int mVertCount = (mDivisions + 1)*(mDivisions + 1);

	Vertices.AddZeroed(mVertCount);
	UV0.AddZeroed(mVertCount);
	Triangles.AddZeroed(mDivisions * mDivisions * 6);

	for (int i = 0; i < mVertCount; i++)
	{
		Normals.Add(FVector(1, 0, 0));
		Tangents.Add(FProcMeshTangent(0, 1, 0));
		VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	}
}

void MeshData::AddTriangle(int a, int b, int c)
{
	Triangles[triangleIndex] = a;
	Triangles[triangleIndex + 1] = b;
	Triangles[triangleIndex + 2] = c;
	triangleIndex += 3;
}
