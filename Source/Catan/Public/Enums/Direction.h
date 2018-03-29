// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Direction.generated.h"

UENUM()
enum class EDirection : uint8
{
	Direction_Up = 1,
	Direction_Down = 2,
	Direction_Left = 3,
	Direction_Right = 4
};