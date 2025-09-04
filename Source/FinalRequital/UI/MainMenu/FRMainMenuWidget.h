// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FRStackBaseWidget.h"
#include "FRMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRMainMenuWidget : public UFRStackBaseWidget
{
	GENERATED_BODY()

public:
	UFRMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
protected:

	// UI REF Class, Ptr 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIClassRef")
	TSubclassOf<class UFRGameSettingWidget> FRGameSettingWidgetClass;
	UPROPERTY()
	TObjectPtr<UFRGameSettingWidget> GameSettingWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIClassRef")
	TSubclassOf<class UFRProfileWidget> FRProfileWidgetClass;
	UPROPERTY()
	TObjectPtr<UFRProfileWidget> ProfileWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIClassRef")
	TSubclassOf<class UFRTitleExitWidget> FRTitleExitWidgetClass;
	UPROPERTY()
	TObjectPtr<UFRTitleExitWidget> TitleExitWidget;

	//Button 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Profile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_GameSettingMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_TitleMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Back;

private:
	//Button Callback
	UFUNCTION()
	void OnProfileBtnCallback();
	UFUNCTION()
	void OnGameSettingMenuBtnCallback();
	UFUNCTION()
	void OnTitleMenuBtnCallback();
	UFUNCTION()
	void OnBackBtnCallback();

};
