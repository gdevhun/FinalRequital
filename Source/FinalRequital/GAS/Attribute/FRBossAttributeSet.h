// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FRBossAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutBossOfHealthDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossTakeDamageDelegate);
/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRBossAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
    UFRBossAttributeSet();

	ATTRIBUTE_ACCESSORS(UFRBossAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UFRBossAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UFRBossAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UFRBossAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UFRBossAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UFRBossAttributeSet, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(UFRBossAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UFRBossAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UFRBossAttributeSet, ReceivedBossDamage);

    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	mutable FOutBossOfHealthDelegate OnBossOutOfHealth;
	mutable FOnBossTakeDamageDelegate OnBossTakeDamage;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ReceivedBossDamage;

	bool bOutOfHealth = false;

};
