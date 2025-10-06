// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRPushableActor.h"
#include "NavModifierComponent.h"
#include "NavAreas/NavArea_Obstacle.h"

AFRPushableActor::AFRPushableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMesh;

	StaticMesh->SetCanEverAffectNavigation(false);
	StaticMesh->SetSimulatePhysics(true);

	StaticMesh->SetRenderCustomDepth(false);
	StaticMesh->SetCustomDepthStencilValue(250);

	NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));
	NavModifier->SetAreaClass(UNavArea_Obstacle::StaticClass());
	NavModifier->SetCanEverAffectNavigation(true);
}

void AFRPushableActor::Highlight()
{
	StaticMesh->SetRenderCustomDepth(true);
}

void AFRPushableActor::UnHighlight()
{
	StaticMesh->SetRenderCustomDepth(false);
}

void AFRPushableActor::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFRPushableActor::DisablePhysics, 2.0f, false);
}

void AFRPushableActor::DisablePhysics()
{
	if (StaticMesh)
	{
		StaticMesh->SetSimulatePhysics(false);
		StaticMesh->SetEnableGravity(true); 
	}
}

