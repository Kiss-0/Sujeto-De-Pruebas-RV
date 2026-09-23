// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimonButton.generated.h"
//Forward declaration
class UStaticMeshComponent;
class UMaterialInstanceDynamic;
class UBoxComponent;
class UPrimitiveComponent;   
class USoundBase;

UCLASS()
class TESTSUBJECT_API ASimonButton : public AActor
{
	GENERATED_BODY()
	
public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonPressed, int32, ButtonIndex);
	// Sets default values for this actor's properties
	ASimonButton();

	UFUNCTION(BlueprintCallable, Category = "Simon")
	void Highlight(bool SoundOn = true);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simon")
	int32 ButtonIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simon")
	FLinearColor ButtonColor = FLinearColor::Red;

	UPROPERTY(BlueprintAssignable, Category = "Simon")
	FOnButtonPressed OnButtonPressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simon")
	USoundBase* PressSound;

	UPROPERTY()
	bool IsAllowedToInteract = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StopHighlight();

	// Collider for touch detection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simon")
	UBoxComponent* TouchBox;

	// Callbacks
	UFUNCTION()
	void OnTouchBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTouchEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Is already touched by a finger
	bool bIsTouched = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simon")
	UStaticMeshComponent* ButtonMesh;

	// Material 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simon")
	UMaterialInterface* BaseMaterial;

	// Dynamic material instance for runtime changes
	UPROPERTY()
	UMaterialInstanceDynamic* DynMaterial;

	//	Intensity values for the material
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simon")
	float OffStrength = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simon")
	float OnStrength = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simon")
	float HighlightDuration = 0.4f;

	FTimerHandle HighlightTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
