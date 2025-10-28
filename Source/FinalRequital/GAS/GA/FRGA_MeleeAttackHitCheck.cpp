// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/FRGA_MeleeAttackHitCheck.h"
#include "GAS/AT/FRAT_Trace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FRDebugHelper.h"
#include "FRGameplayTag.h"
#include "Actor/FRHintBox.h"
#include "Actor/Boss/FRBossPhase1Jangseung.h"
#include "Actor/Boss/FRBossPhase2Jangseung.h"
#include "Character/FRBossAsura.h"
#include "Character/FRMonsterBase.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"
#include "Player/FRGASCharacterPlayer.h"
#include "Player/FRWeaponComponent.h"

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
	/* 무기 타입 확인
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	AFRGASCharacterPlayer* OwnerCharacter = Cast<AFRGASCharacterPlayer>(AvatarActor);
	if (!OwnerCharacter)
		return;
	
	 //캐릭터가 가지고 있는 무기 컴포넌트에서 현재 무기 타입 가져오기
	EWeaponType CurrentWeaponType = OwnerCharacter->GetWeaponComponent()->GetCurrentWeaponType();
	bool bIsMace = (CurrentWeaponType == EWeaponType::IronMace);
	bool bIsSword = (CurrentWeaponType == EWeaponType::Sword);*/

	// ===========================
	// 단일 대상 감지
	// ===========================
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		AActor* HitActor = HitResult.GetActor();

		if (!HitActor)
			return;

		// 유효 액터 체크
		bool bIsMonster = HitActor->IsA(AFRMonsterBase::StaticClass());
		bool bIsHintBox = HitActor->IsA(AFRHintBox::StaticClass());
		bool bIsBoss = HitActor->IsA(AFRBossAsura::StaticClass());
		bool bIsBossPhase1Jangseung = HitActor->IsA(AFRBossPhase1Jangseung::StaticClass());
		bool bIsBossPhase2Jangseung = HitActor->IsA(AFRBossPhase2Jangseung::StaticClass());

		if (!bIsMonster && !bIsHintBox && !bIsBoss && !bIsBossPhase1Jangseung && !bIsBossPhase2Jangseung)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
			return;
		}

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!SourceASC || !TargetASC)
			return;

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

	// ===========================
	// 다수 대상 감지
	// ===========================
	else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		//  필터링
		TArray<TWeakObjectPtr<AActor>> FilteredActors;
		TArray<TWeakObjectPtr<AActor>> AllActors = TargetDataHandle.Data[0].Get()->GetActors();

		for (const TWeakObjectPtr<AActor>& ActorPtr : AllActors)
		{
			if (!ActorPtr.IsValid())
				continue;

			AActor* Target = ActorPtr.Get();
			if (Target->IsA(AFRMonsterBase::StaticClass()) || Target->IsA(AFRBossAsura::StaticClass()))
			{
				FilteredActors.Add(ActorPtr);
			}
		}

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
			// 이펙트 적용
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
