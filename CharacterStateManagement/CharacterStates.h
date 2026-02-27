#pragma once

#include "CharacterStateManagement/CharacterBaseState.h"
#include "CharacterStateManagement/CharacterStateManagerComponent.h"

/** Idle state. */
class FIdleState : public FCharacterBaseState
{
public:
	explicit FIdleState(UCharacterStateManagerComponent* InOwner);
	virtual void Enter() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Exit() override;
};

/** Walking state. */
class FWalkingState : public FCharacterBaseState
{
public:
	explicit FWalkingState(UCharacterStateManagerComponent* InOwner);
	virtual void Enter() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Exit() override;
};

/** Sliding state. */
class FSlidingState : public FCharacterBaseState
{
public:
	explicit FSlidingState(UCharacterStateManagerComponent* InOwner);
	virtual void Enter() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Exit() override;
};

/** Mid-air state (jumping/falling). */
class FMidAirState : public FCharacterBaseState
{
public:
	explicit FMidAirState(UCharacterStateManagerComponent* InOwner);
	virtual void Enter() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Exit() override;
};

/** Wall run state; only from MidAir, only to Idle/Walking. */
class FWallRunState : public FCharacterBaseState
{
public:
	explicit FWallRunState(UCharacterStateManagerComponent* InOwner);
	virtual void Enter() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Exit() override;
};

/** Sprinting state; cannot go to WallRun or Crouch. */
class FSprintingState : public FCharacterBaseState
{
public:
	explicit FSprintingState(UCharacterStateManagerComponent* InOwner);
	virtual void Enter() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Exit() override;
};

/** Crouch state. */
class FCrouchState : public FCharacterBaseState
{
public:
	explicit FCrouchState(UCharacterStateManagerComponent* InOwner);
	virtual void Enter() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Exit() override;
};

/** Grapple state; cannot transition to Sliding. */
class FGrappleState : public FCharacterBaseState
{
public:
	explicit FGrappleState(UCharacterStateManagerComponent* InOwner);
	virtual void Enter() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Exit() override;
};
