// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/FRSoulShieldAttributeSet.h"
#include "FRDebugHelper.h"
#include "GameplayEffectExtension.h"

UFRSoulShieldAttributeSet::UFRSoulShieldAttributeSet() 
{
    InitMaxHealth(5.0f);
    InitHealth(GetMaxHealth());

    InitMaxShield(15.0f);
    InitShield(GetMaxShield());
}

void UFRSoulShieldAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		D(FString::Printf(TEXT("Current shield: %f, Max shield: %f"),  GetShield(), GetMaxShield() ));
		OnDamagedOfShield.Broadcast();

		float NewShield = GetShield();
		if (NewShield <= 0.f)
		{
			SetShield(0.f);
			OnOutOfShield.Broadcast(); // Shield -> 0
		}
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		OnDamagedOfHealth.Broadcast();

		float NewHealth = GetHealth();
		if (NewHealth <= 0.f)
		{
			SetHealth(0.f);
			OnOutOfHealth.Broadcast(); // hp -> 0
		}
	}
}
