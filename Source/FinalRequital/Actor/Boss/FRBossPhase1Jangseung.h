// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "FRBossPhase1Jangseung.generated.h"

UCLASS()
class FINALREQUITAL_API AFRBossPhase1Jangseung : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AFRBossPhase1Jangseung();


protected:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UFRPhase1JangseungAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class USceneComponent> RootScene;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UFRWidgetComponent> HpBar;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnOutOfHealth();
	virtual void OnOutOfHealth_Implementation();


};
