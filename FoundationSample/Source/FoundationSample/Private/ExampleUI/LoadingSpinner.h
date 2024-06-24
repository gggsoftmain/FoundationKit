// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingSpinner.generated.h"

/**
 * 
 */
UCLASS()
class FOUNDATIONSAMPLE_API ULoadingSpinner : public UUserWidget
{
	GENERATED_BODY()
public:
	static void StartLoading(UWidget* owner);
	static void EndLoading();
	static UUserWidget* LoadingSpinner;
};
