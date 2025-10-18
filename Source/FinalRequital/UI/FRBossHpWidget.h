// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FRUserWidget.h"
#include "FRBossHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRBossHpWidget : public UFRUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_Hp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_Name;

	UFUNCTION()
	void UpdateHpBar();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleOutOfHealth();
	void HandleOutOfHealth_Implementation();
};
