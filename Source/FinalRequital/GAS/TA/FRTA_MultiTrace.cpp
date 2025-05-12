﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TA/FRTA_MultiTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FRDebugHelper.h"
#include "Physics/FRCollision.h"
#include "Engine/OverlapResult.h"

FGameplayAbilityTargetDataHandle AFRTA_MultiTrace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		FR_LOG(FRLOG, Error, TEXT("ASC Not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UFRCharacterAttributeSet* AttributeSet = ASC->GetSet<UFRCharacterAttributeSet>();
	if (!AttributeSet)
	{
		FR_LOG(FRLOG, Error, TEXT("FRCharacterAttributeSet Not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	// 1.5, 3배 확장된 범위
	const float AttackRange = AttributeSet->GetAttackRange() * 1.25f;
	const float AttackRadius = AttributeSet->GetAttackRadius() * 3.0f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UFRTA_MultiTrace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FRotationMatrix::MakeFromZ(Forward).ToQuat(),
		CCHANNEL_FRACTION,
		FCollisionShape::MakeCapsule(AttackRadius, AttackRange * 0.5f), // 캡슐형 감지로 대체 (더 넓고 납작하게)
		Params
	);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (bHit)
	{
		TSet<AActor*> UniqueActors;
		FGameplayAbilityTargetData_ActorArray* TargetData = new FGameplayAbilityTargetData_ActorArray();

		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != Character && !UniqueActors.Contains(HitActor))
			{
				UniqueActors.Add(HitActor);
				TargetData->TargetActorArray.Add(HitActor);
			}
		}

		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = bHit ? FColor::Green : FColor::Red;

		DrawDebugCapsule(
			GetWorld(),
			CapsuleOrigin,
			CapsuleHalfHeight,
			AttackRadius,
			FRotationMatrix::MakeFromZ(Forward).ToQuat(),
			DrawColor,
			false,
			4.0f
		);
	}
#endif

	return DataHandle;
}
