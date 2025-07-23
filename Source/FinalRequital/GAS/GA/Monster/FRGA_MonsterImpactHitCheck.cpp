// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/Monster/FRGA_MonsterImpactHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FRDebugHelper.h"
#include "FRGameplayTag.h"
#include "GAS/AT/FRAT_MonsterTrace.h"
#include "Player/FRGASCharacterPlayer.h"

UFRGA_MonsterImpactHitCheck::UFRGA_MonsterImpactHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_MonsterImpactHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UFRAT_MonsterTrace* AttackTraceTask = UFRAT_MonsterTrace::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UFRGA_MonsterImpactHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UFRGA_MonsterImpactHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 단일 대상 감지 Trace 기반
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		// 타겟데이터 0번째 배열에 결과값이 존재하는지
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

		// 플레이어 클래스인지 확인
		AFRGASCharacterPlayer* TargetPlayer = Cast<AFRGASCharacterPlayer>(HitResult.GetActor());
		if (!TargetPlayer)
		{
			// 플레이어가 아니면 처리하지 않음
			bool bReplicatedEndAbility = true;
			bool bWasCancelled = false;
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
			return;
		}

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (!SourceASC || !TargetASC)
		{
			// 감지한 액터의 ASC(타겟ASC)가 없고 나의 ASC(소스ASC)의 액터중 하나라도 없으면 에러
			FR_LOG(FRLOG, Error, TEXT("ASC Not Found!"));
			return;
		}
		if (TargetPlayer->GetCharacterMovement())
		{
			FVector PushDir = TargetPlayer->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation();
			PushDir.Z = 0.f;
			PushDir = PushDir.GetSafeNormal();
			TargetPlayer->LaunchCharacter(PushDir * PushForce, true, false);


			FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
			EffectContext.AddHitResult(HitResult);

			FGameplayCueParameters CueParameters;
			CueParameters.EffectContext = EffectContext;

			// GameplayCue 실행
			TargetASC->ExecuteGameplayCue(GAMEPLAYCUE_CHARACTER_MELEEATTACKHIT, CueParameters);

		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
