// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCMenuContainer.h"

#include "Engine/Public/TimerManager.h"
#include "VCMenuHttp.h"
#include "EngineUtils.h"

#include "Engine/NetConnection.h"

#include "UMG/Public/Components/Button.h"

// Setup widget
bool UVCMenuContainer::Initialize()
{
	bool bResult = Super::Initialize();

	// Check for base class
	if (!bResult)
	{
		return false;
	}

	// Cache text block to the property member of the class 
	//UsernameTextBox = Cast<UEditableTextBox>(GetWidgetFromName("UsernameTextBox"));

	return true;
}

void UVCMenuContainer::NativeConstruct()
{
	Super::NativeConstruct();
	//if (QueueButton != nullptr)
	//{
	//	QueueButton->OnClicked.AddDynamic(this, &UVCMenuContainer::EnterQueue);
	//}

	for (TActorIterator<AVCMenuHttp> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		MenuHttp = *ActorItr;
		UE_LOG(LogTemp, Log, TEXT("%s"), *MenuHttp->GetName());
	}

	//if (PlayerName.IsEmpty() == true)
	//{
	//	// TODO CHANGE TO IP AND NULLCHECK
	//	FString playerID = FString::FromInt(GetWorld()->GetGameState()->PlayerArray[0]->PlayerId);
	//	PlayerName = playerID;
	//}


}
//
//void UVCMenuContainer::EnterQueue()
//{
//	if (MenuHttp != nullptr)
//	{
//		UWorld * World = GetWorld();
//		if (World == nullptr)
//		{
//			return;
//		}
//
//		FTimerDelegate TimerDel;
//
//		FClientCheckSession_Request CheckSessionRequest;
//		CheckSessionRequest.player = PlayerName;
//		TimerDel = FTimerDelegate::CreateUObject(MenuHttp, &AVCMenuHttp::PostCheckSession, CheckSessionRequest);
//
//		World->GetTimerManager().SetTimer(TimerHandle_Queue, TimerDel, 1.0f, true, 1.0f);
//
//		FClientEnterQueue_Request EnterQueueRequest;
//		UE_LOG(LogTemp, Log, TEXT("%s"), *PlayerName);
//
//		EnterQueueRequest.player = PlayerName;
//		MenuHttp->PostEnterQueue(EnterQueueRequest);
//
//	}
//}
//
//void UVCMenuContainer::ExitQueue()
//{
//	FClientExitQueue_Request ExitQueueRequest;
//	ExitQueueRequest.player = PlayerName;
//	MenuHttp->PostExitQueue(ExitQueueRequest);
//
//	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Queue);
//}
