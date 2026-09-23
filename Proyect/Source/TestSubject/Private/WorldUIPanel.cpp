// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.

#include "WorldUIPanel.h"
#include "Components/WidgetComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

AWorldUIPanel::AWorldUIPanel()
{
    PrimaryActorTick.bCanEverTick = false;

    WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
    SetRootComponent(WidgetComp);

    WidgetComp->SetWidgetSpace(EWidgetSpace::World);
    WidgetComp->SetDrawSize(FVector2D(500.f, 300.f));
    WidgetComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
}

void AWorldUIPanel::BeginPlay()
{
    Super::BeginPlay();
	HidePanel(); // Start hidden
}

void AWorldUIPanel::ShowPanel()
{
	// Reallocate the panel in front of the player camera
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC && PC->PlayerCameraManager)
    {
        const FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
        const FRotator CamRot = PC->PlayerCameraManager->GetCameraRotation();

        FRotator FlatRot(0.f, CamRot.Yaw, 0.f);
        FVector TargetLoc = CamLoc + FlatRot.Vector() * DistanceFromCamera;
        TargetLoc.Z += VerticalOffset;

        SetActorLocation(TargetLoc);

		// Orientate the panel to face the player camera
        SetActorRotation(FRotator(0.f, CamRot.Yaw + 180.f, 0.f));

        WidgetComp->PrimaryComponentTick.bTickEvenWhenPaused = true;
    }

    SetActorHiddenInGame(false);
    WidgetComp->SetVisibility(true);
	WidgetComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // re-enable collision for the widget component
}

void AWorldUIPanel::HidePanel()
{
    SetActorHiddenInGame(true);
    WidgetComp->SetVisibility(false);
    WidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
}