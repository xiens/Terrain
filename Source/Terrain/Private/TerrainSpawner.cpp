// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainSpawner.h"
#include "PerlinNoiseTerrain.h"
#include "DiamondSquareTerrain.h"
#include "DelaunayTriangulation.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ATerrainSpawner::ATerrainSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UStaticMeshComponent *CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	RootComponent = CollisionMesh;
	USphereComponent *Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	Sphere->InitSphereRadius(100.0f);
	Sphere->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATerrainSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnTerrainActors();
}

// Called every frame
void ATerrainSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATerrainSpawner::SpawnTerrainActors() {

	int TerrainSamplesNum = 3;
	float ChangeRate = 10.0f;

	//Starting parameters:
	int   StartDivisions = Divisions;
	float StartSize = Size;
	float StartHeight = Height;
	float StartLacunarity = Lacunarity;
	float StartPersistance = Persistance;
	float StartScale = Scale;

	FTransform SpawnLocAndRotation;
	SpawnLocAndRotation = FTransform(GetActorTransform());
	FVector CurrentLocation = GetActorLocation();

	SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Height", ChangeRate, CurrentLocation);
	SpawnDelaunayAtLocation(TerrainSamplesNum, "Height", ChangeRate, CurrentLocation);
	SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Lacunarity", 0.1f, CurrentLocation);
	SpawnDelaunayAtLocation(TerrainSamplesNum, "Lacunarity", 0.1f, CurrentLocation);
	SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Persistance", -0.1f, CurrentLocation);
	SpawnDelaunayAtLocation(TerrainSamplesNum, "Persistance", -0.1f, CurrentLocation);
	SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Scale", ChangeRate, CurrentLocation);
	SpawnDelaunayAtLocation(TerrainSamplesNum, "Scale", ChangeRate, CurrentLocation);
	//SpawnDiamondSquareAtLocation(TerrainSamplesNum, "Height", ChangeRate, CurrentLocation);
	//SpawnDiamondSquareAtLocation(TerrainSamplesNum, "Roughness", 0.2f, CurrentLocation);



}

void ATerrainSpawner::SpawnPerlinNoiseAtLocation(int TerrainSamplesNum, FString ParameterToChange, float ChangeRate, FVector &StartLoc)
{
	FVector Location = StartLoc;

	float Parameter = Height;
	for (int i = 0; i < TerrainSamplesNum; i++)
	{
		if (ParameterToChange == "Height") {
			Parameter = Height;
			SpawnPerlinNoiseTerrain(Divisions, Size, Parameter+i*ChangeRate, Lacunarity, Scale, Persistance, Location);
			UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain height = %f"), Parameter + i * ChangeRate)

		}
		else if(ParameterToChange == "Lacunarity") {
			Parameter = Lacunarity;
			SpawnPerlinNoiseTerrain(Divisions, Size, Height, Parameter + i * ChangeRate, Scale, Persistance, Location);
			UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain lacunarity = %f"), Parameter + i * ChangeRate)

		}
		else if (ParameterToChange == "Persistance") {
			Parameter = Persistance;
			SpawnPerlinNoiseTerrain(Divisions, Size, Height, Lacunarity, Scale, Parameter + i * ChangeRate, Location);
			UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain persistance = %f"), Parameter + i * ChangeRate)
		}
		else if (ParameterToChange == "Scale") {
			Parameter = Scale;
			SpawnPerlinNoiseTerrain(Divisions, Size, Height, Lacunarity, Parameter + i * ChangeRate, Persistance, Location);
			UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain scale = %f"), Parameter + i * ChangeRate)

		}
		Location += MoveInX;
	}
	StartLoc += MoveInY;
}

