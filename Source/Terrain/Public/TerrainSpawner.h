// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainSpawner.generated.h"

class APerlinNoiseTerrain;
class ADiamondSquareTerrain;
class ADelaunayTriangulation;

UCLASS()
class TERRAIN_API ATerrainSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrainSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnTerrainActors();
	void SpawnPerlinNoiseAtLocation(int TerrainSamplesNum, FString ParameterToChange, float ChangeRate, FVector &StartLoc, bool log = true);
	void SpawnDiamondSquareAtLocation(int TerrainSamplesNum, FString ParameterToChange, float ChangeRate, FVector &StartLoc, bool log = true);
	void SpawnDelaunayAtLocation(int TerrainSamplesNum, FString ParameterToChange, float ChangeRate, FVector &StartLoc, bool log = true);

	void SpawnPerlinNoiseTerrain(int Divisions, float Size, float Height, float Lacunarity, float Scale, float Persistance, FVector TerrainLoc);
	void SpawnDiamondSquareTerrain(int Divisions, float Size, float Height, float Roughness, FVector TerrainLoc);
	void SpawnDelaunayTerrain(int Divisions, float Size, float Height, float Lacunarity, float Scale, float Persistance, FVector TerrainLoc);

	int Divisions = 10;
	float Size = 1000.0f;
	float Height = 25.0f;
	
	//PerlinNoise
	float Lacunarity = 0.5f;
	float Persistance = 1.0f;
	float Scale = 5.0f;
	
	//DiamondSquare
	float Roughness = 0.55f;


	FVector MoveInX = FVector(1.7f*Size, 0, 0);
	FVector MoveInY = FVector(0, 1.7f*Size, 0);
};
