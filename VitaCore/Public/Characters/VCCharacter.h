// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VCAbilitySystemInterface.h" 
#include "VCEntityInterface.h"
#include "VCGameTypes.h"
#include "VCCharacterTypes.h"
#include "GameplayTags.h"
#include "VCCharacter.generated.h"

class UImage;
class USpringArmComponent;
class UCameraComponent;
class UGameplayEffect;
class UGameplayAbility;
class UAkComponent;
class UAkAudioEvent;
class AVCGameState;
class UVCWidgetComponent;
class AVCPlayerController;
class AVCAIController;
class AVCTeam;
class UVCAbilitySystemComponent;
class UVCHealthAttributeSet;
class UVCTraitAttributeSet;


UCLASS(hideCategories = ( Input, Actor, "Actor Tick", LOD, Variable))
class VITACORE_API AVCCharacter : public ACharacter, public IVCAbilitySystemInterface, public IVCEntityInterface
{
	GENERATED_BODY()

// ============== VC CHARACTER =========================
public:
	AVCCharacter();

	// TODO Make a tag based implementation
	UFUNCTION(BlueprintCallable) void DoStun(float Duration);
	
	UFUNCTION() virtual void DoHit(const FHitInfo& HitInfo) override;
	
	// Enable input controller for Player and AI
	UFUNCTION() void EnableCharacterInput();

	// Disable input controller for Player and AI
	UFUNCTION() void DisableCharacterInput();
	
	// TODO Check if neccessary / protected maybe
	UFUNCTION(BlueprintCallable) void SetupStatusBar();
	
	// Set Ground Friction that is used by Movement Component
	UFUNCTION(BlueprintCallable) void SetGroundFriction(bool IsFrictionOn);
	
	// Update visibility of the status bar
	UFUNCTION(BlueprintCallable) void SetStatusBarVisibility(bool IsVisible);

	// Get the look direction of the character
	UFUNCTION() FRotator GetLookDirection() const;

	// Get generic name of the VC entity
	UFUNCTION(BlueprintCallable) virtual FString GetEntityName() const override;
	
	// Gets AI controller that controls this pawn
	UFUNCTION() AVCAIController* GetAIController() const;

	// Gets player controller that controls this pawn
	UFUNCTION() AVCPlayerController* GetPlayerController() const;

	// Get current type of camera view that character has
	UFUNCTION(BlueprintCallable) ECameraViewTypes GetCameraViewType() const { return CameraViewType;  }
	
	// Set camera view of the character
	UFUNCTION(BlueprintCallable) void SetCameraView(ECameraViewTypes NewCameraViewType);
	
	// Get Camera component of character (Only active when player is controlling)
	UFUNCTION(BlueprintCallable) UCameraComponent* GetCameraComponent() const;

	// Get Spring arm that Camera component is attached to
	UFUNCTION(BlueprintCallable) USpringArmComponent* GetSpringArmComponent() const;
	
protected:

	// This is called when controller changes and specific controllers are cached
	// It will be called both on server and client
	UFUNCTION() virtual void OnControllerChanged();
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Config")
		bool bShouldHideHealthBarOnDeath;

	// Type that determines camera transform based on the properties set
	UPROPERTY(EditAnywhere, Category = "Config|Camera")
 		ECameraViewTypes CameraViewType;

	// Properties that can be customized based on allowed camera view types
	UPROPERTY(EditAnywhere, Category = "Config|Camera")
		TMap<ECameraViewTypes, FCharacterViewProps> CameraViewProperties;
	
	// Cached ground friction to be used to reset it
	UPROPERTY() float InitialGroundFriction;

	UPROPERTY() FGameplayTag Tag_StatusInCombat;

	FTimerHandle TimerHandle_Stun;
	
	// Spring arm for camera collision
	UPROPERTY(EditAnywhere) USpringArmComponent* CameraSpringComponent;

	// Camera component for player controller
	UPROPERTY(EditAnywhere) UCameraComponent* CameraComponent;
	
	// Character in game status bar UI
	UPROPERTY(VisibleAnywhere) UVCWidgetComponent* StatusBarComponent;

	// Cached AI Controller for the character
	UPROPERTY(Transient) AVCAIController* MyAIController;
	
	// Cached Player Controller for the character
	UPROPERTY(Transient) AVCPlayerController* MyPlayerController;

	// Cached VCGame State
	UPROPERTY(Transient) AVCGameState* VCGameState;
	
// ============== TEAM =========================
public:

