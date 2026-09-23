// Raul Hidalgo Nicolas - Copyright (C) 2026. All Rights Reserved.

#include "RVPawn.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "XRDeviceVisualizationComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "DrawDebugHelpers.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "MainGameMode.h"
#include "Components/SphereComponent.h"

// Sets default values
ARVPawn::ARVPawn()
{
 	// Set this pawn to call Tick() every frame.  
	PrimaryActorTick.bCanEverTick = true;

	// Force tick to be called after the controllers have been updated.
    PrimaryActorTick.TickGroup = TG_PostUpdateWork;

	// Tracking origin
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	SetRootComponent(VROrigin); 
	// Camera attachment to the VR origin, automatically handles HMD tracking and position
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VROrigin);

    // Left controller
    LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
    LeftController->SetupAttachment(VROrigin);
    LeftController->SetTrackingMotionSource(FName("Left"));

    LeftControllerVis = CreateDefaultSubobject<UXRDeviceVisualizationComponent>(TEXT("LeftControllerVis"));
    LeftControllerVis->SetupAttachment(LeftController);   
    LeftControllerVis->SetIsVisualizationActive(true);

    LeftFinger = CreateDefaultSubobject<USphereComponent>(TEXT("LeftFinger"));
    LeftFinger->SetupAttachment(LeftController);
    LeftFinger->SetSphereRadius(2.f);                       
    LeftFinger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    LeftFinger->SetCollisionObjectType(ECC_Pawn);
    LeftFinger->SetCollisionResponseToAllChannels(ECR_Overlap);
    LeftFinger->SetGenerateOverlapEvents(true);

	// Right controller
    RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
    RightController->SetupAttachment(VROrigin);
    RightController->SetTrackingMotionSource(FName("Right"));

    RightControllerVis = CreateDefaultSubobject<UXRDeviceVisualizationComponent>(TEXT("RightControllerVis"));
    RightControllerVis->SetupAttachment(RightController);
    RightControllerVis->SetIsVisualizationActive(true);

    RightFinger = CreateDefaultSubobject<USphereComponent>(TEXT("RightFinger"));
    RightFinger->SetupAttachment(RightController);
    RightFinger->SetSphereRadius(2.f);
    RightFinger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    RightFinger->SetCollisionObjectType(ECC_Pawn);
    RightFinger->SetCollisionResponseToAllChannels(ECR_Overlap);
    RightFinger->SetGenerateOverlapEvents(true);

	// Input mapping context and actions, setted via Blueprint
    DefaultMappingContext = nullptr;
	TeleportAction = nullptr;
	MenuAction = nullptr;
	SelectAction = nullptr;
    
	// Teleport marker. it will be updated every frame while aiming
    TeleportMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportMarker"));
    TeleportMarker->SetupAttachment(VROrigin);
    TeleportMarker->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	TeleportMarker->SetHiddenInGame(true); // Disabled by default

	// Spline component to draw the teleportation arc
    TeleportSpline = CreateDefaultSubobject<USplineComponent>(TEXT("TeleportSpline"));
    TeleportSpline->SetupAttachment(VROrigin);
    TeleportSpline->SetMobility(EComponentMobility::Movable);

	// Spline mesh asset and material, setted via Blueprint
	SplineMeshAsset = nullptr;
	SplineMaterial = nullptr;
	SplineMaterialValid = nullptr;
	SplineMaterialInvalid = nullptr;

	// Widget interaction component to interact with UMG widgets in VR
    WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
    WidgetInteraction->SetupAttachment(RightController);
    WidgetInteraction->InteractionDistance = 500.f; 
    WidgetInteraction->bShowDebug = true;
	
}

// Called when the game starts or when spawned
void ARVPawn::BeginPlay()
{
	Super::BeginPlay();

    UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);

    UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(0.f);
	
	// Activate the default mapping context
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            if (DefaultMappingContext)
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0); 
            }
            
        }
	}

    AddTickPrerequisiteComponent(RightController);
}

// Called every frame
void ARVPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsAiming)
    {
        UpdateTeleportArc();
    }

}

void ARVPawn::UpdateTeleportArc()
{
	// 1) Launch a projectile from the right controller and predict its path
    FPredictProjectilePathParams Params;
    Params.StartLocation = RightController->GetComponentLocation();
    Params.LaunchVelocity = RightController->GetForwardVector() * TeleportLaunchSpeed;
    Params.bTraceWithCollision = true;
    Params.ProjectileRadius = 5.f;
    Params.MaxSimTime = 2.f;
    Params.SimFrequency = 30.f;
    Params.ActorsToIgnore.Add(this);
    Params.TraceChannel = ECollisionChannel::ECC_WorldStatic;

    FPredictProjectilePathResult Result;
    bool bHit = UGameplayStatics::PredictProjectilePath(this, Params, Result);

	// 2) Validate the hit location and check if it is on the navigation mesh
    bHasValidLocation = false;
    if (bHit)
    {
        if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
        {
            FNavLocation NavLoc;
            if (NavSys->ProjectPointToNavigation(Result.HitResult.Location, NavLoc, FVector(100.f, 100.f, 100.f)))
            {
                bHasValidLocation = true;
                ValidTeleportLocation = NavLoc.Location;
            }
        }
    }

    // 3) Draw the arc
    TArray<FVector> ArcPoints;
    for (const FPredictProjectilePathPointData& P : Result.PathData)
    {
        ArcPoints.Add(P.Location);
    }
	SplineMaterial = bHasValidLocation ? SplineMaterialValid : SplineMaterialInvalid;
    DrawTeleportSpline(ArcPoints);

	// 4) Show/hide the teleport marker and update its position
    TeleportMarker->SetHiddenInGame(!bHasValidLocation);
    if (bHasValidLocation)
    {
        TeleportMarker->SetWorldLocation(ValidTeleportLocation);
    }
}

