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

    VisualMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh2"));
    VisualMesh2->SetupAttachment(GetCapsuleComponent());
    VisualMesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    VisualMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh3"));
    VisualMesh3->SetupAttachment(GetCapsuleComponent());
    VisualMesh3->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AFRSoul::BeginPlay()
{
	Super::BeginPlay();
    // 초기에는 Mesh1만 보이게
    VisualMesh->SetVisibility(true);
    VisualMesh2->SetVisibility(false);
    VisualMesh3->SetVisibility(false);
}

void AFRSoul::ActiveFirstVisualMesh()
{
    VisualMesh2->SetVisibility(true);
}

void AFRSoul::ActiveSecondVisualMesh()
{
    VisualMesh3->SetVisibility(true);
}


