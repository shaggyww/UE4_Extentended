// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamNameCustomization.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/STextComboBox.h"

#include "TeamsAI.h"
#include "GenericTeamAgentInterface.h"
#include "TeamAIController.h"

#define LOCTEXT_NAMESPACE "TeamNameCustomization"

TSharedRef<IPropertyTypeCustomization> FTeamNameCustomization::MakeInstance()
{
	return MakeShareable(new FTeamNameCustomization());
}

void FTeamNameCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	/*
	uint32 NumChildren;
	StructPropertyHandle->GetNumChildren(NumChildren);
	//const UClass *propertyClass = StructPropertyHandle->GetPropertyClass();
	
	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		const TSharedRef< IPropertyHandle > ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();

		if (ChildHandle->GetProperty()->GetName() == TEXT("name"))
		{
			UPropertyTeamNameHandle = ChildHandle;
		}
	}
	*/
	UPropertyTeamNameHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FTeamName, name));

	check(UPropertyTeamNameHandle.IsValid());

	/*
	UTeamsAI::teamNames.Add(TSharedPtr<FString>(new FString("One")));
	UTeamsAI::teamNames.Add(TSharedPtr<FString>(new FString("Two")));
	UTeamsAI::teamNames.Add(TSharedPtr<FString>(new FString("Three")));
	UTeamsAI::teamNames.Add(TSharedPtr<FString>(new FString("Four")));
	*/

	//ATeamAIController propertyOwnerUObject = dynamic_cast<ATeamAIController>(StructPropertyHandle->GetProperty()->GetOwnerClass());

	TeamNameOptions.Empty();
	TeamNameOptions = TeamsAI->teamNames.Array();

	HeaderRow.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget(LOCTEXT("Task", "Team Name"), LOCTEXT("IDK", "Name of the team that/n the AI is a member of"), false, true, false)
		]
	.ValueContent()
		.MinDesiredWidth(250)
		[
			SNew(STextComboBox)
			.OptionsSource(&TeamNameOptions)
			//._OnSelectionChanged.Execute(TSharedPtr<FString>(new FString("One")), ESelectInfo::OnMouseClick)
			.InitiallySelectedItem(GetPropertyType())
			.OnSelectionChanged(this, &FTeamNameCustomization::SetTeamForActor )
		];
}

void FTeamNameCustomization::SetTeamForActor(TSharedPtr<FString> ItemSelected, ESelectInfo::Type SelectInfo)
{
	if (!ItemSelected.IsValid())
	{
		return;
	}

	UPropertyTeamNameHandle->SetValue(*ItemSelected);
}


TSharedPtr<FString> FTeamNameCustomization::GetPropertyType() const
{
	if (!UPropertyTeamNameHandle.IsValid())
		return TeamNameOptions[0];

	UProperty *PropertyValue = NULL;
	UObject *ObjPtr = NULL;
	UPropertyTeamNameHandle->GetValue(ObjPtr);
	PropertyValue = Cast<UProperty>(ObjPtr);

	FString teamName;
	UPropertyTeamNameHandle->GetValueAsDisplayString(teamName);

	if (!teamName.IsEmpty())
	{
		for (int32 i = 0; i < TeamNameOptions.Num(); ++i)
		{
			if (TeamNameOptions[i].IsValid() && TeamNameOptions[i].Get()->Equals(teamName))
			{
				return TeamNameOptions[i];
			}
		}
	}

	return TeamNameOptions[0]; // This should always be none
}

void FTeamNameCustomization::CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
}

#undef LOCTEXT_NAMESPACE