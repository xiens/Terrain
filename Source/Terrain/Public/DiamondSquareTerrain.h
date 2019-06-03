// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "SimpleDelaunay.hpp"
#include "DiamondSquareTerrain.generated.h"


class MeshGenerator;
class MeshData;

UCLASS()
class TERRAIN_API ADiamondSquareTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADiamondSquareTerrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& transform) override;

public:

	UPROPERTY(EditAnywhere)
	int mDivisions = 8; //number of faces
	UPROPERTY(EditAnywhere)
	float mSize = 100.0f;
	UPROPERTY(EditAnywhere)
	float mHeight = 15.0f;
	UPROPERTY(EditAnywhere)
	float mRoughness = 0.4;

	void GenerateTerrain2(int Divisions,  float Height, float Size, float Roughness);

	void SetTerrainParams(int Divisions, float Height, float Size, float Roughness);

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * mesh;

	//Mesh components
	MeshGenerator * meshGenerator = NULL;
	MeshData * meshData = NULL;

	void PostActorCreated();
	void PostLoad();
	void GenerateTerrain();
	void DiamondSquare(int row, int col, int size, float offset);

	int mVertCount = (mDivisions + 1) * (mDivisions + 1);

};
