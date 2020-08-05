// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamsAI.h"
#include "BufferArchive.h"
#include "Misc/FileHelper.h"
#include "Serialization/MemoryReader.h"
#include "PropertyHandle.h"

#include "Set.h"

UTeamsAI::UTeamsAI()
{
	Inst = this;

	teamsData = TMap<int, FTeamData>();
	teamNames = TSet< TSharedPtr<FString>>();
	LoadData();

	for (int i = 0; i < teamsData.Num(); i++)
	{
		teamNames.Add(MakeShareable(&teamsData[i].teamName));
	}

	AddTeam("none");
}

UTeamsAI* UTeamsAI::Inst = NULL;

bool UTeamsAI::AddTeam(FString newName)
{

	if (newName.IsEmpty())
		return false;

	for (TSharedPtr<FString> name : teamNames)
	{
		if (*name == newName)
			return false;
	}

	teamsData.Add(teamsData.Num(), FTeamData(newName));
	teamNames.Add(MakeShareable(&teamsData[teamsData.Num() - 1].teamName));

	SaveData();

	return true;
}

bool UTeamsAI::RemoveTeam(int TeamID)
{
	if (TeamID == 0)
		return false;

	teamsData.Remove(TeamID);
	teamNames.Remove(teamNames[FSetElementId::FromInteger(TeamID)]);
	
	teamsData.Compact();
	teamNames.CompactStable();
	teamsData.Shrink();
	teamNames.Shrink();

	SaveData();
	return true;
}

bool UTeamsAI::RenameTeam(int TeamID, FString newName)
{
	if (newName.IsEmpty() || TeamID == 0)
		return false;

	for (TSharedPtr<FString> name : teamNames)
	{
		if (*name == newName)
			return false;
	}

	//teamsData[TeamID].SetTeamName(newName);
	*teamNames[FSetElementId::FromInteger(TeamID)] = newName;

	SaveData();

	return true;
}

void UTeamsAI::RemoveAllTeams()
{
	teamsData.Empty();
	teamNames.Empty();

	AddTeam("none");
}


TArray<FString> UTeamsAI::GetTeamNames()
{
	TArray<FString> teamNamesArray = TArray<FString>();
	for (TSharedPtr<FString> teamName : teamNames)
	{
		teamNamesArray.Add(*teamName);
	}

	return teamNamesArray;
}

TSet<int>& UTeamsAI::GetFriends(int TeamID)
{
	return teamsData[TeamID].friendlies;
}

TSet<int>& UTeamsAI::GetHostiles(int TeamID)
{
	return teamsData[TeamID].hostiles;
}

int UTeamsAI::GetTeamID(FString findName)
{
	for (TSharedPtr<FString> name : teamNames)
	{
		if (*name == findName)
		{
			FSetElementId id = teamNames.FindId(name);
			return id.AsInteger();
		}
	}

	return 0;
}


void UTeamsAI::SetFriends(int TeamID, TSet<int> friendsSet)
{
	teamsData[TeamID].SetFriends(friendsSet);

	SaveData();
}

void UTeamsAI::SetHostiles(int TeamID, TSet<int> hostilesSet)
{
	teamsData[TeamID].SetFriends(hostilesSet);

	SaveData();
}


bool UTeamsAI::AddFriend(int TeamID, int friendTeamID)
{
	if (TeamID == friendTeamID || friendTeamID == 0 || TeamID == 0)
		return false;

	teamsData[TeamID].friendlies.Add(friendTeamID);

	SaveData();
	return true;
}

void UTeamsAI::AddFriends(int TeamID, TSet<int> friendsSet)
{
	for (int friendID : friendsSet)
	{
		AddFriend(TeamID, friendID);
	}

	SaveData();
}

bool UTeamsAI::AddHostile(int TeamID, int hostileTeamID)
{
	if (TeamID == hostileTeamID || hostileTeamID == 0 || TeamID == 0)
		return false;

	teamsData[TeamID].hostiles.Add(hostileTeamID);

	SaveData();
	return true;
}

void UTeamsAI::AddHostiles(int TeamID, TSet<int> hostilesSet)
{
	for (int hostileID : hostilesSet)
	{
		AddHostile(TeamID, hostileID);
	}

	SaveData();
}

void UTeamsAI::RemoveFriend(int TeamID, int friendTeamID)
{
	teamsData[TeamID].friendlies.Remove(friendTeamID);

	teamsData[TeamID].friendlies.CompactStable();
	teamsData[TeamID].friendlies.Shrink();

	SaveData();
}

