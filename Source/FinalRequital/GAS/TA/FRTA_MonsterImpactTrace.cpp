// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TA/FRTA_MonsterImpactTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FRDebugHelper.h"
#include "GameFramework/Character.h"
#include "GAS/Attribute/FRMonsterAttributeSet.h"
#include "Physics/FRCollision.h"

FGameplayAbilityTargetDataHandle AFRTA_MonsterImpactTrace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	// 최신 상태의 능력치 값을 얻기 위해 ASC에서 해당 AttributeSet을 가져와야 함
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		FR_LOG(FRLOG, Error, TEXT("ASC Not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UFRMonsterAttributeSet* AttributeSet = ASC->GetSet<UFRMonsterAttributeSet>();
	if (!AttributeSet)
	{
		FR_LOG(FRLOG, Error, TEXT("FRMonsterAttributeSet Not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const float Radius = AttributeSet->GetAttackRadius();

	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ImpactTrace), false, Character);
	const FVector Start = Character->GetActorLocation();

	bool bHit = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		Start, // OwnPosition
		FQuat::Identity,
		CCHANNEL_FRMONSTERACTION,
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	FGameplayAbilityTargetDataHandle Handle;
	if (bHit)
	{
		auto* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		Handle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		DrawDebugSphere(GetWorld(), Start, Radius, 16, bHit ? FColor::Green : FColor::Red, false, 2.f);
	}
#endif

	return Handle;
}
