// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/DiamondSquareTerrain.h"

// Sets default values
ADiamondSquareTerrain::ADiamondSquareTerrain()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;

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
	mesh->AddLocalRotation(FRotator(0, 0, 90));
	//CreateQuad();
}

void ADiamondSquareTerrain::PostLoad()
{
	Super::PostLoad();
	GenerateTerrain();
	//mesh->AddLocalRotation(FRotator(0, 0, 90));
	//CreateQuad();
}

void ADiamondSquareTerrain::CreateQuad()
{
	TArray<FVector> Vertices;
	Vertices.Add(FVector(0, 0, 0));
	Vertices.Add(FVector(0, 100, 0));
	Vertices.Add(FVector(100, 0, 0));
	Vertices.Add(FVector(100, 100, 0));

	TArray<int32> Triangles;
	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(3);
	Triangles.Add(3);
	Triangles.Add(2);
	Triangles.Add(0);

	TArray<FVector> Normals;
	Normals.Add(FVector(1, 0, 0));
	Normals.Add(FVector(1, 0, 0));
	Normals.Add(FVector(1, 0, 0));
	Normals.Add(FVector(1, 0, 0));

	TArray<FVector2D> UV0;
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(10, 0));
	UV0.Add(FVector2D(0, 10));
	UV0.Add(FVector2D(10, 10));


	TArray<FProcMeshTangent> Tangents;
	Tangents.Add(FProcMeshTangent(0, 1, 0));
	Tangents.Add(FProcMeshTangent(0, 1, 0));
	Tangents.Add(FProcMeshTangent(0, 1, 0));
	Tangents.Add(FProcMeshTangent(0, 1, 0));

	TArray<FLinearColor> VertexColors;
	VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));

	mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

void ADiamondSquareTerrain::GenerateTerrain()
{

	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	Vertices.AddZeroed(mVertCount);
	UV0.AddZeroed(mVertCount);
	Triangles.AddZeroed(mDivisions * mDivisions * 6);

	//mVerts = new FVector[mVertCount];
	//FVector2D *uvs = new FVector2D[mVertCount];
	//int * tris = new int[mDivisions * mDivisions * 6];

	float halfSize = mSize * 0.5f;
	float divisionSize = mSize / mDivisions;

	int triOffset = 0;

	for (int x = 0; x <= mDivisions; x++)
	{
		for (int y = 0; y <= mDivisions; y++)
		{
			Vertices[x * (mDivisions + 1) + y] = FVector(-halfSize + y * divisionSize, 0.0f, halfSize - x * divisionSize);
			UV0[x * (mDivisions + 1) + y] = FVector2D((float)x / mDivisions, (float)y / mDivisions);
			//UE_LOG(LogTemp, Warning, TEXT("uv[%d]: (%s)"), x * (mDivisions + 1) + y, *uvs[x * (mDivisions + 1) + y].ToString())
			if (x < mDivisions && y < mDivisions)//don't need triangles when at corners
			{
				int topLeft = x * (mDivisions + 1) + y;
				int bottomLeft = (x + 1) * (mDivisions + 1) + y;

				Triangles[triOffset] = topLeft;
				Triangles[triOffset + 1] = topLeft + 1;
				Triangles[triOffset + 2] = bottomLeft + 1;

				Triangles[triOffset + 3] = topLeft;
				Triangles[triOffset + 4] = bottomLeft + 1;
				Triangles[triOffset + 5] = bottomLeft;

				triOffset += 6;
			}
		}
	}

	//DiamondSquare initializing corners
	Vertices[0].Y = FMath::FRandRange(-mHeight, mHeight);
	Vertices[mDivisions].Y = FMath::FRandRange(-mHeight, mHeight);
	Vertices[mVertCount - 1].Y = FMath::FRandRange(-mHeight, mHeight);
	Vertices[mVertCount - 1 - mDivisions].Y = FMath::FRandRange(-mHeight, mHeight);

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

	for (int i = 0; i < mVertCount; i++)
	{
		Normals.Add(FVector(1, 0, 0));
		Tangents.Add(FProcMeshTangent(0, 1, 0));
		VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	}

	mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

void ADiamondSquareTerrain::DiamondSquare(int row, int col, int size, float offset)
{
	int halfSize = (int)(size * 0.5f);
	int topLeft = row * (mDivisions + 1) + col;
	int bottomLeft = (row + size) * (mDivisions + 1) + col;

	int mid = (int)(row + halfSize) * (mDivisions + 1) + (int)(col + halfSize);

	Vertices[mid].Y = (Vertices[topLeft].Y + Vertices[topLeft + size].Y + Vertices[bottomLeft].Y + Vertices[bottomLeft + size].Y)*0.25f + FMath::FRandRange(-offset, offset);

	Vertices[topLeft + halfSize].Y = (Vertices[topLeft].Y + Vertices[topLeft + size].Y + Vertices[mid].Y) / 3 + FMath::FRandRange(-offset, offset);
	Vertices[mid - halfSize].Y = (Vertices[topLeft].Y + Vertices[bottomLeft].Y + Vertices[mid].Y) / 3 + FMath::FRandRange(-offset, offset);
	Vertices[mid + halfSize].Y = (Vertices[topLeft + size].Y + Vertices[mid].Y + Vertices[bottomLeft + size].Y) / 3 + FMath::FRandRange(-offset, offset);
	Vertices[bottomLeft + halfSize].Y = (Vertices[bottomLeft].Y + Vertices[bottomLeft + size].Y + Vertices[mid].Y) / 3 + FMath::FRandRange(-offset, offset);

}

