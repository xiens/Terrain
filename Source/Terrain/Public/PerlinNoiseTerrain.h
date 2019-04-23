// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PerlinNoiseTerrain.generated.h"

class PerlinNoise;

UCLASS()
class TERRAIN_API APerlinNoiseTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerlinNoiseTerrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	int mDivisions = 128; //number of faces
	UPROPERTY(EditAnywhere)
	float mSize = 100;
	UPROPERTY(EditAnywhere)
	float mHeight = 50;

	//Perlin Noise parameters
	UPROPERTY(EditAnywhere)
	float scale = 20.0f;
	UPROPERTY(EditAnywhere)
	int octaves = 10;
	float persistance = 1;
	UPROPERTY(EditAnywhere)
	float lacunarity = 0.7f;
	UPROPERTY(EditAnywhere)
	FVector2D offset = FVector2D(0, 0);
	UPROPERTY(EditAnywhere)
	int seed = 5;

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * mesh;

	int mVertCount = (mDivisions + 1) * (mDivisions + 1);
	float maxNoiseHeight = 10000;
	float minNoiseHeight = -10000;

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
	void GenerateTerrain();
};
