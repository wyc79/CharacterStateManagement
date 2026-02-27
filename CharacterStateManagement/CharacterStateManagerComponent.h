
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStateManagement/CharacterStateEnum.h"
#include "CharacterStateManagerComponent.generated.h"

class FCharacterBaseState;
class USkeletalMeshComponent;
class UAnimInstance;

/** Manages character state machine: Idle, Walking, Sliding, MidAir, WallRun, Sprinting, Crouch, Grapple. */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CD_TEMP_API UCharacterStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterStateManagerComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Current state enum (for Blueprint/query). */
	UPROPERTY(BlueprintReadOnly, Category = "State")
	ECharacterState CurrentStateEnum = ECharacterState::Idle;

	/** Switch to a new state by pointer (C++ only); respects illegal transitions. */
	void SwitchState(FCharacterBaseState* NewState);

	/** Switch to a new state by enum (Blueprint-callable). Returns true if the switch was performed, false if the transition is illegal or state not found. */
	UFUNCTION(BlueprintCallable, Category = "State", meta = (DisplayName = "Switch State By Enum"))
	bool SwitchStateByEnum(ECharacterState NewState);

	/** Optional display name for logging. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FString ObjectName;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	TObjectPtr<USkeletalMeshComponent> MeshComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;

	/** Set from character to enable animation triggers. */
	void SetAnimInterface(USkeletalMeshComponent* InMesh, UAnimInstance* InAnimInstance);

	// ---- State instances (owned by this component) ----
	FCharacterBaseState* IdleState = nullptr;
	FCharacterBaseState* WalkingState = nullptr;
	FCharacterBaseState* SlidingState = nullptr;
	FCharacterBaseState* MidAirState = nullptr;
	FCharacterBaseState* WallRunState = nullptr;
	FCharacterBaseState* SprintingState = nullptr;
	FCharacterBaseState* CrouchState = nullptr;
	FCharacterBaseState* GrappleState = nullptr;

	/** Normal grounded states (Idle/Walking). */
	UPROPERTY(EditDefaultsOnly, Category = "State")
	TSet<ECharacterState> NormalStates;

	/** Air states: MidAir, WallRun, Grapple. If not in one of these and not grounded, we switch to MidAir. */
	UPROPERTY(EditDefaultsOnly, Category = "State")
	TSet<ECharacterState> AirStates;

	/** Ground states: Idle, Walking, Sliding, Sprinting, Crouch. */
	UPROPERTY(EditDefaultsOnly, Category = "State")
	TSet<ECharacterState> GroundStates;

	/** Horizontal speed above this threshold enters Walking; below enters Idle. Used when landing (e.g. from MidAir). */
	UPROPERTY(EditDefaultsOnly, Category = "State", meta = (ClampMin = "0"))
	float NormalStateWalkThreshold = 10.f;

	/** Horizontal speed below this threshold exits Sprinting. */
	UPROPERTY(EditDefaultsOnly, Category = "State", meta = (ClampMin = "0"))
	float SprintingMinSpeed = 600.f;

	/** Target capsule half-height while crouched. */
	UPROPERTY(EditDefaultsOnly, Category = "State", meta = (ClampMin = "0"))
	float CrouchCapsuleHalfHeight = 44.f;

	/** Cached default capsule half-height from BeginPlay. */
	UPROPERTY(VisibleInstanceOnly, Category = "State")
	float DefaultCapsuleHalfHeight = 0.f;

	/** Switch to Idle or Walking based on current horizontal speed (e.g. call when landing from MidAir). */
	void SwitchToNormalState();

	/** Illegal transitions: current state -> set of states we cannot transition to. */
	TMap<ECharacterState, TSet<ECharacterState>> IllegalTransitions;

	/** Get current state object (C++ only). */
	FCharacterBaseState* GetCurrentState() const { return CurrentState; }

	/** Get current state as enum (Blueprint and C++). */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	ECharacterState GetCurrentStateEnum() const { return CurrentStateEnum; }

	/** Helpers that states may call. */
	UFUNCTION(BlueprintCallable, Category = "State")
	virtual bool IsGrounded() const;

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual FVector GetLinearVelocity() const;

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetLinearVelocity(FVector Velocity);

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetAnimTrigger(const FName& TriggerName);

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void ResetAnimTrigger(const FName& TriggerName);

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void UpdateCapsuleHalfHeight(float NewHalfHeight, bool bUpdateOverlaps = true);

	float GetDefaultCapsuleHalfHeight() const { return DefaultCapsuleHalfHeight; }
	float GetCrouchCapsuleHalfHeight() const { return CrouchCapsuleHalfHeight; }

protected:
	void SetupIllegalTransitions();
	void CreateStates();
	void DestroyStates();

	FCharacterBaseState* CurrentState = nullptr;
};
