// Fill out your copyright notice in the Description page of Project Settings.




#include "BreakingGlass.h"
#include "Public\Point2.h"
#include <vector>
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "CoreUObject/Public/UObject/Class.h"


// Sets default values
ABreakingGlass::ABreakingGlass()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;
	
}

// Called when the game starts or when spawned
void ABreakingGlass::BeginPlay()
{
	Super::BeginPlay();
	
}

// This is called when actor is spawned
void ABreakingGlass::PostActorCreated() {
	Super::PostActorConstruction();
	UE_LOG(LogTemp,Warning, TEXT("PostActorConstruction"))
	CreateQuad();
}

// This is called when actor is already in level and map is opened
void ABreakingGlass::PostLoad() {
	Super::PostLoad();
}

// Called every frame
void ABreakingGlass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakingGlass::DelabellaTest() {
	// somewhere in your code ...

	int POINTS = 1000000;

	struct MyPoint
	{
		float x;
		float y;
	};

	MyPoint* cloud = new MyPoint[POINTS];

	srand(36341);

	// gen some random input
	for (int i = 0; i < POINTS; i++)
	{
		cloud[i].x = rand();
		cloud[i].y = rand();
	}

	IDelaBella* idb = IDelaBella::Create();

	int verts = idb->Triangulate(POINTS, &cloud->x, &cloud->y, sizeof(MyPoint));

	// if positive, all ok 
	if (verts > 0)
	{
		int tris = verts / 3;
		const DelaBella_Triangle* dela = idb->GetFirstDelaunayTriangle();

		for (int i = 0; i < tris; i++)
		{
			// do something with dela triangle 
			// ...
			dela = dela->next;
		}
	}
	else
	{
		// no points given or all points are colinear
		// make emergency call ...
	}

	delete[] cloud;
	idb->Destroy();
}

