// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

// Forward declaration
class UButton;

UCLASS()
class TESTSUBJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	// Binded buttons from the UMG widget
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Start;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Quit;

	// Callback functions for button clicks
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnQuitClicked();

	// Name of the level to transition when start is clicked.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu")
	FName GameLevelName = "MainGameLevel";

};
