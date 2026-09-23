// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

// Forward declaration
class UButton;


UCLASS()
class TESTSUBJECT_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	// Binded buttons from the UMG widget
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Resume;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_QuitToMenu;

	// Callback functions for button clicks
	UFUNCTION()
	void OnResumeClicked();

	UFUNCTION()
	void OnQuitToMenuClicked();

	// Name of the level to transition when start is clicked.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu")
	FName GameLevelName = "MenuGameLevel";
	
};
