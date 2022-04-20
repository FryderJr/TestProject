// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

void UTurret::Rotate(float RelativeSpeed)
{
    RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
    //UE_LOG(LogTemp, Warning, TEXT("Relative speed is %f"), RelativeSpeed);
    auto RotationChange = RelativeSpeed * MaxRotationSpeed/* * GetWorld()->DeltaTimeSeconds*/;
    UE_LOG(LogTemp, Warning, TEXT("Rotation is %f"), RotationChange);
    AddLocalRotation(FRotator(0, RotationChange, 0));
    /*auto RawNewRoation = GetRelativeRotation().Yaw + RotationChange;

    UE_LOG(LogTemp, Warning, TEXT("Rotation is %f"), RawNewRoation);
    if (RawNewRoation > 179)
    {
        RawNewRoation = (RawNewRoation - 180) - 180;
    }
    if (RawNewRoation < -179)
    {
        RawNewRoation = 180 - (RawNewRoation + 180);
    }
    UE_LOG(LogTemp, Warning, TEXT("Rotation is %f"), RawNewRoation);
    SetRelativeRotation(FRotator(0, RawNewRoation, 0));*/
}