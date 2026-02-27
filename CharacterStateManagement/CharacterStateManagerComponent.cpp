
#include "CharacterStateManagement/CharacterStateManagerComponent.h"
#include "CharacterStateManagement/CharacterStates.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Engine/Engine.h"

UCharacterStateManagerComponent::UCharacterStateManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCharacterStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* Mesh = Char->GetMesh())
		{
			MeshComponent = Mesh;
			AnimInstance = Mesh->GetAnimInstance();
		}
	}

	SetupIllegalTransitions();
	CreateStates();

	CurrentState = IdleState;
	CurrentStateEnum = ECharacterState::Idle;
	if (CurrentState)
	{
		CurrentState->Enter();
	}
}

void UCharacterStateManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyStates();
	Super::EndPlay(EndPlayReason);
}

void UCharacterStateManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If not in an air state and not grounded, switch to MidAir
	if (!AirStates.Contains(CurrentStateEnum) && !IsGrounded())
	{
		SwitchState(MidAirState);
	}

	if (CurrentState)
	{
		CurrentState->Tick(DeltaTime);
		CurrentStateEnum = CurrentState->GetState();
	}
}


// note: always check with transition matrix in Combat Design General Notion Page
// https://www.notion.so/Combat-Design-General-2f14c4ee0575801a82bbc0c8a4f9afe8?source=copy_link
void UCharacterStateManagerComponent::SetupIllegalTransitions()
{
	// Called once from BeginPlay(); transition rules are fixed for the lifetime of the component.
	IllegalTransitions.Empty();

	// From Idle: cannot go to WallRun
	IllegalTransitions.Add(ECharacterState::Idle, { ECharacterState::WallRun });

	// From Walking: cannot go to WallRun
	IllegalTransitions.Add(ECharacterState::Walking, { ECharacterState::WallRun });

	// From Sliding: cannot go to WallRun, Crouch
	IllegalTransitions.Add(ECharacterState::Sliding, { ECharacterState::WallRun, ECharacterState::Crouch });

	// From MidAir: cannot go to Sliding, Sprinting, Crouch
	IllegalTransitions.Add(ECharacterState::MidAir, { ECharacterState::Sliding, ECharacterState::Sprinting, ECharacterState::Crouch });

	// From WallRun: cannot go to Sliding, Sprinting, Crouch
	IllegalTransitions.Add(ECharacterState::WallRun, { ECharacterState::Sliding, ECharacterState::Sprinting, ECharacterState::Crouch });

	// From Sprinting: cannot go to WallRun, Crouch (slide instead)
	IllegalTransitions.Add(ECharacterState::Sprinting, { ECharacterState::WallRun, ECharacterState::Crouch });

	// From Crouch: cannot go to WallRun
	IllegalTransitions.Add(ECharacterState::Crouch, { ECharacterState::WallRun });

	// From Grapple: cannot go to Sliding, Sprinting, Crouch
	IllegalTransitions.Add(ECharacterState::Grapple, { ECharacterState::Sliding, ECharacterState::Sprinting, ECharacterState::Crouch });

	// setup state categories
	NormalStates = { ECharacterState::Idle, ECharacterState::Walking };
	AirStates = { ECharacterState::MidAir, ECharacterState::WallRun, ECharacterState::Grapple };
	GroundStates = { ECharacterState::Idle, ECharacterState::Walking, ECharacterState::Sliding, ECharacterState::Sprinting, ECharacterState::Crouch };
}

void UCharacterStateManagerComponent::CreateStates()
{
	DestroyStates();
	IdleState = new FIdleState(this);
	WalkingState = new FWalkingState(this);
	SlidingState = new FSlidingState(this);
	MidAirState = new FMidAirState(this);
	WallRunState = new FWallRunState(this);
	SprintingState = new FSprintingState(this);
	CrouchState = new FCrouchState(this);
	GrappleState = new FGrappleState(this);
}

void UCharacterStateManagerComponent::DestroyStates()
{
	auto DeleteState = [](FCharacterBaseState*& S) { delete S; S = nullptr; };
	DeleteState(IdleState);
	DeleteState(WalkingState);
	DeleteState(SlidingState);
	DeleteState(MidAirState);
	DeleteState(WallRunState);
	DeleteState(SprintingState);
	DeleteState(CrouchState);
	DeleteState(GrappleState);
	if (CurrentState) { CurrentState = nullptr; }
}

