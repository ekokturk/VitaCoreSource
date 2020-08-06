// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCAnimInstance.h"
#include "VCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UVCAnimInstance::UVCAnimInstance()
{
	
}

void UVCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// Initialize pawn as game character
	OwnerCharacter = Cast<AVCCharacter>(TryGetPawnOwner());
}

void UVCAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	// Check for pawn or reinitialize
	if (OwnerCharacter == nullptr)
	{
		NativeInitializeAnimation();
		return;
	}

	// Get Character Info
	FTransform CharacterTransform = OwnerCharacter->GetActorTransform();
	FRotator CharacterRotation = OwnerCharacter->GetActorRotation();
	FVector CharacterVelocity = OwnerCharacter->GetVelocity();
	float CharacterDirection = CalculateDirection(CharacterVelocity, CharacterRotation);

	bool IsCharacterFalling = OwnerCharacter->GetMovementComponent()->IsFalling();
	FRotator CharacterLookDirection = OwnerCharacter->GetLookDirection();

	
	// Set Instance Variables
	//Velocity = UKismetMathLibrary::InverseTransformDirection(CharacterTransform, CharacterVelocity);
	Velocity = FVector(CharacterVelocity.Size() * FMath::Cos(FMath::DegreesToRadians(CharacterDirection)), 
						CharacterVelocity.Size() * FMath::Sin(FMath::DegreesToRadians(CharacterDirection)), 
						CharacterVelocity.Z);
	//Velocity = OwnerCharacter->GetVelocity();
	//if(Velocity.X < 0)
	//{
	//	Velocity.Y *= -1;
	//}
	//
	//Direction = CharacterDirection;
	Speed = CharacterVelocity.Size();
	IsFalling = IsCharacterFalling;
	AimPitch = CharacterLookDirection.Pitch;
	if(Speed != 0)
	{
		IsMoving = true;
	}
	else
	{
		IsMoving = false;
	}
}
