// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/MyCharacter.h"
#include "BreakingGlass.h"
#include "Components/InputComponent.h"
#include "Classes/Kismet/GameplayStatics.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("BreakGlass", IE_Pressed, this, &AMyCharacter::BreakGlass);
}

void AMyCharacter::BreakGlass()
{
	TArray<ABreakingGlass> BreakingGlasses;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABreakingGlass::StaticClass(), FoundActors);

	for(ABreakingGlass &BreakingGlass : BreakingGlasses) {
		BreakingGlass.DestroyQuad();
	}
	UE_LOG(LogTemp, Warning, TEXT("BreakGlass"))
}


