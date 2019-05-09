// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/DelaunayTriangulation.h"
#include "Public\Point2.h"
#include <vector>
#include "Public/PerlinNoise.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "CoreUObject/Public/UObject/Class.h"
#include "Classes/Materials/MaterialInstanceDynamic.h"


// Sets default values
ADelaunayTriangulation::ADelaunayTriangulation()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> ConcreteMaterialAsset(TEXT("Material'/Game/StarterContent/Materials/M_AssetPlatform.M_AssetPlatform'"));
	if (ConcreteMaterialAsset.Succeeded())
	{
		auto* MaterialInstance = UMaterialInstanceDynamic::Create(ConcreteMaterialAsset.Object, ConcreteMaterialAsset.Object);
		mesh->SetMaterial(0, MaterialInstance);
	}

}

// Called when the game starts or when spawned
void ADelaunayTriangulation::BeginPlay()
{
	Super::BeginPlay();
}

// This is called when actor is spawned
void ADelaunayTriangulation::PostActorCreated() {
	Super::PostActorConstruction();
	double start = FPlatformTime::Seconds();
	CreateQuad();
	GenerateTerrain();
	double end = FPlatformTime::Seconds();
	double TimeElapsed = end - start;
	//UE_LOG(LogActor, Warning, TEXT("Tick Timer: %.6f Start: %.6f"), end - start, start);

	UE_LOG(LogTemp, Warning, TEXT("Delaunay Triangulation Terrain generation time: %f"), TimeElapsed);

}

// This is called when actor is already in level and map is opened
void ADelaunayTriangulation::PostLoad() {
	Super::PostLoad();

}

// Called every frame
void ADelaunayTriangulation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

std::vector<int> ADelaunayTriangulation::CalculateTriangleIndices(std::vector<DelaBella_Triangle> triangles,
	std::vector<DelaBella_Vertex> &triangleVertices)
{

	std::vector<int> allIndices;; //All unique indices of vertices of triangles
	std::vector<int> indices; //vertices of triangles in proper order for creating mesh

	DelaBella_Vertex currentVertex = *triangles[0].v[0];

	int triangleCnt = 0;
	int vertexInd = 0;
	int i = 0;
	int oldIndex = 0;

	while (triangleCnt != triangles.size())
	{

		if (vertexInd == 0) currentVertex = *triangles[triangleCnt].v[0];
		else if (vertexInd == 1) currentVertex = *triangles[triangleCnt].v[1];
		else if (vertexInd == 2) currentVertex = *triangles[triangleCnt].v[2];


		if (IsVertexDefined(triangleVertices, currentVertex, allIndices, oldIndex))
		{
			indices.push_back(oldIndex);
		}
		else
		{
			indices.push_back(i);
			allIndices.push_back(i);
			triangleVertices.push_back(currentVertex);
			i++;
		}
		vertexInd++; //next vertex of current triangle

		if (vertexInd == 3) //next triangle
		{
			vertexInd = 0;
			triangleCnt++;
		}
	}

	return indices;

}


bool ADelaunayTriangulation::IsVertexDefined(std::vector<DelaBella_Vertex> triangleVertices,
	DelaBella_Vertex v,
	std::vector<int>indices, int &oldIndex)
{

	for (int i = 0; i < triangleVertices.size(); i++)
	{

		if (v.x == triangleVertices[i].x && v.y == triangleVertices[i].y)
		{
			oldIndex = indices[i];
			return true;
		}

	}

	return false;
}


void ADelaunayTriangulation::CreateTriangle(int i)
{
	TArray<FVector> triVerts;
	TArray<int> triIndices;
	TArray<FVector> _normals;
	TArray<FVector2D> _UV0;
	TArray<FProcMeshTangent> _tangents;
	TArray<FLinearColor> _vertexColors;

	for (int32 v = 0; v < 3; v++) {
		triVerts.Add(Vertices[Triangles[i + v]]);
		triIndices.Add(v);
		_UV0.Add(UV0[Triangles[i + v]]);
		_tangents.Add(FProcMeshTangent(0, 1, 0));
		_vertexColors.Add(FLinearColor(1, 0, 0, 1.0));
		_normals.Add(FVector(1, 0, 0));
	}
	//UE_LOG(LogTemp, Warning, TEXT("(v %d): (%s) (v %d): (%s) (v %d): (%s) "),triIndices[0],*triVerts[0].ToString(), triIndices[1], *triVerts[1].ToString(), triIndices[2], *triVerts[2].ToString())

	mesh->CreateMeshSection_LinearColor(triangleInd, triVerts, triIndices, _normals, _UV0, _vertexColors, _tangents, true);
	triangleInd++;
}

