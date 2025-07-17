// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/FRGA_SpecialMeleeAttack.h"
#include "Character/FRCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/FRPlayerController.h"
#include "UI/HUD/FRHUDWidget.h"
#include "UI/HUD/FRWeaponSlotWidget.h"

UFRGA_SpecialMeleeAttack::UFRGA_SpecialMeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_SpecialMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	AFRCharacterBase* TargetCharacter = CastChecked<AFRCharacterBase>(ActorInfo->AvatarActor.Get());
	TargetCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
	(this, TEXT("PlaySpecialMeleeAttack"), SpecialAttackActionMontage, 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UFRGA_SpecialMeleeAttack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UFRGA_SpecialMeleeAttack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();


	float TimeRemaining = 0.f;
	float CooldownDuration = 0.f;

	GetCooldownTimeRemainingAndDuration(Handle, ActorInfo, TimeRemaining, CooldownDuration);

	if (AFRPlayerController* PC = Cast<AFRPlayerController>(ActorInfo->PlayerController.Get()))
	{
		if (UFRHUDWidget* HUD = PC->GetHUDWidget())
		{
			if (bIsSwordAttack)
			{
				HUD->WBP_WeaponSlot_1->StartSlotCooldown(TimeRemaining, CooldownDuration);
			}
			else
			{
				HUD->WBP_WeaponSlot_3->StartSlotCooldown(TimeRemaining, CooldownDuration);
			}

		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("remain: %.2f / total: %.2f"), TimeRemaining, CooldownDuration);
}

void UFRGA_SpecialMeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	AFRCharacterBase* FRCharacterBase = CastChecked<AFRCharacterBase>(ActorInfo->AvatarActor.Get());
	FRCharacterBase->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFRGA_SpecialMeleeAttack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UFRGA_SpecialMeleeAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
