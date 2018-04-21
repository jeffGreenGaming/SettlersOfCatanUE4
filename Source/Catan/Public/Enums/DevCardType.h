// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DevCardType.generated.h"

UENUM()
enum class EDevCardType : uint8
{
	DevCardType_VictoryPoints,
	DevCardType_Knight,
	DevCardType_RoadBuilding,
	DevCardType_YearOfPlenty,
	DevCardType_Monopoly
};