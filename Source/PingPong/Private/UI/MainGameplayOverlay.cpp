// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainGameplayOverlay.h"
#include "Components/TextBlock.h"

void UMainGameplayOverlay::SetNewTextScore(const FText& NewScore)
{
	Score->SetText(NewScore);
}
