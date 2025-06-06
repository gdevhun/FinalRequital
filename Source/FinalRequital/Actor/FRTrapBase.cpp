// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRTrapBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Player/FRGASCharacterPlayer.h"
#include "FRDebugHelper.h"

AFRTrapBase::AFRTrapBase()
{
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
    CollisionBox->SetGenerateOverlapEvents(true);

    // TrapMesh
    TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMesh"));
    TrapMesh->SetupAttachment(RootComponent); 
    TrapMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

    // ASC 
    ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

}

void AFRTrapBase::BeginPlay()
{
    Super::BeginPlay();

    if (ASC)
    {
        ASC->InitAbilityActorInfo(this, this);
    }
}

class UAbilitySystemComponent* AFRTrapBase::GetAbilitySystemComponent() const
{
    return ASC;
}

