#pragma once

#include "CoreMinimal.h"
#include "CharacterStateManagement/CharacterStateEnum.h"

class UCharacterStateManagerComponent;

/**
 * Base class for character states. Mirrors Unity CharacterBaseState:
 * Enter() on state entry, Tick() each frame, Exit() on state exit.
 */
class CD_TEMP_API FCharacterBaseState
{
public:
	explicit FCharacterBaseState(UCharacterStateManagerComponent* InOwner, ECharacterState InState);
	virtual ~FCharacterBaseState() = default;

	ECharacterState GetState() const { return State; }

	virtual void Enter() {}
	virtual void Tick(float DeltaTime) {}
	virtual void Exit() {}

protected:
	UCharacterStateManagerComponent* StateManager = nullptr;
	ECharacterState State;
};
