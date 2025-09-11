// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/FRGameSettingWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Player/FRPlayerController.h"
#include "System/FRGameInstance.h"

UFRGameSettingWidget::UFRGameSettingWidget(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{

}

void UFRGameSettingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼 바인딩
    if (BTN_MasterVolume_Left) BTN_MasterVolume_Left->OnClicked.AddDynamic(this, &UFRGameSettingWidget::OnMasterVolumeDown);
    if (BTN_MasterVolume_Right) BTN_MasterVolume_Right->OnClicked.AddDynamic(this, &UFRGameSettingWidget::OnMasterVolumeUp);

    if (BTN_BGMVolume_Left) BTN_BGMVolume_Left->OnClicked.AddDynamic(this, &UFRGameSettingWidget::OnBGMVolumeDown);
    if (BTN_BGMVolume_Right) BTN_BGMVolume_Right->OnClicked.AddDynamic(this, &UFRGameSettingWidget::OnBGMVolumeUp);

    if (BTN_SFXVolume_Left) BTN_SFXVolume_Left->OnClicked.AddDynamic(this, &UFRGameSettingWidget::OnSFXVolumeDown);
    if (BTN_SFXVolume_Right) BTN_SFXVolume_Right->OnClicked.AddDynamic(this, &UFRGameSettingWidget::OnSFXVolumeUp);

    if (BTN_MouseSensitivity_Left) BTN_MouseSensitivity_Left->OnClicked.AddDynamic(this, &UFRGameSettingWidget::MouseSensitivityDown);
    if (BTN_MouseSensitivity_Right) BTN_MouseSensitivity_Right->OnClicked.AddDynamic(this, &UFRGameSettingWidget::MouseSensitivityUp);

    if (BTN_Back) BTN_Back->OnClicked.AddDynamic(this, &UFRGameSettingWidget::BackBTNCallback);
    // 처음 위젯 켜졌을 때 현재 값 반영
    UpdateAllGauges();

}

void UFRGameSettingWidget::OnMasterVolumeDown()
{
    if (UFRGameInstance* GI = GetWorld()->GetGameInstance<UFRGameInstance>())
    {
        GI->CurrentSettingValue.MasterVolumeValue = StepValue(GI->CurrentSettingValue.MasterVolumeValue, -0.1f);
        GI->ApplyMasterVolume(GI->CurrentSettingValue.MasterVolumeValue);
        UpdateGauge(IMG_MasterVolumeGauge, GI->CurrentSettingValue.MasterVolumeValue);
    }
}

void UFRGameSettingWidget::OnMasterVolumeUp()
{
    if (UFRGameInstance* GI = GetWorld()->GetGameInstance<UFRGameInstance>())
    {
        GI->CurrentSettingValue.MasterVolumeValue = StepValue(GI->CurrentSettingValue.MasterVolumeValue, +0.1f);
        GI->ApplyMasterVolume(GI->CurrentSettingValue.MasterVolumeValue);
        UpdateGauge(IMG_MasterVolumeGauge, GI->CurrentSettingValue.MasterVolumeValue);
    }
}

void UFRGameSettingWidget::OnBGMVolumeDown()
{
    if (UFRGameInstance* GI = GetWorld()->GetGameInstance<UFRGameInstance>())
    {
        GI->CurrentSettingValue.MusicVolumeValue = StepValue(GI->CurrentSettingValue.MusicVolumeValue, -0.1f);
        GI->ApplyMusicVolume(GI->CurrentSettingValue.MusicVolumeValue);
        UpdateGauge(IMG_BGMVolumeGauge, GI->CurrentSettingValue.MusicVolumeValue);
    }
}

void UFRGameSettingWidget::OnBGMVolumeUp()
{
    if (UFRGameInstance* GI = GetWorld()->GetGameInstance<UFRGameInstance>())
    {
        GI->CurrentSettingValue.MusicVolumeValue = StepValue(GI->CurrentSettingValue.MusicVolumeValue, +0.1f);
        GI->ApplyMusicVolume(GI->CurrentSettingValue.MusicVolumeValue);
        UpdateGauge(IMG_BGMVolumeGauge, GI->CurrentSettingValue.MusicVolumeValue);
    }
}

