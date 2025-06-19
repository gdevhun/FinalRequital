// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/TA/FRTA_RangeMultiTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FRDebugHelper.h"
#include "Physics/FRCollision.h"
#include "Engine/OverlapResult.h"

FGameplayAbilityTargetDataHandle AFRTA_RangeMultiTrace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		FR_LOG(FRLOG, Error, TEXT("ASC Not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	TArray<FOverlapResult> Overlaps;
	const float SkillRadius = 350.0f;

	FVector Origin = Character->GetActorLocation();
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AFRTA_RangeMultiTrace), false, Character);
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, CCHANNEL_FRACTION, FCollisionShape::MakeSphere(SkillRadius), Params);

	TArray<TWeakObjectPtr<AActor>> HitActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
		}
	}
	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();
	ActorsData->SetActors(HitActors);

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16, DrawColor, false, 5.0f);
	}

#endif

	return FGameplayAbilityTargetDataHandle(ActorsData);
}
