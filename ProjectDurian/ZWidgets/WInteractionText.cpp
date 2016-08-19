// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectDurian.h"
#include "WInteractionText.h"

void UWInteractionText::ShowMessage()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UWInteractionText::HideMessage()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UWInteractionText::SetMessage(FString pMessage)
{
	mMessage = pMessage;
}

FString UWInteractionText::GetMessage()
{
	return mMessage;
}