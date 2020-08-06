// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void UVCWidgetComponent::InitWidget()
{
	Super::InitWidget();
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetCollisionProfileName("NoCollision");
	SetGenerateOverlapEvents(false);
	bShouldFaceCamera = true;
}

void UVCWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(bShouldFaceCamera == true)
	{
		LookAtLocalCamera();
	}
}


void UVCWidgetComponent::LookAtLocalCamera()
{
	if(LocalCameraManager != nullptr)
	{
		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(this->GetComponentLocation(), LocalCameraManager->GetCameraLocation());
		this->SetWorldRotation(LookRotation);
	}
	else
	{
		SetLocalCameramanager();
	}
	
}

void UVCWidgetComponent::SetLocalCameramanager()
{
	LocalCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
}
