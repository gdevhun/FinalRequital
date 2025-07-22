// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/Monster/FRGA_MonsterImpactHitCheck.h"

UFRGA_MonsterImpactHitCheck::UFRGA_MonsterImpactHitCheck()
{
}

void UFRGA_MonsterImpactHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UFRGA_MonsterImpactHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
}
