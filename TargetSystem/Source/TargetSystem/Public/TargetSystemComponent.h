// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerController.h"
#include "TargetSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComponentOnTargetLockedOnOff, AActor*, TargetActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TARGETSYSTEM_API UTargetSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetSystemComponent();

	// The minimum distance to enable target locked on.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	float MinimumDistanceToEnable;

	// The Widget Class to use when locked on Target. If not defined, will fallback to a Text-rendered widget with a single O character.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	TSubclassOf<UUserWidget> TargetLockedOnWidgetClass;

	// The Widget Draw Size for the Widget class to use when locked on Target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	float TargerLockedOnWidgetDrawSize;

	// The amount of time to break line of sight when actor gets behind an Object.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	float BreakLineOfSightDelay;

	// Lower this value is, easier it will be to switch new target on right or left.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	float StartRotatingThreshold;

	// Whether or not the character rotation should be controlled when Target is locked on.
	// If true, it'll set the value of bUseControllerRotationYaw and bOrientationToMovement variables on Target locked on / off.
	// Set it to true if you want the character to rotate around the locked on target to enable you to setup strafe animations.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	bool ShouldControlRotationWhenLockedOn;

	// The Relative Location to apply on Target LockedOn Widget when attached to a target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	FVector TargerLockedOnWidgetRelativeLocation;

	// The AActor Subclass to search for targetable Actors.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	TSubclassOf<AActor> TargetableActors;

	// Function to call to target a new actor.
	UFUNCTION(BlueprintCallable, Category = "Target System")
	void TargetActor();

	// Function to call to target a new actor.
	UFUNCTION(BlueprintCallable, Category = "Target System")
	void TargetActorWithAxisInput(float AxisValue);

	// Called when a target is locked off, either if it is out of reach (based on MinimumDistanceToEnable) or behind an Object.
	UPROPERTY(BlueprintAssignable, Category = "Target System")
	FComponentOnTargetLockedOnOff OnTargetLockedOff;

	// Called when a target is locked on
	UPROPERTY(BlueprintAssignable, Category = "Target System")
	FComponentOnTargetLockedOnOff OnTargetLockedOn;

private:

	float ClosestTargetDistance;
	bool TargetLocked;
	AActor* CharacterReference;
	AController* CharacterController;
	APlayerController* PlayerController;
	AActor* NearestTarget;
	UWidgetComponent* TargetLockedOnWidgetComponent;
	FTimerHandle LineOfSightBreakTimerHandle;
	bool bIsBreakingLineOfSight;

	TArray<AActor*> GetAllActorsOfClass(TSubclassOf<AActor> ActorClass);
	AActor* FindNearestTarget(TArray<AActor*> Actors);
	bool LineTrace(FHitResult& HitResult, AActor* OtherActor, TArray<AActor*> ActorsToIgnore = TArray<AActor*>());
	bool LineTraceForActor(AActor* OtherActor, TArray<AActor*> ActorsToIgnore = TArray<AActor*>());
	FRotator GetControlRotationOnTarget(AActor* OtherActor);
	void SetControlRotationOnTarget(AActor* TargetActor);
	void CreateAndAttachTargetLockedOnWidgetComponent(AActor* TargetActor);
	bool ShouldBreakLineOfSight();
	void BreakLineOfSight();
	void ControlRotation(bool ShouldControlRotation);
	bool IsInViewport(AActor* TargetActor);

	void TargetLockOn();
	void TargetLockOff();

	float GetDistanceFromCharacter(AActor* OtherActor);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
