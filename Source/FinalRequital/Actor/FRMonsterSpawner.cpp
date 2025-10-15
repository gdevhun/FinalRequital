// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRMonsterSpawner.h"
#include "Character/FRMonsterBase.h"
#include "Kismet/KismetMathLibrary.h"

AFRMonsterSpawner::AFRMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	SpawnInterval = 5.0f;
	SpecialSpawnPercentage = 10.0f; 
}

void AFRMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
}
void AFRMonsterSpawner::StartSpawn()
{
	if (GetWorld())
	{

		SpawnMonster();

		GetWorld()->GetTimerManager().SetTimer(
			SpawnTimerHandle,
			this,
			&AFRMonsterSpawner::SpawnMonster,
			SpawnInterval,
			true 
		);
	}
}

void AFRMonsterSpawner::StopSpawn()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

void AFRMonsterSpawner::SpawnMonster()
{
	if (!GetWorld()) return;

	TSubclassOf<AFRMonsterBase> MonsterToSpawn = nullptr;

	const float RandomValue = UKismetMathLibrary::RandomFloatInRange(0.f, 100.f);

	if (RandomValue <= SpecialSpawnPercentage)
	{
		const int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, 2);
		switch (RandomIndex)
		{
		case 0: MonsterToSpawn = DamageUpMonsterClass; break;
		case 1: MonsterToSpawn = HpUpMonsterClass; break;
		case 2: MonsterToSpawn = SpeedUpMonsterClass; break;
		}
	}
	else
	{
		MonsterToSpawn = BaseMonsterClass;
	}

	if (MonsterToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AFRMonsterBase>(
			MonsterToSpawn,
			GetActorLocation(),
			GetActorRotation(),
			SpawnParams
		);
	}
}

