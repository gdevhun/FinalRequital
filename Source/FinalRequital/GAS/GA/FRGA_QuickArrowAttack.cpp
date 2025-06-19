// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/FRGA_QuickArrowAttack.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FRDebugHelper.h"
#include "Character/FRCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UFRGA_QuickArrowAttack::UFRGA_QuickArrowAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	//NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UFRGA_QuickArrowAttack::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UFRGA_QuickArrowAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
	(this, TEXT("PlayArrowAttack"), ArrowAttackMontage, 0.75f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UFRGA_QuickArrowAttack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UFRGA_QuickArrowAttack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

}

void UFRGA_QuickArrowAttack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UFRGA_QuickArrowAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UFRGA_QuickArrowAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	OnInterruptedCallback();
}

void UFRGA_QuickArrowAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
