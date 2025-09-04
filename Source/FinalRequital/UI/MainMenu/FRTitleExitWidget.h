// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MainMenu/FRStackBaseWidget.h"
#include "FRTitleExitWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRTitleExitWidget : public UFRStackBaseWidget
{
	GENERATED_BODY()
public:
	UFRTitleExitWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_No;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Yes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Back;

private:
	UFUNCTION()
	void OnNoBtnCallback();
	UFUNCTION()
	void OnYesBtnCallback();
	UFUNCTION()
	void OnBackBtnCallback();

};
