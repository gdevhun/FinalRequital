// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/FRCharacterAttributeSet.h"
#include "FRDebugHelper.h"
#include "FRGameplayTag.h"
#include "GameplayEffectExtension.h"

UFRCharacterAttributeSet::UFRCharacterAttributeSet() :
	AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.f),
	MaxAttackRadius(150.0f),
	AttackRate(20.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f),
	Mana(0.0f),
	MaxMana(4.0f),
	ReceivedPlayerDamage(0.0f)
{
	InitHealth(GetMaxHealth());
	InitMana(GetMana());
}

void UFRCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	/*if (Attribute == GetHealthAttribute())
	{
		// MaxHealth값을 넘거나 0아래로 내려가지 않도록 설정
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}*/

	if(Attribute == GetReceivedPlayerDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

bool UFRCharacterAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	// 게임플레이 이펙트가 적용되기전 사전 처리용 함수
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute==GetReceivedPlayerDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude>0.0f)
		{
			if (Data.Target.HasMatchingGameplayTag(FRTAG_CHARACTER_INVISIBLE))
			{
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}
	return true;
}

void UFRCharacterAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	//DATA -> GameplayEffect가 적용될 때 ASC가 내부적으로 AttributeSet에 넘겨주는 데이터 구조
	Super::PostGameplayEffectExecute(Data);
	//Target -> 이 AttributeSet이 붙은 Actor의 ASC(UAbilitySystemComponent&)
	//EffectSpec -> 어떤 GE(GameplayEffect)가 실행됐는지 정보 포함
	//EvaluatedData.Attribute -> 영향을 받은 속성(예: Health, Damage 등)
	//EvaluatedData.Magnitude -> 적용된 변화량
	//Context -> Effect를 유발한 원인(시전자, 무기, 위치 등 정보 포함)

	float MinimumHealth = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Health 값이 직접 수정된 경우 → 클램프만
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetReceivedPlayerDamageAttribute())
	{
		// 체력 감소는 여기서만 적용
		const float DamageDone = GetReceivedPlayerDamage();
		if (DamageDone > 0.0f)
		{
			const float NewHealth = GetHealth() - DamageDone;
			SetHealth(FMath::Clamp(NewHealth, MinimumHealth, GetMaxHealth()));

			OnTakeDamage.Broadcast();
			D(FString::Printf(TEXT("TRIGGER12!")));
		}

		// 다음 틱에서 중복 안되게 리셋
		SetReceivedPlayerDamage(0.0f);
	}
	//죽음 체크
	if (GetHealth() <= 0.0f && !bOutOfHealth)
	{
		//Data.Target.AddLooseGameplayTag(FRTAG_CHARACTER_ISDEAD);
		//OnOutOfHealth.Broadcast();
		Data.Target.AddLooseGameplayTag(FRTAG_CHARACTER_ISDEAD);
		OnOutOfHealth.Broadcast();
	}
	bOutOfHealth = (GetHealth() <= 0.0f);
}

/*void UFRCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    // 이펙트로만 활성화 하면 의미없음.
	if (Attribute == GetHealthAttribute())
	{
		FR_LOG(FRLOG, Log, TEXT("HEALTH: %f -> %f"), OldValue, NewValue);
	}
}*/
