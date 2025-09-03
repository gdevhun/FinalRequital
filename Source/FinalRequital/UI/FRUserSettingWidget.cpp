// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FRUserSettingWidget.h"

#include "System/FRGameInstance.h"

UFRUserSettingWidget::UFRUserSettingWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UFRUserSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UFRUserSettingWidget::OnMasterVolumeDown()
{
    if (UFRGameInstance* GI = GetWorld()->GetGameInstance<UFRGameInstance>())
    {
        GI->CurrentSettingValue.MasterVolumeValue = StepValue(GI->CurrentSettingValue.MasterVolumeValue, -0.1f);
        GI->ApplyMasterVolume(GI->CurrentSettingValue.MasterVolumeValue);
        UpdateGauge(IMG_MasterVolumeGauge, GI->CurrentSettingValue.MasterVolumeValue);
    }
}

void UFRUserSettingWidget::OnMasterVolumeUp()
{
    if (UFRGameInstance* GI = GetWorld()->GetGameInstance<UFRGameInstance>())
    {
        GI->CurrentSettingValue.MasterVolumeValue = StepValue(GI->CurrentSettingValue.MasterVolumeValue, +0.1f);
        GI->ApplyMasterVolume(GI->CurrentSettingValue.MasterVolumeValue);
        UpdateGauge(IMG_MasterVolumeGauge, GI->CurrentSettingValue.MasterVolumeValue);
    }
}

static float StepValue(float Current, float Delta)
{
    return FMath::Clamp(Current + Delta, 0.0f, 1.0f);
}