std::vector<int> ABreakingGlass::CalculateTriangleIndices(std::vector<DelaBella_Triangle> triangles,
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


bool ABreakingGlass::IsVertexDefined(std::vector<DelaBella_Vertex> triangleVertices,
	DelaBella_Vertex v,
	std::vector<int>indices,  int &oldIndex)
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


void ABreakingGlass::CreateTriangle(TArray<FVector> &vertices, TArray<int32> &triangleIndices, TArray<FVector> &normals,
	TArray<FVector2D> &UV0,
	TArray<FProcMeshTangent> &tangents,
	TArray<FLinearColor> &vertexColors , int i)
{
	UProceduralMeshComponent * triangle;

	TArray<FVector> triVerts;
	TArray<int32> triIndices;
	TArray<FVector> _normals;
	TArray<FVector2D> _UV0;
	TArray<FProcMeshTangent> _tangents;
	TArray<FLinearColor> _vertexColors;

	for (int v = 0; v < 3; v++) {
		triVerts.Add(vertices[i + v]);
		triIndices.Add(triangleIndices[i + v]);
		_normals.Add(normals[i + v]);
		_UV0.Add(UV0[i + v]);
		_tangents.Add(tangents[i + v]);
		_vertexColors.Add(vertexColors[i + v]);
	}
	triangle = CreateDefaultSubobject<UProceduralMeshComponent>(FName("triangle"));
	triangle->CreateMeshSection_LinearColor(0, triVerts, triIndices, _normals, _UV0, _vertexColors, _tangents, true);

	triangle->SetupAttachment(RootComponent);
	triangle->SetRelativeRotation(FRotator(FMath::FRandRange(-180.0f, 180.0f), FMath::FRandRange(-180.0f, 180.0f), FMath::FRandRange(-180.0f, 180.0f)));
	triangle->SetRelativeLocation(FVector(FMath::FRandRange(0, width), FMath::FRandRange(0, width), FMath::FRandRange(0, width)));
}

void ABreakingGlass::DestroyQuad()
{
	UE_LOG(LogTemp, Warning, TEXT("destroying"))
	mesh->ClearMeshSection(0);
	int i = 0;
	CreateTriangle(vertices, Triangles, normals, UV0, tangents, vertexColors, i);
}

void ABreakingGlass::CreateQuad() {

	Point2* cloud = new Point2[POINTS];

	if (POINTS < 5) return;

	//srand(36341);
	cloud[0] = Point2(-0.5f*width, -0.5f*height);
	cloud[1] = Point2(0.5f*width, -0.5f*height);
	cloud[3] = Point2(-0.5f*width, 0.5f*height);
	cloud[2] = Point2(0.5f*width, 0.5f*height);
	cloud[4] = Point2(-0.5f*width, -0.501f*height);

	// gen some random input
	for (int i = 5; i < POINTS; i++)
	{
		cloud[i].x = FMath::FRandRange(-0.5f * width, 0.5f * width);
		cloud[i].y = FMath::FRandRange(-0.5f * height, 0.5f * height);
	}

	IDelaBella* idb = IDelaBella::Create();
	//int verts = idb->Triangulate(POINTS, &cloud->x, &cloud->y, sizeof(Point2));
	int verts = idb->Triangulate(POINTS, &cloud->x, &cloud->y, sizeof(Point2));
	
	for (int i = 0; i < verts; i++)
	{
		vertices.Add(FVector(cloud[i].x, cloud[i].y, 0));
	}
	std::vector<DelaBella_Triangle> triangles;
	std::vector<DelaBella_Vertex> triangleVertices;  //All unique vertices of triangles

	UE_LOG(LogTemp, Warning, TEXT("number of vertices: %d"), verts)
	 //if positive, all ok 
	if (verts > 0)
	{
		int tris = verts / 3;
		const DelaBella_Triangle* dela = idb->GetFirstDelaunayTriangle();

		for (int i = 0; i < tris; i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("v1= (%f, %f) v2= (%f, %f) v3= (%f, %f) "), dela->v[0]->x, dela->v[0]->y, dela->v[1]->x
				, dela->v[1]->y, dela->v[2]->x, dela->v[2]->y)
				
				triangles.push_back(*dela);
				dela = dela->next;
		}
	}
	else
	{
		// no points given or all points are colinear
		UE_LOG(LogTemp, Warning, TEXT("no points given or all points are colinear"))
	}


	//Calculate triangle indices
	std::vector<int> triangleIndices;
	
	triangleIndices = CalculateTriangleIndices(triangles, triangleVertices);

	//int i = 0;
	//for (auto &index : triangleIndices) {
	//	UE_LOG(LogTemp, Warning, TEXT("Index %d"), index)
	//		i++;
	//}

	for (int i = 0; i < triangleIndices.size(); i++)
	{
		Triangles.Add(triangleIndices[i]);
	}



	for (int i = 0; i < triangleVertices.size(); i++)
	{
		normals.Add(FVector(1, 0, 0));
		UV0.Add(FVector2D(triangleVertices[i].x, triangleVertices[i].y));
		tangents.Add(FProcMeshTangent(0, 1, 0));
		vertexColors.Add(FLinearColor(1, 0, 0, 1.0));
	}

	mesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
	
	//// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);



	//int cnt = 0;
	////print triangle vertices and vertices
	//for (auto &vert : vertices)
	//{
	//	cnt++;
	//	UE_LOG(LogTemp, Warning, TEXT("v:  (%f, %f)"), vert.X, vert.Y)
	//}

	//for (size_t i = 0; i < triangles.size(); i++)
	//{
	//	for (size_t j = 0; j < 3; j++)
	//	{
	//		if((i+j) < cnt)
	//			UE_LOG(LogTemp, Warning, TEXT("v:  (%f, %f)"), vertices[i+j].X, vertices[i+j].Y)
	//		//UE_LOG(LogTemp, Warning, TEXT("v:  (%f, %f)"), triangles[i].v[j]->x, , triangles[i].v[j]->y)
	//			UE_LOG(LogTemp, Warning, TEXT("v:  (%f, %f)"), triangles[i].v[j]->x, triangles[i].v[j]->y)
	//			//UE_LOG(LogTemp, Warning, TEXT("v:  %f"), triangles[i].v[j]->y)
	//	}

	//}
}
