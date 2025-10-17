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

	// 최신 상태의 능력치 값을 얻기 위해 ASC에서 해당 AttributeSet을 가져와야 함
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
	const float AttackRange = AttributeSet->GetAttackRange();
	const float AttackRadius = AttributeSet->GetAttackRadius();

	// 여기서 3번째 인자는 무시할 자기 자신을 의미.
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UFRTA_MonsterTrace), false, Character);

	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel
	(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_FRMONSTERACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(),
			CapsuleOrigin,
			CapsuleHalfHeight,
			AttackRadius,
			FRotationMatrix::MakeFromZ(Forward).ToQuat(),
			DrawColor,
			false,
			4.0f);
	}
#endif
	return DataHandle;
}
