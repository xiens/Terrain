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

	int TerrainSamplesNum = 1;
	float ChangeRate = 10.0f;

	//Starting parameters:
	int   StartDivisions = Divisions;
	float StartSize = Size;
	float StartHeight = Height;
	float StartLacunarity = Lacunarity;
	float StartPersistance = Persistance;
	float StartScale = Scale;

	FVector CurrentLocation = GetActorLocation();

	std::vector<double> PerlinNoiseTimes;
	std::vector<double> DelaunayTriangulationTimes;
	std::vector<double> DiamondSquareTimes;

	//SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Height", ChangeRate, CurrentLocation);
	//SpawnDelaunayAtLocation(TerrainSamplesNum, "Height", ChangeRate, CurrentLocation);
	//SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Lacunarity", 0.1f, CurrentLocation);
	//SpawnDelaunayAtLocation(TerrainSamplesNum, "Lacunarity", 0.1f, CurrentLocation);
	//SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Persistance", -0.1f, CurrentLocation);
	//SpawnDelaunayAtLocation(TerrainSamplesNum, "Persistance", -0.1f, CurrentLocation);
	//SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Scale", ChangeRate, CurrentLocation);
	//SpawnDelaunayAtLocation(TerrainSamplesNum, "Scale", ChangeRate, CurrentLocation);
	////UE_LOG(LogTemp, Warning, TEXT(" divisions: %d"), Divisions)


	//SpawnDiamondSquareAtLocation(TerrainSamplesNum, "Height", ChangeRate, CurrentLocation);
	//SpawnDiamondSquareAtLocation(TerrainSamplesNum, "Roughness", 0.2f, CurrentLocation);

		//UE_LOG(LogTemp, Warning, TEXT("Mesh generation time: %f"), MeshGenerationTime);


	//PERLIN NOISE TESTS
	//SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Divisions", 5, CurrentLocation);
	//Divisions = StartDivisions;
	//SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Lacunarity", 0.2f, CurrentLocation);
	//SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Persistance", -0.2f, CurrentLocation);
	//SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Height", 5.0f, CurrentLocation);
	//SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Scale", 10.0f, CurrentLocation);


	//DIAMOND SQUARE TESTS
	//SpawnDiamondSquareAtLocation(TerrainSamplesNum, "Roughness", -0.2f, CurrentLocation);


	//DELAUNAY TRIANGULATION TESTS
	//SpawnDelaunayAtLocation(TerrainSamplesNum, "Height", ChangeRate, CurrentLocation);
	//SpawnDelaunayAtLocation(TerrainSamplesNum, "Lacunarity", 0.1f, CurrentLocation);
	//SpawnDelaunayAtLocation(TerrainSamplesNum, "Persistance", -0.1f, CurrentLocation);
	//SpawnDelaunayAtLocation(TerrainSamplesNum, "Scale", ChangeRate, CurrentLocation);
	/*SpawnDelaunayAtLocation(TerrainSamplesNum, "Divisions", 4, CurrentLocation, true);
	Divisions = StartDivisions;*/

	//SPAWN TIME TESTS
	SpawnPerlinNoiseAtLocation(TerrainSamplesNum, "Divisions", 3, CurrentLocation, true);
	Divisions = StartDivisions;
	/*SpawnDelaunayAtLocation(TerrainSamplesNum, "Divisions", 3, CurrentLocation, true);
	Divisions = StartDivisions;
	SpawnDiamondSquareAtLocation(TerrainSamplesNum, "Divisions", 3, CurrentLocation, true);*/



}

