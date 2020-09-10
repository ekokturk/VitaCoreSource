// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VCGameTypes.h"
#include "VCPickup.generated.h"

class AVCCharacter;
class UGameplayEffect;
class UAbilitySystemComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class VITACORE_API AVCPickup : public AActor
{
	GENERATED_BODY()

public:
	AVCPickup();

protected:
	UFUNCTION() void DoCollect(AVCCharacter* OtherCharacter);
	
	// Pickup collect event
	UFUNCTION(BlueprintImplementableEvent, Category = "Config", meta = (DisplayName = "OnCollect"))
		void BP_OnCollect();
	
	UFUNCTION() void ApplyEffect(AVCCharacter* OtherCharacter)  const;

	UFUNCTION() void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	
protected:

	// Type of the pick up which might change who can collect it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		ETeamType TeamType;

	// Gameplay effect that will be applied to the VCCharacter that picks it up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		TSubclassOf<UGameplayEffect> Effect;

	// Level of the gameplay effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float EffectLevel;

	// Determines which team type can collect this pickup (Own team or all the other teams)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool bIsOwnTeamCollecting;

	// Determines if the effect applies to the character or player team that character is subscribed to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool bIsTeamEffect;
	
	UPROPERTY(VisibleAnywhere) USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere) UStaticMeshComponent* MeshComponent;

protected:
	virtual void BeginPlay() override;

};
