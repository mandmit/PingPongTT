// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameplayOverlay.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS(Abstract)
class PINGPONG_API UMainGameplayOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SetNewTextScore(const FText& NewScore);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Score;
	
};
