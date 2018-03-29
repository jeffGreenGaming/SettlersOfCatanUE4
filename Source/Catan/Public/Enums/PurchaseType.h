// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PurchaseType.generated.h"



UENUM()
enum class EPurchaseType : uint8
{
	Purchase_Settlement,
	Purchase_City,
	Purchase_Road,
	Purchase_DevelopmentCard
};