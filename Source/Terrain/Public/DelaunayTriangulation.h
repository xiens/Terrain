// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "delabella.h"
#include "SimpleDelaunay.hpp"
#include "ProceduralMeshComponent.h"
#include <vector>
#include "DelaunayTriangulation.generated.h"

class Point2;
class PerlinNoise;
class MeshGenerator;
class MeshData;
class TerrainType;

UCLASS()
class TERRAIN_API ADelaunayTriangulation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADelaunayTriangulation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Terrain parameters
	UPROPERTY(EditAnywhere)
	float mHeight = 15;
	UPROPERTY(EditAnywhere)
	int mDivisions = 8; //number of faces 
	UPROPERTY(EditAnywhere)
	float mSize = 300;
	//float size = 5;
	//int gridSize = 10;
	float maxNoiseHeight = 10000;
	float minNoiseHeight = -10000;

	//Perlin Noise parameters
	UPROPERTY(EditAnywhere, Category = "Noise parameters")
	float scale = 20.0f;
	UPROPERTY(EditAnywhere, Category = "Noise parameters")
	int octaves = 10;
	UPROPERTY(EditAnywhere, Category = "Noise parameters", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float persistance = 1; 
	UPROPERTY(EditAnywhere, Category = "Noise parameters")
	float lacunarity = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Noise parameters")
	FVector2D offset = FVector2D(0, 0);
	UPROPERTY(EditAnywhere, Category = "Noise parameters")
	int seed = 5;

	void GenerateTerrain2(float Divisions, float Height, float Size, float Lacunarity, float Scale, float Persistance);

	void SetTerrainParams(float Divisions, float Height, float Size, float Lacunarity, float Scale, float Persistance);

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * mesh;

	int mVertCount = (mDivisions + 1) * (mDivisions + 1);

	UPROPERTY(EditAnywhere)
	int Points = (mDivisions + 1) * (mDivisions + 1);//AT LEAST 20

	float Width = 100.0f;
	float Height = 100.0f;

	int triangleInd = 0;

	//Mesh components
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	//Mesh components
	MeshGenerator * meshGenerator = NULL;
	MeshData * meshData = NULL;
	TerrainType * terrainType = NULL;

	void PostActorCreated();
	void PostLoad();
	void CreateSmoothlyShadedQuad(); //vertices don't repeat

	std::vector<int> CalculateTriangleIndices(std::vector<DelaBella_Triangle> triangles, std::vector<DelaBella_Vertex> &triangleVertices);

	bool IsVertexDefined(std::vector<DelaBella_Vertex> triangleVertices,
		DelaBella_Vertex v,
		std::vector<int>indices, int &oldIndex);

	void GenerateTerrain();

	void GenerateTerrainForTriangulation();

	void TestDelaunay3D();

	FVector2D CalculateUV(FVector Vertices);
};
