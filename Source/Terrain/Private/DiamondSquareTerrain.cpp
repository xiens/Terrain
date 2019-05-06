// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/DiamondSquareTerrain.h"
#include "Public/MeshGenerator.h"
#include "Public/MeshData.h"

// Sets default values
ADiamondSquareTerrain::ADiamondSquareTerrain()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;

	meshGenerator = new MeshGenerator();
	meshData = meshGenerator->GenerateMesh(mDivisions, mSize);
}

// Called when the game starts or when spawned
void ADiamondSquareTerrain::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADiamondSquareTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADiamondSquareTerrain::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateTerrain();
}

void ADiamondSquareTerrain::PostLoad()
{
	Super::PostLoad();
}

void ADiamondSquareTerrain::GenerateTerrain()
{
	//DiamondSquare initializing corners
	meshData->Vertices[0].Z = FMath::FRandRange(-mHeight, mHeight);
	meshData->Vertices[mDivisions].Z = FMath::FRandRange(-mHeight, mHeight);
	meshData->Vertices[mVertCount - 1].Z = FMath::FRandRange(-mHeight, mHeight);
	meshData->Vertices[mVertCount - 1 - mDivisions].Z = FMath::FRandRange(-mHeight, mHeight);

	int iterations = (int)FMath::LogX(2, mDivisions); //changed from log
	int numSquares = 1;
	int squareSize = mDivisions;

	for (int i = 0; i < iterations; i++)
	{
		int row = 0;
		for (int j = 0; j < numSquares; j++)
		{
			int col = 0;
			for (int k = 0; k < numSquares; k++)
			{
				DiamondSquare(row, col, squareSize, mHeight);
				col += squareSize;

			}
			row += squareSize;
		}
		numSquares *= 2;
		squareSize /= 2;
		mHeight *= 0.5f;//u can change this parameter
	}

	mesh->CreateMeshSection_LinearColor(0, meshData->Vertices, meshData->Triangles, meshData->Normals,
		meshData->UV0, meshData->VertexColors, meshData->Tangents, true);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

void ADiamondSquareTerrain::DiamondSquare(int row, int col, int size, float offset)
{
	int halfSize = (int)(size * 0.5f);
	int topLeft = row * (mDivisions + 1) + col;
	int bottomLeft = (row + size) * (mDivisions + 1) + col;

	int mid = (int)(row + halfSize) * (mDivisions + 1) + (int)(col + halfSize);

	meshData->Vertices[mid].Z = (meshData->Vertices[topLeft].Z + meshData->Vertices[topLeft + size].Z + meshData->Vertices[bottomLeft].Z + meshData->Vertices[bottomLeft + size].Z)*0.25f + FMath::FRandRange(-offset, offset);

	meshData->Vertices[topLeft + halfSize].Z = (meshData->Vertices[topLeft].Z + meshData->Vertices[topLeft + size].Z + meshData->Vertices[mid].Z) / 3 + FMath::FRandRange(-offset, offset);
	meshData->Vertices[mid - halfSize].Z = (meshData->Vertices[topLeft].Z + meshData->Vertices[bottomLeft].Z + meshData->Vertices[mid].Z) / 3 + FMath::FRandRange(-offset, offset);
	meshData->Vertices[mid + halfSize].Z = (meshData->Vertices[topLeft + size].Z + meshData->Vertices[mid].Z + meshData->Vertices[bottomLeft + size].Z) / 3 + FMath::FRandRange(-offset, offset);
	meshData->Vertices[bottomLeft + halfSize].Z = (meshData->Vertices[bottomLeft].Z + meshData->Vertices[bottomLeft + size].Z + meshData->Vertices[mid].Z) / 3 + FMath::FRandRange(-offset, offset);
}

