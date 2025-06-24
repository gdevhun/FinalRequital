// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TA/FRTA_FindSoulTrace.h"
#include "GAS/TA/FRTA_MultiTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "FRDebugHelper.h"
#include "Actor/FRSoulPiece.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"
#include "Physics/FRCollision.h"

FGameplayAbilityTargetDataHandle AFRTA_FindSoulTrace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);
	if (!Character) return FGameplayAbilityTargetDataHandle();

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		FR_LOG(FRLOG, Error, TEXT("ASC Not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UFRCharacterAttributeSet* AttributeSet = ASC->GetSet<UFRCharacterAttributeSet>();
	if (!AttributeSet)
	{
		FR_LOG(FRLOG, Error, TEXT("AttributeSet Not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	// 원형 장판 설정
	const float CircleRadius = AttributeSet->GetAttackRange() * 1.5f; // 장판 반지름

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UFRTA_FindSoulTrace), false, Character);

	// 캐릭터 중심 위치
	const FVector CharacterLocation = Character->GetActorLocation();
	const FVector CircleCenter = CharacterLocation; // 캐릭터 바로 아래

	// 원형 영역에서 단일 타겟 검색
	FHitResult OutHit;
	bool bHit = GetWorld()->SweepSingleByChannel(
		OutHit,
		CircleCenter,
		CircleCenter,FQuat::Identity,
		CCHANNEL_FRACTION,FCollisionShape::MakeSphere(CircleRadius),Params);

	FGameplayAbilityTargetDataHandle DataHandle;

	if (bHit && OutHit.GetActor()->IsA<AFRSoulPiece>())
	{
		DataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(OutHit));
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FColor DrawColor = bHit ? FColor::Green : FColor::Red;

		DrawDebugSphere(GetWorld(),CircleCenter,CircleRadius,16,
		DrawColor,false, 5.0f);
	}

#endif
	return DataHandle;
}