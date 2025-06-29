// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/FRMonsterAttributeSet.h"
#include "FRGameplayTag.h"
#include "GameplayEffectExtension.h"

UFRMonsterAttributeSet::UFRMonsterAttributeSet() :
	AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.f),
	MaxAttackRadius(150.0f),
	AttackRate(20.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f),
	ReceivedMonsterDamage(0.0f)
{
	InitHealth(GetMaxHealth());
}

void UFRMonsterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetReceivedMonsterDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

bool UFRMonsterAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	// 게임플레이 이펙트가 적용되기전 사전 처리용 함수
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetReceivedMonsterDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			if (Data.Target.HasMatchingGameplayTag(FRTAG_CHARACTER_INVISIBLE))
			{
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
			if (bOutOfHealth || Data.Target.HasMatchingGameplayTag(FRTAG_CHARACTER_ISDEAD))
			{
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}
	return true;
}

void UFRMonsterAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	//DATA -> GameplayEffect가 적용될 때 ASC가 내부적으로 AttributeSet에 넘겨주는 데이터 구조
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetReceivedMonsterDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetReceivedMonsterDamage(), MinimumHealth, GetMaxHealth()));
		OnMonsterTakeDamage.Broadcast();
		SetReceivedMonsterDamage(0.0f);
	}
	// 죽는 기능 구현
	if (GetHealth() <= 0.0f && !bOutOfHealth)
	{
		Data.Target.AddLooseGameplayTag(FRTAG_CHARACTER_ISDEAD);
		OnMonsterOutOfHealth.Broadcast();
	}
	bOutOfHealth = (GetHealth() <= 0.0f);
}

