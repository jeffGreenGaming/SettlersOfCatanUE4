// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Port.generated.h"

UENUM()
enum class EPort : uint8
{
	Port_Brick,
	Port_Sheep,
	Port_Stone,
	Port_Wheat,
	Port_Wood,
	Port_Any
};
