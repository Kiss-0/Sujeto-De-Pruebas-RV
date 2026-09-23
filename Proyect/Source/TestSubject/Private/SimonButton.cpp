// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.


#include "SimonButton.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASimonButton::ASimonButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	SetRootComponent(ButtonMesh);

	TouchBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TouchBox"));
	TouchBox->SetupAttachment(ButtonMesh);
	TouchBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TouchBox->SetCollisionObjectType(ECC_WorldDynamic);
	TouchBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	TouchBox->SetGenerateOverlapEvents(true);

	// Base material is set via Blueprint
	BaseMaterial = nullptr;
	// Created at runtime
	DynMaterial = nullptr;

}

// Called when the game starts or when spawned
void ASimonButton::BeginPlay()
{
	Super::BeginPlay();

	if (BaseMaterial)
	{
		DynMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		ButtonMesh->SetMaterial(0, DynMaterial);
	}

	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue(FName("Color"), ButtonColor);
		DynMaterial->SetScalarParameterValue(FName("EmissiveStrength"), OffStrength);
	}

	FVector StaticMeshSize = ButtonMesh->GetStaticMesh()->GetBounds().BoxExtent;
	TouchBox->SetBoxExtent(StaticMeshSize);

	TouchBox->OnComponentBeginOverlap.AddDynamic(this, &ASimonButton::OnTouchBegin);
	TouchBox->OnComponentEndOverlap.AddDynamic(this, &ASimonButton::OnTouchEnd);
	
}

void ASimonButton::OnTouchBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (!IsAllowedToInteract || bIsTouched || (OtherActor == nullptr || OtherActor == this))
	{
		return;
	}

	bIsTouched = true;
	Highlight();

	OnButtonPressed.Broadcast(ButtonIndex);
}

void ASimonButton::OnTouchEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsTouched = false;
}

void ASimonButton::Highlight(bool SoundOn)
{
	if (DynMaterial)
	{
		// Make the button glow 
		DynMaterial->SetScalarParameterValue(FName("EmissiveStrength"), OnStrength);
	}

	if (PressSound && SoundOn)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PressSound, GetActorLocation());
	}

	// Program a timer to stop highlighting after HighlightDuration seconds
	GetWorldTimerManager().SetTimer(
		HighlightTimerHandle, this, &ASimonButton::StopHighlight, HighlightDuration, false);
}

void ASimonButton::StopHighlight()
{
	if (DynMaterial)
	{
		DynMaterial->SetScalarParameterValue(FName("EmissiveStrength"), OffStrength);
	}
}

// Called every frame
void ASimonButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