	// Set new team type for the character
	UFUNCTION(BlueprintCallable) void SetTeamType(ETeamType NewTeamType);
	
	// Get Team type of the character
	UFUNCTION(BlueprintCallable)
		virtual ETeamType GetTeamType() const override { return TeamType; };

	// Get reference to the team actor if one exists
	UFUNCTION() AVCTeam* GetTeam() const { return MyTeam; };
	
	// Check the affiliation of the an actor to this character
	UFUNCTION(BlueprintCallable)
		virtual EAffiliationType GetAffiliation(AActor* Other) const override;

	// Callback which triggers when player Team is changed
	FOnTeamChanged OnTeamChanged;
	
protected:
	// Internal method to handle team change
	virtual void HandleTeamChange(ETeamType PrevTeamType, ETeamType NewTeamType);

	// Client side Team Type update
	UFUNCTION() void OnRep_TeamType(ETeamType PrevTeamType);
	
protected:
	// Team Type of the character
	UPROPERTY(ReplicatedUsing = OnRep_TeamType, EditAnywhere, Category = "Config")
		ETeamType TeamType;
	
	// Team actor that this character subscribes to if this exists
	UPROPERTY(Transient) AVCTeam* MyTeam;

// ==================== SPAWN / DEATH ============================
public:
	// TODO Maybe Entity Interface method
	// Spawn character
	virtual void DoSpawn();
	
	// Kill the character (Death info will be replicated)
	virtual void DoDeath(FDeathInfo DeathInfo = FDeathInfo()) override;

	// Return either character or team delegate for death event
	virtual FOnEntityDeath& GetOnEntityDeathDelegate() override;
	
	// Check if character is alive
	UFUNCTION() virtual bool IsAlive() const override;
	
protected:
	// Internal function to be called both on server and client for death state
	UFUNCTION() virtual void HandleDeathStateChange(bool IsDead);

	// Client method to be called on DeathInfo replication
	UFUNCTION() virtual void OnRep_DeathInfo(FDeathInfo PrevDeathInfo);
	
	FOnEntityDeath OnDeath;

protected:
	// Death state information of the character
	UPROPERTY(ReplicatedUsing = OnRep_DeathInfo)
		FDeathInfo CharacterDeathInfo;

	// Tag for Dead Status
	UPROPERTY() FGameplayTag Tag_StatusDead;

	// Callback that triggers when character dies

	FTimerHandle TimerHandle_Respawn;
	
// ========================================================

// ==================== LEVEL / XP ============================

public:
	// Increment level of the character by
	UFUNCTION() void IncrementLevel();

	// Apply XP to the character or the team of the character
	UFUNCTION() void ApplyXP(int XP);

	// Get current character level
	UFUNCTION() virtual int GetCharacterLevel() const;
	
	// Get current XP from either team or character
	UFUNCTION() int GetCharacterXP() const;

	// Get level bracket total experience of the certain character level
	UFUNCTION() int GetTotalLevelBracketXP(int Level) const;

	// Get current xp ratio based on the bracket for either team or character
	UFUNCTION() float GetCurrentXPRatio() const;

	// Check if character is at max level
	UFUNCTION() bool IsCharacterLevelMaxed() const;

	// Get the XP change delegate either from character or team
	FOnXPChanged& GetXPChangedDelegate();

	// Get the level change delegate either from character or team
	FOnLevelChanged& GetOnLevelChangedDelegate();

protected:
	
	// Internal method to extend functionality on level change
	UFUNCTION() virtual void HandleLevelChange(int PrevLevel, int CurrentLevel);
	
	// Client side character level change callback
	UFUNCTION() void OnRep_CharacterLevel(int PrevLevel);

	// Client side character xp change callback
	UFUNCTION() void OnRep_CharacterXP(int PrevXP);
	
protected:
	
	// Character level which will be used by abilities and stat scaling
	UPROPERTY(ReplicatedUsing = OnRep_CharacterLevel, EditAnywhere, Category = "Config", meta = (EditCondition = "!bShouldUseTeamLevel"))
		int CharacterLevel;

	FOnLevelChanged OnLevelChanged;

	// Character level which will be used by abilities and stat scaling
	UPROPERTY(ReplicatedUsing = OnRep_CharacterXP, EditAnywhere, Category = "Config", meta = (EditCondition = "!bShouldUseTeamLevel"))
		int CharacterXP;

