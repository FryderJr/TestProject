// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrel.h"

void UBarrel::Elevate(float RelativeSpeed)
{
    RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
    //UE_LOG(LogTemp, Warning, TEXT("Relative speed is %f"), RelativeSpeed);
    auto ElevationChange = RelativeSpeed * MaxDegreePerSecond * GetWorld()->DeltaTimeSeconds;
    //auto ClampedElevation = FMath::Clamp<float>(ElevationChange, MinElevation, MaxElevation);
    //AddLocalRotation(FRotator(ElevationChange, 0, 0));
    auto RawNewElevation = GetRelativeRotation().Pitch + ElevationChange;
    auto ClampedElevation = FMath::Clamp<float>(RawNewElevation, MinElevation, MaxElevation);
    SetRelativeRotation(FRotator(ClampedElevation, 0, 0));
}