void UFRGameSettingWidget::OnSFXVolumeDown()
{
    if (UFRGameInstance* GI = GetWorld()->GetGameInstance<UFRGameInstance>())
    {
        GI->CurrentSettingValue.SFXVolumeValue = StepValue(GI->CurrentSettingValue.SFXVolumeValue, -0.1f);
        GI->ApplySFXVolume(GI->CurrentSettingValue.SFXVolumeValue);
        UpdateGauge(IMG_SFXVolumeGauge, GI->CurrentSettingValue.SFXVolumeValue);
    }
}

void UFRGameSettingWidget::OnSFXVolumeUp()
{
    if (UFRGameInstance* GI = GetWorld()->GetGameInstance<UFRGameInstance>())
    {
        GI->CurrentSettingValue.SFXVolumeValue = StepValue(GI->CurrentSettingValue.SFXVolumeValue, +0.1f);
        GI->ApplySFXVolume(GI->CurrentSettingValue.SFXVolumeValue);
        UpdateGauge(IMG_SFXVolumeGauge, GI->CurrentSettingValue.SFXVolumeValue);
    }
}

void UFRGameSettingWidget::MouseSensitivityDown()
{
    if (UFRGameInstance* GI = GetWorld()->GetGameInstance<UFRGameInstance>())
    {
        GI->CurrentSettingValue.MouseSensitivityValue = StepValue(GI->CurrentSettingValue.MouseSensitivityValue, -0.1f);
        GI->ApplyMouseSensitivityValue(GI->CurrentSettingValue.MouseSensitivityValue);
        UpdateGauge(IMG_MouseSensitivityGauge, GI->CurrentSettingValue.MouseSensitivityValue);
    }
}

void UFRGameSettingWidget::MouseSensitivityUp()
{
    if (UFRGameInstance* GI = GetWorld()->GetGameInstance<UFRGameInstance>())
    {
        GI->CurrentSettingValue.MouseSensitivityValue = StepValue(GI->CurrentSettingValue.MouseSensitivityValue, +0.1f);
        GI->ApplyMouseSensitivityValue(GI->CurrentSettingValue.MouseSensitivityValue);
        UpdateGauge(IMG_MouseSensitivityGauge, GI->CurrentSettingValue.MouseSensitivityValue);
    }
}

void UFRGameSettingWidget::BackBTNCallback()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AFRPlayerController* MyPC = Cast<AFRPlayerController>(PC))
        {
            MyPC->PopUIWidgetFromStack();
        }
    }
}

float UFRGameSettingWidget::StepValue(float Current, float Delta)
{
    // 0.0 ~ 1.0 사이에서 0.1 단위 보장
    float NewValue = Current + Delta;
    NewValue = FMath::Clamp(NewValue, 0.0f, 1.0f);
    return FMath::GridSnap(NewValue, 0.1f);
}

void UFRGameSettingWidget::UpdateGauge(UImage* GaugeImage, float Value)
{
    if (!GaugeImage || ProgressBarIMG.Num() < 10) return;

    // 0.0 ~ 1.0 → 0~9 인덱스로 매핑
    int32 Index = FMath::Clamp(FMath::RoundToInt(Value * 10), 0, 10);

    if (ProgressBarIMG.IsValidIndex(Index))
    {
        GaugeImage->SetBrushFromTexture(ProgressBarIMG[Index]);
    }
}

void UFRGameSettingWidget::UpdateAllGauges()
{
    if (UFRGameInstance* GI = GetGameInstance<UFRGameInstance>())
    {
        UpdateGauge(IMG_MasterVolumeGauge, GI->CurrentSettingValue.MasterVolumeValue);
        UpdateGauge(IMG_BGMVolumeGauge, GI->CurrentSettingValue.MusicVolumeValue);
        UpdateGauge(IMG_SFXVolumeGauge, GI->CurrentSettingValue.SFXVolumeValue);
        UpdateGauge(IMG_MouseSensitivityGauge, GI->CurrentSettingValue.MouseSensitivityValue);
    }
}
