#include "InputConfig.h"
#include "../Common/WarfareLogger.h"

void UInputConfig::PostLoad()
{
	Super::PostLoad();

	LOG_NULL(DefaultMappingContext);
	LOG_NULL(Movement);
	LOG_NULL(Look);
	LOG_NULL(Crouch);
	LOG_NULL(Walk);
	LOG_NULL(Jump);
	LOG_NULL(ThrowWeapon);
	LOG_NULL(EquipPistol);
	LOG_NULL(EquipRifle);
	LOG_NULL(Test);
}
