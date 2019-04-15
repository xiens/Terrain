// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "delabella.h"
#include <vector>
#include "ProceduralMeshComponent.h"
#include "BreakingGlass.generated.h"

//struct MyPoint
//{
//	float x;
//	float y;
//
//	MyPoint() {};
//	MyPoint(float _x, float _y) :x(_x), y(_y) {};
//};
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

	void DestroyQuad();

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * mesh;

	UPROPERTY(EditAnywhere)
	int POINTS = 30; //AT LEAST 20

	float width = 80.0f;
	float height = 150.0f;
	
	int count = 0;
	//Mesh components
	TArray<FVector> vertices;
	TArray<int32> Triangles;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	void PostActorCreated();
	void PostLoad();
	void DelabellaTest();
	void CreateQuad();


	std::vector<int> CalculateTriangleIndices(std::vector<DelaBella_Triangle> triangles, std::vector<DelaBella_Vertex> &triangleVertices);

	bool IsVertexDefined(std::vector<DelaBella_Vertex> triangleVertices,
		DelaBella_Vertex v,
		std::vector<int>indices, int &oldIndex);

	void CreateTriangle(TArray<FVector> &vertices, TArray<int32> &triangleIndices, TArray<FVector> &normals,
		TArray<FVector2D> &UV0,
		TArray<FProcMeshTangent> &tangents,
		TArray<FLinearColor> &vertexColors, int i);




};
