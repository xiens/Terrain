// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/MeshGenerator.h"
#include "Public/MeshData.h"

MeshGenerator::MeshGenerator()
{
}

MeshGenerator::~MeshGenerator()
{
}

MeshData * MeshGenerator::GenerateMesh(int mDivisions, int mSize)
{

	meshData = new MeshData(mDivisions);

	float halfSize = mSize * 0.5f;
	float divisionSize = mSize / mDivisions;

	int triOffset = 0;

	for (int x = 0; x <= mDivisions; x++)
	{
		for (int y = 0; y <= mDivisions; y++)
		{
			meshData->Vertices[x * (mDivisions + 1) + y] = FVector(-halfSize + y * divisionSize, halfSize - x * divisionSize, 0.0f);
			//meshData->UV0[x * (mDivisions + 1) + y] = FVector2D((float)x / mDivisions, (float)y / mDivisions);
			meshData->UV0[x * (mDivisions + 1) + y] =  FVector2D(1 - ((float)y + mSize / 2.0f) / mSize, ((float)x + mSize / 2.0f) / mSize);
			if (x < mDivisions && y < mDivisions)//don't need triangles when at corners
			{
				int topLeft = x * (mDivisions + 1) + y;
				int bottomLeft = (x + 1) * (mDivisions + 1) + y;

				meshData->AddTriangle(topLeft, topLeft + 1, bottomLeft + 1);
				meshData->AddTriangle(topLeft, bottomLeft+1, bottomLeft);
			}
		}
	}

	return meshData;
}

MeshData * MeshGenerator::GenerateMesh2(int mDivisions, int mSize, int offset)
{

	meshData = new MeshData(mDivisions);

	float halfSize = mSize * 0.5f;
	float divisionSize = mSize / mDivisions;

	int triOffset = 0;

	for (int x = 0; x <= mDivisions; x++)
	{
		for (int y = 0; y <= mDivisions; y++)
		{
			meshData->Vertices[x * (mDivisions + 1) + y] = FVector(-halfSize + y * divisionSize, offset+halfSize - x * divisionSize, 0.0f);
			meshData->UV0[x * (mDivisions + 1) + y] = FVector2D((float)x / mDivisions, (float)y / mDivisions);
			//meshData->UV0[x * (mDivisions + 1) + y] = FVector2D(1 - ((float)y + mSize / 2.0f) / mSize, ((float)x + mSize / 2.0f) / mSize);


			if (x < mDivisions && y < mDivisions)//don't need triangles when at corners
			{
				int topLeft = x * (mDivisions + 1) + y;
				int bottomLeft = (x + 1) * (mDivisions + 1) + y;

				meshData->AddTriangle(topLeft, topLeft + 1, bottomLeft + 1);
				meshData->AddTriangle(topLeft, bottomLeft + 1, bottomLeft);
			}
		}
	}

	return meshData;
}

MeshData * MeshGenerator::GenerateMeshForTriangulation(int mDivisions, int mSize)
{

	meshData = new MeshData(mDivisions);

	float halfSize = mSize * 0.5f;
	float divisionSize = mSize / mDivisions;

	int triOffset = 0;

	for (int x = 0; x <= mDivisions; x++)
	{
		for (int y = 0; y <= mDivisions; y++)
		{
			//meshData->Vertices[x * (mDivisions + 1) + y] = FVector(-halfSize + y * divisionSize, halfSize - x * divisionSize, 0.0f);
			meshData->UV0[x * (mDivisions + 1) + y] = FVector2D((float)x / mDivisions, (float)y / mDivisions);
		}
	}

	return meshData;
}
