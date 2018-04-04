// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "CatanPlayerController.h"
#include "MonopolyCard.h"




AMonopolyCard::AMonopolyCard() {
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("/Game/Content/Meshes/MonopolyCard.MonopolyCard"));
	Mesh->SetStaticMesh(MeshRef.Object);
	Mesh->OnClicked.AddDynamic(this, &AMonopolyCard::use);
}

void AMonopolyCard::use(UPrimitiveComponent* pComponent) {
	ACatanPlayerController * controller = dynamic_cast<ACatanPlayerController*>(GetOwner());
	UClass* monopolyUI = LoadObject<UClass>(nullptr, TEXT("/Game/Content/Blueprints/UI/MonopolyUI.MonopolyUI_C"));
	controller->setHUD(monopolyUI);
	Super::use(pComponent);
}