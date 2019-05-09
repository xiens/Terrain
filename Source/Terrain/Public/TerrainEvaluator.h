// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class APerlinNoiseTerrain;
/**
 * 
 */
class TERRAIN_API TerrainEvaluator
{
public:
	TerrainEvaluator();
	~TerrainEvaluator();

private:
	//Evaluation parameters
	//generation time
	float GenerationTime = 0;
	//number of vertices
	int32 NumOfVertices = 0;
	//handle
	FTimerHandle InputTimerHandle;
	bool bTimerExpired = true;

	void ToggleTimer();

	//UFUNCTION()
	void SpawnTerrainActors();
};
