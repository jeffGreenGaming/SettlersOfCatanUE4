// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "CatanPlayerController.h"
#include "YearOfPlentyCard.h"


AYearOfPlentyCard::AYearOfPlentyCard() {
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("/Game/Content/Meshes/YearOfPlentyCard.YearOfPlentyCard"));
	Mesh->SetStaticMesh(MeshRef.Object);
	Mesh->OnClicked.AddDynamic(this, &AYearOfPlentyCard::use);
}

void AYearOfPlentyCard::use(UPrimitiveComponent* pComponent) {
	ACatanPlayerController * controller = dynamic_cast<ACatanPlayerController*>(GetOwner());
	UClass* yearOfPlentyUI = LoadObject<UClass>(nullptr, TEXT("/Game/Content/Blueprints/UI/YearOfPlentyUI.YearOfPlentyUI_C"));
	controller->setHUD(yearOfPlentyUI);
	Super::use(pComponent);
}