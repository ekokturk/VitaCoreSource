// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VCKillZVolume.generated.h"

class UBoxComponent;

UCLASS()
class VITACORE_API AVCKillZVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVCKillZVolume();

	UPROPERTY(EditAnywhere, Category = "VCKillVolume")
		UBoxComponent * KillVolumeCollider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION() void OnOverlapBegin(AActor * OverlappedActor, AActor * OtherActor);

};
