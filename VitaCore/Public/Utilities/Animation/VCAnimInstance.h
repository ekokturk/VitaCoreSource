// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VCAnimInstance.generated.h"


class AVCCharacter;

UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class VITACORE_API UVCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UVCAnimInstance();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		bool IsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		bool IsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		bool ShouldDoFullBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		FVector Velocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		float AimPitch;

	UPROPERTY(BlueprintReadWrite)
		AVCCharacter * OwnerCharacter;


	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
};
