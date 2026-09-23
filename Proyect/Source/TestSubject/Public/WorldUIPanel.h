// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldUIPanel.generated.h"

class UWidgetComponent;

UCLASS()
class TESTSUBJECT_API AWorldUIPanel : public AActor
{
    GENERATED_BODY()

public:
    AWorldUIPanel();

    // Show panel in front of the player
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowPanel();

    // Hide panel
    UFUNCTION(BlueprintCallable, Category = "UI")
    void HidePanel();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* WidgetComp;

	// Padding from the camera to avoid clipping with the player's view
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    float DistanceFromCamera = 200.f;

	// Relative vertical offset from the camera's position to place the panel
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    float VerticalOffset = 0.f;
};