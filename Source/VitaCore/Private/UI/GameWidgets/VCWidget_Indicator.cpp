// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCWidget_Indicator.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMG/Public/Components/Overlay.h"
#include "Engine.h"
#include "VCPlayerController.h"
#include "Engine/UserInterfaceSettings.h"

bool UVCWidget_Indicator::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	TargetOverlay = Cast<UOverlay>(GetWidgetFromName("TargetOverlay"));
	TargetDirectionImage = Cast<UImage>(GetWidgetFromName("TargetDirectionImage"));


	return true;
}


void UVCWidget_Indicator::NativeConstruct()
{
	Super::NativeConstruct();

	TargetOverlaySlot = Cast< UCanvasPanelSlot >(TargetOverlay->Slot);
	InitialTargetDirectionAngle = TargetDirectionImage->RenderTransform.Angle;
}

void UVCWidget_Indicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Override Target Location if there is a target actor
	if(TargetActor)
	{
		TargetLocation = TargetActor->GetActorLocation() + TargetLocation;
	}
		
	// Hide the Indicator if the pawn is close enough (If distance zero do not hide)
	if(VCPlayerController->GetPawn() != nullptr && TargetHideDistance != 0)
	{
		const FVector PawnLocation = VCPlayerController->GetPawn()->GetActorLocation();
		const float PawnDistance = FVector::Dist(FVector(PawnLocation.X,PawnLocation.Y, 0.0f), FVector(TargetLocation.X, TargetLocation.Y, 0.0f));

		PawnDistance <= TargetHideDistance ? TargetOverlay->SetVisibility(ESlateVisibility::Hidden) :
											TargetOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	// Position the indicator depending on the projection of screen
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	FVector2D TargetScreenLocation;
	FVector2D ClampedTargetScreenLocation;
	VCPlayerController->ProjectWorldLocationToScreen(TargetLocation, TargetScreenLocation, false);

	const float DPI = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
	
	ClampedTargetScreenLocation.X = FMath::Clamp(TargetScreenLocation.X, ViewportSize.X * HorizontalLimit.X, ViewportSize.X * HorizontalLimit.Y);
	ClampedTargetScreenLocation.Y = FMath::Clamp(TargetScreenLocation.Y, ViewportSize.Y * VerticalLimit.X, ViewportSize.Y * VerticalLimit.Y);
	
	TargetOverlaySlot->SetPosition(FVector2D(ClampedTargetScreenLocation * (1 / DPI)));


	const bool IsClampedX = ClampedTargetScreenLocation.X != TargetScreenLocation.X;
	const bool IsClampedY = ClampedTargetScreenLocation.Y != TargetScreenLocation.Y;

	if(IsClampedX == true || IsClampedY == true)
	{
		TargetDirectionImage->SetVisibility(ESlateVisibility::Visible);

		const FVector2D PositionFromCenter(TargetScreenLocation.X - ViewportSize.X / 2, TargetScreenLocation.Y - ViewportSize.Y / 2);
		float TargetAngle = -FMath::RadiansToDegrees(FMath::Atan2(PositionFromCenter.Y, PositionFromCenter.X)) + InitialTargetDirectionAngle;
		//UE_LOG(LogTemp, Log, TEXT("%f"), TargetAngle);
		if(IsClampedX == true)
		{
			TargetAngle += 180.0f;
		}
		//TargetDirectionImage->SetRenderAngle(TargetAngle); // TODO Check for UE4.24.2
		TargetDirectionImage->SetRenderTransformAngle(TargetAngle);
	}
	else
	{
		TargetDirectionImage->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UVCWidget_Indicator::SetTargetActor(AActor* Target)
{
	TargetActor = Target;
}

