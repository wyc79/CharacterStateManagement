#pragma once

#include "CoreMinimal.h"

/** Character state identifiers. */
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle, // for idle only
	Walking, // for walking and runnning
	Sliding, 
	MidAir, // anytime when foot is not on ground (and not in other states)
	WallRun, //
	Sprinting,
	Crouch,
	Grapple // when grappled to something
};
