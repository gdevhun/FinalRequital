// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/FRBossAttributeSet.h"

UFRBossAttributeSet::UFRBossAttributeSet() :
	AttackRadius(50.f),
	MaxAttackRadius(150.0f),
	AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRate(25.0f),
	MaxAttackRate(35.0f),
	MaxHealth(1000.0f),
	ReceivedBossDamage(0.0f)
{
	InitHealth(GetMaxHealth());
}


void UFRBossAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetReceivedBossDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

bool UFRBossAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UFRBossAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
