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
