// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "FRHintBox.generated.h"

UCLASS()
class FINALREQUITAL_API AFRHintBox : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AFRHintBox();

protected:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UFRHintBoxAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class USceneComponent> RootScene;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UFRWidgetComponent> HpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<class USphereComponent> DetectSphere;

public:	
	UFUNCTION(BlueprintNativeEvent)
	void OnOutOfHealth();
	virtual void OnOutOfHealth_Implementation();

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
