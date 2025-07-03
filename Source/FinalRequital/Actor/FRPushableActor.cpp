// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRPushableActor.h"

AFRPushableActor::AFRPushableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMesh;

	StaticMesh->SetCanEverAffectNavigation(false);
}

void AFRPushableActor::BeginPlay()
{
	Super::BeginPlay();
}

