// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Boss/FRBossPhase1Jangseung.h"
#include "GAS/Attribute/FRPhase1JangseungAttributeSet.h"
#include "Physics/FRCollision.h"
#include "UI/FRWidgetComponent.h"

AFRBossPhase1Jangseung::AFRBossPhase1Jangseung()
{
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	//Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JangseungMesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Mesh->SetCollisionResponseToChannel(CCHANNEL_FRACTION, ECR_Block);

	// ASC
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UFRPhase1JangseungAttributeSet>(TEXT("JangseungAttributeSet"));

	// HPBar UI
	HpBar = CreateDefaultSubobject<UFRWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(RootComponent);
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Boss/WBP_Phase1JangseungHpBar.WBP_Phase1JangseungHpBar_C'"));
	if (HpWidgetRef.Succeeded())
	{
		HpBar->SetWidgetClass(HpWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(160.0f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

class UAbilitySystemComponent* AFRBossPhase1Jangseung::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRBossPhase1Jangseung::BeginPlay()
{
	Super::BeginPlay();

	ASC->InitAbilityActorInfo(this, this);
	HpBar->SetVisibility(true);

	if (AttributeSet)
	{
		AttributeSet->OnOutOfHealth.AddDynamic(this, &AFRBossPhase1Jangseung::OnOutOfHealth);
	}

}

void AFRBossPhase1Jangseung::OnOutOfHealth_Implementation()
{
	Destroy();
}




