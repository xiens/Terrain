// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/DelaunayTriangulation.h"
#include "Public/Point2.h"
#include "Public/MeshGenerator.h"
#include "Public/MeshData.h"
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
	meshGenerator = new MeshGenerator();

	//static ConstructorHelpers::FObjectFinder<UMaterial> ConcreteMaterialAsset(TEXT("Material'/Game/StarterContent/Materials/M_AssetPlatform.M_AssetPlatform'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> ConcreteMaterialAsset(TEXT("Material'/Game/StarterContent/Materials/M_Concrete_Poured.M_Concrete_Poured'"));

	if (ConcreteMaterialAsset.Succeeded())
	{
		auto* MaterialInstance = UMaterialInstanceDynamic::Create(ConcreteMaterialAsset.Object, ConcreteMaterialAsset.Object);
		mesh->SetMaterial(0, MaterialInstance);
	}
	mesh->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));
}

// Called when the game starts or when spawned
void ADelaunayTriangulation::BeginPlay()
{
	Super::BeginPlay();
}

void ADelaunayTriangulation::OnConstruction(const FTransform & transform)
{
	Super::OnConstruction(transform);

	Points = (mDivisions + 1) * (mDivisions + 1);
	meshData = meshGenerator->GenerateMeshForTriangulation(mDivisions, mSize);
	GenerateTerrain2(mDivisions, mHeight, mSize, lacunarity, scale, persistance);
}

