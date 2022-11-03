// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UpTimePlayerCharacter.h"

#include "UpTime.h"
#include "Gameplay/UpTimePlayerController.h"
#include "Actors/Components/PowerSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/UpTimeGameInstance.h"
#include "Gameplay/Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
#include "Items/Weapons/Weapon.h"

AUpTimePlayerCharacter::AUpTimePlayerCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(
		TEXT("Material'/Game/UpTime/Actors/Player/Cursor_Decal_M.Cursor_Decal_M'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Initialize power system
	PowerSystemComponent = CreateDefaultSubobject<UPowerSystemComponent>(FName("PowerSystem"));
	PowerSystemComponent->SetCurrentPower(109.f);
	
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AUpTimePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PowerSystemComponent->OnPowerDrained.BindDynamic(this, &AUpTimePlayerCharacter::OnPowerDrained);
	UUpTimeGameInstance* GameInstance = Cast<UUpTimeGameInstance>(GetGameInstance());

	if (!GameInstance)
	{
		UE_LOG(LogUpTime, Error, TEXT("Could not get GameInstance on PlayerCharacter::BeginPlay"));
		return;
	}

	if (!GameInstance->IsFirstLevel())
	{
		const float StartPower = FMath::Max(GameInstance->GetPlayerPower(), 19.f); // Always start with at least 19 Power
		PowerSystemComponent->SetCurrentPower(StartPower);

		StartWeapon = GameInstance->GetPlayerWeapon();
	}
			
	if (StartWeapon)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const auto NewWeapon = GetWorld()->SpawnActor<AWeapon>(StartWeapon, SpawnInfo);
		Equip(NewWeapon);
	}
}

void AUpTimePlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AUpTimePlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUpTimePlayerCharacter::MoveRight);

	// Rotation
	PlayerInputComponent->BindAxis("LookUp", this, &AUpTimePlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AUpTimePlayerCharacter::LookRight);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AUpTimePlayerCharacter::OnInteract);
	PlayerInputComponent->BindAction("FireWeapon", IE_Pressed, this, &AUpTimePlayerCharacter::StartFiring);
	PlayerInputComponent->BindAction("FireWeapon", IE_Released, this, &AUpTimePlayerCharacter::StopFiring);
}

void AUpTimePlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AUpTimePlayerController* PlayerController = Cast<AUpTimePlayerController>(GetController());
	
	if (!PlayerController || !IsAlive())
	{
		return;
	}

	if(PlayerController->IsKeyboardInput())
	{
		if(CursorToWorld->bHiddenInGame)
		{
			CursorToWorld->SetHiddenInGame(false);
		}
		
		RotateToMousePosition(PlayerController);
	}
	else
	{
		CursorToWorld->SetHiddenInGame(true);
	}
}

void AUpTimePlayerCharacter::OnPowerDrained()
{
	UE_LOG(LogTemp, Warning, TEXT("All energy drained!"))

	Kill(GetController(), this);
}

void AUpTimePlayerCharacter::ChargePower(int ChargeValue)
{
	ensureMsgf(PowerSystemComponent, TEXT("PowerSystem not available"));
	PowerSystemComponent->ChargePower(ChargeValue);
}

void AUpTimePlayerCharacter::FocusInteractable(IInteractable* Interactable)
{
	UObject* Object = Cast<UObject>(Interactable);
	
	if(!Interactable || !Object)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not register interactable"))
		return;
	}

	FocussedInteractable.SetObject(Object);
	FocussedInteractable.SetInterface(Interactable);
}

void AUpTimePlayerCharacter::UnfocusInteractable()
{
	FocussedInteractable.SetObject(nullptr);
	FocussedInteractable.SetInterface(nullptr);
}

void AUpTimePlayerCharacter::Equip_Implementation(AWeapon* NewWeapon)
{
	if (Weapon && Weapon->GetCurrentState() == EWeaponState::Firing)
	{
		Weapon->StopFiring();
	}
	
	Weapon = NewWeapon;
	NewWeapon->OnEquipped(this);
}

void AUpTimePlayerCharacter::StartFiring_Implementation()
{
	if(!Weapon)
	{
		return;
	}

	Weapon->StartFiring();
}

void AUpTimePlayerCharacter::StopFiring_Implementation()
{
	if(!Weapon)
	{
		return;
	}

	Weapon->StopFiring();
}

bool AUpTimePlayerCharacter::CanFire_Implementation() const
{
	return IsAlive() && Weapon;
}

bool AUpTimePlayerCharacter::IsFiring_Implementation() const
{
	if(!Weapon)
	{
		return false;
	}
	
	return Weapon->GetCurrentState() == EWeaponState::Firing;
}

AWeapon* AUpTimePlayerCharacter::GetWeapon_Implementation() const
{
	return Weapon;
}

