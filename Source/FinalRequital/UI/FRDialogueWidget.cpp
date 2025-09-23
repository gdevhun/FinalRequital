// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FRDialogueWidget.h"

#include "Components/TextBlock.h"

UFRDialogueWidget::UFRDialogueWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFRDialogueWidget::UpdateDialogueText(const FText& NewText)
{
	if (TXT_Dialogue)
	{
		TXT_Dialogue->SetText(NewText);
	}
}
