// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/FRBossAttributeSet.h"

UFRBossAttributeSet::UFRBossAttributeSet()
{
}

void UFRBossAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

bool UFRBossAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UFRBossAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
