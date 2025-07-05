// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FRSoul.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"


AFRSoul::AFRSoul()
{
	PrimaryActorTick.bCanEverTick = false;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(GetCapsuleComponent());
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFRSoul::BeginPlay()
{
	Super::BeginPlay();
}
