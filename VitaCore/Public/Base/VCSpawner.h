// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VCGameTypes.h"
#include "VCSpawner.generated.h"

class UCapsuleComponent;
class UBillboardComponent;

// Actions types that will determine what will spawner do after actor is spawned
UENUM()
enum class EAfterSpawnActionTypes : uint8
{
	NoAction = 0,
	Destroy ,
	Deactivate,
	Respawn_OnDeath,
	Respawn_OnDestroy
};

UCLASS(Blueprintable)
class VITACORE_API AVCSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AVCSpawner();

	// Create an actor at the spawner location
	UFUNCTION() void DoSpawn();

	// Teleport an actor to the spawn location (This should be used if 
	UFUNCTION() void DoTeleport(AActor* SpawningActor)  const;

	UFUNCTION() FTransform GetSpawnTransform() const;

	// Get team type if spawner is using it
	UFUNCTION() bool GetTeamType(ETeamType& OutTeamType) const;
	
	// Check if spawner is activated for spawn
	UFUNCTION() bool IsActive() const { return bIsActive; };

	UFUNCTION() TSubclassOf<AActor> GetSpawningActorClass() const { return ActorClassToSpawn; };

protected:
	
	UFUNCTION() void SpawnActorClass();

	UFUNCTION() void HandleAfterSpawnAction(AActor * SpawnedActor);

	// Function for OnDestroyed callback in spawned actor
	UFUNCTION() void SpawnOnSpawnedActorDestroyed(AActor* DestroyedActor);
	
protected:

	// If true spawning will commence when the Begin Play is called
	UPROPERTY(EditAnywhere, Category = "Config", meta=(EditCondition="bShouldUseTeamType"))
		ETeamType TeamType;

	// Flag to determine
	UPROPERTY(EditAnywhere, meta=(InlineEditConditionToggle))
		bool bShouldUseTeamType;
	
	// If true spawning will commence when the Begin Play is called
	UPROPERTY(EditAnywhere, Category = "Config")
		bool bShouldSpawnAtBeginPlay;
	
	UPROPERTY(EditAnywhere, Category = "Config")
		float SpawnDelay;
	
	UPROPERTY(EditAnywhere, Category = "Config")
		EAfterSpawnActionTypes AfterSpawnActionType;
	
	// Check if spawner is active or not (Only server has to know about this)
	UPROPERTY(EditAnywhere, Category = "Config") bool bIsActive;

	// Capsule for checking collision
	UPROPERTY(EditAnywhere, Category = "Spawner")
		UCapsuleComponent* SpawnCapsule;
	
	// For indicating spawner in the map
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
		UBillboardComponent* SpawnSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		TSubclassOf<AActor> ActorClassToSpawn;

	FTimerHandle TimerHandle_SpawnTimer;
	
protected:
	virtual void BeginPlay() override;


};
