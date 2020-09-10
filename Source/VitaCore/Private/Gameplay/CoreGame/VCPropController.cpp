// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCPropController.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UnrealNetwork.h"
#include "Particles/Emitter.h"

// Sets default values
AVCPropController::AVCPropController()
{
	bIsFireTitanPropsActive = false;
	bIsIceTitanPropsActive = false;

	bReplicates = true;
	
}

void AVCPropController::BeginPlay()
{
	Super::BeginPlay();
	
	for (AActor* Prop : FireTitanProps)
	{
		DeactiveProp(Prop);
	}
	for (AActor* Prop : IceTitanProps)
	{
		DeactiveProp(Prop);
	}
}

void AVCPropController::OnRep_IsFireTitanPropsActive()
{
	if (bIsFireTitanPropsActive == true)
	{
		for (AActor* Prop : FireTitanProps)
		{
			ActiveProp(Prop);
		}
	}
	else
	{
		for (AActor* Prop : FireTitanProps)
		{
			DeactiveProp(Prop);
		}
	}
}

void AVCPropController::OnRep_IsIceTitanPropsActive()
{
	if(bIsIceTitanPropsActive == true)
	{
		for (AActor* Prop : IceTitanProps)
		{
			ActiveProp(Prop);
		}
	}
	else
	{
		for (AActor* Prop : IceTitanProps)
		{
			DeactiveProp(Prop);
		}
	}
}

void AVCPropController::ActiveProp(AActor* Prop)
{
	if(Prop == nullptr)
	{
		return;
	}
	AStaticMeshActor * StaticMeshProp = Cast<AStaticMeshActor>(Prop);
	if(StaticMeshProp != nullptr)
	{
		UStaticMeshComponent * MeshComponent = StaticMeshProp->GetStaticMeshComponent();
		if(MeshComponent != nullptr)
		{
			MeshComponent->SetVisibility(true);
			MeshComponent->SetGenerateOverlapEvents(true);
			MeshComponent->SetCollisionProfileName("BlockAll");
		}
	}

	AEmitter * Emitter = Cast<AEmitter>(Prop);
	if(Emitter != nullptr)
	{
		UParticleSystemComponent * ParticleSystemComponent = Emitter->GetParticleSystemComponent();
		if(ParticleSystemComponent != nullptr)
		{
			ParticleSystemComponent->Activate();
		}
	}
}

void AVCPropController::DeactiveProp(AActor* Prop)
{
	if (Prop == nullptr)
	{
		return;
	}
	AStaticMeshActor * StaticMeshProp = Cast<AStaticMeshActor>(Prop);
	if (StaticMeshProp != nullptr)
	{
		UStaticMeshComponent * MeshComponent = StaticMeshProp->GetStaticMeshComponent();
		if (MeshComponent != nullptr)
		{
			MeshComponent->SetVisibility(false);
			MeshComponent->SetGenerateOverlapEvents(false);
			MeshComponent->SetCollisionProfileName("NoCollision");
		}
	}

	AEmitter * Emitter = Cast<AEmitter>(Prop);
	if (Emitter != nullptr)
	{
		Emitter->Deactivate();
	}
}

void AVCPropController::SetFireTitanPropsActive(bool IsActive)
{
	bIsFireTitanPropsActive = IsActive;
	OnRep_IsFireTitanPropsActive();
}

void AVCPropController::SetIceTitanPropsActive(bool IsActive)
{
	bIsIceTitanPropsActive = IsActive;
	OnRep_IsIceTitanPropsActive();
}


// Replicate properties
void AVCPropController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVCPropController, bIsFireTitanPropsActive);
	DOREPLIFETIME(AVCPropController, bIsIceTitanPropsActive);

}