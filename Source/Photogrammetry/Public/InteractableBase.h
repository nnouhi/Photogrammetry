// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

UCLASS()
class PHOTOGRAMMETRY_API AInteractableBase : public AActor , public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Interact")
		FString InteractionText;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnInteract();

	void StartFocus();

	void EndFocus();

	virtual FString GetText() override;
};
