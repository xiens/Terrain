// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class MeshData;
/**
 * 
 */
class TERRAIN_API MeshGenerator
{
public:
	MeshGenerator();
	~MeshGenerator();

	double MeshGenerationTime = 0;

	MeshData * meshData = NULL;

	MeshData * GenerateMesh(int mDivisions, int mSize);

	MeshData * GenerateMesh2(int mDivisions, int mSize, int offset);

	MeshData * GenerateMeshForTriangulation(int mDivisions, int mSize);
};
