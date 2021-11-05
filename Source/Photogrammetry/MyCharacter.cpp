// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "InteractInterface.h"
#include "InteractableBase.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true; //Disable tik for now 

	/*Set size for collision capsule*/
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);
	
	/*Create the Camera Component*/
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(-36.56f, 1.75f, 64.0f));
	CameraComponent->bUsePawnControlRotation = true;

	TraceDistance = 300.0f;
	bInteract = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceForward_Implementation();
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
}

/*Methods for Character Movement*/
void AMyCharacter::MoveForwardBackwards(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyCharacter::MoveLeftRight(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMyCharacter::InteractPress()
{
	if (bInteract)
	{
		UE_LOG(LogTemp, Warning, TEXT("Test"));
	}
}

void AMyCharacter::TraceForward_Implementation()
{
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	//Update Loc and Rot
	GetController()->GetPlayerViewPoint(Loc, Rot);

	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * TraceDistance); //End will be 2000 units (for now)

	FCollisionQueryParams TraceParams;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	/*DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);*/

	//If hit do something
	if (bHit)
	{
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
			bInteract = false;
			OnActorInSight(FText::FromString(""));
		}

	}
	//Remove Text
	//else
	//{
	//	bInteract = false;
	//	OnActorInSight(FText::FromString(""));
	//}
}

