// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VCGameTypes.h"
#include "GameplayTagContainer.h"
#include "VCPlayerController.generated.h"

class UAkAudioEvent;

class UVCHUD_Player;
class UVCHUD_CharacterSelection;
class UVCHUD_Game;
class UVCHUD_PauseGame;
class UGameplayAbility;
class AVCCharacter;
class AVCGameState;
class UVCDebugManager;
class AVCPlayerState;

DECLARE_DELEGATE_OneParam(FInputToggleDelegate, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnControlledCharacterChanged, AVCCharacter*);

UCLASS(hideCategories = (Game, Replication, Actor, "Actor Tick", LOD, Variable))
class VITACORE_API AVCPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AVCPlayerController();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerCreateCharacter(int CharacterIndex, ETeamType PlayerTeam);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerSetTeam(ETeamType PlayerTeam);
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerSetPlayerName(const FString& NewPlayerName);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
		void ServerStartMatch();

public:

	UFUNCTION(BlueprintCallable) void ShowMissionObjective();

	UFUNCTION(BlueprintCallable) void NotifyPlayerDamage(float Damage);


	
	// Post an Ak Audio Event to camera manager so that only local player can hear it
	UFUNCTION() void PostLocalAkEvent(UAkAudioEvent* AudioEvent)  const;

	// Get cached controlled character
	UFUNCTION() AVCCharacter* GetControlledCharacter() const { return MyCharacter; }

	UFUNCTION() AVCPlayerState* GetVCPlayerState() const { return VCPlayerState; }

	
	// Get Character HUD
	UFUNCTION() UVCHUD_Player* GetPlayerHUD() const { return PlayerHUD; };

	template<typename HUDClass>
	void AttachHUD(TSubclassOf<HUDClass> Class, HUDClass*& Ref, bool ShouldShowMouse, int32 ZOrder = 0)
	{
		if (IsLocalPlayerController() && Class)
		{
			if (Ref)
			{
				Ref->RemoveFromViewport();
				Ref = nullptr;
			}

			Ref = CreateWidget<HUDClass>(this, Class);
			Ref->AddToViewport(ZOrder);
			bShowMouseCursor = ShouldShowMouse;
		}
	}

	FOnControlledCharacterChanged OnControlledCharacterChanged;
	
protected:

	// Called when player controller is created and player state is initialized
	UFUNCTION() virtual void HandlePlayerLogin();

	// TODO implement this
	// Called when player controller is destroyed
	UFUNCTION() virtual void HandlePlayerLogout();


	// Called both on server and client when controller possesses a valid character
	UFUNCTION() virtual void HandlePossess(AVCCharacter* InCharacter);

	// Called both on server and client when controller unpossesses a valid character
	UFUNCTION() virtual void HandleUnPossess(AVCCharacter* OutCharacter);
	
	UFUNCTION(BlueprintCallable) void ToggleEscapeMenu();
	
	// Show character info for the controlled pawn
	UFUNCTION() void ToggleCharacterInfo(bool IsOn);

	// Show player statistics for the game
	UFUNCTION() void ToggleGameStats(bool IsOn);

	// Callback for whenever a ability fails due to cost/cooldown
	UFUNCTION() virtual void HandlePawnAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& TagContainer);


	
protected:
	
	// AkEvent that is played whenever an ability fails to be casted 
	UPROPERTY(EditAnywhere, Category = "Config|Audio")
		UAkAudioEvent* AkEvent_AbilityFail;

	// Tag for the ability that will determine if "Ability Fail" event will play or not
	UPROPERTY() FGameplayTag Tag_MuteAbilityFailAkEvent;

	UPROPERTY(EditDefaultsOnly, Category = "Config|UserInterface")
		TSubclassOf<UVCHUD_Player> PlayerHUDClass;
	
	UPROPERTY(Transient) UVCHUD_Player* PlayerHUD;

	UPROPERTY(EditDefaultsOnly, Category = "Config|UserInterface")
		TSubclassOf<UVCHUD_PauseGame> InGameMenuClass;

	UPROPERTY(Transient) UVCHUD_PauseGame* InGameMenu;
	
	// Cached Reference to the possessed character
	UPROPERTY() AVCCharacter* MyCharacter;
	UPROPERTY(Transient) AVCGameState* VCGameState;
	UPROPERTY(Transient) AVCPlayerState* VCPlayerState;
	
// ==============  PLAYER CONTROLLER =======================
protected:
	virtual void PostActorCreated() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void PawnLeavingGame() override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Pawn() override;
};
