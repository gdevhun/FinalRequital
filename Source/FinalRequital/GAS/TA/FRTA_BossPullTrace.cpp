// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TA/FRTA_BossPullTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"
#include "FRDebugHelper.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GAS/Attribute/FRBossAttributeSet.h"
#include "Physics/FRCollision.h"

FGameplayAbilityTargetDataHandle AFRTA_BossPullTrace::MakeTargetData() const
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

	// 어트리뷰트 기반 거리, 반경
	const float AttackRange = AttributeSet->GetAttackRange() * 20.0f;  // 약 10m
	const float AttackWidth = AttributeSet->GetAttackRadius() * 3.f;  // 좌우 폭
	const float AttackHeight = 150.0f;                                   // 상하 높이 (살짝 얕게)
	const float DownOffset = 600.0f;                                     // 아래로 내릴 정도

	const FVector Forward = Character->GetActorForwardVector();

	// 살짝 아래로 내린 시작 위치
	FVector Start = Character->GetActorLocation() - FVector(0, 0, DownOffset);
	Start += Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();

	// 전방 끝점
	const FVector End = Start + Forward * AttackRange;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(BossPullTraceBox), false, Character);

	// 박스 반쪽 크기 (절반값)
	const FVector BoxHalfExtent(AttackRange * 0.5f, AttackWidth * 0.5f, AttackHeight * 0.5f);

	// Sweep (박스 형태)
	bool bHit = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FRotationMatrix::MakeFromX(Forward).ToQuat(), // 전방 방향 회전
		CCHANNEL_FRMONSTERACTION,
		FCollisionShape::MakeBox(BoxHalfExtent),
		Params
	);

	// 감지된 타겟 데이터 처리
	FGameplayAbilityTargetDataHandle DataHandle;
	if (bHit)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FVector BoxCenter = Start + (End - Start) * 0.5f;
		FColor DrawColor = bHit ? FColor::Green : FColor::Red;

		DrawDebugBox(
			GetWorld(),
			BoxCenter,
			BoxHalfExtent,
			FRotationMatrix::MakeFromX(Forward).ToQuat(),
			DrawColor,
			false,
			4.0f
		);
	}
#endif

	return DataHandle;
}