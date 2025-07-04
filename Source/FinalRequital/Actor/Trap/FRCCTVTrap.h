// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "FRCCTVTrap.generated.h"

UCLASS()
class FINALREQUITAL_API AFRCCTVTrap : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AFRCCTVTrap();

protected:
	virtual void BeginPlay() override;

protected:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USceneComponent> RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> CCTVMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USpotLightComponent> SpotLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> StunEffectClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StartDelay")
	float StartDelay = 0.0f;

private:
	void DetectByLineTrace();
	FTimerHandle TraceTimerHandle;

};
