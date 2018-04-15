// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "CatanHUD.h"

ACatanHUD::ACatanHUD() {

	static ConstructorHelpers::FClassFinder<UCatanWidget> WidgetAsset(TEXT("/Game/Content/Blueprints/UI/PlacementSettlementUI"));
	static ConstructorHelpers::FClassFinder<UCatanWidget> TradeOverlayAsset(TEXT("/Game/Content/Blueprints/UI/TradeOverlay"));
	static ConstructorHelpers::FClassFinder<UCatanWidget> TradeOfferOverlayAsset(TEXT("/Game/Content/Blueprints/UI/TradeOfferOverlay"));

	if (WidgetAsset.Succeeded())
	{
		WidgetClass = WidgetAsset.Class;
	}

	if (TradeOverlayAsset.Succeeded())
	{
		TradeOverlayClass = TradeOverlayAsset.Class;
	}

	if (TradeOfferOverlayAsset.Succeeded())
	{
		TradeOfferOverlayClass = TradeOfferOverlayAsset.Class;
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

void ACatanHUD::spawnTradeOverlay() {

	removeTradeOverlay();

	TradeOverlayWidget = CreateWidget<UCatanWidget>(GetWorld(), TradeOverlayClass);
	if (TradeOverlayWidget)
	{
		TradeOverlayWidget->AddToViewport();
	}

}

void ACatanHUD::removeTradeOverlay() {

	if (TradeOverlayWidget != nullptr) {
		TradeOverlayWidget->RemoveFromParent();
	}

}


void ACatanHUD::spawnTradeOfferOverlay() {

	removeTradeOfferOverlay();

	TradeOfferOverlayWidget = CreateWidget<UCatanWidget>(GetWorld(), TradeOfferOverlayClass);
	if (TradeOfferOverlayWidget)
	{
		TradeOfferOverlayWidget->AddToViewport();
	}

}

void ACatanHUD::removeTradeOfferOverlay() {

	if (TradeOfferOverlayWidget != nullptr) {
		TradeOfferOverlayWidget->RemoveFromParent();
	}

}

