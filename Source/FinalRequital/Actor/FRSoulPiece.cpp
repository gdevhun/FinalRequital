// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRSoulPiece.h"
#include "AbilitySystemComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/FRCollision.h"

AFRSoulPiece::AFRSoulPiece()
{
	PrimaryActorTick.bCanEverTick = false;
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

void AFRSoulPiece::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->SetVisibility(false);
}

class UAbilitySystemComponent* AFRSoulPiece::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRSoulPiece::OnDetectedOnce()
{
	if (!bIsDetected)
	{
		bIsDetected = true;

		MeshComp->SetVisibility(true, true);
		MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

		if (FXComponent)
		{
			FXComponent->ActivateSystem();
		}
	}
}
