// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "InteractInterface.h"
#include "InteractableBase.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;

	/*Set size for collision capsule*/
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);

	/*Create the Camera Component*/
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(-36.56f, 1.75f, 64.0f));
	CameraComponent->bUsePawnControlRotation = true;

	HoldingComponent = CreateDefaultSubobject<USceneComponent>("HoldingComponent");
	HoldingComponent->SetupAttachment(CameraComponent);

	TraceDistance = 200.0f;
	bInteract = false;
	CurrentItem = NULL;
	bCanMove = true;
	bInspecting = false;
	bHoldingItem = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Get Cameras Y(InPitch) Rotation
	PitchMax = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax;
	PitchMin = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin;

}
// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//TraceForward_Implementation();

	//Line tracing implementation
	Start = CameraComponent->GetComponentLocation();
	ForwardVector = CameraComponent->GetForwardVector();
	End = ((ForwardVector * TraceDistance) + Start);

	if (!bHoldingItem)
	{
		//Activate line trace
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultReponseParams))
		{
			//If we hit a pick-up actor
			if (Hit.GetActor()->GetClass()->IsChildOf(APickupActor::StaticClass()))
			{
				CurrentItem = Cast<APickupActor>(Hit.GetActor());
			}
		}
		else
		{
			CurrentItem = NULL;
		}
	}
	


	if (bInspecting)
	{
		if (bHoldingItem)
		{
			
			//Set back the camera to its original 90 degrees using lerp (Allows for smooth transitions)
			CameraComponent->SetFieldOfView(FMath::Lerp(CameraComponent->FieldOfView, 90.0f, 0.1f));
			HoldingComponent->SetRelativeLocation(FVector(300.0f, 0.0f, 50.0f)); //when inspecting.
			
			//Change the pictch max&min to allow for 360 rotation of the actor
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.90000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -179.90000002f;
			CurrentItem->RotateActor();
		}
		//if not holding item zoom out camera again
		else
		{
			CameraComponent->SetFieldOfView(FMath::Lerp(CameraComponent->FieldOfView, 45.0f, 0.1f));
		}
	}
	else
	{
		CameraComponent->SetFieldOfView(FMath::Lerp(CameraComponent->FieldOfView, 90.0f, 0.1f));
		//if not ispecting but holding reset its position
		if (bHoldingItem)
		{
			
			HoldingComponent->SetRelativeLocation(FVector(150.0f, 0.0f, 0.0f)); //will adjust these in the future.
			HoldingComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f)); //Rotate when holding.
		}
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Axis Mappings (Turn-LookUp)
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	//Axis Mappings (Move Left-Right Forward-Backwards)
	PlayerInputComponent->BindAxis("Move", this, &AMyCharacter::MoveForwardBackwards);
	PlayerInputComponent->BindAxis("Strafe", this, &AMyCharacter::MoveLeftRight);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMyCharacter::InteractPress);
	PlayerInputComponent->BindAction("Inspect", IE_Pressed, this, &AMyCharacter::OnInspect);
	PlayerInputComponent->BindAction("Inspect", IE_Released, this, &AMyCharacter::OnInspectRealeased);

}

/*Methods for Character Movement*/
void AMyCharacter::MoveForwardBackwards(float Value)
{
	//If not inspecting move
	if (Value != 0 && bCanMove)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyCharacter::MoveLeftRight(float Value)
{
	//If not inspecting move
	if (Value != 0 && bCanMove)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}


void AMyCharacter::InteractPress()
{
	if (CurrentItem!=NULL && !bInspecting)
	{
		ToggleItemPickUp();
	}
}

void AMyCharacter::OnInspect()
{
	if (bHoldingItem)
	{
		//When you hold the actor save the players rotation to restore after
		LastRotation = GetControlRotation();
		ToggleMovement();

	}
	else
	{
<<<<<<< HEAD
		bInspecting = true;
	}
}
=======
		AActor* HitActor = Hit.GetActor();
		if (HitActor->GetClass()->IsChildOf(AInteractableBase::StaticClass()))
		{
			/*UE_LOG(LogTemp, Warning, TEXT("Test"));*/
			DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
			AInteractableBase* InteractRef = Cast<AInteractableBase>(HitActor);
			if (IsValid(InteractRef))
			{
				bInteract = true;
				FString ActorText = InteractRef->GetText();
				OnActorInSight(FText::FromString(ActorText));
			}
		}
		else
		{
			//Remove Text and make it not be able to interact
			bInteract = false;
			OnActorInSight(FText::FromString(""));
		}
<<<<<<< Updated upstream
=======
>>>>>>> 234e328f5495ca787bf2df292b5f1c53e6356f16
>>>>>>> Stashed changes

void AMyCharacter::OnInspectRealeased()
{
	if (bInspecting && bHoldingItem)
	{
		//Reset rotation of character and cameras
		GetController()->SetControlRotation(LastRotation);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = PitchMax;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = PitchMin;
		ToggleMovement();
	}
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
	else
	{
		bInspecting = false;
	}
>>>>>>> Stashed changes
}

void AMyCharacter::ToggleItemPickUp()
{
	if (CurrentItem)
	{
		bHoldingItem = !bHoldingItem;
		//either bring it to use or throw it
		CurrentItem->Pickup();
		if (!bHoldingItem)
		{
			CurrentItem = NULL;
		}
	}
=======
>>>>>>> 234e328f5495ca787bf2df292b5f1c53e6356f16
}

void AMyCharacter::ToggleMovement()
{
	bCanMove = !bCanMove;
	bInspecting = !bInspecting;
	CameraComponent->bUsePawnControlRotation = !CameraComponent->bUsePawnControlRotation;
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
}

//void AMyCharacter::TraceForward_Implementation()
//{
//	//Player rotation and location
//	FVector Loc;
//	FRotator Rot;
//	FHitResult Hit;
//
//	//Update Loc and Rot
//	GetController()->GetPlayerViewPoint(Loc, Rot);
//
//	FVector Start = Loc;
//	FVector End = Start + (Rot.Vector() * TraceDistance); //End will be 2000 units (for now)
//
//	FCollisionQueryParams TraceParams;
//
//	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
//
//	/*DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);*/
//
//	//If hit do something
//	if (bHit)
//	{
//		AActor* HitActor = Hit.GetActor();
//		if (HitActor->GetClass()->IsChildOf(AInteractableBase::StaticClass()))//hit an interactable actor
//		{
//			/*UE_LOG(LogTemp, Warning, TEXT("Test"));*/
//			DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f); //used for debuggin (remove later)
//			AInteractableBase* InteractRef = Cast<AInteractableBase>(HitActor);
//			if (IsValid(InteractRef))
//			{
//				bInteract = true;
//				FString ActorText = InteractRef->GetText();
//				OnActorInSight(FText::FromString(ActorText));
//			}
//		}
//	}
//	//Remove Text
//	else
//	{
//		bInteract = false;
//		OnActorInSight(FText::FromString(""));
//	}
//}

