// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	int mDivisions = 64; //number of faces
	UPROPERTY(EditAnywhere)
	float mSize = 100;
	UPROPERTY(EditAnywhere)
	float mHeight = 25;

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
