// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ResourceType.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	ResourceType_Stone,
	ResourceType_Wheat,
	ResourceType_Wood,
	ResourceType_Sheep,
	ResourceType_Brick,
	ResourceType_Desert,
	ResourceType_None // needed as default value
};