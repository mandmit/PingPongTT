// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UUserWidget;
class UMainGameplayOverlay;
/**
 * 
 */
UCLASS(Abstract)
class PINGPONG_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void UpdateScorePannel(const FText& Text);

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> GameplayOverlayClass;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMainGameplayOverlay> GameplayOverlay;
	
};