// Called to bind functionality to input
void ARVPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind input actions for teleportation and pause menu
    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (TeleportAction)
        {
            EIC->BindAction(TeleportAction, ETriggerEvent::Started, this, &ARVPawn::StartAiming);
            EIC->BindAction(TeleportAction, ETriggerEvent::Completed, this, &ARVPawn::ExecuteTeleport);
        }

        if (MenuAction)
        {
            EIC->BindAction(MenuAction, ETriggerEvent::Completed, this, &ARVPawn::OnMenu);
        }

        if (SelectAction)
        {
            EIC->BindAction(SelectAction, ETriggerEvent::Started, this, &ARVPawn::OnSelectPressed);
            EIC->BindAction(SelectAction, ETriggerEvent::Completed, this, &ARVPawn::OnSelectReleased);
        }
    }

}

void ARVPawn::OnSelectPressed()
{
    if (WidgetInteraction)
    {
        WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
    }
}

void ARVPawn::OnSelectReleased()
{
    if (WidgetInteraction)
    {
        WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
    }
}

void ARVPawn::StartAiming()
{
    if (AMainGameMode* GM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        if (GM->getGameState() != ERVGameState::Playing )
        {
			return; // Do not allow aiming if the game is not playing
        }
    }
    bIsAiming = true;
    RightController->bDisableLowLatencyUpdate = true;
}

void ARVPawn::ExecuteTeleport()
{
    if (AMainGameMode* GM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        if (GM->getGameState() != ERVGameState::Playing)
        {
            return; // Do not allow aiming if the game is not playing
        }
    }
    bIsAiming = false;
    ClearTeleportSpline();

    RightController->bDisableLowLatencyUpdate = false;

    TeleportMarker->SetHiddenInGame(true);

    if (!bHasValidLocation)
    {
		return; // Early exit if there is no valid teleport location
    }

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC && PC->PlayerCameraManager)
    {
		// Black fade out the camera before teleporting
        PC->PlayerCameraManager->StartCameraFade(0.f, 1.f, FadeDuration, FColor::Black, false, true);
    }

	// Perform real teleportation after the fade duration.
    GetWorldTimerManager().SetTimer(FadeTimerHandle, this, &ARVPawn::FinishTeleport, FadeDuration, false);
}

void ARVPawn::FinishTeleport()
{
    // To prevent head been on the ground.
    FVector CameraOffset = Camera->GetComponentLocation() - GetActorLocation();
    CameraOffset.Z = 0.f;

    FVector NewLocation = ValidTeleportLocation - CameraOffset;
    SetActorLocation(NewLocation);

    
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (PC->PlayerCameraManager)
        {
            PC->PlayerCameraManager->StartCameraFade(1.f, 0.f, FadeDuration, FColor::Black, false, false);
        }
    }
}

void ARVPawn::DrawTeleportSpline(const TArray<FVector>& PathPoints)
{
    if (PathPoints.Num() < 2 || !SplineMeshAsset)
    {
        ClearTeleportSpline();
        return;
    }

    TeleportSpline->SetSplinePoints(PathPoints, ESplineCoordinateSpace::World, true);

    const int32 NumSegments = PathPoints.Num() - 1;

    while (SplineMeshPool.Num() < NumSegments)
    {
        USplineMeshComponent* NewMesh = NewObject<USplineMeshComponent>(this);
        NewMesh->SetMobility(EComponentMobility::Movable);
        NewMesh->AttachToComponent(TeleportSpline, FAttachmentTransformRules::KeepRelativeTransform);
        NewMesh->SetStaticMesh(SplineMeshAsset);
        NewMesh->SetForwardAxis(ESplineMeshAxis::Z); 
        NewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        NewMesh->RegisterComponent(); 
        SplineMeshPool.Add(NewMesh);
    }

    
    for (int32 i = 0; i < SplineMeshPool.Num(); ++i)
    {
        USplineMeshComponent* MeshSeg = SplineMeshPool[i];

        if (i < NumSegments)
        {
            const FVector StartPos = TeleportSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
            const FVector StartTan = TeleportSpline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
            const FVector EndPos = TeleportSpline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
            const FVector EndTan = TeleportSpline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

            MeshSeg->SetStartAndEnd(StartPos, StartTan, EndPos, EndTan);
            MeshSeg->SetStartScale(FVector2D(SplineMeshWidth, SplineMeshWidth));
            MeshSeg->SetEndScale(FVector2D(SplineMeshWidth, SplineMeshWidth));

            MeshSeg->SetMaterial(0, SplineMaterial);
			

            MeshSeg->SetVisibility(true);
        }
        else
        {
            
            MeshSeg->SetVisibility(false);
        }
    }
}

void ARVPawn::ClearTeleportSpline()
{
    for (USplineMeshComponent* MeshSeg : SplineMeshPool)
    {
        if (MeshSeg)
        {
            MeshSeg->SetVisibility(false);
        }
    }
}

void ARVPawn::OnMenu()
{
    if (AMainGameMode* GM = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        GM->TogglePause();
    }
}

