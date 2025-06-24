// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/FRGA_SoulPieceHitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FRDebugHelper.h"
#include "Actor/FRSoulPiece.h"
#include "GAS/AT/FRAT_Trace.h"

UFRGA_SoulPieceHitCheck::UFRGA_SoulPieceHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_SoulPieceHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UFRAT_Trace* AttackTraceTask = UFRAT_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UFRGA_SoulPieceHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
	FR_LOG(FRLOG, Error, TEXT("EXECUTE"));
}

void UFRGA_SoulPieceHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		FR_LOG(FRLOG, Error, TEXT("EXECUTE1"));
		if (AFRSoulPiece* SoulPiece = Cast<AFRSoulPiece>(HitResult.GetActor()))
		{
			SoulPiece->OnDetectedOnce();
			FR_LOG(FRLOG, Error, TEXT("EXECUTE DETECTED"));
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
