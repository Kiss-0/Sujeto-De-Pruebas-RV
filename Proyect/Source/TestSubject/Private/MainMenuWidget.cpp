// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the button click events to their respective callback functions
	if (Btn_Start)
	{
		Btn_Start->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartClicked);
	}
	if (Btn_Quit)
	{
		Btn_Quit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	}
}

void UMainMenuWidget::OnStartClicked()
{
	// Load game level
	UGameplayStatics::OpenLevel(this, GameLevelName);
}

void UMainMenuWidget::OnQuitClicked()
{
	// Quit the game
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

