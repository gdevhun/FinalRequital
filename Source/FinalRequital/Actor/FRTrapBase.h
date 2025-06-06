// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "FRTrapBase.generated.h"


UCLASS()
class FINALREQUITAL_API AFRTrapBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AFRTrapBase();

protected:
	virtual void BeginPlay() override;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trap")
	TObjectPtr<class UStaticMeshComponent> TrapMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trap")
	TObjectPtr<class UBoxComponent> CollisionBox;

	/*UPROPERTY(EditAnywhere, Category = "Trap")
	TSubclassOf<class UGameplayEffect> DamageEffectClass;*/

};
