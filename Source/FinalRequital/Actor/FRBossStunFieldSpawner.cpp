

#include "Actor/FRBossStunFieldSpawner.h"
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

	// 기존 장판 제거
	for (AActor* Field : ActiveFields)
	{
		if (Field && IsValid(Field))
		{
			Field->Destroy();
		}
	}
	ActiveFields.Empty();

	FVector PlayerLocation = TargetPlayer->GetActorLocation();

	// 지정된 개수만큼 장판 생성
	for (int32 i = 0; i < FieldCount; ++i)
	{
		FVector SpawnLocation = GetRandomLocationAroundPlayer(PlayerLocation);
		SpawnSingleField(SpawnLocation);
	}
}

FVector AFRBossStunFieldSpawner::GetRandomLocationAroundPlayer(const FVector& PlayerLocation)
{
	// 랜덤 각도와 거리 생성
	float RandomAngle = FMath::RandRange(0.0f, 360.0f);
	float RandomDistance = FMath::RandRange(MinDistanceFromPlayer, MaxDistanceFromPlayer);

	// 2D 원형 범위에서 랜덤 위치 계산
	float RadianAngle = FMath::DegreesToRadians(RandomAngle);
	FVector Offset = FVector(
		FMath::Cos(RadianAngle) * RandomDistance,
		FMath::Sin(RadianAngle) * RandomDistance,
		0.0f
	);

	FVector TargetLocation = PlayerLocation + Offset;

	// 바닥에 맞추기 위한 LineTrace
	FHitResult HitResult;
	FVector TraceStart = TargetLocation + FVector(0.0f, 0.0f, 500.0f);
	FVector TraceEnd = TargetLocation - FVector(0.0f, 0.0f, 500.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		// 바닥을 찾았으면 그 위치 사용
		return HitResult.Location;
	}

	// 바닥을 못 찾았으면 플레이어와 같은 Z 높이 사용
	TargetLocation.Z = PlayerLocation.Z;
	return TargetLocation;
}

void AFRBossStunFieldSpawner::SpawnSingleField(const FVector& Location)
{
	if (!StunFieldActorClass || !GetWorld())
	{
		return;
	}

	// 경고 파티클 먼저 생성 (있으면)
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

	// SpawnDelay 후 실제 장판 생성
	FTimerHandle SpawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		[this, Location]()
		{
			// 블루프린트 액터 생성
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

				// 일정 시간 후 자동 제거
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