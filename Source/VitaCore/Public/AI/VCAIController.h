// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "VCGameTypes.h"
#include "VCAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
class UBehaviourTree;
class AVCCharacter;

class AVCHeroCharacter;

UENUM(Blueprintable)
enum class ETitanFocusType : uint8
{
	CLOSEST = 0,
	RANDOM,
	HIGHEST_HEALTH
};


UCLASS()
class VITACORE_API AVCAIController : public AAIController
{
	GENERATED_BODY()
// ============== VC AI CONTROLLER =========================

public:
	AVCAIController();

	// Update all targets for the AI actor
	UFUNCTION() void UpdateTargets();

	// Get currently focused actor
	UFUNCTION(BlueprintCallable) void SetFocusedTarget(AActor * TargetActor);

	// Clear currently focused actor
	UFUNCTION(BlueprintCallable) void ResetTarget();

	// Get all characters around the AI pawn based on their affiliation
	UFUNCTION() TArray<AVCCharacter*> GetCharactersAroundPawn(float Radius, EAffiliationType AffiliationType) const;
	
protected:

	// If true, actor that damaged controlled pawn will be added to targets
	UPROPERTY(EditAnywhere, Category = "VC AI Controller - Config")
		bool bShouldTargetDamager;

	// If true, pawn will check for all enemies around itself
	UPROPERTY(EditAnywhere, Category = "VC AI Controller - Config")
		bool bShouldCheckEnemyCharactersAround;

	// Radius for checking enemies around
	UPROPERTY(EditAnywhere, Category = "VC AI Controller - Config", meta=(EditCondition="bShouldCheckEnemyCharactersAround"))
		float EnemyCharactersCheckRadius;

	UPROPERTY(EditDefaultsOnly, Category = "VC AI Controller - Config")
		UBehaviorTree* BehaviorTree;

	// Blackboard key for focused target
	UPROPERTY()FName TargetKey;

	// Callback function that will be called when controlled pawn is damaged
	UFUNCTION() virtual void OnPawnDamaged(AActor* Damager, float Damage, float CurrentHealth, float MaxHealth);

	// Callback function that will be called when controlled pawn is dead
	UFUNCTION() virtual void OnPawnDead(const FDeathInfo DeathInfo);
	

protected:
	
	UPROPERTY(EditInstanceOnly) UBehaviorTreeComponent* BehaviorComponent;
	UPROPERTY(EditInstanceOnly) UBlackboardComponent* BlackboardComponent;
	
	// Owned character pawn
	UPROPERTY(Transient) AVCCharacter* MyCharacter;

	// Targeted actors
	UPROPERTY(Transient) TArray<AActor*> Targets;

	// Currently focused target
	UPROPERTY(Transient) AActor* FocusedTarget;
	
protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
};
