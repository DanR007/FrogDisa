// Fill out your copyright notice in the Description page of Project Settings.


#include "MineActor.h"

AMineActor::AMineActor()
{

}

void AMineActor::Create()
{

}

void AMineActor::Launch()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "MineActor");
}


void AMineActor::AttachToCharacter(AActor* player_Character)
{
	OwnerPlayer = player_Character;
	this->AttachToActor(OwnerPlayer, FAttachmentTransformRules::KeepWorldTransform);
	//ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_RSocket"));
	SetActorRelativeLocation(FVector(100, 100, 0));
}