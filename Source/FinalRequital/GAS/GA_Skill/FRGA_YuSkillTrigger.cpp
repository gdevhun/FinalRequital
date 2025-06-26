// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Skill/FRGA_YuSkillTrigger.h"

UFRGA_YuSkillTrigger::UFRGA_YuSkillTrigger()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_YuSkillTrigger::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
