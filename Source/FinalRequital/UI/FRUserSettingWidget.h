// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FRUserSettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRUserSettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFRUserSettingWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class UTexture>> ProgressBarIMG;

	//게이지 이미지 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_MouseSensitivityGauge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_MasterVolumeGauge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_BGMVolumeGauge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_SFXVolumeGauge;

	//민감도 좌우 버튼
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_MouseSensitivity_Left;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_MouseSensitivity_Right;

	//마스터 볼륨 좌우 버튼
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_MasterVolume_Left;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_MasterVolume_Right;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))

	//BGM 볼륨 좌우 버튼
	TObjectPtr<class UButton> BTN_BGMVolume_Left;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_BGMVolume_Right;

	//SFX 볼륨 좌우 버튼
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_SFXVolume_Left;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_SFXVolume_Right;

private:
	//버튼 콜백 함수
	void OnMasterVolumeDown();
	void OnMasterVolumeUp();

	void OnBGMVolumeDown();
	void OnBGMVolumeUp();

	void OnSFXVolumeDown();
	void OnSFXVolumeUp();

	void MouseSensitivityDown();
	void MouseSensitivityUp();

	//게이지이미지 업데이트 초기화 함수
	void UpdateGauge(UImage* GaugeImage, float Value);
	void UpdateAllGauges();

	//수치 보장
	static float StepValue(float Current, float Delta);
};
