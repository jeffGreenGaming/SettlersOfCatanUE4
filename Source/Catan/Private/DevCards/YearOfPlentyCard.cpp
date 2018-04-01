// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "YearOfPlentyCard.h"


AYearOfPlentyCard::AYearOfPlentyCard() {
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("/Game/Content/Meshes/YearOfPlentyCard.YearOfPlentyCard"));
	Mesh->SetStaticMesh(MeshRef.Object);
}

