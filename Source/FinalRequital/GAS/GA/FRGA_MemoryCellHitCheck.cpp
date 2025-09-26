// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/FRGA_MemoryCellHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Actor/FRMemoryCell.h"
#include "GAS/AT/FRAT_Trace.h"
#include "FRDebugHelper.h"

UFRGA_MemoryCellHitCheck::UFRGA_MemoryCellHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_MemoryCellHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//D(FString::Printf(TEXT("TRIGGER!")));
	UFRAT_Trace* AttackTraceTask = UFRAT_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UFRGA_MemoryCellHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
	D(FString::Printf(TEXT("Detected!")));
}

void UFRGA_MemoryCellHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		if (AFRMemoryCell* MemoryCell = Cast<AFRMemoryCell>(HitResult.GetActor()))
		{
			MemoryCell->OnDetectedOnce();
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
