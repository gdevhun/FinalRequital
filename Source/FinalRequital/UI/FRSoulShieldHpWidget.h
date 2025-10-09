// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FRUserWidget.h"
#include "FRSoulShieldHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRSoulShieldHpWidget : public UFRUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_Hp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_Shield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> HpTexture;
public:
	void ChangeImageName();

	UFUNCTION()
	void UpdateShieldBar();

	UFUNCTION()
	void UpdateHpBar();

	UFUNCTION()
	void HandleOutOfShield();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleOutOfHealth();
	void HandleOutOfHealth_Implementation();
};
