// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "MonopolyCard.h"




AMonopolyCard::AMonopolyCard() {
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("/Game/Content/Meshes/MonopolyCard.MonopolyCard"));
	Mesh->SetStaticMesh(MeshRef.Object);
}