float AUpTimePlayerCharacter::GetCurrentPower()
{
	if (!PowerSystemComponent)
	{
		UE_LOG(LogUpTime, Error, TEXT("PowerSystem not available in GetCurrentPower"))
		return 0.0f;
	}
	
	return PowerSystemComponent->GetCurrentPower();
}

int AUpTimePlayerCharacter::GetCurrentPowerCells()
{
	if (!PowerSystemComponent)
	{
		UE_LOG(LogUpTime, Error, TEXT("PowerSystem not available in GetCurrentPowerCells"))
		return 0;
	}
	
	return PowerSystemComponent->GetCurrentPowerCells();
}

bool AUpTimePlayerCharacter::Kill(AController* EventInstigator,	AActor* DamageCauser)
{
	if(!Super::Kill(EventInstigator, DamageCauser))
	{
		return false;
	}

	if (Weapon && Weapon->GetCurrentState() == EWeaponState::Firing)
	{
		Weapon->StopFiring();
	}
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(!PlayerController)
	{
		return false;
	}
	
	DisableInput(PlayerController);
	return true;
}

float AUpTimePlayerCharacter::GetHitpoints() const
{
	return PowerSystemComponent->GetCurrentPower();
}

void AUpTimePlayerCharacter::SetHitpoints(float NewHitpoints)
{
	PowerSystemComponent->SetCurrentPower(NewHitpoints);
}

TArray<FVector> AUpTimePlayerCharacter::GetWeaponMuzzleLocations_Implementation(EMuzzleConfiguration MuzzleConfig)
{
	TArray<FVector> MuzzleLocations;
	USkeletalMeshComponent* MeshComponent = GetMesh();

	if(!ensureMsgf(MeshComponent, TEXT("Player Mesh is null")))
	{
		return MuzzleLocations;	
	}

	switch (MuzzleConfig)
	{
		case EMuzzleConfiguration::Single:
		{
			const FVector Center = MeshComponent->GetSocketLocation("Muzzle_C");
			MuzzleLocations.Add(Center);
			break;
		}

		case EMuzzleConfiguration::Dual:
		{			
			const FVector Left = MeshComponent->GetSocketLocation("Muzzle_L");
			MuzzleLocations.Add(Left);
			const FVector Right = MeshComponent->GetSocketLocation("Muzzle_R");
			MuzzleLocations.Add(Right);
			break;
		}

		case EMuzzleConfiguration::Triple:
		{			
			const FVector Center = MeshComponent->GetSocketLocation("Muzzle_C");
			MuzzleLocations.Add(Center);
			const FVector Left = MeshComponent->GetSocketLocation("Muzzle_L");
			MuzzleLocations.Add(Left);
			const FVector Right = MeshComponent->GetSocketLocation("Muzzle_R");
			MuzzleLocations.Add(Right);
			break;
		}
	}
	
	return MuzzleLocations;
}

void AUpTimePlayerCharacter::MoveForward(const float Value)
{
	const auto direction = FVector(1, 0, 0);
	AddMovementInput(direction, Value);
}

void AUpTimePlayerCharacter::MoveRight(const float Value)
{
	const auto direction = FVector(0, 1, 0);
	AddMovementInput(direction, Value);
}

void AUpTimePlayerCharacter::LookUp(const float Value)
{
	const float Y = GetInputAxisValue("LookRight");
	const auto Direction = FVector(Value, Y, 0);

	// Prevent actor from snapping back to his default facing when the stick is released
	if (Direction.Size() < 0.25)
	{
		return;
	}

	SetActorRotation(Direction.Rotation());
}

void AUpTimePlayerCharacter::LookRight(const float Value)
{
	const float X = GetInputAxisValue("LookUp");
	const auto Direction = FVector(X, Value, 0);

	// Prevent actor from snapping back to his default facing when the stick is released
	if (Direction.Size() < 0.25)
	{
		return;
	}

	SetActorRotation(Direction.Rotation());
}

void AUpTimePlayerCharacter::RotateToMousePosition(AUpTimePlayerController* PlayerController)
{
	FHitResult HitResult;
	if (!PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
	{
		return;
	}

	FVector TargetVector = HitResult.Location - GetActorLocation();
	TargetVector.Normalize();
	const auto TargetRotation = FRotator(0.f, TargetVector.Rotation().Yaw, 0.f);

	SetActorRotation(TargetRotation);

	if (!CursorToWorld)
	{
		return;
	}

	const FVector CursorNormal = HitResult.ImpactNormal;
	const FRotator CursorRotation = CursorNormal.Rotation();
	
	CursorToWorld->SetWorldLocation(HitResult.Location);
	CursorToWorld->SetWorldRotation(CursorRotation);
}

void AUpTimePlayerCharacter::OnInteract()
{
	if(!FocussedInteractable)
	{
		return;
	}

	IInteractable::Execute_OnInteract(FocussedInteractable.GetObject(), this);
}
