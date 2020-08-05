// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamNamePin.h"
#include "KismetEditorUtilities.h"
#include "STextComboBox.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphSchema.h"
#include "TeamsAI.h"

void STeamNamePin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	TeamNameOptions.Empty();
	
	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);

}

TSharedRef<SWidget>	STeamNamePin::GetDefaultValueWidget()
{
	
	TeamNameOptions.Empty();
	TeamNameOptions = TeamsAI->teamNames.Array();

	FString CurrentDefaultValue = GraphPinObj->GetDefaultAsString();
	if (CurrentDefaultValue.IsEmpty())
	{
		CurrentDefaultValue = TEXT("(");

		//now set here proerty name from USTRUCT(), \" - will add opening "
		// so it will look like AttributeName="
		CurrentDefaultValue += TEXT("name=\"");
		//add value you want to set to your property"
		CurrentDefaultValue += *TeamNameOptions[0];
		//close with "
		CurrentDefaultValue += TEXT("\"");

		//and at last add ) so it will look like (AttributeName="Value");
		CurrentDefaultValue += TEXT(")");

		GraphPinObj->GetSchema()->TrySetDefaultValue(*GraphPinObj, CurrentDefaultValue);
	}

	//FString defaultPinValue = GraphPinObj->DefaultValue;
	//defaultPinValue.RemoveAt(0, 7);
	//defaultPinValue.RemoveFromEnd("\")");
	/*
	TSharedPtr<FString> curSelectedValue;
	
	for (TSharedPtr<FString> name : UTeamsAI::teamNames)
	{
		if (*name == CurrentDefaultValue)
			curSelectedValue = name;
	}
	*/

	return	SNew(STextComboBox) //note you can display any widget here
		.OptionsSource(&TeamNameOptions)
		.InitiallySelectedItem(GetCurrentlySelectedValue())
		.OnSelectionChanged(this, &STeamNamePin::OnTeamNameSelected);

}

TSharedPtr<FString> STeamNamePin::GetCurrentlySelectedValue() const
{
	FString CurrentDefaultValue = GraphPinObj->GetDefaultAsString();

	CurrentDefaultValue.RemoveAt(0, 7);
	CurrentDefaultValue.RemoveFromEnd("\")");

	for (int i = 0; i < TeamNameOptions.Num(); i++)
	{
		if (*TeamNameOptions[i] == CurrentDefaultValue)
			return TeamNameOptions[i];
			
	}

	return TeamNameOptions[0];
}

void STeamNamePin::OnTeamNameSelected(TSharedPtr<FString> ItemSelected, ESelectInfo::Type SelectInfo)
{
	if (!ItemSelected.IsValid())
		return;

	//FString CurrentValue = GraphPinObj->GetDefaultAsString();
	FString CurrentDefaultValue = GraphPinObj->GetDefaultAsString();
	FString teamName = *ItemSelected;
	if (CurrentDefaultValue.IsEmpty())
	{
		CurrentDefaultValue = FString(TEXT("()"));
	}
	//here we construct, setter for value in struct.
	//open it with (
	FString propertyString = TEXT("(");
	if (!teamName.IsEmpty())
	{
		//now set here proerty name from USTRUCT(), \" - will add opening "
		// so it will look like AttributeName="
		propertyString += TEXT("name=\"");
		//add value you want to set to your property"
		propertyString += teamName;
		//close with "
		propertyString += TEXT("\"");
	}
	//and at last add ) so it will look like (AttributeName="Value");
	propertyString += TEXT(")");
	//and here we set our value to parameter if it different than last one.
	if (!CurrentDefaultValue.Equals(propertyString))
	{
		GraphPinObj->GetSchema()->TrySetDefaultValue(*GraphPinObj, propertyString);
	}
}
