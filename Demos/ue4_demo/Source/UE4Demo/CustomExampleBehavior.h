// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <map>

#include "Windows/AllowWindowsPlatformTypes.h"
#include "mmu_types.h"
#include "mmi_constants.h"
#include "InstructionWrapper.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "BlueprintTools.h"
#include "Containers/Map.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/Vector.h"

#include "AvatarBehavior.h"



#include "CustomExampleBehavior.generated.h"


class AMMIAvatar;
class UMMISceneObject;
class ASimulationController;

/**
 * 
 */
UCLASS(ClassGroup = (MOSIM), meta = (BlueprintSpawnableComponent))
class UE4DEMO_API UCustomExampleBehavior : public UAvatarBehavior
{
	GENERATED_BODY()
	
public:
	/*** Schedule a walk instruction
	* @param avatar - An avatar to run the instruction
	* @param walkTarget - The walk destination as a reference to an AActor with a MMISceneObject component
	* @param previousInstruction - The reference to previous instruction. If not passed, this instruction will be chained to the last scheduled instruction.
	* @param duration - The duration of instruction in seconds. Forces the end condition for instruction if set to value greater than zero.
	* @param delay - The delay between current and previous instructions in seconds
	* @param velocity - The walking speed of the character
	* @param reuseEnvironment - If set to false, rebuild the scene in path planner
	* @param areaCosts - The map of navigation area names with overriden costs.
	* If reuseEnvironment parameter is set to false, overriden values will reset to defaults.
	* @return The instruction data. Can be chained to next instructions.*/
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "areaCosts"))
		UPARAM(DisplayName = "Instruction")
		UInstructionWrapper* WalkInstruction(AMMIAvatar* avatar, AActor* walkTarget,
			const TMap<TEnumAsByte<NavMeshArea>, float>& areaCosts,
			UInstructionWrapper* previousInstruction = nullptr,
			float duration = 0, float delay = 0.01f, float velocity = 1.0f,
			bool reuseEnvironment = true);

	/*** Schedule a reach instruction
	* @param avatar - An avatar to run the instruction
	* @param reachTarget - The object to reach as a reference to an AActor with a MMISceneObject component
	* @param hand - The hand to use to reach the object
	* @param previousInstruction - The reference to previous instruction. If not passed, this instruction will be chained to the last scheduled instruction.
	* @param duration - The duration of instruction in seconds. Forces the end condition for instruction if set to value greater than zero.
	* @param delay - The delay between current and previous instructions in seconds
	* @return The instruction data. Can be chained to next instructions.*/
		UFUNCTION(BlueprintCallable)
			UPARAM(DisplayName = "Instruction")
			UInstructionWrapper* ReachInstruction(AMMIAvatar* avatar, AActor* reachTarget, Hand hand,
				UInstructionWrapper* previousInstruction = nullptr, float duration = 0, float delay = 0.01f);

};
