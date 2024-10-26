// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/MainGameplayOverlay.h"

void AMainHUD::UpdateScorePannel(const FText& Text)
{
	if (IsValid(GameplayOverlay))
	{
		GameplayOverlay->SetNewTextScore(Text);
	}
}

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	GameplayOverlay = CreateWidget<UMainGameplayOverlay>(GetOwningPlayerController(), GameplayOverlayClass);
	GameplayOverlay->AddToViewport();
}
