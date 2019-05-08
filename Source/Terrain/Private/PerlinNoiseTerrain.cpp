// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/PerlinNoiseTerrain.h"
#include "Public/PerlinNoise.h"
#include "Public/MeshGenerator.h"
#include "Public/MeshData.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "CoreUObject/Public/UObject/Class.h"
#include "Classes/Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"

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
	meshData = meshGenerator->GenerateMesh(mDivisions, mSize);

	static ConstructorHelpers::FObjectFinder<UMaterial> ConcreteMaterialAsset(TEXT("Material'/Game/StarterContent/Materials/M_Concrete_Poured.M_Concrete_Poured'"));
	if (ConcreteMaterialAsset.Succeeded())
	{
		auto* MaterialInstance = UMaterialInstanceDynamic::Create(ConcreteMaterialAsset.Object, ConcreteMaterialAsset.Object);
		mesh->SetMaterial(0, MaterialInstance);
	}
	mesh->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));
}

// Called when the game starts or when spawned
void APerlinNoiseTerrain::BeginPlay()
{
	Super::BeginPlay();
	double start = FPlatformTime::Seconds();
	GenerateTerrain();
	double end = FPlatformTime::Seconds();
	double TimeElapsed = end - start;
	//UE_LOG(LogActor, Warning, TEXT("Tick Timer: %.6f Start: %.6f"), end - start, start);

	UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain generation time: %f"), TimeElapsed + meshGenerator->MeshGenerationTime);
	//TODO Generate bigger terrain with more vertices and measure time for each generation
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

