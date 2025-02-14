// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/PerlinNoiseTerrain.h"
#include "Public/PerlinNoise.h"
#include "Public/MeshGenerator.h"
#include "Public/MeshData.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "CoreUObject/Public/UObject/Class.h"
#include "Classes/Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"
#include "TerrainType.h"

// Sets default values
APerlinNoiseTerrain::APerlinNoiseTerrain()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("PerlinNoiseTerrain"));
	RootComponent = mesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshGenerator = new MeshGenerator();
	terrainType = new TerrainType();
	if(terrainType)
		terrainType->SetMeshMaterial(mesh);

	//mesh->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));
}

// Called when the game starts or when spawned
void APerlinNoiseTerrain::BeginPlay()
{
	Super::BeginPlay();

	//TODO Generate bigger terrain with more vertices and measure time for each generation
}



void APerlinNoiseTerrain::OnConstruction(const FTransform & transform)
{
	Super::OnConstruction(transform);

	mVertCount = (mDivisions + 1) * (mDivisions + 1);
	double start = FPlatformTime::Seconds();

	if(meshGenerator)
		meshData = meshGenerator->GenerateMesh(mDivisions, mSize);
	
	
	GenerateTerrain2(mDivisions, mHeight, mSize, lacunarity, scale, persistance);

	double end = FPlatformTime::Seconds();
	double TimeElapsed = end - start;

	UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain generation time: %f"), TimeElapsed + meshGenerator->MeshGenerationTime);
}

// Called every frame
void APerlinNoiseTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void APerlinNoiseTerrain::PostActorCreated()
{
	Super::PostActorConstruction();


}

void APerlinNoiseTerrain::PostLoad()
{
	Super::PostLoad();

}

void APerlinNoiseTerrain::GenerateTerrain()
{




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
			float xCoord = meshData->Vertices[i].X  * scale * frequency + octaveOffsets[octave].X;
			float yCoord = meshData->Vertices[i].Y  * scale * frequency + octaveOffsets[octave].Y;

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

		meshData->Vertices[j].Z = PerlinValue;
		j++;
	}
	
	mesh->CreateMeshSection_LinearColor(0, meshData->Vertices, meshData->Triangles, meshData->Normals, 
		meshData->UV0, meshData->VertexColors, meshData->Tangents, true);

	//// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

void APerlinNoiseTerrain::GenerateTerrain2(float Divisions, float Height, float Size, float Lacunarity, float Scale, float Persistance)
{
	TArray <FVector> temp;
	//temp.AddZeroed(meshData->Vertices.Num());

	/*for (size_t i = 0; i < meshData->Vertices.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Meshdata vertices: %s"), *(meshData->Vertices[i]).ToString())
		temp.Add(meshData->Vertices[i]);
	}*/
	//mDivisions = Divisions;
	//scale = Scale;
	//lacunarity = Lacunarity;
	//mHeight = Height;
	//mSize = Size;
	//persistance = Persistance;


	for (size_t i = 0; i < meshData->Vertices.Num(); i++)
	{
		temp.Add(meshData->Vertices[i]);
	}

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
			float xCoord = temp[i].X  * scale * frequency + octaveOffsets[octave].X;
			float yCoord = temp[i].Y  * scale * frequency + octaveOffsets[octave].Y;

			PerlinValue = pn.noise(xCoord, yCoord, 0.8);//* mHeight;

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

		//if (noiseHeight > maxNoiseHeight) PerlinValue = maxNoiseHeight;
		//if (noiseHeight < minNoiseHeight) PerlinValue = minNoiseHeight;

		temp[j].Z = noiseHeight;
		j++;
	}

	for (int i = 0; i < mVertCount; i++)
	{

		temp[i].Z = FMath::GetMappedRangeValueUnclamped(FVector2D(minNoiseHeight, 0), FVector2D(maxNoiseHeight, 0), temp[i].Z)*mHeight;
	}
	TArray<float> VertHeights;
	for (auto &vert : temp) {
		VertHeights.Add(vert.Z);
	}
	meshData->VertexColors = terrainType->AssignRegionToHeights(meshData->VertexColors, VertHeights);

	mesh->CreateMeshSection_LinearColor(0, temp, meshData->Triangles, meshData->Normals,
		meshData->UV0, meshData->VertexColors, meshData->Tangents, true);
	

	//// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);



	delete meshData;
}

void APerlinNoiseTerrain::SetTerrainParams(float Divisions, float Height, float Size, float Lacunarity, float Scale, float Persistance)
{
	mDivisions = Divisions;
	mHeight = Height;
	mSize = Size;
	lacunarity = Lacunarity;
	scale = Scale;
	persistance = Persistance;
}

