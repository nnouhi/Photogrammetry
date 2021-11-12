// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PickupActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyCharacter.generated.h"

UCLASS()
class PHOTOGRAMMETRY_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	/*First Person Camera*/
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	//Will act as a dummy for pickup actor location,rotation
	UPROPERTY(EditAnywhere)
	class USceneComponent* HoldingComponent;

	UPROPERTY(EditAnywhere)
	class APickupActor* CurrentItem;

	bool bCanMove;
	bool bHoldingItem;
	bool bInspecting;

	//will relate to camera rotation
	float PitchMax;
	float PitchMin;

	FVector HoldingCompVector;
	FRotator LastRotation;

	//for line tract
	FVector Start;
	FVector ForwardVector;
	FVector End;
	FHitResult Hit;
	FComponentQueryParams DefaultComponentQueryParams;
	FCollisionResponseParams DefaultReponseParams;
	float TraceDistance;
	bool bInteract;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*void TraceForward_Implementation();*/

	UFUNCTION(BlueprintImplementableEvent, Category = "InteractableHUD")
		void OnActorInSight(const FText& Text);

	/*Methods for Character Movement*/
	UFUNCTION()
	void MoveForwardBackwards(float Value);
	UFUNCTION()
	void MoveLeftRight(float Value);
	//we dont want our character to move when he is inspecting an actor
	UFUNCTION()
	void ToggleMovement();


	/*Methos for interacting with pick-up actor*/
	UFUNCTION()
	void InteractPress();
	UFUNCTION()
	void OnInspect(); 
	UFUNCTION()
	void OnInspectRealeased();
	UFUNCTION()
	void ToggleItemPickUp();

	//

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
