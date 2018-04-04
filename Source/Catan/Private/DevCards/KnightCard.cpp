// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "CatanPlayerController.h"
#include "KnightCard.h"

AKnightCard::AKnightCard() {
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("/Game/Content/Meshes/KnightCard.KnightCard"));
	Mesh->SetStaticMesh(MeshRef.Object);
	Mesh->OnClicked.AddDynamic(this, &AKnightCard::use);
}


void AKnightCard::use(UPrimitiveComponent* pComponent) {
	ACatanPlayerController * controller = dynamic_cast<ACatanPlayerController*>(GetOwner());
	UClass* knightUI = LoadObject<UClass>(nullptr, TEXT("/Game/Content/Blueprints/UI/KnightUI.KnightUI_C"));
	controller->setHUD(knightUI);
	Super::use(pComponent);
}