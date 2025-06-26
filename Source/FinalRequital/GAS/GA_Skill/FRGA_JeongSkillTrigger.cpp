// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Skill/FRGA_JeongSkillTrigger.h"

UFRGA_JeongSkillTrigger::UFRGA_JeongSkillTrigger()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_JeongSkillTrigger::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
