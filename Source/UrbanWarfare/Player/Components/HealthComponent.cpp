
#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, UiHealth);
	DOREPLIFETIME(UHealthComponent, UiArmor);
}

void UHealthComponent::OnDamage(const float InDamage)
{
	float RemainingDamage = InDamage;

	if (CurrentArmor > 0)
	{
		float Absorbable = InDamage * 0.5f;
		float ArmorDamage = FMath::Min(CurrentArmor, Absorbable);

		CurrentArmor -= ArmorDamage;
		RemainingDamage -= ArmorDamage;

		UiArmor = FMath::CeilToInt(FMath::Clamp(CurrentArmor, 0.f, MaxArmor));
	}
	else
		UiArmor = 0;

	CurrentHealth -= RemainingDamage;

	if (CurrentHealth > 0)
		UiHealth = FMath::CeilToInt(FMath::Clamp(CurrentHealth, 0.f, MaxHealth));
	else
	{
		UiHealth = 0;
		//사망 처리 구현 필요
	}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		CurrentHealth = MaxHealth;
		CurrentArmor = MaxArmor;

		UiHealth = static_cast<int8>(MaxHealth);
		UiArmor = static_cast<int8>(MaxArmor);
	}
}

void UHealthComponent::OnRep_UiHealth()
{
	OnUiHealthChange.ExecuteIfBound(UiHealth);
}

void UHealthComponent::OnRep_UiArmor()
{
	OnUiArmorChange.ExecuteIfBound(UiArmor);
}

