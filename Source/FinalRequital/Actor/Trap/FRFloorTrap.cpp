// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Trap/FRFloorTrap.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

AFRFloorTrap::AFRFloorTrap()
{
    PrimaryActorTick.bCanEverTick = false;

    // Scene Component Root
    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;

    // TrapMesh
    TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMesh"));
    TrapMesh->SetupAttachment(RootComponent);

    // CollisionBox
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
    CollisionBox->SetGenerateOverlapEvents(true);

    // ParticleSystem
    ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystem->SetupAttachment(RootComponent);

    // ASC 
    ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}
void AFRFloorTrap::BeginPlay()
{
	Super::BeginPlay();
    if (ASC)
    {
        ASC->InitAbilityActorInfo(this, this);
    }
}

class UAbilitySystemComponent* AFRFloorTrap::GetAbilitySystemComponent() const
{
    return ASC;
}

