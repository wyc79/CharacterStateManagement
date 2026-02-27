
#include "CharacterStateManagement/CharacterStates.h"

#define LOG_STATE(StateName) UE_LOG(LogTemp, Log, TEXT("%s: %s"), *StateManager->ObjectName, TEXT(StateName))

// ---- Idle ----
FIdleState::FIdleState(UCharacterStateManagerComponent* InOwner)
	: FCharacterBaseState(InOwner, ECharacterState::Idle)
{
}

void FIdleState::Enter()
{
	LOG_STATE("Entering Idle State");
	// StateManager->ResetAnimTrigger(FName("IdleTrigger"));
	// StateManager->SetAnimTrigger(FName("IdleTrigger"));
}

void FIdleState::Tick(float DeltaTime)
{
}

void FIdleState::Exit()
{
	LOG_STATE("Exiting Idle State");
	// StateManager->ResetAnimTrigger(FName("IdleTrigger"));
}

// ---- Walking ----
FWalkingState::FWalkingState(UCharacterStateManagerComponent* InOwner)
	: FCharacterBaseState(InOwner, ECharacterState::Walking)
{
}

void FWalkingState::Enter()
{
	LOG_STATE("Entering Walking State");
	// StateManager->ResetAnimTrigger(FName("WalkingTrigger"));
	// StateManager->SetAnimTrigger(FName("WalkingTrigger"));
}

void FWalkingState::Tick(float DeltaTime)
{
}

void FWalkingState::Exit()
{
	LOG_STATE("Exiting Walking State");
	// StateManager->ResetAnimTrigger(FName("WalkingTrigger"));
}

// ---- Sliding ----
FSlidingState::FSlidingState(UCharacterStateManagerComponent* InOwner)
	: FCharacterBaseState(InOwner, ECharacterState::Sliding)
{
}

void FSlidingState::Enter()
{
	LOG_STATE("Entering Sliding State");
	// StateManager->ResetAnimTrigger(FName("SlidingTrigger"));
	// StateManager->SetAnimTrigger(FName("SlidingTrigger"));
}

void FSlidingState::Tick(float DeltaTime)
{
}

void FSlidingState::Exit()
{
	LOG_STATE("Exiting Sliding State");
	// StateManager->ResetAnimTrigger(FName("SlidingTrigger"));
}

// ---- MidAir ----
FMidAirState::FMidAirState(UCharacterStateManagerComponent* InOwner)
	: FCharacterBaseState(InOwner, ECharacterState::MidAir)
{
}

void FMidAirState::Enter()
{
	LOG_STATE("Entering MidAir State");
	// StateManager->ResetAnimTrigger(FName("MidAirTrigger"));
	// StateManager->SetAnimTrigger(FName("MidAirTrigger"));
}

void FMidAirState::Tick(float DeltaTime)
{
	if (StateManager->IsGrounded())
	{
		StateManager->SwitchToNormalState();
	}
}

void FMidAirState::Exit()
{
	LOG_STATE("Exiting MidAir State");
	// StateManager->ResetAnimTrigger(FName("MidAirTrigger"));
}

// ---- WallRun ----
FWallRunState::FWallRunState(UCharacterStateManagerComponent* InOwner)
	: FCharacterBaseState(InOwner, ECharacterState::WallRun)
{
}

void FWallRunState::Enter()
{
	LOG_STATE("Entering WallRun State");
	// StateManager->ResetAnimTrigger(FName("WallRunTrigger"));
	// StateManager->SetAnimTrigger(FName("WallRunTrigger"));
}

void FWallRunState::Tick(float DeltaTime)
{
}

void FWallRunState::Exit()
{
	LOG_STATE("Exiting WallRun State");
	// StateManager->ResetAnimTrigger(FName("WallRunTrigger"));
}

// ---- Sprinting ----
FSprintingState::FSprintingState(UCharacterStateManagerComponent* InOwner)
	: FCharacterBaseState(InOwner, ECharacterState::Sprinting)
{
}

void FSprintingState::Enter()
{
	LOG_STATE("Entering Sprinting State");
	// StateManager->ResetAnimTrigger(FName("SprintingTrigger"));
	// StateManager->SetAnimTrigger(FName("SprintingTrigger"));
}

void FSprintingState::Tick(float DeltaTime)
{
	const float HorizontalSpeed = StateManager->GetLinearVelocity().Size2D();
	if (HorizontalSpeed < StateManager->SprintingMinSpeed)
	{
		StateManager->SwitchStateByEnum(ECharacterState::Walking);
	}
}

void FSprintingState::Exit()
{
	LOG_STATE("Exiting Sprinting State");
	// StateManager->ResetAnimTrigger(FName("SprintingTrigger"));
}

// ---- Crouch ----
FCrouchState::FCrouchState(UCharacterStateManagerComponent* InOwner)
	: FCharacterBaseState(InOwner, ECharacterState::Crouch)
{
}

void FCrouchState::Enter()
{
	LOG_STATE("Entering Crouch State");
	StateManager->UpdateCapsuleHalfHeight(StateManager->GetCrouchCapsuleHalfHeight());
	// StateManager->ResetAnimTrigger(FName("CrouchTrigger"));
	// StateManager->SetAnimTrigger(FName("CrouchTrigger"));
}

void FCrouchState::Tick(float DeltaTime)
{
}

void FCrouchState::Exit()
{
	LOG_STATE("Exiting Crouch State");
	const float DefaultHalfHeight = StateManager->GetDefaultCapsuleHalfHeight();
	if (DefaultHalfHeight > 0.f)
	{
		StateManager->UpdateCapsuleHalfHeight(DefaultHalfHeight);
	}
	// StateManager->ResetAnimTrigger(FName("CrouchTrigger"));
}

// ---- Grapple ----
FGrappleState::FGrappleState(UCharacterStateManagerComponent* InOwner)
	: FCharacterBaseState(InOwner, ECharacterState::Grapple)
{
}

void FGrappleState::Enter()
{
	LOG_STATE("Entering Grapple State");
	// StateManager->ResetAnimTrigger(FName("GrappleTrigger"));
	// StateManager->SetAnimTrigger(FName("GrappleTrigger"));
}

void FGrappleState::Tick(float DeltaTime)
{
}

void FGrappleState::Exit()
{
	LOG_STATE("Exiting Grapple State");
	// StateManager->ResetAnimTrigger(FName("GrappleTrigger"));
}

#undef LOG_STATE