void ADelaunayTriangulation::CreateQuad() {

	Point2* cloud = new Point2[Points];

	if (Points < 5) return;

	//Create the corners of quad
	cloud[0] = Point2(-0.5f*Width, -0.5f*Height);
	cloud[1] = Point2(0.5f*Width, -0.5f*Height);
	cloud[3] = Point2(-0.5f*Width, 0.5f*Height);
	cloud[2] = Point2(0.5f*Width, 0.5f*Height);
	cloud[4] = Point2(-0.5f*Width, -0.501f*Height);

	//gen some random input
	for (int i = 5; i < Points; i++)
	{
		cloud[i].x = FMath::FRandRange(-0.5f * Width, 0.5f * Width);
		cloud[i].y = FMath::FRandRange(-0.5f * Height, 0.5f * Height);
	}

	//Use the 2D Delaunay Triangulation on the generated points
	IDelaBella* idb = IDelaBella::Create();
	int verts = idb->Triangulate(Points, &cloud->x, &cloud->y, sizeof(Point2));
	UE_LOG(LogTemp, Warning, TEXT("number of triangulation vertices: %d"), verts)

	std::vector<DelaBella_Triangle> triangles;
	std::vector<DelaBella_Vertex> triangleVertices;  //All unique vertices of triangles

	if (verts > 0)
	{
		int TrianglesNum = verts / 3;
		const DelaBella_Triangle* dela = idb->GetFirstDelaunayTriangle();

		for (int i = 0; i < TrianglesNum; i++)
		{
			//UE_LOG(LogTemp, Warning, TEXT("v1= (%f, %f) v2= (%f, %f) v3= (%f, %f) "), dela->v[0]->x, dela->v[0]->y, dela->v[1]->x
				//, dela->v[1]->y, dela->v[2]->x, dela->v[2]->y)

			triangles.push_back(*dela);
			dela = dela->next;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no points given or all points are colinear"))
	}

	 std::vector<int> triangleIndices = CalculateTriangleIndices(triangles, triangleVertices);

	for (int i = 0; i < triangleIndices.size(); i++)
	{
		Triangles.Add(triangleIndices[i]);
	}
	UV0.AddZeroed(triangleVertices.size());

	for (int i = 0; i < triangleVertices.size(); i++)
	{
		Vertices.Add(FVector(triangleVertices[i].x, triangleVertices[i].y, 0));
		Normals.Add(FVector(1, 0, 0));
		//float mid = (Width + Height) / 2.0f;
		//UV0.Add(FVector2D(i*(mid /100.0f)/triangleVertices.size(), i*(mid / 100.0f)/triangleVertices.size()));
		//UE_LOG(LogTemp, Warning, TEXT("uv: (%s)"), *UV0[i].ToString())

		Tangents.Add(FProcMeshTangent(0, 1, 0));
		VertexColors.Add(FLinearColor(1, 0, 0, 1.0));
	}
	float size = FMath::Sqrt(triangleVertices.size());
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			UV0[x*size + y] = FVector2D(x / (size - 1), y / (size - 1));
		}
	}
	for (size_t i = 0; i < triangleVertices.size(); i++) {
		//UE_LOG(LogTemp, Warning, TEXT("uv: (%s)"), *UV0[i].ToString())
	}


	/*delete[] cloud;
	idb->Destroy();*/
}

void ADelaunayTriangulation::GenerateTerrain() {

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

	int j = 0;
	//PerlinNoise
	for (int i = 0; i < Points; i++)
	{
		float amplitude = 1;
		float frequency = 1;
		float noiseHeight = 0;
		float PerlinValue = 0;

		for (int octave = 0; octave < octaves; octave++)
		{
			float xCoord = Vertices[i].X  * scale * frequency + octaveOffsets[octave].X;
			float yCoord = Vertices[i].Y * scale  * frequency + octaveOffsets[octave].Y;

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

