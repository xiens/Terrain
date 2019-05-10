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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Terrain parameters
	UPROPERTY(EditAnywhere)
	float mHeight = 35;
	UPROPERTY(EditAnywhere)
		int mDivisions = 63; //number of faces 
	UPROPERTY(EditAnywhere)
		float mSize = 100;
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

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * mesh;

	int mVertCount = (mDivisions + 1) * (mDivisions + 1);

	UPROPERTY(EditAnywhere)
	int Points = 4225; //AT LEAST 20

	float Width = 500.0f;
	float Height = 500.0f;

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

	void PostActorCreated();
	void PostLoad();
	void CreateQuad();


	std::vector<int> CalculateTriangleIndices(std::vector<DelaBella_Triangle> triangles, std::vector<DelaBella_Vertex> &triangleVertices);

	bool IsVertexDefined(std::vector<DelaBella_Vertex> triangleVertices,
		DelaBella_Vertex v,
		std::vector<int>indices, int &oldIndex);

	void CreateTriangle(int i);

	void GenerateTerrain();

	void GenerateTerrainForTriangulation();

	void TestDelaunay3D();
};
