// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FRSoulShieldAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfSoulShieldDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDamagedOfSoulShieldDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfSoulHealthDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDamagedOfSoulHealthDelegate);

UCLASS()
class FINALREQUITAL_API UFRSoulShieldAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UFRSoulShieldAttributeSet();

public:
	ATTRIBUTE_ACCESSORS(UFRSoulShieldAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UFRSoulShieldAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UFRSoulShieldAttributeSet, Shield);
	ATTRIBUTE_ACCESSORS(UFRSoulShieldAttributeSet, MaxShield);

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// Delegate
	mutable FOutOfSoulHealthDelegate OnOutOfHealth;
	mutable FOutOfSoulShieldDelegate OnOutOfShield;
	mutable FDamagedOfSoulHealthDelegate OnDamagedOfHealth;
	mutable FDamagedOfSoulShieldDelegate OnDamagedOfShield;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Shield", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Shield;

	UPROPERTY(BlueprintReadOnly, Category = "Shield", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxShield;
};
