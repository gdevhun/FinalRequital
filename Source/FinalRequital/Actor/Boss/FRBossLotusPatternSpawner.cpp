// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Boss/FRBossLotusPatternSpawner.h"
#include "GameFramework/ProjectileMovementComponent.h"

AFRBossLotusPatternSpawner::AFRBossLotusPatternSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AFRBossLotusPatternSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFRBossLotusPatternSpawner::ActivateLotusPattern(AActor* TargetPlayer)
{
	if (!TargetPlayer) return;

	const bool bIsHealing = ShouldSpawnHealingPattern();
	TSubclassOf<AActor> ActorClassToSpawn = bIsHealing ? HealingPatternActorClass : ProjectilePatternActorClass;
	if (!ActorClassToSpawn) return;

	const FVector PlayerLocation = TargetPlayer->GetActorLocation();
	const FVector SpawnLocation = GetActorLocation();
	FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();
	FRotator SpawnRotation = Direction.Rotation();

	FVector FinalSpawnLocation = SpawnLocation;

	if (bIsHealing)
	{
		// �÷��̾� �ٶ󺸴� ���� �������� ��¦ ������
		FinalSpawnLocation += Direction * 400.f;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ActorClassToSpawn,
		FinalSpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (!SpawnedActor) return;

	// �߻�ü �̵� ó�� (������ ����, �Ϲ��� ���ư�)
	if (!bIsHealing)
	{
		if (auto* ProjectileComp = SpawnedActor->FindComponentByClass<UProjectileMovementComponent>())
		{
			ProjectileComp->Velocity = Direction * ProjectileComp->InitialSpeed;
		}
	}
}

bool AFRBossLotusPatternSpawner::ShouldSpawnHealingPattern() const
{
	return FMath::RandBool();
}