void ATerrainSpawner::SpawnDiamondSquareAtLocation(int TerrainSamplesNum, FString ParameterToChange, float ChangeRate, FVector & StartLoc)
{
	FVector Location = StartLoc;

	float Parameter = Height;
	for (int i = 0; i < TerrainSamplesNum; i++)
	{
		if (ParameterToChange == "Height") {
			Parameter = Height;
			SpawnDiamondSquareTerrain(Divisions, Parameter + i * ChangeRate, Roughness, Location);
			UE_LOG(LogTemp, Warning, TEXT("Diamond Square Terrain height = %f"), Parameter + i * ChangeRate)

		}
		else if (ParameterToChange == "Roughness") {
			Parameter = Roughness;
			SpawnDiamondSquareTerrain(Divisions, Height, Parameter + i * ChangeRate, Location);
			UE_LOG(LogTemp, Warning, TEXT("Diamond Square Terrain roughness = %f"), Parameter + i * ChangeRate)

		}
		else if (ParameterToChange == "Divisions") {
			UE_LOG(LogTemp, Warning, TEXT("Diamond Square Not yet xfloolz = %f"))
		}

		Location += MoveInX;
	}
	StartLoc += MoveInY;
}

void ATerrainSpawner::SpawnDelaunayAtLocation(int TerrainSamplesNum, FString ParameterToChange, float ChangeRate, FVector & StartLoc)
{
	FVector Location = StartLoc;

	float Parameter = Height;
	for (int i = 0; i < TerrainSamplesNum; i++)
	{
		if (ParameterToChange == "Height") {
			Parameter = Height;
			SpawnDelaunayTerrain(Divisions, Size, Parameter + i * ChangeRate, Lacunarity, Scale, Persistance, Location);
			//UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain height = %f"), Parameter + i * ChangeRate)

		}
		else if (ParameterToChange == "Lacunarity") {
			Parameter = Lacunarity;
			SpawnDelaunayTerrain(Divisions, Size, Height, Parameter + i * ChangeRate, Scale, Persistance, Location);
			//UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain lacunarity = %f"), Parameter + i * ChangeRate)

		}
		else if (ParameterToChange == "Persistance") {
			Parameter = Persistance;
			SpawnDelaunayTerrain(Divisions, Size, Height, Lacunarity, Scale, Parameter + i * ChangeRate, Location);
			//UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain persistance = %f"), Parameter + i * ChangeRate)
		}
		else if (ParameterToChange == "Scale") {
			Parameter = Scale;
			SpawnDelaunayTerrain(Divisions, Size, Height, Lacunarity, Parameter + i * ChangeRate, Persistance, Location);
			//UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain scale = %f"), Parameter + i * ChangeRate)

		}
		Location += MoveInX;
	}
	StartLoc += MoveInY;
}

void ATerrainSpawner::SpawnPerlinNoiseTerrain(int Divisions, float Size, float Height, float Lacunarity, float Scale, float Persistance, FVector TerrainLoc)
{
	APerlinNoiseTerrain* MyActor = GetWorld()->SpawnActorDeferred<APerlinNoiseTerrain>(APerlinNoiseTerrain::StaticClass(), FTransform(TerrainLoc));
	MyActor->GenerateTerrain2(Height, Lacunarity, Scale, Persistance);
	MyActor->FinishSpawning(FTransform(TerrainLoc));

}

void ATerrainSpawner::SpawnDiamondSquareTerrain(int Divisions, float Height, float Roughness, FVector TerrainLoc)
{
	ADiamondSquareTerrain* MyActor = GetWorld()->SpawnActorDeferred<ADiamondSquareTerrain>(ADiamondSquareTerrain::StaticClass(), FTransform(TerrainLoc));
	MyActor->GenerateTerrain2(Divisions, Height, Roughness);
	MyActor->FinishSpawning(FTransform(TerrainLoc));
}

void ATerrainSpawner::SpawnDelaunayTerrain(int Divisions, float Size, float Height, float Lacunarity, float Scale, float Persistance, FVector TerrainLoc)
{
	ADelaunayTriangulation* MyActor = GetWorld()->SpawnActorDeferred<ADelaunayTriangulation>(ADelaunayTriangulation::StaticClass(), FTransform(TerrainLoc));
	MyActor->GenerateTerrain2(Height, Lacunarity, Scale, Persistance);
	MyActor->FinishSpawning(FTransform(TerrainLoc));
}

