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
	//double start = FPlatformTime::Seconds();
	GenerateTerrain();
	DelaunayTriangulation();
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
		mHeight *= roughness;//u can change this parameter
	}

	//mesh->CreateMeshSection_LinearColor(0, meshData->Vertices, meshData->Triangles, meshData->Normals,
	//	meshData->UV0, meshData->VertexColors, meshData->Tangents, true);

	//// Enable collision data
	//mesh->ContainsPhysicsTriMeshData(true);
}

void ADiamondSquareTerrain::GenerateTerrain2(float mDivisions, float mHeight, float roughness)
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
		mHeight *= roughness;//u can change this parameter
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

void ADiamondSquareTerrain::DelaunayTriangulation()
{
	UE_LOG(LogTemp, Warning, TEXT(" DIAMONDSQUARE"))
	UE_LOG(LogTemp, Warning, TEXT(" "))
	UE_LOG(LogTemp, Warning, TEXT(" "))
	UE_LOG(LogTemp, Warning, TEXT(" "))
	UE_LOG(LogTemp, Warning, TEXT(" "))
	UE_LOG(LogTemp, Warning, TEXT(" "))
	UE_LOG(LogTemp, Warning, TEXT(" "))

	//second approach
	std::vector<double> verts;
	verts.reserve(meshData->Vertices.Num() * 3);

	for (auto &v : meshData->Vertices) {
		verts.push_back(v.X);
		verts.push_back(v.Y);
		verts.push_back(v.Z);
	}

	std::vector<int> indices;
	indices.reserve(meshData->Vertices.Num());

	for (int i = 0; i < meshData->Vertices.Num(); i++) {
		indices.push_back(i);
	}

	UE_LOG(LogTemp, Warning, TEXT("meshData->Vertices.Num(): %d"), meshData->Vertices.Num())
	UE_LOG(LogTemp, Warning, TEXT("verts.size(): %d"), verts.size())
	std::vector<int> connectivity2 = SimpleDelaunay::compute<3>(&verts[0], verts.size(), &indices[0], meshData->Vertices.Num());

	UE_LOG(LogTemp, Warning, TEXT("connectivity"))
	for (size_t i = 0; i < connectivity2.size(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), connectivity2[i])
	}
	meshData->Triangles.SetNumZeroed(connectivity2.size(), true);

	for (int i = 0; i < connectivity2.size(); i++)
	{
		meshData->Triangles[i] = connectivity2[i];
	}
	meshData->UV0.SetNumZeroed(meshData->Vertices.Num());
	meshData->Normals.SetNumZeroed(meshData->Vertices.Num());
	meshData->Tangents.SetNumZeroed(meshData->Vertices.Num());
	meshData->VertexColors.SetNumZeroed(meshData->Vertices.Num());

	//UE_LOG(LogTemp, Warning, TEXT("heights: "))

	for (int i = 0; i < meshData->Vertices.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("meshData->Vertices[i].Z %f"), meshData->Vertices[i].Z)
			//Vertices.Add(FVector(verts[i].X, verts[i].Y, verts[i].Z));
		meshData->Normals.Add(FVector(1, 0, 0));
		float mid = 100.0f;
		meshData->UV0.Add(FVector2D(i*(mid / 100.0f) / meshData->Vertices.Num(), i*(mid / 100.0f) / meshData->Vertices.Num()));
		meshData->Tangents.Add(FProcMeshTangent(0, 1, 0));
		meshData->VertexColors.Add(FLinearColor(1, 0, 0, 1.0));
	}

	UE_LOG(LogTemp, Warning, TEXT("num of vertices: %d"), meshData->Vertices.Num())
		TArray <int32> unique_verts;

	for (size_t i = 0; i < meshData->Triangles.Num(); i++)
	{
		if (!unique_verts.Contains(meshData->Triangles[i])) {
			unique_verts.Add(meshData->Triangles[i]);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("num of vertices according to Triangles table: %d"), unique_verts.Num())
	for(auto &v : unique_verts) {
		UE_LOG(LogTemp, Warning, TEXT("%d"),v)
	}


	//mesh->CreateMeshSection_LinearColor(0, meshData->Vertices, meshData->Triangles, meshData->Normals,
	//	meshData->UV0, meshData->VertexColors, meshData->Tangents, true);

	////// Enable collision data
	//mesh->ContainsPhysicsTriMeshData(true);
}