// This is called when actor is spawned
void ADelaunayTriangulation::PostActorCreated() {
	Super::PostActorConstruction();
	/*double start = FPlatformTime::Seconds();
	CreateQuad();
	GenerateTerrain();
	double end = FPlatformTime::Seconds();
	double TimeElapsed = end - start;*/

	//UE_LOG(LogTemp, Warning, TEXT("Delaunay Triangulation Terrain generation time: %f"), TimeElapsed);
	//TestDelaunay3D();
	//CreateSmoothlyShadedQuad();
	//GenerateTerrain();
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

void ADelaunayTriangulation::CreateSmoothlyShadedQuad() {

	Point2* cloud = new Point2[Points];

	//if (Points < 5) return;

	////Create the corners of quad
	//cloud[0] = Point2(-0.5f*Width, -0.5f*Height);
	//cloud[1] = Point2(0.5f*Width, -0.5f*Height);
	//cloud[3] = Point2(-0.5f*Width, 0.5f*Height);
	//cloud[2] = Point2(0.5f*Width, 0.5f*Height);
	//cloud[4] = Point2(-0.5f*Width, -0.501f*Height);
	Width = Height = mSize;

	//gen some random input
	for (int i = 0; i < Points; i++)
	{
		cloud[i].x = FMath::FRandRange(-0.5f * Width, 0.5f * Width);
		//cloud[i].x = pn.noise(meshData->Vertices[i].X, meshData->Vertices[i].Y, 0.8)* Width;
		cloud[i].y = FMath::FRandRange(-0.5f * Height, 0.5f * Height);
		//cloud[i].y = pn.noise(meshData->Vertices[i].X, meshData->Vertices[i].Y, 0.8)* Height;
		/*cloud[i].x = meshData->Vertices[i].X;
		cloud[i].y = meshData->Vertices[i].Y;*/
	}


	//Use the 2D Delaunay Triangulation on the generated points
	IDelaBella* idb = IDelaBella::Create();
	int verts = idb->Triangulate(Points, &cloud->x, &cloud->y, sizeof(Point2));


	std::vector<DelaBella_Triangle> triangles;
	std::vector<DelaBella_Vertex> triangleVertices;  //All unique vertices of triangles

	if (verts > 0)
	{
		int TrianglesNum = verts / 3;
		const DelaBella_Triangle* dela = idb->GetFirstDelaunayTriangle();

		for (int i = 0; i < TrianglesNum; i++)
		{
			/*UE_LOG(LogTemp, Warning, TEXT("v1= (%f, %f) v2= (%f, %f) v3= (%f, %f) "), dela->v[0]->x, dela->v[0]->y, dela->v[1]->x
				, dela->v[1]->y, dela->v[2]->x, dela->v[2]->y)*/

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
		Tangents.Add(FProcMeshTangent(0, 1, 0));
		VertexColors.Add(FLinearColor(1, 0, 0, 1.0));
	}


	for (size_t i = 0; i < triangleVertices.size(); i++) {
		UV0[i] = CalculateUV(Vertices[i]);
	}

	delete[] cloud;
	idb->Destroy();
}

FVector2D ADelaunayTriangulation::CalculateUV(FVector Vertices)
{
	FVector2D UV;

	float halfSize = mSize / 2;

	UV = FVector2D(1 - (Vertices.Y + halfSize) / mSize, (Vertices.X + halfSize) / mSize);

	return UV;
}

void ADelaunayTriangulation::GenerateTerrain() {

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

void ADelaunayTriangulation::GenerateTerrain2(float Divisions, float Height, float Size, float Lacunarity, float Scale, float Persistance)
{
	double start = FPlatformTime::Seconds();

	CreateSmoothlyShadedQuad();

	mHeight = Height;
	mSize = Size;
	lacunarity = Lacunarity;
	scale = Scale;
	persistance = Persistance;
	

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

	mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, /*meshData->*/UV0, VertexColors, Tangents, true);

	////// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);

	double end = FPlatformTime::Seconds();
	double TimeElapsed = end - start;

	UE_LOG(LogTemp, Warning, TEXT("Delaunay Triangulation Terrain generation time: %f"), TimeElapsed);
}



void ADelaunayTriangulation::SetTerrainParams(float Divisions, float Height, float Size, float Lacunarity, float Scale, float Persistance)
{
	mDivisions = Divisions;
	mHeight = Height;
	mSize = Size;
	lacunarity = Lacunarity;
	scale = Scale;
	persistance = Persistance;
}


void ADelaunayTriangulation::TestDelaunay3D()
{

	//Generate terrain vertices using perlin noise
	//GenerateTerrainForTriangulation(); 

	//std::vector<double> verts;
	//
	//verts.reserve(meshData->Vertices.Num()*3);

	//for (auto &v : meshData->Vertices) {

	//	v.X;
	//	v.Y;
	//	v.Z;
	//	UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), v.X, v.Y, v.Z)
	//}

	//for (auto &v : meshData->Vertices) {
	//	verts.push_back(v.X);
	//	verts.push_back(v.Y);
	//	verts.push_back(v.Z);
	//}

	//for (int i = 0; i < verts.size(); i++)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%f"), verts[i])

	//}

	//UE_LOG(LogTemp, Warning, TEXT("num of verts *3 %d"), verts.size())
	////Call Simple Delaunay for the specific number of dimensions (<3>)
	////std::vector<int> connectivity = SimpleDelaunay::compute<3>(verts);

	//int size = verts.size() / 3;
	//UE_LOG(LogTemp, Warning, TEXT("num of verts %d"), size)
	//// Array here as non-vector container
	//std::vector<int> indices;
	//indices.reserve(size);

	//for (int i = 0; i < size; i++) {
	//	indices.push_back(i);
	//}

	//// Call Simple Delaunay with pointers and sizes
	//std::vector<int> connectivity = SimpleDelaunay::compute<3>(&verts[0], verts.size(), &indices[0], size);

	//UE_LOG(LogTemp, Warning, TEXT("connectivity"))
	//	for (size_t i = 0; i < connectivity.size(); i++)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("%d"), connectivity[i])
	//	}

	////SECOND APPROACH
	//PerlinNoise pn(seed);
	//TArray<FVector> cloud;
	//cloud.AddZeroed(Points);
	//std::vector<double> verts2;

	////gen some random input
	//for (int i = 0; i < Points; i++)
	//{
	//	cloud[i].X = FMath::FRandRange(-0.5f * Width, 0.5f * Width);
	//	cloud[i].Y = FMath::FRandRange(-0.5f * Height, 0.5f * Height);
	//	cloud[i].Z = pn.noise(cloud[i].X, cloud[i].Y, 0.8)* mHeight;

	//}
	//for (auto &c : cloud) {
	//	verts2.push_back(c.X);
	//	verts2.push_back(c.Y);
	//	verts2.push_back(c.Z);
	//}
	//int size2 = verts2.size() / 3;

	//std::vector<int> indices2;
	//indices2.reserve(size2);

	//for (int i = 0; i < size2; i++) {
	//	indices2.push_back(i);
	//}

	//std::vector<int> connectivity2 = SimpleDelaunay::compute<3>(&verts2[0], verts2.size(), &indices2[0], size2);

	//UE_LOG(LogTemp, Warning, TEXT("connectivity"))
	//for (size_t i = 0; i < connectivity2.size(); i++)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%d"), connectivity2[i])
	//}

	//for (int i = 0; i < connectivity2.size(); i++)
	//{
	//	Triangles.Add(connectivity2[i]);
	//}
	//UV0.AddZeroed(cloud.Num());

	//UE_LOG(LogTemp, Warning, TEXT("heights: "))

	//for (int i = 0; i < cloud.Num(); i++)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("cloud[i].Z %f"), cloud[i].Z)

	//	Vertices.Add(FVector(cloud[i].X, cloud[i].Y, cloud[i].Z));
	//	Normals.Add(FVector(1, 0, 0));
	//	float mid = (Width + Height) / 2.0f;
	//	UV0.Add(FVector2D(i*(mid /100.0f)/ cloud.Num(), i*(mid / 100.0f)/ cloud.Num()));
	//	Tangents.Add(FProcMeshTangent(0, 1, 0));
	//	VertexColors.Add(FLinearColor(1, 0, 0, 1.0));
	//}

	//mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);

	////// Enable collision data
	//mesh->ContainsPhysicsTriMeshData(true);
//END OF SECOND APPROACH

	//float size = FMath::Sqrt(cloud.Num());
	//for (int x = 0; x < size; x++)
	//{
	//	for (int y = 0; y < size; y++)
	//	{
	//		UV0[x*size + y] = FVector2D(x / (size - 1), y / (size - 1));
	//	}
	//}
	//for (size_t i = 0; i < triangleVertices.size(); i++) {
	//	//UE_LOG(LogTemp, Warning, TEXT("uv: (%s)"), *UV0[i].ToString())
	//}
		

}



void ADelaunayTriangulation::GenerateTerrainForTriangulation()
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
}

