// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	IMenuInterface* MenuInterface;

public:
	void SetMenuInterface(IMenuInterface* Value);

	void Setup();

	virtual void NativeDestruct() override;
};
