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
	MeshData * meshData = NULL;

	MeshData * GenerateMesh(int mVertCount, int mSize);
};
