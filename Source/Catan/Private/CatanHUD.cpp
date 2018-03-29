// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "CatanHUD.h"

ACatanHUD::ACatanHUD() {

	static ConstructorHelpers::FClassFinder<UCatanWidget> WidgetAsset(TEXT("/Game/Content/Blueprints/UI/MainUI"));
	if (WidgetAsset.Succeeded())
	{
		WidgetClass = WidgetAsset.Class;
	}

}
 
void ACatanHUD::DrawHUD()
{
	Super::DrawHUD();

}

void ACatanHUD::BeginPlay() {
	setHUDClass(WidgetClass);
}

void ACatanHUD::setHUDClass(TSubclassOf<class UCatanWidget> newHUD) {

	if (newHUD)
	{
		if (Widget != nullptr) {
			Widget->RemoveFromParent();
		}
		Widget = CreateWidget<UCatanWidget>(GetWorld(), newHUD);
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}

}