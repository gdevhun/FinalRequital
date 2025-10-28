// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "FRPhase3JangseungAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfPhase3JangseungHealthDelegate);

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRPhase3JangseungAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UFRPhase3JangseungAttributeSet();

	ATTRIBUTE_ACCESSORS(UFRPhase3JangseungAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UFRPhase3JangseungAttributeSet, MaxHealth);


	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// Delegate
	mutable FOutOfPhase3JangseungHealthDelegate OnOutOfHealth;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
};
