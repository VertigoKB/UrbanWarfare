
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "UrbanWarfare/Player/PlayerBase.h"
#include "UrbanWarfare/Player/WarfareAnim.h"
#include "UrbanWarfare/Player/Components/RegisterInputComponent.h"
#include "UrbanWarfare/Common/WarfareLogger.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld())
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, UiHealth);
	DOREPLIFETIME(UHealthComponent, UiArmor);
	DOREPLIFETIME(UHealthComponent, bIsDeath);
}

void UHealthComponent::Server_OnDamage(const float InDamage)
{
	float RemainingDamage = InDamage;

	if (CurrentArmor > 0)
	{
		float Absorbable = InDamage * 0.55f;
		float ArmorDamage = FMath::Min(CurrentArmor, Absorbable);

		CurrentArmor -= ArmorDamage;
		RemainingDamage -= ArmorDamage * 1.2f;

		UiArmor = FMath::CeilToInt(FMath::Clamp(CurrentArmor, 0.f, MaxArmor));

		if (bIsLocalHost)
			OnRep_UiArmor();
	}
	else
	{
		UiArmor = 0;
		if (bIsLocalHost)
			OnRep_UiArmor();
	}

	CurrentHealth -= RemainingDamage;

	if (CurrentHealth > 0)
	{
		UiHealth = FMath::CeilToInt(FMath::Clamp(CurrentHealth, 0.f, MaxHealth));
		if (bIsLocalHost)
			OnRep_UiHealth();
	}
	else
	{
		UiHealth = 0;
		if (bIsLocalHost)
			OnRep_UiHealth();

		bIsDeath = true;
		OnRep_bIsDeath();

		Server_OnRespawnQue();
	}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = GetOwner<APlayerBase>();
	RegisterInputComponent = OwnerPawn->GetRegInputComp();
	RegisterInputComponent->OnTestInput.BindUObject(this, &UHealthComponent::Server_TempRespawn);

	if (OwnerPawn->HasAuthority())
	{
		CurrentHealth = MaxHealth;
		CurrentArmor = MaxArmor;

		UiHealth = static_cast<int8>(MaxHealth);
		UiArmor = static_cast<int8>(MaxArmor);
	}

	if (GetWorld())
		GetWorld()->GetTimerManager().SetTimer(InitHandle, FTimerDelegate::CreateLambda([this]() {

		bIsInitSuccess = InitConstruct();

		if (bIsInitSuccess)
		{
			LOG_EFUNC(TEXT("Initialization success."));
			GetWorld()->GetTimerManager().ClearTimer(InitHandle);
		}
			}), 0.1f, true);
}

bool UHealthComponent::InitConstruct()
{
	USkeletalMeshComponent* TheMesh = OwnerPawn->GetTheMesh();
	if (!TheMesh)
		return false;

	USkeletalMeshComponent* ThirdMesh = OwnerPawn->GetThirdMesh();

	FirstAnimInst = Cast<UWarfareAnim>(TheMesh->GetAnimInstance());
	if (!FirstAnimInst)
		return false;

	ThirdAnimInst = Cast<UWarfareAnim>(ThirdMesh->GetAnimInstance());
	if (!ThirdAnimInst)
		return false;

	return true;
}

void UHealthComponent::OnRep_UiHealth()
{
	OnUiHealthChange.ExecuteIfBound(UiHealth);
}

void UHealthComponent::OnRep_UiArmor()
{
	OnUiArmorChange.ExecuteIfBound(UiArmor);
}

void UHealthComponent::OnRep_bIsDeath()
{
	if (bIsInitSuccess)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(OnRepDeathHandle))
			GetWorld()->GetTimerManager().ClearTimer(OnRepDeathHandle);

		OnDeathFlagChange.ExecuteIfBound(bIsDeath);
		FirstAnimInst->SetDeathFlag(bIsDeath);
		ThirdAnimInst->SetDeathFlag(bIsDeath);

		if (bIsDeath)
		{
			FirstAnimInst->PlayMontage_Death();
			ThirdAnimInst->PlayMontage_Death();
		}
		else
		{
			// 리스폰
		}
	}
	else
	{
		if (GetWorld())
		{
			if (!(GetWorld()->GetTimerManager().IsTimerActive(OnRepDeathHandle)))
				GetWorld()->GetTimerManager().SetTimer(OnRepDeathHandle, this, &UHealthComponent::OnRep_bIsDeath, 0.1f, true);
		}
	}
}

void UHealthComponent::Server_OnRespawnQue()
{

}

void UHealthComponent::Server_TempRespawn_Implementation()
{
	bIsDeath = false;
	/*FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	OwnerPawn->GetTheMesh()->AttachToComponent(OwnerPawn->GetRootCapsule(), Rules);*/
}