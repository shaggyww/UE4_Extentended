// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SlateBasics.h"
#include "SGraphPin.h"


class STeamNamePin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(STeamNamePin) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);
	//this override is used to display slate widget used forcustomization.
	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;
	void OnTeamNameSelected(TSharedPtr<FString> ItemSelected, ESelectInfo::Type SelectInfo);
	TSharedPtr<FString> GetCurrentlySelectedValue() const;
private:
	TArray<TSharedPtr<FString>> TeamNameOptions;
};
