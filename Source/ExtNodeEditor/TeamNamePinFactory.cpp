#include "TeamNamePinFactory.h"
// Fill out your copyright notice in the Description page of Project Settings.

TSharedPtr<class SGraphPin> FTeamNamePinFactory::CreatePin(UEdGraphPin * InPin) const
{

	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	/*
			Check if pin is struct, and then check if that pin is of struct type we want customize
			*/
	if (InPin->PinType.PinCategory == K2Schema->PC_Struct
		&& InPin->PinType.PinSubCategoryObject == FTeamName::StaticStruct())
	{
		return SNew(STeamNamePin, InPin); // return custom pin
	}
	return nullptr;

}
