// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

/**
 * 
 */
class TERRAIN_API MeshData
{
public:
	MeshData();
	~MeshData();

	//Mesh components
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	int triangleIndex = 0;

	MeshData(int mDivisions);

	void AddTriangle(int a, int b, int c);

};
