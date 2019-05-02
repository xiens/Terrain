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
	int mDivisions = 32; //number of faces
	UPROPERTY(EditAnywhere)
	float mSize = 100;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float mHeight = 15;

	//Perlin Noise parameters
	UPROPERTY(EditAnywhere, Category = "Noise parameters")
	float scale = 20.0f;
	UPROPERTY(EditAnywhere, Category = "Noise parameters")
	int octaves = 10;
	UPROPERTY(EditAnywhere, Category = "Noise parameters", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float persistance = 1; //Should be from 0 to 1
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
