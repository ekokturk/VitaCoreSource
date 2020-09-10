// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "VCCharacterTypes.generated.h"

class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class ECameraViewTypes : uint8
{
	Third_Person = 0,
	First_Person,
	Front,
};

USTRUCT(BlueprintType)
struct FCharacterViewProps
{
	GENERATED_BODY()

	FCharacterViewProps();

	UPROPERTY(EditAnywhere) float Distance;
	UPROPERTY(EditAnywhere) FVector Offset;
	UPROPERTY(EditAnywhere) FRotator Orientation;

	void SetCameraView(UCameraComponent* Camera, USpringArmComponent* CameraSpring) const;
};