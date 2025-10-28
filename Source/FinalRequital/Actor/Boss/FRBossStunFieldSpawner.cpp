// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Boss/FRBossStunFieldSpawner.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

AFRBossStunFieldSpawner::AFRBossStunFieldSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFRBossStunFieldSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AFRBossStunFieldSpawner::SpawnStunFields(AActor* TargetPlayer)
{
	if (!TargetPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnStunFields: TargetPlayer is null!"));
		return;
	}

	if (!StunFieldActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnStunFields: StunFieldActorClass is not set!"));
		return;
	}

	// ���� ���� ����
	for (AActor* Field : ActiveFields)
	{
		if (Field && IsValid(Field))
		{
			Field->Destroy();
		}
	}
	ActiveFields.Empty();

	FVector PlayerLocation = TargetPlayer->GetActorLocation();

	// ������ ������ŭ ���� ����
	for (int32 i = 0; i < FieldCount; ++i)
	{
		FVector SpawnLocation = GetRandomLocationAroundPlayer(PlayerLocation);
		SpawnSingleField(SpawnLocation);
	}
}

FVector AFRBossStunFieldSpawner::GetRandomLocationAroundPlayer(const FVector& PlayerLocation)
{
	// ���� ������ �Ÿ� ����
	float RandomAngle = FMath::RandRange(0.0f, 360.0f);
	float RandomDistance = FMath::RandRange(MinDistanceFromPlayer, MaxDistanceFromPlayer);

	// 2D ���� �������� ���� ��ġ ���
	float RadianAngle = FMath::DegreesToRadians(RandomAngle);
	FVector Offset = FVector(
		FMath::Cos(RadianAngle) * RandomDistance,
		FMath::Sin(RadianAngle) * RandomDistance,
		0.0f
	);

	FVector TargetLocation = PlayerLocation + Offset;

	// �ٴڿ� ���߱� ���� LineTrace
	FHitResult HitResult;
	FVector TraceStart = TargetLocation + FVector(0.0f, 0.0f, 500.0f);
	FVector TraceEnd = TargetLocation - FVector(0.0f, 0.0f, 500.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		// �ٴ��� ã������ �� ��ġ ���
		return HitResult.Location;
	}

	// �ٴ��� �� ã������ �÷��̾�� ���� Z ���� ���
	TargetLocation.Z = PlayerLocation.Z;
	return TargetLocation;
}

void AFRBossStunFieldSpawner::SpawnSingleField(const FVector& Location)
{
	if (!StunFieldActorClass || !GetWorld())
	{
		return;
	}

	// ��� ��ƼŬ ���� ���� (������)
	if (WarningEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			WarningEffect,
			Location,
			FRotator::ZeroRotator,
			FVector(1.0f),
			true,
			EPSCPoolMethod::AutoRelease,
			true
		);
	}

	// SpawnDelay �� ���� ���� ����
	FTimerHandle SpawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		[this, Location]()
		{
			// ��������Ʈ ���� ����
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AActor* FieldActor = GetWorld()->SpawnActor<AActor>(
				StunFieldActorClass,
				Location,
				FRotator::ZeroRotator,
				SpawnParams
			);

			if (FieldActor)
			{
				ActiveFields.Add(FieldActor);

				// ���� �ð� �� �ڵ� ����
				if (FieldLifetime > 0.0f)
				{
					FTimerHandle DestroyTimerHandle;
					GetWorld()->GetTimerManager().SetTimer(
						DestroyTimerHandle,
						[FieldActor]()
						{
							if (FieldActor && IsValid(FieldActor))
							{
								FieldActor->Destroy();
							}
						},
						FieldLifetime,
						false
					);
				}
			}
		},
		1.0f,
		false
	);

}