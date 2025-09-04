// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/FRStackBaseWidget.h"
#include "Player/FRPlayerController.h"

UFRStackBaseWidget::UFRStackBaseWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFRStackBaseWidget::RemoveUIWithPlayerController()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AFRPlayerController* MyPC = Cast<AFRPlayerController>(PC))
        {
            MyPC->PopUIWidgetFromStack();
        }
    }
}

void UFRStackBaseWidget::AddUIWithPlayerController(UFRStackBaseWidget* NewWidget)
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AFRPlayerController* MyPC = Cast<AFRPlayerController>(PC))
        {
            MyPC->PushUIWidgetToStack(NewWidget);
        }
    }
}