void ATerrainSpawner::SpawnPerlinNoiseAtLocation(int TerrainSamplesNum, FString ParameterToChange, float ChangeRate, FVector &StartLoc, bool log)
{
	FVector Location = StartLoc;

	float Parameter = Height;
	for (int i = 0; i < TerrainSamplesNum; i++)
	{
		if (ParameterToChange == "Height") {
			Parameter = Height;
			SpawnPerlinNoiseTerrain(Divisions, Size, Parameter+i*ChangeRate, Lacunarity, Scale, Persistance, Location);
			if (log) UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain height = %f"), Parameter + i * ChangeRate)

		}
		else if(ParameterToChange == "Lacunarity") {
			Parameter = Lacunarity;
			SpawnPerlinNoiseTerrain(Divisions, Size, Height, Parameter + i * ChangeRate, Scale, Persistance, Location);
			if (log) UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain lacunarity = %f"), Parameter + i * ChangeRate)

		}
		else if (ParameterToChange == "Persistance") {
			Parameter = Persistance;
			SpawnPerlinNoiseTerrain(Divisions, Size, Height, Lacunarity, Scale, Parameter + i * ChangeRate, Location);
			if (log) UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain persistance = %f"), Parameter + i * ChangeRate)
		}
		else if (ParameterToChange == "Scale") {
			Parameter = Scale;
			SpawnPerlinNoiseTerrain(Divisions, Size, Height, Lacunarity, Parameter, Persistance, Location);
			if (log) UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain scale = %f"), Parameter)
				Scale /= 5.0f;
		}
		else if (ParameterToChange == "Divisions") {
			Parameter = Divisions;
			SpawnPerlinNoiseTerrain(Divisions, Size, Height, Lacunarity, Scale, Persistance, Location);
			if (log) UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain number of vertices = %.0f"), (Parameter + 1)*(Parameter + 1))
			Divisions *= 2;

			//if (log) UE_LOG(LogTemp, Warning, TEXT("Perlin Noise Terrain number of vertices = %.0f"), ((Parameter + i * ChangeRate)+1)*((Parameter + i * ChangeRate) + 1))
		}
		Location += MoveInX;
	}
	StartLoc += MoveInY;
	if (log) UE_LOG(LogTemp, Warning, TEXT(""))
}

void ATerrainSpawner::SpawnDiamondSquareAtLocation(int TerrainSamplesNum, FString ParameterToChange, float ChangeRate, FVector & StartLoc, bool log)
{
	FVector Location = StartLoc;

	float Parameter = Height;
	for (int i = 0; i < TerrainSamplesNum; i++)
	{
		if (ParameterToChange == "Height") {
			Parameter = Height;
			SpawnDiamondSquareTerrain(Divisions, Size, Parameter + i * ChangeRate, Roughness, Location);
			if(log) UE_LOG(LogTemp, Warning, TEXT("Diamond Square Terrain height = %f"), Parameter + i * ChangeRate)

		}
		else if (ParameterToChange == "Roughness") {
			Parameter = Roughness;
			SpawnDiamondSquareTerrain(Divisions,  Size, Height, Parameter + i * ChangeRate, Location);
			if (log) UE_LOG(LogTemp, Warning, TEXT("Diamond Square Terrain roughness = %f"), Parameter + i * ChangeRate)

		}
		else if (ParameterToChange == "Divisions") {
			Parameter = Divisions;
			SpawnDiamondSquareTerrain(Divisions, Size, Height, Roughness, Location);
			//if (log) UE_LOG(LogTemp, Warning, TEXT("Diamond Square Terrain number of vertices = %.0f"), (Parameter+1)*(Parameter+1))
			Divisions *= 2;

		}

		Location += MoveInX;
	}
	StartLoc += MoveInY;
	if (log) UE_LOG(LogTemp, Warning, TEXT(""))
}

