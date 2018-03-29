// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Vertex.generated.h"

UENUM()
enum class EVertex : uint8
{
	Vertex_TopRight = 0,
	Vertex_MiddleRight = 1,
	Vertex_BottomRight = 2,
	Vertex_BottomLeft = 3,
	Vertex_MiddleLeft = 4,
	Vertex_TopLeft = 5
};