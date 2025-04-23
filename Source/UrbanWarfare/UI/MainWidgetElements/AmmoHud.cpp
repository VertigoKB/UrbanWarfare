// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoHud.h"
#include "UrbanWarfare/UI/MainWidget.h"

void UAmmoHud::ExternalInitialize(UUserWidget* const InOwner, UWorld* const InWorld)
{
	MainWidget = Cast<UMainWidget>(InOwner);
	World = InWorld;
}
