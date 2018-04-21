// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "CatanPlayerController.h"
#include "RoadBuildingCard.h"


ARoadBuildingCard::ARoadBuildingCard() {
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("/Game/Content/Meshes/RoadBuildingCard.RoadBuildingCard"));
	Mesh->SetStaticMesh(MeshRef.Object);
	Mesh->OnClicked.AddDynamic(this, &ARoadBuildingCard::use);
}

void ARoadBuildingCard::use(UPrimitiveComponent* pComponent) {
	ACatanPlayerController * controller = dynamic_cast<ACatanPlayerController*>(GetOwner());
	UClass* roadBuildingUI = LoadObject<UClass>(nullptr, TEXT("/Game/Content/Blueprints/UI/RoadBuildingUI.RoadBuildingUI_C"));
	controller->setHUD(roadBuildingUI);
	Super::use(pComponent);
}