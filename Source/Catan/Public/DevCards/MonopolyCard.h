// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DevelopmentCard.h"
#include "MonopolyCard.generated.h"

/**
 * 
 */
UCLASS()
class CATAN_API AMonopolyCard : public ADevelopmentCard
{
	GENERATED_BODY()
	
public:
	AMonopolyCard();

	UFUNCTION()
	virtual void use(UPrimitiveComponent* pComponent) override;
	
};
