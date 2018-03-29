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

private:
	/** Class of user widget, loaded from Content Browser */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCatanWidget> WidgetClass;

	/* Reference to created user widget*/
	UPROPERTY(EditAnywhere)
	class UCatanWidget *Widget;
};
