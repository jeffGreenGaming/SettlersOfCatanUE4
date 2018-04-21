// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CatanWidget.h"
#include "GameFramework/HUD.h"
#include "CatanHUD.generated.h"
/**
 * 
 */
UCLASS()
class CATAN_API ACatanHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	ACatanHUD();

	void DrawHUD() override;
	
	void BeginPlay() override;

	void setHUDClass(TSubclassOf<class UCatanWidget> newHUD);

	void spawnTradeOverlay();

	void removeTradeOverlay();

	void spawnTradeOfferOverlay();

	void removeTradeOfferOverlay();

private:
	/** Class of user widget, loaded from Content Browser */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCatanWidget> WidgetClass;

	/** Class of user widget for the trade overlay -- so we can easily spawn and despawn */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCatanWidget> TradeOverlayClass;

	UPROPERTY(EditAnywhere)
	class UCatanWidget * TradeOverlayWidget;

	/** Class of user widget for the trade offer overlay -- so we can easily spawn and despawn */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCatanWidget> TradeOfferOverlayClass;

	UPROPERTY(EditAnywhere)
	class UCatanWidget * TradeOfferOverlayWidget;

	/* Reference to created user widget*/
	UPROPERTY(EditAnywhere)
	class UCatanWidget *Widget;
};
