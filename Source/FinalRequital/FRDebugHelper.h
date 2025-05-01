// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define D(x) if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Cyan, x); }
#define FR_LOG(CategoryName, Verbosity, Format, ...) \
	UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__)

DECLARE_LOG_CATEGORY_EXTERN(FRLOG, Log, All);
/*
*	<Debug�� ��ũ��Ʈ>
*	����� CPP���Ͽ� FRDebugHelper.h �߰��� ��
*	������ϰ� ���� ���� ���� ������ ���� ��� ���� :
*
*	�ش� �������� CurrentHealth, MaxHealth ������ ���� ������ ���,
*	D(FString::Printf(TEXT("Current Health: %f, Max Health: %f"), CurrentHealth, MaxHealth));
*	D(FString::Printf(TEXT("TRIGGER!")));
*   
*   FR_LOG(FRLOG, Error, TEXT("ASC Not Found!"));
*/