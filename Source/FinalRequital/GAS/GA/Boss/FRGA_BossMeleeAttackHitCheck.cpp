// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/Boss/FRGA_BossMeleeAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FRDebugHelper.h"
#include "FRGameplayTag.h"
#include "GAS/AT/FRAT_BossTrace.h"
#include "Player/FRGASCharacterPlayer.h"

UFRGA_BossMeleeAttackHitCheck::UFRGA_BossMeleeAttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_BossMeleeAttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UFRAT_BossTrace* AttackTraceTask = UFRAT_BossTrace::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UFRGA_BossMeleeAttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UFRGA_BossMeleeAttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
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

		if (bIsBossPullAttack)
		{
			AActor* BossActor = GetAvatarActorFromActorInfo();
			float PullDuration = 0.35f;
			float PullSpeed = 2000.0f;

			// 공유 포인터로 변수들 관리
			TSharedPtr<FTimerHandle> PullTimerHandle = MakeShared<FTimerHandle>();
			TSharedPtr<float> ElapsedTime = MakeShared<float>(0.0f);

			FTimerDelegate PullDelegate;
			PullDelegate.BindLambda([TargetPlayer, BossActor, ElapsedTime, PullDuration, PullSpeed, PullTimerHandle, this]()
				{
					*ElapsedTime += 0.016f;

					if (*ElapsedTime >= PullDuration || !TargetPlayer || !BossActor)
					{
						if (PullTimerHandle.IsValid() && PullTimerHandle->IsValid())
						{
							GetWorld()->GetTimerManager().ClearTimer(*PullTimerHandle);
						}
						return;
					}

					// 시간에 따라 속도 증가 (가속도 효과)
					float SpeedMultiplier = FMath::Lerp(0.5f, 2.0f, *ElapsedTime / PullDuration);

					FVector Direction = (BossActor->GetActorLocation() - TargetPlayer->GetActorLocation()).GetSafeNormal();
					FVector NewLocation = TargetPlayer->GetActorLocation() + Direction * PullSpeed * SpeedMultiplier * 0.016f;
					TargetPlayer->SetActorLocation(NewLocation, true);
				});

			GetWorld()->GetTimerManager().SetTimer(*PullTimerHandle, PullDelegate, 0.016f, true);
		}

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, 1);
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

	bool bReplicatedEndAbility = true;

	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
