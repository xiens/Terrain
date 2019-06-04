// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/DiamondSquareTerrain.h"
#include "Public/MeshGenerator.h"
#include "Public/MeshData.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "CoreUObject/Public/UObject/Class.h"
#include "Classes/Materials/MaterialInstanceDynamic.h"

// Sets default values
ADiamondSquareTerrain::ADiamondSquareTerrain()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;
	meshGenerator = new MeshGenerator();

	//meshData = meshGenerator->GenerateMesh(mDivisions, mSize);
	//UE_LOG(LogTemp, Warning, TEXT("Constructor mDivisions: %d"), mDivisions);


	static ConstructorHelpers::FObjectFinder<UMaterial> ConcreteMaterialAsset(TEXT("Material'/Game/StarterContent/Materials/M_Concrete_Poured.M_Concrete_Poured'"));
	if (ConcreteMaterialAsset.Succeeded())
	{
		auto* MaterialInstance = UMaterialInstanceDynamic::Create(ConcreteMaterialAsset.Object, ConcreteMaterialAsset.Object);
		mesh->SetMaterial(0, MaterialInstance);
	}

	mesh->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));

}

// Called when the game starts or when spawned
void ADiamondSquareTerrain::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Beginplay mDivisions: %d"), mDivisions);

}

void ADiamondSquareTerrain::OnConstruction(const FTransform & transform)
{
	Super::OnConstruction(transform);

	//UE_LOG(LogTemp, Warning, TEXT("OnConstruction mDivisions: %d"), mDivisions);
	mVertCount = (mDivisions + 1) * (mDivisions + 1);
	meshData = meshGenerator->GenerateMesh(mDivisions, mSize);
	GenerateTerrain2(mDivisions, mHeight, mSize, mRoughness);
	//GenerateTerrain();
}

void ADiamondSquareTerrain::PostActorCreated()
{
	Super::PostActorCreated();
	//double start = FPlatformTime::Seconds();
	//GenerateTerrain();
	//DelaunayTriangulation();
	/*double end = FPlatformTime::Seconds();
	double TimeElapsed = end - start;*/
	//UE_LOG(LogActor, Warning, TEXT("Tick Timer: %.6f Start: %.6f"), end - start, start);

	//UE_LOG(LogTemp, Warning, TEXT("Diamond Square Terrain generation time: %f"), TimeElapsed + meshGenerator->MeshGenerationTime);
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
		mHeight *= mRoughness;//u can change this parameter
	}

	mesh->CreateMeshSection_LinearColor(0, meshData->Vertices, meshData->Triangles, meshData->Normals,
		meshData->UV0, meshData->VertexColors, meshData->Tangents, true);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

void ADiamondSquareTerrain::GenerateTerrain2(int Divisions, float Height, float Size, float Roughness)
{
	//mDivisions = Divisions;
	//mHeight = Height;
	//mRoughness = Roughness;
	//mSize = Size;

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
		mHeight *= mRoughness;//u can change this parameter
	}

	mesh->CreateMeshSection_LinearColor(0, meshData->Vertices, meshData->Triangles, meshData->Normals,
		meshData->UV0, meshData->VertexColors, meshData->Tangents, true);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

void ADiamondSquareTerrain::SetTerrainParams(int Divisions, float Height, float Size, float Roughness)
{


	mDivisions = Divisions;
	mHeight = Height;
	mSize = Size;
	mRoughness = Roughness;
	//UE_LOG(LogTemp, Warning, TEXT("SetTerrainParams mDivisions: %d"), mDivisions);
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


