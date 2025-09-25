// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FRDialogueWidget.h"
#include "Actor/Interactable/FRDialogueActor.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

UFRDialogueWidget::UFRDialogueWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFRDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PC->SetIgnoreMoveInput(true);
    }
}

void UFRDialogueWidget::NativeDestruct()
{
	Super::NativeDestruct();
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PC->SetIgnoreMoveInput(false);
        //PC->SetIgnoreLookInput(false);
    }
}

void UFRDialogueWidget::UpdateDialogue(const FDialogueLine& Line)
{
    if (Line.bIsPlayerMonologue)
    {
        TXT_Monologue->SetText(Line.Text);
        TXT_Monologue->SetVisibility(ESlateVisibility::Visible);
        IMG_BG_Monologue->SetVisibility(ESlateVisibility::Visible);

        IMG_BG_Dialogue->SetVisibility(ESlateVisibility::Collapsed);
        TXT_Dialogue->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        TXT_Dialogue->SetText(Line.Text);
        TXT_Dialogue->SetVisibility(ESlateVisibility::Visible);
        IMG_BG_Dialogue->SetVisibility(ESlateVisibility::Visible);

        IMG_BG_Monologue->SetVisibility(ESlateVisibility::Collapsed);
        TXT_Monologue->SetVisibility(ESlateVisibility::Collapsed);
    }
}
