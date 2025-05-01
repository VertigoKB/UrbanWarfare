// Fill out your copyright notice in the Description page of Project Settings.


#include "ValueBarHud.h"

#include "UrbanWarfare/UI/MainWidget.h"
#include "UrbanWarfare/Player/Components/HealthComponent.h"

void UValueBarHud::ExternalInitialize(UUserWidget* const InOwner, UWorld* const InWorld, UHealthComponent* const InHealthComp)
{
	MainWidget = Cast<UMainWidget>(InOwner);
	World = InWorld;
	HealthComponent = InHealthComp;

	HealthComponent->OnUiHealthChange.BindUObject(this, &UValueBarHud::OnHealthChange);
	HealthComponent->OnUiArmorChange.BindUObject(this, &UValueBarHud::OnArmorChange);

	MaxHealth = HealthComponent->GetMaxHealth();
	MaxArmor = HealthComponent->GetMaxArmor();

	MaxUiHealth = static_cast<int8>(MaxHealth);
	MaxUiArmor = static_cast<int8>(MaxArmor);

	OnHealthChange(MaxUiHealth);
	OnArmorChange(MaxUiArmor);
}

void UValueBarHud::OnHealthChange(int8 InValue)
{

	float Mapped = FMath::GetMappedRangeValueClamped(FVector2D(0, MaxUiHealth), FVector2D(0.f, 1.f), InValue);
	MainWidget->SetProgressBarPercentValue(EMainWidgetElem::HealthBar, Mapped);

	FText ValueText = FText::AsNumber(InValue);
	MainWidget->SetTextBlockContent(EMainWidgetElem::HealthValue, ValueText);
}

void UValueBarHud::OnArmorChange(int8 InValue)
{
	float Mapped = FMath::GetMappedRangeValueClamped(FVector2D(0, MaxUiArmor), FVector2D(0.f, 1.f), InValue);
	MainWidget->SetProgressBarPercentValue(EMainWidgetElem::ArmorBar, Mapped);

	FText ValueText = FText::AsNumber(InValue);
	MainWidget->SetTextBlockContent(EMainWidgetElem::ArmorValue, ValueText);
}
