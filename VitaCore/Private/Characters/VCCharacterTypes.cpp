// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCCharacterTypes.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


FCharacterViewProps::FCharacterViewProps()
	:Distance(0.f),
	Offset(FVector::ZeroVector),
	Orientation(FRotator::ZeroRotator)
{
}

void FCharacterViewProps::SetCameraView(UCameraComponent* Camera, USpringArmComponent* CameraSpring) const
{
	if(Camera && CameraSpring)
	{
		Camera->SetWorldRotation(Orientation);
		CameraSpring->SocketOffset = Offset;
		CameraSpring->TargetArmLength = Distance;

		// TODO Angle to position maybe?
		//CameraComponent->SetWorldRotation(FRotator(0, ViewProps.Angle, 0));
		//CameraSpringComponent->TargetArmLength = -ViewProps.Distance;
		//CameraSpringComponent->SocketOffset = FVector(ViewProps.Distance * FMath::Sin(FMath::DegreesToRadians(ViewProps.Angle)), ViewProps.Distance * FMath::Cos(FMath::DegreesToRadians(ViewProps.Angle)), 0);
	}
}
