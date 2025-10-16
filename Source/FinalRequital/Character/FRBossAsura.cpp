// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FRBossAsura.h"
#include "FRCharacterBase.h"
#include "Kismet/GameplayStatics.h"

AFRBossAsura::AFRBossAsura()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AFRBossAsura::BeginPlay()
{
	Super::BeginPlay();

	TargetPlayer = Cast<AFRCharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));

	GetWorldTimerManager().SetTimer(
		LookAtTimerHandle,
		this,
		&AFRBossAsura::LookAtTargetPlayer,
		0.025f,      
		true      
	);

}

void AFRBossAsura::LookAtTargetPlayer()
{
	if (!TargetPlayer) return;

	const FVector MyLocation = GetActorLocation();
	const FVector TargetLocation = TargetPlayer->GetActorLocation();

	const FVector FlatTargetLocation(TargetLocation.X, TargetLocation.Y, MyLocation.Z);

	const FRotator LookAtRot = (FlatTargetLocation - MyLocation).Rotation();
	SetActorRotation(LookAtRot);
}

