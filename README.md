# Character State Management (Unreal C++)

Unreal C++ gameplay state architecture (Enter/Tick/Exit) + transition guards.

## What it is
This repo contains a lightweight, component-driven state machine for character gameplay states (Idle, Walking, MidAir, Sprinting, etc.). Each state is a C++ class with `Enter()`, `Tick()`, and `Exit()` hooks, and transitions are validated against an illegal-transition matrix to prevent invalid state changes.

## Design goals
- Decouple gameplay logic from AnimBP by keeping authoritative state logic in C++.
- Avoid illegal transitions with explicit, readable guard rules.
- Make it easy to extend by adding new state classes or rules without rewiring everything.

## How to integrate (3–5 steps)
1. Add `UCharacterStateManagerComponent` to `ACharacter` subclass.
2. Ensure the component ticks (default in constructor) and call `SetAnimInterface` if want to trigger AnimBP events.
3. Populate or adjust `IllegalTransitions`, thresholds, and state categories in `SetupIllegalTransitions()`.
4. Add/override states in `CharacterStates.{h,cpp}` (or create new ones) and register them in `CreateStates()`.
5. Drive state changes from input or gameplay events via `SwitchStateByEnum(...)`.

## Example usage
```cpp
// Example: transition to Sprinting from input, guarded by rules.
StateManager->SwitchStateByEnum(ECharacterState::Sprinting);
```

## State diagram (simple view)
```
Idle <-> Walking
   \        /
    \      /
     \    /
     MidAir <-> WallRun
        \
         \-> Grapple

Walking <-> Sprinting
Walking <-> Sliding
Walking <-> Crouch
```

## Where to look
- `CharacterStateManagerComponent.{h,cpp}`: component, rules, and transitions.
- `CharacterStates.{h,cpp}`: per-state Enter/Tick/Exit logic.
