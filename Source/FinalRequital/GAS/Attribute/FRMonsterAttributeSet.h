// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FRMonsterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutMonsterOfHealthDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMonsterTakeDamageDelegate);

UCLASS()
class FINALREQUITAL_API UFRMonsterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UFRMonsterAttributeSet();

	ATTRIBUTE_ACCESSORS(UFRMonsterAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UFRMonsterAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UFRMonsterAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UFRMonsterAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UFRMonsterAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UFRMonsterAttributeSet, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(UFRMonsterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UFRMonsterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UFRMonsterAttributeSet, Damage);

	// PreAttributeChange -> 값이 실제로 적용되기 직전에 호출됨
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// PostAttributeChange -> 값이 실제로 변경된 직후에 호출됨
	// virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// 사용하는 ASC에서 특정 어트리뷰트를 접근할 때 가져오는 GetSet함수는 const로 선언되있음.
	// 이 Delegate를 연동하려하면 const를 위반하기 때문에 const cast 대신 mutable 키워드로 const에서 열외시킴.
	mutable FOutMonsterOfHealthDelegate OnMonsterOutOfHealth;
	mutable FOnMonsterTakeDamageDelegate OnMonsterTakeDamage;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	bool bOutOfHealth = false;
};
