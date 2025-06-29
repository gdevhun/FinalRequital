// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Skill/FRGA_MuSkillTrigger.h"

UFRGA_MuSkillTrigger::UFRGA_MuSkillTrigger()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_MuSkillTrigger::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
