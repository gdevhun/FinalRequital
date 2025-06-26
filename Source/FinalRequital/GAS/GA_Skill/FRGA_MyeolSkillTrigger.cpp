// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Skill/FRGA_MyeolSkillTrigger.h"

UFRGA_MyeolSkillTrigger::UFRGA_MyeolSkillTrigger()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_MyeolSkillTrigger::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