void UTeamsAI::RemoveHostile(int TeamID, int hostileTeamID)
{
	teamsData[TeamID].hostiles.Remove(hostileTeamID);

	teamsData[TeamID].hostiles.CompactStable();
	teamsData[TeamID].hostiles.Shrink();

	SaveData();

}

//TSet< TSharedPtr<FString>> UTeamsAI::teamNames = TSet< TSharedPtr<FString>>();
//TMap<int, FTeamData> UTeamsAI::teamsData = TMap<int, FTeamData>();

bool UTeamsAI::IsConflictBetween(FGenericTeamId thisTeamMemberID, FGenericTeamId otherTeamMemberID)
{
	if (GetTeamAttitudeTowards(thisTeamMemberID, otherTeamMemberID) == ETeamAttitude::Hostile || GetTeamAttitudeTowards(otherTeamMemberID, thisTeamMemberID) == ETeamAttitude::Hostile)
		return true;
	else
		return false;
}

ETeamAttitude::Type UTeamsAI::GetTeamAttitudeTowards(FGenericTeamId thisTeamMemberID, FGenericTeamId otherTeamMemberID)
{
	if (teamsData.Find(thisTeamMemberID.GetId())->friendlies.Find(otherTeamMemberID.GetId()))
		return ETeamAttitude::Friendly;

	if (teamsData.Find(thisTeamMemberID.GetId())->hostiles.Find(otherTeamMemberID.GetId()))
		return ETeamAttitude::Hostile;

	return ETeamAttitude::Neutral;
}

void UTeamsAI::SaveLoadData(FArchive& Ar)
{
	//Save or load values
	Ar << teamsData;
}

bool UTeamsAI::SaveData()
{
	//Save the data to binary
	FBufferArchive ToBinary;
	SaveLoadData(ToBinary);

	//No data were saved
	if (ToBinary.Num() <= 0) return false;

	//Save binaries to disk
	bool result = FFileHelper::SaveArrayToFile(ToBinary, TEXT(SAVEDATAFILENAME));

	//Empty the buffer's contents
	ToBinary.FlushCache();
	ToBinary.Empty();

	return result;
}

bool UTeamsAI::LoadData()
{
	TArray<uint8> BinaryArray;

	//load disk data to binary array
	if (!FFileHelper::LoadFileToArray(BinaryArray, TEXT(SAVEDATAFILENAME))) return false;

	if (BinaryArray.Num() <= 0) return false;

	//Memory reader is the archive that we're going to use in order to read the loaded data
	FMemoryReader FromBinary = FMemoryReader(BinaryArray, true);
	FromBinary.Seek(0);

	SaveLoadData(FromBinary);

	//Empty the buffer's contents
	FromBinary.FlushCache();
	BinaryArray.Empty();
	//Close the stream
	FromBinary.Close();

	return true;
}

/*
void UTeamsAI::PreSave(const ITargetPlatform * targetPlatform)
{
	SaveData();
	Super::PreSave(targetPlatform);
}
*/

/*
void UTeamsAI::PostEditChangeProperty( FPropertyChangedEvent & e)
{
	FName findProperty = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (findProperty == GET_MEMBER_NAME_CHECKED(UTeamsAI, teamNames) || findProperty == GET_MEMBER_NAME_CHECKED(UTeamsAI, teamsData))
	{
		SaveData();
	}
	Super::PostEditChangeProperty(e);
}
*/

FTeamData::FTeamData()
{
	this->teamName = "none";
	friendlies = TSet<int>();
	hostiles = TSet<int>();
}

FTeamData::FTeamData(FString teamName)
{
	this->teamName = teamName;
	friendlies = TSet<int>();
	hostiles = TSet<int>();
}

void FTeamData::SetTeamName(FString teamName)
{
	this->teamName = teamName;
}

void FTeamData::SetFriends(TSet<int> friendsSet)
{
	friendlies = friendsSet;
}

void FTeamData::SetHostiles(TSet<int> hostilesSet)
{
	hostiles = hostilesSet;
}

FArchive & operator<<(FArchive & Ar, FTeamData & TeamDataRef)
{
	Ar << TeamDataRef.teamName;
	Ar << TeamDataRef.friendlies;
	Ar << TeamDataRef.hostiles;

	return Ar;
}

FTeamName::FTeamName()
{
	this->name = "none";
}

FTeamName::FTeamName(FString teamName)
{
	this->name = teamName;
}

