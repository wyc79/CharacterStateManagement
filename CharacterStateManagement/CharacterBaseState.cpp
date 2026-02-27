
#include "CharacterStateManagement/CharacterBaseState.h"
#include "CharacterStateManagement/CharacterStateManagerComponent.h"

FCharacterBaseState::FCharacterBaseState(UCharacterStateManagerComponent* InOwner, ECharacterState InState)
	: StateManager(InOwner)
	, State(InState)
{
}
