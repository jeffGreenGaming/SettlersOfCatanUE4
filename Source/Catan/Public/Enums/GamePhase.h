// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GamePhase.generated.h"

UENUM()
enum class EGamePhase : uint8
{
	GamePhase_Placement1 = 1,
	GamePhase_Placement2 = 2,
	GamePhase_MainGame = 3
};
