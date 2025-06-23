// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/FRGA_FindSoulPiece.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/FRCharacterBase.h"

UFRGA_FindSoulPiece::UFRGA_FindSoulPiece()
{
}

void UFRGA_FindSoulPiece::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UFRGA_FindSoulPiece::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UFRGA_FindSoulPiece::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AFRCharacterBase* FRCharacterBase = CastChecked<AFRCharacterBase>(ActorInfo->AvatarActor.Get());
	FRCharacterBase->GetCharacterMovement()->SetMovementMode(MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
	(this, TEXT("PlayFindingSoulPiece"), FindSoulPieceMontage, 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UFRGA_FindSoulPiece::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UFRGA_FindSoulPiece::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
}

void UFRGA_FindSoulPiece::CancelAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UFRGA_FindSoulPiece::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AFRCharacterBase* FRCharacterBase = CastChecked<AFRCharacterBase>(ActorInfo->AvatarActor.Get());
	FRCharacterBase->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void UFRGA_FindSoulPiece::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UFRGA_FindSoulPiece::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
