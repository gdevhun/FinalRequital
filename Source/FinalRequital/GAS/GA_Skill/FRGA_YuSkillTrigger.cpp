// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Skill/FRGA_YuSkillTrigger.h"

#include "GAS/Attribute/FRCharacterAttributeSet.h"

UFRGA_YuSkillTrigger::UFRGA_YuSkillTrigger()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFRGA_YuSkillTrigger::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();

	if (SelfApplyEffectClass && ASC)
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		const UFRCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UFRCharacterAttributeSet>();

		if (SourceAttribute)
		{
			float CurrentMana = SourceAttribute->GetMana();

			// 마나 + 1 한 값을 1~4 사이로 Clamp
			int32 EffectLevel = FMath::Clamp(FMath::RoundToInt(CurrentMana) + 1, 1, 4);

			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(SelfApplyEffectClass, EffectLevel, EffectContext);
			if (SpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
