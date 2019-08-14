// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class UProceduralMeshComponent;

struct TerrainRegion {
	FString name;
	float height;
	FLinearColor color;

	TerrainRegion(FString _name, float _height, FLinearColor _color) {
		name = _name;
		height = _height;
		color = _color;
	}
};

/**
 * 
 */
class TERRAIN_API TerrainType
{
public:
	TerrainType();
	~TerrainType();

	/*THIS METHOD CAN ONLY BE USED IN CONSTRUCTOR OF CLASS (uses ConstructorHelpers)*/
	void SetMeshMaterial(UProceduralMeshComponent *mesh);

	TArray<FLinearColor> AssignRegionToHeights(const TArray<FLinearColor> &VerticesColors, const TArray<float> &VerticesHeights);

private:
	TArray<TerrainRegion> TerrainRegions;

	



};
