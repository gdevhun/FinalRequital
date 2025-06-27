// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Skill/FRGA_MyeolSkillTrigger.h"

#include "AbilitySystemComponent.h"

UFRGA_MyeolSkillTrigger::UFRGA_MyeolSkillTrigger()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_MyeolSkillTrigger::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();
	if (SelfApplyEffectClass)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(SelfApplyEffectClass, 1.0f, EffectContext);
		if (SpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		}
	}
	

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
