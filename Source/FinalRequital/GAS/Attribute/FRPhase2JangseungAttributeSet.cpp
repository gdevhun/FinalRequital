// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/FRPhase2JangseungAttributeSet.h"
#include "GameplayEffectExtension.h"

UFRPhase2JangseungAttributeSet::UFRPhase2JangseungAttributeSet()
{
	InitMaxHealth(4.0f);
	InitHealth(GetMaxHealth());
}

void UFRPhase2JangseungAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		float NewHealth = GetHealth();
		float ClampedHealth = FMath::Clamp(NewHealth, 0.f, GetMaxHealth());
		SetHealth(ClampedHealth);

		// hp 0
		if (ClampedHealth <= 0.f)
		{
			OnOutOfHealth.Broadcast();
		}
	}
}
