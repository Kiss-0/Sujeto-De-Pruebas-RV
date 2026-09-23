// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RVPawn.generated.h"

// Foward declarations
class UCameraComponent;
class UMotionControllerComponent;
class UXRDeviceVisualizationComponent;
class UInputMappingContext;
class UInputAction;
class UStaticMeshComponent;
class USplineComponent;
class USplineMeshComponent;
class UWidgetInteractionComponent;
class USphereComponent;

UCLASS()
class TESTSUBJECT_API ARVPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARVPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	USceneComponent* VROrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	UMotionControllerComponent* LeftController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	UXRDeviceVisualizationComponent* LeftControllerVis;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	UMotionControllerComponent* RightController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	UXRDeviceVisualizationComponent* RightControllerVis;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simon")
	USphereComponent* LeftFinger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simon")
	USphereComponent* RightFinger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TeleportAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SelectAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Teleport")
	UStaticMeshComponent* TeleportMarker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	float TeleportLaunchSpeed = 900.f;   

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	float FadeDuration = 0.1f;     

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Teleport")
	USplineComponent* TeleportSpline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	UStaticMesh* SplineMeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	UMaterialInterface* SplineMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	UMaterialInterface* SplineMaterialValid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	UMaterialInterface* SplineMaterialInvalid;

	UPROPERTY()
	TArray<USplineMeshComponent*> SplineMeshPool;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	float SplineMeshWidth = 0.02f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetInteractionComponent* WidgetInteraction;

	// Nedeed variables to manage teleportation state
	bool bIsAiming = false;
	bool bHasValidLocation = false;
	FVector ValidTeleportLocation = FVector::ZeroVector;
	FTimerHandle FadeTimerHandle;

	// Internal function to perform the teleportation logic
	void StartAiming();        // (Started)
	void ExecuteTeleport();    // (Completed)
	void UpdateTeleportArc();  // Called every frame while aiming
	void FinishTeleport();     // Performs the actual teleportation.
	void DrawTeleportSpline(const TArray<FVector>& PathPoints);
	void ClearTeleportSpline();

	// Function to handle the menu action
	void OnSelectPressed();
	void OnSelectReleased();


	void OnMenu();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
