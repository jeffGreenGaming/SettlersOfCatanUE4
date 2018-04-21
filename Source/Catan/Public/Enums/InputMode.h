// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/UserDefinedEnum.h"
#include "InputMode.generated.h"

UENUM()
enum class EInputMode : uint8
{
	InputMode_Main,
	InputMode_Purchase
};
