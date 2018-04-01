// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "VictoryPointCard.h"


AVictoryPointCard::AVictoryPointCard() {
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("/Game/Content/Meshes/VictoryPointCard.VictoryPointCard"));
	Mesh->SetStaticMesh(MeshRef.Object);
}
