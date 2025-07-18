// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Skill/FRGA_SkillBase.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FRDebugHelper.h"
#include "Character/FRCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Player/FRPlayerController.h"
#include "UI/HUD/FRHUDWidget.h"
#include "UI/HUD/FRSkillSlotWidget.h"

void UFRGA_SkillBase::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UFRGA_SkillBase::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UFRGA_SkillBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AFRCharacterBase* FRCharacterBase = CastChecked<AFRCharacterBase>(ActorInfo->AvatarActor.Get());

	if (bIsDisableMovementSkill) FRCharacterBase->GetCharacterMovement()->SetMovementMode(MOVE_None);
	else
	{
		if (AFRPlayerController* PC = Cast<AFRPlayerController>(FRCharacterBase->GetController()))
		{
			if (UFRHUDWidget* HUD = PC->GetHUDWidget())
			{
				HUD->ShowCrossHair(true);
			}
		}
	}

	float TimeRemaining = 0.f;
	float CooldownDuration = 0.f;

	GetCooldownTimeRemainingAndDuration(Handle, ActorInfo, TimeRemaining, CooldownDuration);

	if (AFRPlayerController* PC = Cast<AFRPlayerController>(ActorInfo->PlayerController.Get()))
	{
		if (UFRHUDWidget* HUD = PC->GetHUDWidget())
		{

			HUD->WBP_SkillSlot->StartSkillSlotCooldown(TimeRemaining, CooldownDuration);
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("remain: %.2f / total: %.2f"), TimeRemaining, CooldownDuration);
	// GameplayCue Execute
	if (GameplayCueToTrigger.IsValid())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = FRCharacterBase->GetActorLocation();
		CueParams.SourceObject = this;

		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		if (ASC)
		{
			ASC->AddGameplayCue(GameplayCueToTrigger, CueParams); 
		}
	}

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
	(this, TEXT("PlaySkillAction"), SkillActionMontage, 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UFRGA_SkillBase::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UFRGA_SkillBase::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

}

void UFRGA_SkillBase::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

void UFRGA_SkillBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AFRCharacterBase* FRCharacterBase = CastChecked<AFRCharacterBase>(ActorInfo->AvatarActor.Get());
	if (GameplayCueToTrigger.IsValid())
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveGameplayCue(GameplayCueToTrigger);
		}
	}

	if (bIsDisableMovementSkill) FRCharacterBase->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	else
	{
		if (AFRPlayerController* PC = Cast<AFRPlayerController>(FRCharacterBase->GetController()))
		{
			if (UFRHUDWidget* HUD = PC->GetHUDWidget())
			{
				HUD->ShowCrossHair(false);
			}
		}
	}
}

void UFRGA_SkillBase::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UFRGA_SkillBase::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
