// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainType.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Classes/Materials/MaterialInstanceDynamic.h"
#include "ProceduralMeshComponent.h"

TerrainType::TerrainType()
{
	TerrainRegion water = TerrainRegion("water", 0.25f, FLinearColor(0, 0, 1.0f, 1.0f));
	TerrainRegion sand = TerrainRegion("sand", 0.45f, FLinearColor(1.0f, 0, 0, 1.0f));
	TerrainRegion mountain = TerrainRegion("mountain", 0.6f, FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
	TerrainRegions.Add(water);
	TerrainRegions.Add(sand);
	TerrainRegions.Add(mountain);
}

TerrainType::~TerrainType()
{
}


void TerrainType::SetMeshMaterial(UProceduralMeshComponent * mesh) {

	static ConstructorHelpers::FObjectFinder<UMaterial> ConcreteMaterialAsset(TEXT("Material'/Game/Materials/MyMat.MyMat'"));
	if (ConcreteMaterialAsset.Succeeded())
	{
		auto* MaterialInstance = UMaterialInstanceDynamic::Create(ConcreteMaterialAsset.Object, ConcreteMaterialAsset.Object);
		if(mesh) mesh->SetMaterial(0, MaterialInstance);
		else {
			UE_LOG(LogTemp, Warning, TEXT("no mesh instance"))
		}
	}
}

TArray<FLinearColor> TerrainType::AssignRegionToHeights(const TArray<FLinearColor> &VerticesColors, const TArray<float> &VerticesHeights)
{
	TArray<FLinearColor> colors = VerticesColors;

	float MaxHeightMultiplier = VerticesHeights[0];
	float MinHeightMultiplier = VerticesHeights[0];

	for (size_t i = 0; i < VerticesHeights.Num(); i++) {
		if (VerticesHeights[i] > MaxHeightMultiplier) MaxHeightMultiplier = VerticesHeights[i];
		if (VerticesHeights[i] < MinHeightMultiplier) MinHeightMultiplier = VerticesHeights[i];
	}
	UE_LOG(LogTemp, Warning, TEXT("Max height: %f Min height: %f"), MaxHeightMultiplier, MinHeightMultiplier)



	for (size_t i = 0; i < VerticesHeights.Num(); i++)
	{
		for (size_t j = 0; j < TerrainRegions.Num(); j++)
		{
			if (VerticesHeights[i] <= TerrainRegions[j].height * (MaxHeightMultiplier-MinHeightMultiplier)) {
				colors[i] = TerrainRegions[j].color;
				UE_LOG(LogTemp, Warning, TEXT("color %d"), j)

				//UE_LOG(LogTemp, Warning, TEXT("Vertex Height: %f Region Height: %f"),VerticesHeights[i], TerrainRegions[j].height * (MaxHeightMultiplier-MinHeightMultiplier))
			}
		}
	}

	return colors;
}
