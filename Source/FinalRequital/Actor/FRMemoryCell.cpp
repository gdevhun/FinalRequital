// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRMemoryCell.h"
#include "AbilitySystemComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/FRCollision.h"

AFRMemoryCell::AFRMemoryCell()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsDetected = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionResponseToChannel(CCHANNEL_FRACTION, ECR_Block);
	MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetupAttachment(RootSceneComponent);

	FXComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FXComponent"));
	FXComponent->SetupAttachment(RootSceneComponent);
	FXComponent->bAutoActivate = false;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootSceneComponent);
	AudioComponent->bAutoActivate = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AFRMemoryCell::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->SetVisibility(false);
}

void AFRMemoryCell::OnDetectedOnce()
{
	UE_LOG(LogTemp, Warning, TEXT("OnDetectedOnce called!"));
	if (!bIsDetected)
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting visibility to true"));
		bIsDetected = true;

		MeshComp->SetVisibility(true);
		MeshComp->SetHiddenInGame(false);
		MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

		if (FXComponent)
		{
			FXComponent->ActivateSystem();
			AttachToSoul();
		}
	}
}

void AFRMemoryCell::AttachToSoul_Implementation()
{
}

class UAbilitySystemComponent* AFRMemoryCell::GetAbilitySystemComponent() const
{
	return ASC;
}


