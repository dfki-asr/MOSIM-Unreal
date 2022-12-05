// Fill out your copyright notice in the Description page of Project Settings.

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "CustomExampleBehavior.h"
#include "MOSIM.h"
#include "SimulationController.h"
#include "UnrealSceneAccess.h"
#include "MMISceneObject.h"
#include "MMIAvatar.h"
#include "mmi_constants.h"
#include "mmu_types.h"

#include <algorithm>


#include "MMUAccess.h"

#include "Engine.h"
#include <format>
#include "CommonTools.h"

//used for culture invariant float to string conversions.
#include <boost/lexical_cast.hpp>


using namespace std;
using namespace MMIStandard;

UInstructionWrapper* UCustomExampleBehavior::WalkInstruction(
	AMMIAvatar* avatar, AActor* walkTarget,
	const TMap<TEnumAsByte<NavMeshArea>, float>& areaCosts,
	UInstructionWrapper* previousInstruction /*= nullptr*/,
	float duration /*= 0*/, float delay /*= 0.01f*/, float velocity /*= 1.0f*/,
	bool reuseEnvironment /*= true*/)
{
	if (checkNullArgument(avatar, "Avatar", "Walk Instruction") ||
		checkNullArgument(walkTarget, "Walk Target", "Walk Instruction"))
		return nullptr;

	UMMISceneObject* walkSceneObject = Cast<UMMISceneObject>(
		walkTarget->GetComponentByClass(UMMISceneObject::StaticClass()));

	if (checkNullComponent(walkSceneObject,
		TCHAR_TO_UTF8(*walkTarget->GetName()),
		"Walk Instruction"))
		return nullptr;

	MInstruction walkInstruction;

	walkInstruction.__set_ID(MMUAccess::GetNewGuid());
	walkInstruction.__set_MotionType("Locomotion/Walk");
	walkInstruction.__set_Name("Walking to target location");
	walkInstruction.__set_AvatarID(avatar->AvatarID);
	

	map<string, string> walkProperties =
	{
		{"TargetID", walkSceneObject->MSceneObject.ID},
		{"UseTargetOrientation", "false"},
		{ "Velocity", boost::lexical_cast<std::string>(velocity) },
		//Default to_string convert bool to 0 or 1
		{"ReuseEnvironment", reuseEnvironment ? "true" : "false"},
		{"PathPlanningID", SimContr->CurrentSceneID},
		{"OffsetY", "0.03"} // Hack, remove once model root is fixed
	};
	if (areaCosts.Num() > 0)
	{
		walkProperties["AreaCosts"] = TCHAR_TO_UTF8(
			*FString::JoinBy(areaCosts, TEXT(","),
				[](auto& areaCost)
				{
					FString areaNameStr = UEnum::GetValueAsString<NavMeshArea>(areaCost.Key.GetValue());
					return areaNameStr + ":" + FString::SanitizeFloat(areaCost.Value);
				}));
	}
	walkInstruction.__set_Properties(walkProperties);

	scheduleNewInstruction(avatar, walkInstruction, previousInstruction, duration, delay);
	if (duration > 0)
		log(string_format("Scheduled Walk Instruction for %s to target object %s for %.2f seconds",
			avatar->baseName.c_str(), walkSceneObject->MSceneObject.Name.c_str(), duration));
	else
		log(string_format("Scheduled Walk Instruction for %s to target object %s",
			avatar->baseName.c_str(), walkSceneObject->MSceneObject.Name.c_str()));

	return UInstructionWrapper::Create(walkInstruction);
}

UInstructionWrapper* UCustomExampleBehavior::ReachInstruction(AMMIAvatar* avatar, AActor* reachTarget, Hand hand,
	UInstructionWrapper* previousInstruction, float duration, float delay)
{
	if (checkNullArgument(avatar, "Avatar", "Reach Instruction") ||
		checkNullArgument(reachTarget, "Reach Target", "Reach Instruction"))
		return nullptr;

	UMMISceneObject* reachSceneObject = Cast<UMMISceneObject>(
		reachTarget->GetComponentByClass(UMMISceneObject::StaticClass()));

	if (checkNullComponent(reachSceneObject,
		TCHAR_TO_UTF8(*reachTarget->GetName()),
		"Reach Instruction"))
		return nullptr;

	MInstruction reachInstruction;
	reachInstruction.__set_ID(MMUAccess::GetNewGuid());
	reachInstruction.__set_MotionType("Pose/Reach");
	reachInstruction.__set_Name("Reaching target object.");
	reachInstruction.__set_AvatarID(avatar->AvatarID);

	string handStr = TCHAR_TO_UTF8(*UEnum::GetValueAsString<Hand>(hand));

	map<string, string> reachProperties =
	{
		{"TargetID", reachSceneObject->MSceneObject.ID},
		{"Hand", handStr}
	};
	reachInstruction.__set_Properties(reachProperties);

	scheduleNewInstruction(avatar, reachInstruction, previousInstruction, duration, delay);

	if (duration > 0)
		log(string_format("Scheduled Reach Instruction for %s with target object %s and %s hand for %.2f seconds",
			avatar->baseName.c_str(), reachSceneObject->MSceneObject.Name.c_str(), handStr.c_str(), duration));
	else
		log(string_format("Scheduled Reach Instruction for %s with target object %s and %s hand",
			avatar->baseName.c_str(), reachSceneObject->MSceneObject.Name.c_str(), handStr.c_str()));

	return UInstructionWrapper::Create(reachInstruction);
}