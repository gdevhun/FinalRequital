// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/FRGA_MeleeAttackHitCheck.h"
#include "GAS/AT/FRAT_Trace.h"
#include "GAS/TA/FRTA_Trace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FRDebugHelper.h"
#include "FRGameplayTag.h"
#include "Character/FRMonsterBase.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"

UFRGA_MeleeAttackHitCheck::UFRGA_MeleeAttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_MeleeAttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//콤보 현재 레벨을 에님노티파이에서 페이로드 데이터로부터 가저온 것으로 적용
	CurrentLevel = TriggerEventData->EventMagnitude;

	UFRAT_Trace* AttackTraceTask = UFRAT_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UFRGA_MeleeAttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();

}
void UFRGA_MeleeAttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 단일 대상 감지 Trace 기반, SweepSingle 등의 HitResult 사용
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		// 타겟데이터 0번째 배열에 결과값이 존재하는지
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

		// 몬스터 클래스인지 확인
		AFRMonsterBase* TargetMonster = Cast<AFRMonsterBase>(HitResult.GetActor());
		if (!TargetMonster)
		{
			// 몬스터가 아니면 처리하지 않음
			bool bReplicatedEndAbility = true;
			bool bWasCancelled = false;
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
			return;
		}

		if (CameraShakeClass)
		{   // 피격시 카메라 쉐이크적용
			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			APawn* SourcePawn = Cast<APawn>(AvatarActor);
			if (SourcePawn)
			{
				APlayerController* SourcePC = Cast<APlayerController>(SourcePawn->GetController());
				if (SourcePC)
				{
					SourcePC->ClientStartCameraShake(CameraShakeClass);
				}
			}
		}

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		const UFRCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UFRCharacterAttributeSet>();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (!SourceASC || !TargetASC)
		{
			// 감지한 액터의 ASC(타겟ASC)가 없고 나의 ASC(소스ASC)의 액터중 하나라도 없으면 에러
			FR_LOG(FRLOG, Error, TEXT("ASC Not Found!"));
			return;
		}

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddHitResult(HitResult);
			FGameplayCueParameters CueParameters;
			CueParameters.EffectContext = CueContextHandle;
			TargetASC->ExecuteGameplayCue(GAMEPLAYCUE_CHARACTER_MELEEATTACKHIT, CueParameters);
		}
	}
	// 다수 대상 감지 - OverlapMulti 의 Actor 배열 사용
	else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		// 몬스터만 필터링
		TArray<TWeakObjectPtr<AActor>> FilteredActors;
		TArray<TWeakObjectPtr<AActor>> AllActors = TargetDataHandle.Data[0].Get()->GetActors();

		for (const TWeakObjectPtr<AActor>& ActorPtr : AllActors)
		{
			if (ActorPtr.IsValid())
			{
				AFRMonsterBase* Monster = Cast<AFRMonsterBase>(ActorPtr.Get());
				if (Monster)
				{
					FilteredActors.Add(ActorPtr);
				}
			}
		}

		// 몬스터가 하나도 없으면 처리하지 않음
		if (FilteredActors.Num() == 0)
		{
			bool bReplicatedEndAbility = true;
			bool bWasCancelled = false;
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
			return;
		}

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			// 필터링된 몬스터들에게만 이펙트 적용
			FGameplayAbilityTargetData_ActorArray* FilteredTargetData = new FGameplayAbilityTargetData_ActorArray();
			FilteredTargetData->SetActors(FilteredActors);
			FGameplayAbilityTargetDataHandle FilteredTargetDataHandle(FilteredTargetData);

			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, FilteredTargetDataHandle);

			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddActors(FilteredActors, false);
			FGameplayCueParameters CueParameters;
			CueParameters.EffectContext = CueContextHandle;

			SourceASC->ExecuteGameplayCue(GAMEPLAYCUE_CHARACTER_MELEEATTACKHIT, CueParameters);
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
