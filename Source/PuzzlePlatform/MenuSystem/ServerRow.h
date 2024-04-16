// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

class UTextBlock;
class UMainMenu;
class UButton;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UMainMenu* ParentMenu;
	uint32 Index;

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HostName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerInfo;

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	void Setup(UMainMenu* InParentMenu, uint32 InIndex);

	UFUNCTION()
	void OnClicked();

	UPROPERTY(BlueprintReadOnly)
	bool IsSelected = false;
	
};
