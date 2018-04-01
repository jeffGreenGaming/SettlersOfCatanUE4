// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "KnightCard.h"

AKnightCard::AKnightCard() {
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("/Game/Content/Meshes/KnightCard.KnightCard"));
	Mesh->SetStaticMesh(MeshRef.Object);
}


