// Fill out your copyright notice in the Description page of Project Settings.


#include "MineActor.h"
#include "FrogDisa/DefaultVariables.h"

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


void AMineActor::AttachToCharacter()
{
	if(PlayerActor)
	this->AttachToActor(PlayerActor, FAttachmentTransformRules::KeepWorldTransform);
	//ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_RSocket"));
	SetActorRelativeLocation(FVector(100, 100, 0));
}