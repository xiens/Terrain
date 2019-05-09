// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/TerrainEvaluator.h"
#include "PerlinNoiseTerrain.h"
#include "Engine/Classes/Components/ActorComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"




TerrainEvaluator::TerrainEvaluator()
{
}

TerrainEvaluator::~TerrainEvaluator()
{
}

void TerrainEvaluator::SpawnTerrainActors() {

	UClass* PerlinNoiseTerrain = APerlinNoiseTerrain::StaticClass();
	//UWorld world = GetWorld();
	//GetWorld()->SpawnActor<APerlinNoiseTerrain>(GetClass(), FVector(0.0f, 0.f, 0.f), FRotator(0.0f, 0.f, 0.f), FActorSpawnParameters());
}

void TerrainEvaluator::ToggleTimer()
{
	
	//UWorld* World = GetWorld();
	//if (World)
	//{
	//	// If the timer has expired or does not exist, start it  
	//	if ((InputTimerHandle.IsValid() == false) || (bTimerExpired))
	//	{
	//		World->GetTimerManager().SetTimer(InputTimerHandle, this, &TerrainEvaluator::SampleTimerExpired, 10.0f);
	//		//World->GetTimerManager().SetTimer(InputTimerHandle, 1.0f, bTimerExpired, 0);
	//		bTimerExpired = false;
	//	}
	//	/*else
	//	{
	//		if (World->GetTimerManager().IsTimerPaused(InputTimeHandle) == true)
	//		{
	//			World->GetTimerManager().UnPauseTimer(InputTimeHandle);
	//		}
	//		else
	//		{
	//			World->GetTimerManager().PauseTimer(InputTimeHandle);
	//		}
	//	}*/

	//}
}


