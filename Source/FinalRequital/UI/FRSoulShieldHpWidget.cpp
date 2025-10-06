// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FRSoulShieldHpWidget.h"
#include "FRDebugHelper.h"
#include "Components/ProgressBar.h"
#include "GAS/Attribute/FRSoulShieldAttributeSet.h"

void UFRSoulShieldHpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ASC)
	{
		if (const UFRSoulShieldAttributeSet* SoulShieldAttribute = ASC->GetSet<UFRSoulShieldAttributeSet>())
		{
			// 최대값 / 초기값 세팅
			PB_Shield->SetPercent(SoulShieldAttribute->GetShield() / SoulShieldAttribute->GetMaxShield());
			PB_Hp->SetPercent(SoulShieldAttribute->GetHealth() / SoulShieldAttribute->GetMaxHealth());

			// Shield / Health 감소 델리게이트 연결
			SoulShieldAttribute->OnDamagedOfShield.AddDynamic(this, &UFRSoulShieldHpWidget::UpdateShieldBar);
			SoulShieldAttribute->OnDamagedOfHealth.AddDynamic(this, &UFRSoulShieldHpWidget::UpdateHpBar);

			// 0이 되었을 때 처리
			SoulShieldAttribute->OnOutOfShield.AddDynamic(this, &UFRSoulShieldHpWidget::HandleOutOfShield);
			SoulShieldAttribute->OnOutOfHealth.AddDynamic(this, &UFRSoulShieldHpWidget::HandleOutOfHealth);
		}
	}
}

void UFRSoulShieldHpWidget::UpdateShieldBar()
{
	D("t");
	if (!ASC) return;
	D("tT");
	if (const UFRSoulShieldAttributeSet* SoulShieldAttribute = ASC->GetSet<UFRSoulShieldAttributeSet>())
	{
		PB_Shield->SetPercent(SoulShieldAttribute->GetShield() / SoulShieldAttribute->GetMaxShield());
	}
}

void UFRSoulShieldHpWidget::UpdateHpBar()
{
	if (!ASC) return;

	if (const UFRSoulShieldAttributeSet* SoulShieldAttribute = ASC->GetSet<UFRSoulShieldAttributeSet>())
	{
		PB_Hp->SetPercent(SoulShieldAttribute->GetHealth() / SoulShieldAttribute->GetMaxHealth());
	}
}

void UFRSoulShieldHpWidget::HandleOutOfShield()
{
	if (PB_Shield)
	{
		PB_Shield->SetVisibility(ESlateVisibility::Hidden); // 쉴드바 안보이게
	}
}

void UFRSoulShieldHpWidget::HandleOutOfHealth_Implementation()
{
	if (PB_Hp)
	{
		PB_Hp->SetPercent(0.f);
	}
}

