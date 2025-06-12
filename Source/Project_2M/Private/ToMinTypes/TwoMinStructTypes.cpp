
#include "ToMinTypes/TwoMinStructTypes.h"

#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"

bool FTwoMinPlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
