// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TA/FRTA_BossTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FRDebugHelper.h"
#include "Abilities/GameplayAbility.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GAS/Attribute/FRBossAttributeSet.h"
#include "Physics/FRCollision.h"

AFRTA_BossTrace::AFRTA_BossTrace()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFRTA_BossTrace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AFRTA_BossTrace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();

		// 어빌리티 태스크에서 구독한 델리게이트 방송으로 호출
		// 타겟엑터 내부에 델리게이트 변수가 존재, 이를 활용
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}
FGameplayAbilityTargetDataHandle AFRTA_BossTrace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		FR_LOG(FRLOG, Error, TEXT("ASC Not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UFRBossAttributeSet* AttributeSet = ASC->GetSet<UFRBossAttributeSet>();
	if (!AttributeSet)
	{
		FR_LOG(FRLOG, Error, TEXT("FRBossAttributeSet Not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	FHitResult OutHitResult;

	// 감지 범위 15배로 확대
	const float AttackRange = AttributeSet->GetAttackRange() * 15.0f;
	const float AttackRadius = AttributeSet->GetAttackRadius() * 15.0f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UFRTA_BossTrace), false, Character);

	// "왼쪽 앞" 방향 계산
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Left = -Character->GetActorRightVector();

	// Forward와 Left를 섞어서 대각선 방향 생성 (45도 방향)
	FVector DiagonalDir = (Forward + Left).GetSafeNormal();

	// 트레이스 시작/끝점 설정
	const FVector Start = Character->GetActorLocation() + DiagonalDir * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + DiagonalDir * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::FindBetweenNormals(FVector::ForwardVector, DiagonalDir), // 방향 회전 적용
		CCHANNEL_FRMONSTERACTION,
		FCollisionShape::MakeBox(FVector(AttackRange * 0.5f, AttackRadius, AttackRadius)), // 박스 크기
		Params
	);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FVector BoxCenter = Start + (End - Start) * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

		DrawDebugBox(
			GetWorld(),
			BoxCenter,
			FVector(AttackRange * 0.5f, AttackRadius, AttackRadius),
			FQuat::FindBetweenNormals(FVector::ForwardVector, DiagonalDir),
			DrawColor,
			false,
			4.0f
		);
	}
#endif

	return DataHandle;
}
