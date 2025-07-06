// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Trap/FRCCTVTrap.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SpotLightComponent.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemComponent.h"
#include "Player/FRGASCharacterPlayer.h"
#include "FRGameplayTag.h"

AFRCCTVTrap::AFRCCTVTrap()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	CCTVMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVMesh"));
	CCTVMesh->SetupAttachment(RootComponent);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(RootComponent);

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AFRCCTVTrap::BeginPlay()
{
	Super::BeginPlay();
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
	}

	SpotLight->OuterConeAngle = DetectionAngleDegrees;

	GetWorld()->GetTimerManager().SetTimer(
		TraceTimerHandle,
		this,
		&AFRCCTVTrap::DetectByLineTrace,
		0.2f, 
		true,
		StartDelay
	);
}

class UAbilitySystemComponent* AFRCCTVTrap::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRCCTVTrap::RotateSpotCycle()
{
}

void AFRCCTVTrap::DetectByLineTrace()
{
	const FVector Start = SpotLight->GetComponentLocation();
	const FVector Forward = SpotLight->GetForwardVector();
	const FVector Right = SpotLight->GetRightVector();
	const FVector Up = SpotLight->GetUpVector();
	const float ConeAngleRadians = FMath::DegreesToRadians(DetectionAngleDegrees);

	for (int i = 0; i < NumDetectionRays; ++i)
	{
		// 원뿔 내부에서 균등하게 분포된 방향 계산
		const float Angle = (2.0f * PI * i) / NumDetectionRays; // 원형 분포
		const float ConeRadius = FMath::FRandRange(0.0f, FMath::Sin(ConeAngleRadians)); // 원뿔 반지름
		const float ConeHeight = FMath::Cos(FMath::Asin(ConeRadius)); // 원뿔 높이

		// 로컬 좌표계에서 방향 벡터 계산
		const FVector LocalDirection = FVector(
			ConeHeight,
			ConeRadius * FMath::Cos(Angle),
			ConeRadius * FMath::Sin(Angle)
		);

		// 스포트라이트의 회전을 적용한 월드 방향
		const FVector WorldDirection = (Forward * LocalDirection.X +
			Right * LocalDirection.Y +
			Up * LocalDirection.Z).GetSafeNormal();
		const FVector End = Start + WorldDirection * DetectionRayLength;

#if WITH_EDITOR
		// 모든 레이를 빨간색으로 먼저 그리기
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.5f, 0, 1.5f);
#endif

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor) continue;

			AFRGASCharacterPlayer* Player = Cast<AFRGASCharacterPlayer>(HitActor);
			if (!Player || !StunEffectClass) continue;

			// 히트 지점이 실제로 원뿔 범위 안에 있는지 다시 확인
			const FVector HitDirection = (Hit.Location - Start).GetSafeNormal();
			const float DotProduct = FVector::DotProduct(Forward, HitDirection);
			const float HitAngle = FMath::Acos(DotProduct);

			if (HitAngle > ConeAngleRadians)
			{
				continue; // 원뿔 범위 밖이면 무시
			}

			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
			if (!TargetASC
				|| TargetASC->HasMatchingGameplayTag(FRTAG_CHARACTER_STUNNED)
				|| TargetASC->HasMatchingGameplayTag(FRTAG_CHARACTER_INVISIBLE))
			{
				continue;
			}

			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(StunEffectClass, 1.0f, EffectContext);
			if (SpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
			}

#if WITH_EDITOR
			DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Green, false, 0.5f, 0, 2.0f);
			DrawDebugSphere(GetWorld(), Hit.Location, 10.0f, 12, FColor::Cyan, false, 0.5f, 0, 2.0f);
#endif
		}
	}
}