	FOnXPChanged OnXPChanged;

	// Use team level for the character instead of individual level
	UPROPERTY(EditAnywhere, Category = "Config")
		bool bShouldUseTeamLevel;
	
// ========================================================

// ================ ABILITY SYSTEM ======================

public:

	// Clear cooldown from all input abilities
	UFUNCTION() void RefreshAbilities() const;
	
	// Ability System Component Getter
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// Get ability level of the character (Ability Level == Character Level)
	UFUNCTION() int GetAbilityLevel() const override;
	
	// Return basVCHealthAttributeSett of the character
	UFUNCTION() UVCHealthAttributeSet* GetHealthAttributeSet() const { return HealthAttributeSet; };

	// Return trait attribute set of the character
	UFUNCTION() UVCTraitAttributeSet* GetTraitAttributeSet() const { return TraitAttributeSet; };
	
	// Multicast delegate to notify change in combat state
	FOnInCombatStateChanged OnInCombatStateChanged;
	
protected:

	// Internal method to extend functionality on combat state changed
	UFUNCTION() virtual void HandleCombatStateChange(const bool IsInCombat);

	// Callback for when damage is applied to health attribute
	UFUNCTION() virtual void OnDamage(AActor* Damager, float Damage, float CurrentHealth, float MaxHealth);
	
	// Setup Registered Abilities
	virtual void InitializeAbilities();

	// This method can be used to initialize abilities contained in a TMap that have enum input bindings
	template<typename T>
	void InitializeInputAbilities(TMap <T, TSubclassOf<UGameplayAbility>>& AbilityMapping)
	{
		if (AbilitySystemComponent && AbilityInputBindEnum)
		{
			for (auto& Ability : AbilityMapping)
			{
				if (Ability.Value)
				{
					AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.Value.GetDefaultObject(), 1,
						static_cast<int>(Ability.Key)));
				}
			}
		}
	}
	// Activate registered effects
	void InitializeEffects();
	
protected:
	
	// Enum pointer for the enum that will be used for input bindings
	UPROPERTY() UEnum* AbilityInputBindEnum;
	
	// Ability System Component
	UPROPERTY(VisibleAnywhere) UVCAbilitySystemComponent* AbilitySystemComponent;

	//VCHealthAttributeSete attributes
	UPROPERTY() UVCHealthAttributeSet* HealthAttributeSet;

	// Character base attributes
	UPROPERTY() UVCTraitAttributeSet* TraitAttributeSet;
	
	// Spawn Ability
	UPROPERTY(EditAnywhere, Category = "Config|Ability System") TSubclassOf<UGameplayAbility> SpawnAbility;

	// Death Ability
	UPROPERTY(EditAnywhere, Category = "Config|Ability System")
		TSubclassOf<UGameplayAbility> DeathAbility;

	UPROPERTY(EditAnywhere, Category = "Config|Ability System")
		TSubclassOf<UGameplayEffect> LevelUpEffect;

	// Effects that will be applied on initialization
	UPROPERTY(EditAnywhere, Category = "Config|Ability System")
		TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

// ========================================================

	
// ==================== AK AUDIO ============================
public:
	// Getter for Ak component
	UFUNCTION(BlueprintCallable) UAkComponent* GetAkComponent() const;

protected:
	// Ak Component for character sound events
	UAkComponent* AKComponent;

	// AkEvent that is called whenever the character is leveled up
	UPROPERTY(EditAnywhere, Category = "Config|Audio")
		UAkAudioEvent* AkEvent_OnLevelUp;

	// AkEvent that is called whenever the character is damaged
	UPROPERTY(EditAnywhere, Category = "Config|Audio")
		UAkAudioEvent* AkEvent_OnDamageHurt;

// ==================== UI ============================
public:
	UTexture2D* GetAvatarImage() const { return AvatarImage; }

protected:
	UPROPERTY(EditAnywhere, Category = "Config|UI") UTexture2D* AvatarImage;

protected:
// ==================== MOVEMENT ============================

	virtual void DoMoveVertical(float InputValue);
	virtual void DoMoveHorizontal(float InputValue);
	virtual void DoLookVertical(float InputValue);
	virtual void DoLookHorizontal(float InputValue);
	virtual void DoJump();

// ==================== CHARACTER ACTOR ============================
protected:
	virtual void PostActorCreated() override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;
	virtual void UnPossessed() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
};


