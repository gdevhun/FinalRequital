// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define D(x) if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Cyan, x); }
/*
*	<Debug�� ��ũ��Ʈ>
*	����� CPP���Ͽ� FRDebugHelper.h �߰��� ��
*	������ϰ� ���� ���� ���� ������ ���� ��� ���� :
*
*	�ش� �������� CurrentHealth, MaxHealth ������ ���� ������ ���,
*	D(FString::Printf(TEXT("Current Health: %f, Max Health: %f"), CurrentHealth, MaxHealth));
*	D(FString::Printf(TEXT("TRIGGER!")));
*/