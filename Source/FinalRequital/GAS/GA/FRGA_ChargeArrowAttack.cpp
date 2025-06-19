// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/GA/FRGA_ChargeArrowAttack.h"
#include "GAS/GA/FRGA_SpawnArrow.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FRDebugHelper.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/FRCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UFRGA_ChargeArrowAttack::UFRGA_ChargeArrowAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_ChargeArrowAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	AFRCharacterBase* FRCharacterBase = CastChecked<AFRCharacterBase>(ActorInfo->AvatarActor.Get());
	FRCharacterBase->GetCharacterMovement()->SetMovementMode(MOVE_None);

	// 카메라 줌인
	FRCharacterBase->SetZooming(true);

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
	(this, TEXT("PlayChargeArrowAttack"), ChargeArrowAttackMontage, 1.0f);

	PlayAttackTask->OnCompleted.AddDynamic(this, &UFRGA_ChargeArrowAttack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UFRGA_ChargeArrowAttack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
	
}
void UFRGA_ChargeArrowAttack::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}
/*void UFRGA_ChargeArrowAttack::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) {
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorInfo->AvatarActor.Get());
	if (ASC && SpawnArrowAbilityClass) 
	{
		FGameplayAbilitySpec AbilitySpec(SpawnArrowAbilityClass, 1, INDEX_NONE);
		ASC->TryActivateAbility(ASC->GiveAbility(AbilitySpec));
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}*/

void UFRGA_ChargeArrowAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UFRGA_ChargeArrowAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	AFRCharacterBase* FRCharacterBase = CastChecked<AFRCharacterBase>(ActorInfo->AvatarActor.Get());
	FRCharacterBase->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// 카메라 줌아웃
	FRCharacterBase->SetZooming(false);
}

void UFRGA_ChargeArrowAttack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UFRGA_ChargeArrowAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}