// Fill out your copyright notice in the Description page of Project Settings.


#include "TranquilizerBoltActor.h"
#include "FrogDisa/DefaultVariables.h"

void ATranquilizerBoltActor::AttachToCharacter()
{
	if (PlayerActor && PlayerActor->FindComponentByClass<UCameraComponent>())
	{
		this->AttachToComponent(PlayerActor->FindComponentByClass<UCameraComponent>(), FAttachmentTransformRules::KeepWorldTransform);
		//ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_RSocket"));
		SetActorRelativeLocation(FVector(0, 0, 0));
		crossbow_bolt_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	}
}