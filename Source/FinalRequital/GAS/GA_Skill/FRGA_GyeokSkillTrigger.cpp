// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Skill/FRGA_GyeokSkillTrigger.h"

UFRGA_GyeokSkillTrigger::UFRGA_GyeokSkillTrigger()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_GyeokSkillTrigger::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
