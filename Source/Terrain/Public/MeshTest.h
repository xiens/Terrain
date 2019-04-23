// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "MeshTest.generated.h"

UCLASS()
class TERRAIN_API AMeshTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshTest();

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



private:
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent * mesh;

	void PostActorCreated();
	void PostLoad();
	void CreateQuad();
	void GenerateTerrain();
	void DiamondSquare(int row, int col, int size, float offset);

	TArray<FVector> mVerts;
	int mVertCount;
};
