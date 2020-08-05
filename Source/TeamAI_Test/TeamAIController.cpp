// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamAIController.h"

ATeamAIController::ATeamAIController()
{
	teamName = FTeamName();
	friendliesOverride = TSet<int>();
	hostilesOverride = TSet<int>();
}

void ATeamAIController::SetTeamName(FTeamName newName)
{
	teamName = newName;

	SetGenericTeamId(TeamsAI->GetTeamID(teamName.name));
}


FGenericTeamId ATeamAIController::GetTeamID() const
{
	return GetGenericTeamId();
}

ETeamAttitude::Type ATeamAIController::GetTeamAttitudeTowards(const AActor & Other) const
{
	const IGenericTeamAgentInterface* TeamAgentOther = Cast<const IGenericTeamAgentInterface>(&Other);

	if (TeamAgentOther)
	{
		return TeamsAI->GetTeamAttitudeTowards(GetGenericTeamId(), TeamAgentOther->GetGenericTeamId());
	}

	return ETeamAttitude::Neutral;
}

ETeamAttitude::Type ATeamAIController::IsFriendOrFoe(AActor *Other)
{
	return GetTeamAttitudeTowards(*Other);
}

/*
void ATeamAIController::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	FName teamName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (teamName == GET_MEMBER_NAME_CHECKED(FTeamName, name))
	{
		SetGenericTeamId(TeamsAI->GetTeamID(this->teamName.name));
	}
	Super::PostEditChangeProperty(e);
}
*/