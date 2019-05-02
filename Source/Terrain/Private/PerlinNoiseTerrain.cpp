// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/PerlinNoiseTerrain.h"
#include "Public/PerlinNoise.h"

// Sets default values
APerlinNoiseTerrain::APerlinNoiseTerrain()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("PerlinNoiseTerrain"));
	RootComponent = mesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APerlinNoiseTerrain::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APerlinNoiseTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
//	PerlinNoise pn(seed);
//	FVector2D *octaveOffsets = new FVector2D[octaves];
//	for (int octave = 0; octave < octaves; octave++)
//	{
//		float offsetX = FMath::FRandRange(-100000, 100000) + offset.X;
//		float offsetY = FMath::FRandRange(-100000, 100000) + offset.Y;
//		octaveOffsets[octave] = FVector2D(offsetX, offsetY);
//	}
//
//	int j = 0; //height index
//
//	for (int i = 0; i < mVertCount; i++)
//	{
//		float amplitude = 1;
//		float frequency = 1;
//		float noiseHeight = 0;
//		float PerlinValue = 0;
//
//		for (int octave = 0; octave < octaves; octave++)
//		{
//			float xCoord = Vertices[i].X  * scale * frequency + octaveOffsets[octave].X;
//			float yCoord = Vertices[i].Y  * scale * frequency + octaveOffsets[octave].Y;
//
//			PerlinValue = pn.noise(xCoord, yCoord, 0.8)* mHeight;
//
//			noiseHeight += PerlinValue * amplitude;
//			amplitude *= persistance; //decreases each octave
//			frequency *= lacunarity;
//		}
//
//		if (noiseHeight > maxNoiseHeight)
//		{
//			maxNoiseHeight = noiseHeight;
//		}
//		else if (noiseHeight < minNoiseHeight)
//		{
//			minNoiseHeight = noiseHeight;
//		}
//
//		Vertices[j].Z = PerlinValue;
//		j++;
//	}
//
//	mesh->UpdateMeshSection_LinearColor(0, Vertices, Normals, UV0, VertexColors, Tangents);
}

void APerlinNoiseTerrain::PostActorCreated()
{
	Super::PostActorConstruction();
	CreateQuad();
	GenerateTerrain();
}

void APerlinNoiseTerrain::PostLoad()
{
	Super::PostLoad();
}

void APerlinNoiseTerrain::CreateQuad()
{
	Vertices.AddZeroed(mVertCount);
	UV0.AddZeroed(mVertCount);
	Triangles.AddZeroed(mDivisions * mDivisions * 6);

	float halfSize = mSize * 0.5f;
	float divisionSize = mSize / mDivisions;

	int triOffset = 0;

	for (int x = 0; x <= mDivisions; x++)
	{
		for (int y = 0; y <= mDivisions; y++)
		{
			Vertices[x * (mDivisions + 1) + y] = FVector(-halfSize + y * divisionSize, halfSize - x * divisionSize, 0.0f);
			UV0[x * (mDivisions + 1) + y] = FVector2D((float)x / mDivisions, (float)y / mDivisions);
		
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

	for (int i = 0; i < mVertCount; i++)
	{
		Normals.Add(FVector(1, 0, 0));
		Tangents.Add(FProcMeshTangent(0, 1, 0));
		VertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	}
}

void APerlinNoiseTerrain::GenerateTerrain()
{
	//System.Random prng = new System.Random(seed);
	//TODO use seed 
	PerlinNoise pn(seed);
	FVector2D *octaveOffsets = new FVector2D[octaves];
	for (int octave = 0; octave < octaves; octave++)
	{
		float offsetX = FMath::FRandRange(-100000, 100000) + offset.X;
		float offsetY = FMath::FRandRange(-100000, 100000) + offset.Y;
		octaveOffsets[octave] = FVector2D(offsetX, offsetY);
	}

	int j = 0; //height index

	for (int i = 0; i < mVertCount; i++)
	{
		float amplitude = 1;
		float frequency = 1;
		float noiseHeight = 0;
		float PerlinValue = 0;

		for (int octave = 0; octave < octaves; octave++)
		{
			float xCoord = Vertices[i].X  * scale * frequency + octaveOffsets[octave].X;
			float yCoord = Vertices[i].Y  * scale * frequency + octaveOffsets[octave].Y;

			PerlinValue = pn.noise(xCoord, yCoord, 0.8)* mHeight;

			noiseHeight += PerlinValue * amplitude;
			amplitude *= persistance; //decreases each octave
			frequency *= lacunarity;
		}

		if (noiseHeight > maxNoiseHeight)
		{
			maxNoiseHeight = noiseHeight;
		}
		else if (noiseHeight < minNoiseHeight)
		{
			minNoiseHeight = noiseHeight;
		}

		Vertices[j].Z = PerlinValue;
		j++;
	}
	
	mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);

	//// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

