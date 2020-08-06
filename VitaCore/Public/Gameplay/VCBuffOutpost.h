// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VCGameTypes.h"
#include "VCAbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "VCCoreGameMode.h"
#include "VCCoreGameTypes.h"

#include "VCBuffOutpost.generated.h"


class UGameplayEffect;
class AAVCVitaCoreShrine;
class USphereComponent;
class AVCCharacter;
class UVCWidgetComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UVCAbilitySystemComponent;

UCLASS()
class VITACORE_API AVCBuffOutpost : public AActor, public IVCAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	

	AVCBuffOutpost();

	bool IsLocked() const { return bIsLocked; }
	float GetCaptureAmount() const { return CaptureAmount; }
	ETeamType GetCapturedTeam() const { return CapturedTeam; }
	virtual int GetAbilityLevel() const override { return 0; };
	UFUNCTION(BlueprintCallable) ECoreOutpostTypes GetOutpostType() const { return OutpostType; };
	UFUNCTION(BlueprintCallable) virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	// Set the team that captured the outpost
	UFUNCTION() void SetCapturedTeam(ETeamType Team);

	// Set color of the outpost material and particles
	UFUNCTION() void SetOutpostColor();

	// Capture outpost
	UFUNCTION() void DoCapture();

	// Reset outpost 
	UFUNCTION() void DoReset();

	// Update capture points based on the actors that are trying to capture it
	UFUNCTION() void UpdateCapturePoints(float DeltaTime);

	// Add capture points
	UFUNCTION() void AddCapturePoints(ETeamType TeamType, float DelaTime);
	
	UFUNCTION() void RemoveExistingBuffFromTeam();

	// Cosmetic blueprint function for capture event
	UFUNCTION(BlueprintImplementableEvent, Category = "Config", meta = (DisplayName = "OnCapture"))
		void BP_OnCapture();

	// Cosmetic blueprint function for reset event
	UFUNCTION(BlueprintImplementableEvent, Category = "Config", meta = (DisplayName = "OnReset"))
		void BP_OnReset();

	// Callback when captured team is changed on the client
	UFUNCTION() void OnRep_CapturedTeam();
	
	// Callback when outpost is locked on the client
	UFUNCTION() virtual	void OnRep_IsLocked();

	// Callback when an actor enters outpost detection
	UFUNCTION() void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	// Callback when an actor exits outpost detection
	UFUNCTION() void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);
	
protected:

	// Team that currently hold the points
	UPROPERTY(ReplicatedUsing = OnRep_CapturedTeam, BlueprintReadWrite, Category = "Config")
		ETeamType CapturedTeam;

	// Effect that will be applied to team that is holding the capture point
	UPROPERTY(EditAnywhere, Category = "Config")
		TSubclassOf<UGameplayEffect>  BuffEffect;
	
	// Type of the outpost
	UPROPERTY(EditAnywhere, Category = "Config")
		ECoreOutpostTypes OutpostType;

	// If true, it means that a team is holding the capture point
	UPROPERTY(Replicated, EditAnywhere, Category = "Config")
		bool bIsHolding;

	// When capture point is locked it cannot be captured or capture value cannot be changed
	UPROPERTY(ReplicatedUsing = OnRep_IsLocked, EditAnywhere, Category = "Config")
		bool bIsLocked;

	// Time that outpost will be locked once it is captured
	UPROPERTY(EditAnywhere, Category = "Config")
		float LockoutDuration;

	// Resets the team to neutural and capture amount to zero if it is true when reset occurs
	UPROPERTY(EditAnywhere, Category = "Config")
		bool bShouldResetAfterLockout;

	// Level of the effect that will be applied
	UPROPERTY(EditAnywhere, Category = "Config")
		float BuffLevel;

	// Remove the buff from other team when it is captured
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool bShouldRemoveBuffFromTeam;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Config")
		float CaptureAmount;

	// Rate to capture for each player (DeltaTime * CaptureRate)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float CaptureRate;

	// Rate to return to max if outpost is currently held (DeltaTime * RefreshRate)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float RefreshRate;

	// Rate that points will decline if no players are at the outpost when it is not locked (DeltaTime * DeclineRate)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float DeclineRate;

	// Tags that are on the actor that will prevent capture
	UPROPERTY(EditAnywhere, Category = "Config")
		FGameplayTagContainer IgnoreCaptureWithTags;
	
	UPROPERTY(EditAnywhere, Category = "BuffOutpost")
		USphereComponent* BuffAreaVolume;

	UPROPERTY(EditAnywhere, Category = "BuffOutpost")
		UVCWidgetComponent* OutpostWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category = "BuffOutpost")
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "BuffOutpost")
		UParticleSystemComponent* LaserParticle;
	
	UPROPERTY() UVCAbilitySystemComponent * AbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		bool bIsDecaying;
	
	UPROPERTY(Transient) TArray<AVCCharacter*> OverlappedPlayers;
	
	FTimerHandle TimerHandle_LockoutTime;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

