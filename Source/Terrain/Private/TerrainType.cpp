// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainType.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Classes/Materials/MaterialInstanceDynamic.h"
#include "ProceduralMeshComponent.h"

TerrainType::TerrainType()
{
	TerrainRegion water = TerrainRegion("water", 0.33f, FLinearColor(0.2f, 0.7f, 0.92f, 1.0f));
	TerrainRegion sand = TerrainRegion("sand", 0.54f, FLinearColor(0.94f, 0.7f, 0.17f, 1.0f));
	TerrainRegion mountain = TerrainRegion("mountain", 1.0f, FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
	
	
	TerrainRegions.Add(mountain);
	TerrainRegions.Add(sand);
	TerrainRegions.Add(water);
}

TerrainType::~TerrainType()
{
}


void TerrainType::SetMeshMaterial(UProceduralMeshComponent * mesh) {

	static ConstructorHelpers::FObjectFinder<UMaterial> MyMatAsset(TEXT("Material'/Game/Materials/MyMat.MyMat'"));
	if (MyMatAsset.Succeeded())
	{
		auto* MaterialInstance = UMaterialInstanceDynamic::Create(MyMatAsset.Object, MyMatAsset.Object);
		if (mesh) {
			mesh->SetMaterial(0, MaterialInstance);
		}
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
	//UE_LOG(LogTemp, Warning, TEXT("Max height: %f Min height: %f"), MaxHeightMultiplier, MinHeightMultiplier)

	for (size_t i = 0; i < VerticesHeights.Num(); i++)
	{
		for (size_t j = 0; j < TerrainRegions.Num(); j++)
		{
			/*if (VerticesHeights[i] <= MinHeightMultiplier + TerrainRegions[j].height * (MaxHeightMultiplier-MinHeightMultiplier)) {
				colors[i] = TerrainRegions[j].color;
			}*/
			colors[i] = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
		}
	}

	return colors;
}
