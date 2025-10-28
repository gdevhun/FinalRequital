// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FRPhase1JangseungAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfPhase1JangseungHealthDelegate);

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRPhase1JangseungAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UFRPhase1JangseungAttributeSet();

	ATTRIBUTE_ACCESSORS(UFRPhase1JangseungAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UFRPhase1JangseungAttributeSet, MaxHealth);


	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// Delegate
	mutable FOutOfPhase1JangseungHealthDelegate OnOutOfHealth;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
};
