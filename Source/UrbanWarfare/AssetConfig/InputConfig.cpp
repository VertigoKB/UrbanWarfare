#include "InputConfig.h"
#include "../Common/WarfareLogger.h"

void UInputConfig::PostLoad()
{
	Super::PostLoad();

	LOG_NULL(DefaultMappingContext);
	LOG_NULL(Movement);
	LOG_NULL(Look);
	LOG_NULL(DefaultMappingContext);
}