void UCharacterStateManagerComponent::SwitchState(FCharacterBaseState* NewState)
{
	if (!NewState) return;

	if (const TSet<ECharacterState>* IllegalSet = IllegalTransitions.Find(CurrentStateEnum))
	{
		if (IllegalSet->Contains(NewState->GetState()))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Invalid transition to state %d"), *ObjectName, static_cast<int32>(NewState->GetState()));
			return;
		}
	}

	const ECharacterState NewStateEnum = NewState->GetState();
	UE_LOG(LogTemp, Log, TEXT("%s: Transitioning to state %d"), *ObjectName, static_cast<int32>(NewStateEnum));

	// On-screen print (like Blueprint Print String)
	if (GEngine)
	{
		static const TCHAR* StateNames[] = { TEXT("Idle"), TEXT("Walking"), TEXT("Sliding"), TEXT("MidAir"), TEXT("WallRun"), TEXT("Sprinting"), TEXT("Crouch"), TEXT("Grapple") };
		const int32 Idx = static_cast<int32>(NewStateEnum);
		const FString Msg = ObjectName.IsEmpty() ? FString(StateNames[Idx]) : FString::Printf(TEXT("%s: %s"), *ObjectName, StateNames[Idx]);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::White, Msg);
	}

	if (CurrentState)
	{
		CurrentState->Exit();
	}
	CurrentState = NewState;
	CurrentStateEnum = NewState->GetState();
	CurrentState->Enter();
}

bool UCharacterStateManagerComponent::SwitchStateByEnum(ECharacterState NewState)
{
	if (CurrentStateEnum == NewState)
	{
		return false;
	}
	FCharacterBaseState* StatePtr = nullptr;
	switch (NewState)
	{
		case ECharacterState::Idle:      StatePtr = IdleState; break;
		case ECharacterState::Walking:   StatePtr = WalkingState; break;
		case ECharacterState::Sliding:   StatePtr = SlidingState; break;
		case ECharacterState::MidAir:    StatePtr = MidAirState; break;
		case ECharacterState::WallRun:   StatePtr = WallRunState; break;
		case ECharacterState::Sprinting: StatePtr = SprintingState; break;
		case ECharacterState::Crouch:    StatePtr = CrouchState; break;
		case ECharacterState::Grapple:   StatePtr = GrappleState; break;
	}
	if (!StatePtr)
	{
		return false;
	}
	if (const TSet<ECharacterState>* IllegalSet = IllegalTransitions.Find(CurrentStateEnum))
	{
		if (IllegalSet->Contains(NewState))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Invalid transition to state %d"), *ObjectName, static_cast<int32>(NewState));
			return false;
		}
	}
	SwitchState(StatePtr);
	return true;
}

void UCharacterStateManagerComponent::SetAnimInterface(USkeletalMeshComponent* InMesh, UAnimInstance* InAnimInstance)
{
	MeshComponent = InMesh;
	AnimInstance = InAnimInstance;
}

void UCharacterStateManagerComponent::SwitchToNormalState()
{
	const FVector Vel = GetLinearVelocity();
	const float HorizontalSpeed = Vel.Size2D();
	if (HorizontalSpeed >= NormalStateWalkThreshold)
	{
		SwitchState(WalkingState);
	}
	else
	{
		SwitchState(IdleState);
	}
}

bool UCharacterStateManagerComponent::IsGrounded() const
{
	ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (UCharacterMovementComponent* Move = Char ? Char->GetCharacterMovement() : nullptr)
	{
		return Move->IsMovingOnGround();
	}
	return false;
}

FVector UCharacterStateManagerComponent::GetLinearVelocity() const
{
	ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (UCharacterMovementComponent* Move = Char ? Char->GetCharacterMovement() : nullptr)
	{
		return Move->Velocity;
	}
	return FVector::ZeroVector;
}

void UCharacterStateManagerComponent::SetLinearVelocity(FVector Velocity)
{
	ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (UCharacterMovementComponent* Move = Char ? Char->GetCharacterMovement() : nullptr)
	{
		Move->Velocity = Velocity;
	}
}

void UCharacterStateManagerComponent::SetAnimTrigger(const FName& TriggerName)
{
	// Override in subclass or wire to AnimBP (e.g. AnimInstance SetTrigger).
}

void UCharacterStateManagerComponent::ResetAnimTrigger(const FName& TriggerName)
{
	// Override in subclass or wire to AnimBP.
}
