// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "delabella.h"
#include <vector>
#include "ProceduralMeshComponent.h"
#include "BreakingGlass.generated.h"

class Point2;

UCLASS()
class TERRAIN_API ABreakingGlass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakingGlass();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Terrain parameters
	UPROPERTY(EditAnywhere)
	float mHeight = 1.2;
	//float size = 5;
	//int gridSize = 10;
	float maxNoiseHeight = 10000;
	float minNoiseHeight = -10000;

	//Perlin Noise parameters
	UPROPERTY(EditAnywhere)
	float scale = 20.0f;
	UPROPERTY(EditAnywhere)
	int octaves = 10;
	float persistance = 1;
	UPROPERTY(EditAnywhere)
	float lacunarity = 0.7f;
	UPROPERTY(EditAnywhere)
	FVector2D offset = FVector2D(0,0);
	UPROPERTY(EditAnywhere)
	int seed = 5;

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * mesh;

	UPROPERTY(EditAnywhere)
	int Points = 1600; //AT LEAST 20

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

	void PostActorCreated();
	void PostLoad();
	void CreateQuad();


	std::vector<int> CalculateTriangleIndices(std::vector<DelaBella_Triangle> triangles, std::vector<DelaBella_Vertex> &triangleVertices);

	bool IsVertexDefined(std::vector<DelaBella_Vertex> triangleVertices,
		DelaBella_Vertex v,
		std::vector<int>indices, int &oldIndex);

	void CreateTriangle(int i);

	void GenerateTerrain();


};
