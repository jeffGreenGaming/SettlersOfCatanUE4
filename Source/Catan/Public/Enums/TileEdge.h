// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileEdge.generated.h"

UENUM()
enum class ETileEdge : uint8
{
	TileEdge_Top = 0,
	TileEdge_TopRight = 1,
	TileEdge_BottomRight = 2,
	TileEdge_Bottom = 3,
	TileEdge_BottomLeft = 4,
	TileEdge_TopLeft = 5

};