void ATerrainSpawner::SpawnDelaunayAtLocation(int TerrainSamplesNum, FString ParameterToChange, float ChangeRate, FVector & StartLoc, bool log)
{
	FVector Location = StartLoc;

	float Parameter = Height;
	for (int i = 0; i < TerrainSamplesNum; i++)
	{
		if (ParameterToChange == "Height") {
			Parameter = Height;
			SpawnDelaunayTerrain(Divisions, Size, Parameter + i * ChangeRate, Lacunarity, Scale, Persistance, Location);
			if(log) UE_LOG(LogTemp, Warning, TEXT("Delaunay Triangulation Terrain height = %f"), Parameter + i * ChangeRate)

		}
		else if (ParameterToChange == "Lacunarity") {
			Parameter = Lacunarity;
			SpawnDelaunayTerrain(Divisions, Size, Height, Parameter + i * ChangeRate, Scale, Persistance, Location);
			if (log) UE_LOG(LogTemp, Warning, TEXT("Delaunay Triangulation Terrain lacunarity = %f"), Parameter + i * ChangeRate)

		}
		else if (ParameterToChange == "Persistance") {
			Parameter = Persistance;
			SpawnDelaunayTerrain(Divisions, Size, Height, Lacunarity, Scale, Parameter + i * ChangeRate, Location);
			if (log) UE_LOG(LogTemp, Warning, TEXT("Delaunay Triangulation Terrain persistance = %f"), Parameter + i * ChangeRate)
		}
		else if (ParameterToChange == "Scale") {
			Parameter = Scale;
			SpawnDelaunayTerrain(Divisions, Size, Height, Lacunarity, Parameter + i * ChangeRate, Persistance, Location);
			if (log) UE_LOG(LogTemp, Warning, TEXT("Delaunay Triangulation Terrain scale = %f"), Parameter + i * ChangeRate)
		}
		else if (ParameterToChange == "Divisions") {
			Parameter = Divisions;
			SpawnDelaunayTerrain(Divisions, Size, Height, Lacunarity, Scale, Persistance, Location);
			//if (log) UE_LOG(LogTemp, Warning, TEXT("Delaunay Triangulation Terrain number of vertices = %.0f"), (Parameter + 1)*(Parameter + 1))
				if(log) UE_LOG(LogTemp, Warning, TEXT("Divisions: %d "), Divisions)
			Divisions *= 2;
		}
		Location += MoveInX;
	}
	StartLoc += MoveInY;
	if (log) UE_LOG(LogTemp, Warning, TEXT(""))
}

void ATerrainSpawner::SpawnPerlinNoiseTerrain(int Divisions, float Size, float Height, float Lacunarity, float Scale, float Persistance, FVector TerrainLoc)
{
	APerlinNoiseTerrain* MyActor = GetWorld()->SpawnActorDeferred<APerlinNoiseTerrain>(APerlinNoiseTerrain::StaticClass(), FTransform(TerrainLoc));
	MyActor->SetTerrainParams(Divisions,  Height, Size, Lacunarity, Scale, Persistance);
	MyActor->FinishSpawning(FTransform(TerrainLoc));

}

void ATerrainSpawner::SpawnDiamondSquareTerrain(int Divisions, float Size, float Height, float Roughness, FVector TerrainLoc)
{
	ADiamondSquareTerrain* MyActor = GetWorld()->SpawnActorDeferred<ADiamondSquareTerrain>(ADiamondSquareTerrain::StaticClass(), FTransform(TerrainLoc));
	//MyActor->GenerateTerrain2(Divisions, Height, Roughness);
	MyActor->SetTerrainParams(Divisions,  Height, Size, Roughness);
	MyActor->FinishSpawning(FTransform(TerrainLoc));
}

void ATerrainSpawner::SpawnDelaunayTerrain(int Divisions, float Size, float Height, float Lacunarity, float Scale, float Persistance, FVector TerrainLoc)
{
	ADelaunayTriangulation* MyActor = GetWorld()->SpawnActorDeferred<ADelaunayTriangulation>(ADelaunayTriangulation::StaticClass(), FTransform(TerrainLoc));
	//Constructor of MyActor is called
	//PostActorCreated of MyActor is also called

	//MyActor->GenerateTerrain2(Divisions, Height, Lacunarity, Scale, Persistance);
	MyActor->SetTerrainParams(Divisions, Height, Size, Lacunarity, Scale, Persistance);
	MyActor->FinishSpawning(FTransform(TerrainLoc));
}

