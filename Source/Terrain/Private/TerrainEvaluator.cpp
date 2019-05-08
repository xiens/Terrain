// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/TerrainEvaluator.h"
#include "Engine/Public/TimerManager.h"
#include "Engine/World.h"
//#include "GameFramework/Actor.h"

TerrainEvaluator::TerrainEvaluator()
{
}

TerrainEvaluator::~TerrainEvaluator()
{
}

void TerrainEvaluator::SampleTimerExpired() {
	bTimerExpired = true;
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


