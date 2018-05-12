// Copyright Pedro Henriques 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MaxUseDistance = 100;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	Controller = GetWorld()->GetFirstPlayerController();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Object %s is missing a PhysicsHandle Component"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Object %s is missing a Input Component"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"), *GetOwner()->GetName());
	auto HitResult = GetUsableInView();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ActorHit->GetActorLocation(), true);
		//PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(),	ComponentToGrab->GetOwner()->GetActorRotation());
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released!"), *GetOwner()->GetName());
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PhysicsHandle->GrabbedComponent)
	{
		FVector CamLoc;
		FRotator CamRot;
		Controller->GetPlayerViewPoint(CamLoc, CamRot);
		PhysicsHandle->SetTargetLocation(CamLoc + (CamRot.Vector() * MaxUseDistance));
	}
}

const FHitResult UGrabber::GetUsableInView()
{
	if (Controller == NULL)
		return FHitResult();

	FHitResult HitResult;

	FVector CamLoc;
	FRotator CamRot;
	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);
	
	// Draw a debug line
	//DrawDebugLine(GetWorld(), CamLoc, TraceEnd, FColor(255, 0, 0), false, 0.0f, 0.0f, 2.0f);

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		CamLoc,
		TraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return HitResult;
}
