// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Boss/FRBossPhase3Jangseung.h"
#include "GAS/Attribute/FRPhase3JangseungAttributeSet.h"
#include "Physics/FRCollision.h"
#include "UI/FRWidgetComponent.h"

AFRBossPhase3Jangseung::AFRBossPhase3Jangseung()
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
	AttributeSet = CreateDefaultSubobject<UFRPhase3JangseungAttributeSet>(TEXT("JangseungAttributeSet"));

	// HPBar UI
	HpBar = CreateDefaultSubobject<UFRWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(RootComponent);
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Boss/WBP_Phase3JangseungHpBar.WBP_Phase3JangseungHpBar_C'"));
	if (HpWidgetRef.Succeeded())
	{
		HpBar->SetWidgetClass(HpWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(160.0f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

class UAbilitySystemComponent* AFRBossPhase3Jangseung::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRBossPhase3Jangseung::BeginPlay()
{
	Super::BeginPlay();

	ASC->InitAbilityActorInfo(this, this);
	HpBar->SetVisibility(true);

	if (AttributeSet)
	{
		AttributeSet->OnOutOfHealth.AddDynamic(this, &AFRBossPhase3Jangseung::OnOutOfHealth);
	}

}

void AFRBossPhase3Jangseung::OnOutOfHealth_Implementation()
{
	Destroy();
}

