// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FRPlayerController.h"
#include "UI/FRHUDWidget.h"

AFRPlayerController::AFRPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}
void AFRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// HUD ������ ȭ�鿡 �߰� -> ���������� �������Ʈ���� ĳ��
	if (FRHUDWidgetClass) // RXHUDWidgetClass�� ��ȿ���� Ȯ��
	{
		AFRHUDWidget = CreateWidget<UFRHUDWidget>(GetWorld(), FRHUDWidgetClass);
		if (AFRHUDWidget)
		{
			// HUD�� ȭ�鿡 �߰�
			AFRHUDWidget->AddToViewport();
		}
	}
	//SetInputMode(FInputModeGameAndUI()); // UI�� ���� ��� �� �� Ȱ��ȭ
	//bShowMouseCursor = true;              // ���콺 Ŀ�� ǥ��

}
