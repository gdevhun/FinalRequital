// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FRStackBaseWidget.h"
#include "FRGameSettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRGameSettingWidget : public UFRStackBaseWidget
{
	GENERATED_BODY()
public:
	UFRGameSettingWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class UTexture2D>> ProgressBarIMG;

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

	//나가기 버튼
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Back;

private:
	//버튼 콜백 함수
	UFUNCTION(BlueprintCallable)
	void OnMasterVolumeDown();
	UFUNCTION(BlueprintCallable)
	void OnMasterVolumeUp();

	UFUNCTION(BlueprintCallable)
	void OnBGMVolumeDown();
	UFUNCTION(BlueprintCallable)
	void OnBGMVolumeUp();

	UFUNCTION(BlueprintCallable)
	void OnSFXVolumeDown();
	UFUNCTION(BlueprintCallable)
	void OnSFXVolumeUp();

	UFUNCTION(BlueprintCallable)
	void MouseSensitivityDown();
	UFUNCTION(BlueprintCallable)
	void MouseSensitivityUp();

	UFUNCTION(BlueprintCallable)
	void BackBTNCallback();

	//게이지이미지 업데이트 초기화 함수
	void UpdateGauge(UImage* GaugeImage, float Value);
	void UpdateAllGauges();

	//수치 보장
	static float StepValue(float Current, float Delta);
};
