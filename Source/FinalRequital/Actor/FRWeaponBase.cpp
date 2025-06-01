// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FRWeaponBase.h"

AFRWeaponBase::AFRWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetOnlyOwnerSee(false);
	WeaponMesh->CastShadow = false;
	WeaponMesh->bCastDynamicShadow = false;
}

void AFRWeaponBase::SetVisible(bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
	SetActorEnableCollision(bVisible);
	SetActorTickEnabled(bVisible);
}