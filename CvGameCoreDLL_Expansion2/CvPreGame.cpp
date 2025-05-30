/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreUtils.h"
#include "CvPreGame.h"
#include "FStlContainerSerialization.h"
#include "FFileStream.h"
#include "ICvDLLDatabaseUtility.h"
#include "CvInfosSerializationHelper.h"
#include "FIGameIniParser.h"

#include <unordered_map>

// Include this after all other headers.
#include "LintFree.h"

//////////////////////////////////////////////////////////////////////////
//
// WARNING: Do not use any of the GC.*Info tables in this file.
//			These Game Core tables are not always current to what the loaded
//			database set contains.  This is the case when setting up a multiplayer
//			game or even in a single player game if you have returned from a multiplayer game.
//			The reason the Game Core data is not kept in sync is because the caching of all the data
//			can take a significant amount of time.


//Basic translation function to help with the glue between the old
//GameOptionTypes enumeration usage vs the newer system that uses strings.
const char* ConvertGameOptionTypeToString(GameOptionTypes eOption)
{
	switch(eOption)
	{
	case NO_GAMEOPTION:
		break; // Use return at end, see comment beneath.
	case GAMEOPTION_NO_CITY_RAZING:
		return "GAMEOPTION_NO_CITY_RAZING";
	case GAMEOPTION_NO_BARBARIANS:
		return "GAMEOPTION_NO_BARBARIANS";
	case GAMEOPTION_RAGING_BARBARIANS:
		return "GAMEOPTION_RAGING_BARBARIANS";
	case GAMEOPTION_ALWAYS_WAR:
		return "GAMEOPTION_ALWAYS_WAR";
	case GAMEOPTION_ALWAYS_PEACE:
		return "GAMEOPTION_ALWAYS_PEACE";
	case GAMEOPTION_ONE_CITY_CHALLENGE:
		return "GAMEOPTION_ONE_CITY_CHALLENGE";
	case GAMEOPTION_NO_CHANGING_WAR_PEACE:
		return "GAMEOPTION_NO_CHANGING_WAR_PEACE";
	case GAMEOPTION_NEW_RANDOM_SEED:
		return "GAMEOPTION_NEW_RANDOM_SEED";
	case GAMEOPTION_LOCK_MODS:
		return "GAMEOPTION_LOCK_MODS";
	case GAMEOPTION_COMPLETE_KILLS:
		return "GAMEOPTION_COMPLETE_KILLS";
	case GAMEOPTION_NO_GOODY_HUTS:
		return "GAMEOPTION_NO_GOODY_HUTS";
	case GAMEOPTION_RANDOM_PERSONALITIES:
		return "GAMEOPTION_RANDOM_PERSONALITIES";
	case GAMEOPTION_POLICY_SAVING:
		return "GAMEOPTION_POLICY_SAVING";
	case GAMEOPTION_PROMOTION_SAVING:
		return "GAMEOPTION_PROMOTION_SAVING";
	case GAMEOPTION_END_TURN_TIMER_ENABLED:
		return "GAMEOPTION_END_TURN_TIMER_ENABLED";
	case GAMEOPTION_QUICK_COMBAT:
		return "GAMEOPTION_QUICK_COMBAT";
	case GAMEOPTION_DISABLE_START_BIAS:
		return "GAMEOPTION_DISABLE_START_BIAS";
	case GAMEOPTION_NO_SCIENCE:
		return "GAMEOPTION_NO_SCIENCE";
	case GAMEOPTION_NO_POLICIES:
		return "GAMEOPTION_NO_POLICIES";
	case GAMEOPTION_NO_HAPPINESS:
		return "GAMEOPTION_NO_HAPPINESS";
	case GAMEOPTION_NO_TUTORIAL:
		return "GAMEOPTION_NO_TUTORIAL";
	case GAMEOPTION_NO_RELIGION:
		return "GAMEOPTION_NO_RELIGION";
	}

	// Option is unrecognized but we still want to return a value because this function
	// may be called from the engine or by network callbacks.
	//
	// A more strict approach would be to use `UNREACHABLE` here but there could be
	// consequences to that if a packet is somehow corrupt or if a malicous client
	// intentionally sends a bad game option index (or if the engine is just broke).
	return NULL;
}

namespace CvPreGame
{

static const GUID s_emptyGUID = { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
static PackageIDList s_emptyGUIDList;

//------------------------------------------------------------------------------
// CustomOption
//------------------------------------------------------------------------------
CustomOption::CustomOption()
	: m_iValue(-1)
{
	memset(m_szOptionName, 0, 64);
}
//------------------------------------------------------------------------------
CustomOption::CustomOption(const char* szOptionName, int iVal)
	: m_iValue(iVal)
{
	strcpy_s(m_szOptionName, 64, szOptionName);
}
//------------------------------------------------------------------------------
CustomOption::CustomOption(const CustomOption& copy)
{
	m_iValue = copy.m_iValue;
	strcpy_s(m_szOptionName, copy.m_szOptionName);
}
//------------------------------------------------------------------------------
CustomOption& CustomOption::operator=(const CustomOption& rhs)
{
	m_iValue = rhs.m_iValue;
	strcpy_s(m_szOptionName, rhs.m_szOptionName);
	return (*this);
}
//------------------------------------------------------------------------------
bool CustomOption::operator ==(const CustomOption& option) const
{
	if(m_iValue == option.m_iValue)
	{
		if(strncmp(m_szOptionName, option.m_szOptionName, 64) == 0)
			return true;
	}

	return false;
}
//------------------------------------------------------------------------------
const char* CustomOption::GetName(size_t& bytes) const
{
	bytes = strlen(m_szOptionName);
	return m_szOptionName;
}
//------------------------------------------------------------------------------
const char* CustomOption::GetName() const
{
	return m_szOptionName;
}
//------------------------------------------------------------------------------
int CustomOption::GetValue() const
{
	return m_iValue;
}
//------------------------------------------------------------------------------
FDataStream& operator>>(FDataStream& stream, CustomOption& option)
{
	CvString optionName;

	stream >> optionName;
	stream >> option.m_iValue;

	strcpy_s(option.m_szOptionName, 64, optionName.c_str());

	return stream;
}
//------------------------------------------------------------------------------
FDataStream& operator<<(FDataStream& stream, const CustomOption& option)
{
	const CvString optionName(option.m_szOptionName);
	stream << optionName;
	stream << option.m_iValue;

	return stream;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void StringToBools(const char* szString, int* iNumBools, bool** ppBools);

PlayerTypes s_activePlayer(NO_PLAYER);
CvString s_adminPassword;
CvString s_alias;
std::vector<ArtStyleTypes> s_artStyles(MAX_PLAYERS);
bool s_autorun(false);
float s_autorunTurnDelay(0.0f);
int s_autorunTurnLimit(0);
BandwidthType s_bandwidth(NO_BANDWIDTH);
CalendarTypes s_calendar(NO_CALENDAR);
CvBaseInfo s_calendarInfo;
std::vector<CvString> s_civAdjectives(MAX_PLAYERS);
std::vector<CvString> s_civDescriptions(MAX_PLAYERS);
std::vector<CivilizationTypes> s_civilizations(MAX_PLAYERS);
std::vector<CvString> s_civPasswords(MAX_PLAYERS);
std::vector<CvString> s_civShortDescriptions(MAX_PLAYERS);
ClimateTypes s_climate(NO_CLIMATE);
CvClimateInfo s_climateInfo;
EraTypes s_era(NO_ERA);
std::vector<CvString> s_emailAddresses(MAX_PLAYERS);
float s_endTurnTimerLength(0.0f);
std::vector<CvString> s_flagDecals(MAX_PLAYERS);
std::vector<bool> s_DEPRECATEDforceControls(7);			//This was removed during the Day 0 patch since it is no longer used anywhere.
GameMode s_gameMode(NO_GAMEMODE);
CvString s_gameName;
GameSpeedTypes s_gameSpeed(NO_GAMESPEED);
bool s_gameStarted(false);
int s_gameTurn(-1);
GameTypes s_pushedGameType = GAME_TYPE_NONE;
GameTypes s_gameType(GAME_TYPE_NONE);
GameMapTypes s_gameMapType(GAME_USER_PARAMETERS);
int s_gameUpdateTime(0);
std::vector<HandicapTypes> s_handicaps(MAX_PLAYERS);
std::vector<HandicapTypes> s_lastHumanHandicaps(MAX_PLAYERS);
bool s_isEarthMap(false);
bool s_isInternetGame(false);
std::vector<LeaderHeadTypes> s_leaderHeads(MAX_PLAYERS);
std::vector<CvString> s_leaderNames(MAX_PLAYERS);
CvString s_loadFileName;
CvString s_localPlayerEmailAddress;
bool s_mapNoPlayers(false);
unsigned int s_mapRandomSeed(0);
bool s_loadWBScenario(false);
bool s_overrideScenarioHandicap(false);
CvString s_mapScriptName;
int s_maxCityElimination(0);
int s_maxTurns(0);
int s_numMinorCivs(-1);
std::vector<MinorCivTypes> s_minorCivTypes(MAX_PLAYERS);
std::vector<bool> s_minorNationCivs(MAX_PLAYERS);
bool s_dummyvalue(false);
std::vector<bool> s_multiplayerOptions(NUM_MPOPTION_TYPES);
std::vector<int> s_netIDs(MAX_PLAYERS);
std::vector<CvString> s_nicknames(MAX_PLAYERS);
int s_numVictoryInfos(0);
int s_pitBossTurnTime(0);
std::vector<bool> s_playableCivs(MAX_PLAYERS);
std::vector<PlayerColorTypes> s_playerColors(MAX_PLAYERS);
bool s_privateGame(false);
bool s_quickCombat(false);
bool s_quickCombatDefault(false);
HandicapTypes s_quickHandicap(NO_HANDICAP);
bool s_quickstart(false);
bool s_randomWorldSize(false);
bool s_randomMapScript(false);
std::vector<bool> s_readyPlayers(MAX_PLAYERS);
SeaLevelTypes s_seaLevel(NO_SEALEVEL);
CvSeaLevelInfo s_seaLevelInfo;
bool s_dummyvalue2(false);
std::vector<SlotClaim> s_slotClaims(MAX_PLAYERS);
std::vector<SlotStatus> s_slotStatus(MAX_PLAYERS);
CvString s_smtpHost;
unsigned int s_syncRandomSeed(0);
int s_targetScore(0);
std::vector<TeamTypes> s_teamTypes(MAX_PLAYERS);
bool s_transferredMap(false);
CvTurnTimerInfo s_turnTimer;
TurnTimerTypes s_turnTimerType(NO_TURNTIMER);
bool s_bCityScreenBlocked(false);
std::vector<bool> s_victories;
std::vector<bool> s_whiteFlags(MAX_PLAYERS);
CvWorldInfo s_worldInfo;
WorldSizeTypes s_worldSize(NO_WORLDSIZE);
std::vector<CustomOption> s_GameOptions;
std::vector<CustomOption> s_MapOptions;
std::string s_versionString;
std::vector<bool> s_turnNotifySteamInvite(MAX_PLAYERS);
std::vector<bool> s_turnNotifyEmail(MAX_PLAYERS);
std::vector<CvString> s_turnNotifyEmailAddress(MAX_PLAYERS);

typedef std::map<uint, uint> HashToOptionMap;

HashToOptionMap s_GameOptionsHash;
HashToOptionMap s_MapOptionsHash;

bool s_multiplayerAIEnabled = true; // default for RTM, change to true on street patch


std::map<PlayerTypes, CvString> s_displayNicknames; // JAR - workaround duplicate IDs vs display names

const std::vector<TeamTypes>& sr_TeamTypes = s_teamTypes;

PackageIDList s_AllowedDLC;

std::vector<CvString> s_civilizationKeys(MAX_PLAYERS);
std::vector<GUID> s_civilizationPackageID(MAX_PLAYERS);
std::vector<bool> s_civilizationKeysAvailable(MAX_PLAYERS);
std::vector<bool> s_civilizationKeysPlayable(MAX_PLAYERS);

std::vector<CvString> s_leaderKeys(MAX_PLAYERS);
std::vector<GUID> s_leaderPackageID(MAX_PLAYERS);
std::vector<bool> s_leaderKeysAvailable(MAX_PLAYERS);
std::vector<PackageIDList> s_DLCPackagesAvailable(MAX_PLAYERS);

std::vector<CvString> s_civAdjectivesLocalized(MAX_PLAYERS);
std::vector<CvString> s_civDescriptionsLocalized(MAX_PLAYERS);
std::vector<CvString> s_civShortDescriptionsLocalized(MAX_PLAYERS);
std::vector<CvString> s_leaderNamesLocalized(MAX_PLAYERS);

GameStartTypes	s_gameStartType;

StorageLocation	s_loadFileStorage;

// only needed in this file, no need to be public since they are quite specific and kinda nasty
void updateKnownPlayersTable();
bool isKnownPlayerReq(PlayerTypes ePlayer);
bool handleKnownPlayerReq(PlayerTypes ePlayer);
bool isKnownPlayer(PlayerTypes eA, PlayerTypes eB); // only accurate if game option enabled, (indirectly) used in Staging Room to determine if other player details should be shown

std::vector<KnownPlayersBitArray> s_knownPlayersTable;


//	-----------------------------------------------------------------------
//	Bind a leader head key to the leader head using the current leader head ID
bool bindLeaderKeys(PlayerTypes p)
{
	LeaderHeadTypes l = leaderHead(p);

	bool bFailed = false;
	if(l != NO_LEADER)
	{
		// During the pre-game, we can't be sure the cached *Infos are current, so query the database
		Database::Results kResults;
		if(DB.Execute(kResults, "SELECT Type, PackageID from Leaders where ID = ? LIMIT 1"))
		{
			kResults.Bind(1, l);
			if(kResults.Step())
			{
				s_leaderKeys[p] = kResults.GetText(0);
				if(!ExtractGUID(kResults.GetText(1), s_leaderPackageID[p]))
					ClearGUID(s_leaderPackageID[p]);
				s_leaderKeysAvailable[p] = true;
			}
			else
				bFailed = true;
		}
		else
			bFailed = true;
	}
	else
		bFailed = true;

	if(bFailed)
	{
		s_leaderKeys[p].clear();
		ClearGUID(s_leaderPackageID[p]);
		s_leaderKeysAvailable[p] = false;
	}

	return bFailed;
}

// Set the unique key value for the civilization
bool bindCivilizationKeys(PlayerTypes p)
{
	bool bFailed = false;
	CivilizationTypes c = civilization(p);
	if(c != NO_CIVILIZATION)
	{
		// During the pre-game, we can't be sure the cached *Infos are current, so query the database
		Database::Results kResults;
		if(DB.Execute(kResults, "SELECT Type, PackageID, Playable from Civilizations where ID = ? LIMIT 1"))
		{
			kResults.Bind(1, c);
			if(kResults.Step())
			{
				s_civilizationKeys[p] = kResults.GetText(0);
				if(!ExtractGUID(kResults.GetText(1), s_civilizationPackageID[p]))
					ClearGUID(s_civilizationPackageID[p]);

				s_civilizationKeysAvailable[p] = true;
				s_civilizationKeysPlayable[p] = kResults.GetBool(2);
			}
			else
				bFailed = true;
		}
		else
			bFailed = true;
	}
	else
		bFailed = true;

	if(bFailed)
	{
		s_civilizationKeys[p].clear();
		ClearGUID(s_civilizationPackageID[p]);
		s_civilizationKeysAvailable[p] = false;
		s_civilizationKeysPlayable[p] = false;
	}
	return bFailed;
}

void writeCivilizations(FDataStream& saveTo)
{
	if(s_gameStarted || !isNetworkMultiplayerGame()){
		//full save, preserve everything.
		saveTo << s_civilizations;
	}
	else{
		//game cfg save only.  Scrub player specific data from our save output.
		//reset all non-AI players to random civ.
		int i = 0;
		std::vector<CivilizationTypes> civsTemp = s_civilizations;
		for(std::vector<CivilizationTypes>::iterator civIter = civsTemp.begin(); civIter != civsTemp.end(); ++civIter, ++i){
			if(slotStatus((PlayerTypes)i) != SS_COMPUTER){
				*civIter = NO_CIVILIZATION;
			}
		}
		saveTo << civsTemp;
	}
}

void writeNicknames(FDataStream& saveTo)
{
	if(s_gameStarted || !isNetworkMultiplayerGame()){
		//full save, preserve everything.
		saveTo << s_nicknames;
	}
	else{
		//game cfg save only.  Scrub player specific data from our save output.
		std::vector<CvString> nicksTemp = s_nicknames;
		for(std::vector<CvString>::iterator nickIter = nicksTemp.begin(); nickIter != nicksTemp.end(); ++nickIter){
			*nickIter = "";
		}
		saveTo << nicksTemp;
	}
}

void writeSlotStatus(FDataStream& saveTo)
{
	if(s_gameStarted || !isNetworkMultiplayerGame()){
		//full save, preserve everything.
		saveTo << s_slotStatus;
	}
	else{
		//game cfg save only.  Scrub player specific data from our save output.
		//Revert human occupied slots to open.
		std::vector<SlotStatus> slotTemp = s_slotStatus;
		for(std::vector<SlotStatus>::iterator slotIter = slotTemp.begin(); slotIter != slotTemp.end(); ++slotIter){
			if(*slotIter == SS_TAKEN){
				*slotIter = SS_OPEN;
			}
		}
		saveTo << slotTemp;
	}
}

//	-----------------------------------------------------------------------
void saveSlotHints(FDataStream& saveTo)
{
	uint uiVersion = 3;
	saveTo << uiVersion;
	saveTo << s_gameSpeed;
	saveTo << s_worldSize;
	saveTo << s_mapScriptName;

	writeCivilizations(saveTo);
	writeNicknames(saveTo);
	writeSlotStatus(saveTo);

	saveTo << s_slotClaims;
	saveTo << s_teamTypes;
	saveTo << s_handicaps;
	saveTo << s_civilizationKeys;
	saveTo << s_leaderKeys;

	// didn't update version number as I am not sure what it means to update it when all the relevant code is stripped out by the preprocessor
	int iKeepUnmet = 0;
	GetGameOption(GAMEOPTION_KEEP_UNMET_PLAYERS_UNKNOWN, iKeepUnmet);
	if(iKeepUnmet)
		updateKnownPlayersTable();
	saveTo << s_knownPlayersTable;
}

void ReseatConnectedPlayers()
{//This function realigns network connected players into the correct slots for the current pregame data. (Typically after loading in saved data)
	//A network player's pregame data can and will be totally wrong until this function is run and the resulting net messages are processed.
	if(isNetworkMultiplayerGame()){
		int i = 0;
		for(i = 0; i < MAX_PLAYERS; ++i){
			// reseat the network connected player in this slot.
			if(gDLL->ReseatConnectedPlayer((PlayerTypes)i)){
				//a player needs to be reseated.  We need to wait for the net message to come from the host.  We will rerun ReseatConnectedPlayers then.
				return;
			}
		}
	}
}


int calcActiveSlotCount(const std::vector<SlotStatus>& slotStatus, const std::vector<SlotClaim>& slotClaims)
{
	int iCount = 0;
	int i = 0;
	int maxPlayers = std::min(slotStatus.size(), slotClaims.size());
	for(i = 0; i < maxPlayers; ++i)
	{
		SlotStatus eStatus = slotStatus[i];
		SlotClaim eClaim = slotClaims[i];

		if((eStatus == SS_TAKEN || eStatus == SS_COMPUTER || eStatus == SS_OBSERVER) 
			&& (eClaim == SLOTCLAIM_ASSIGNED || eClaim == SLOTCLAIM_RESERVED))
			++iCount;
	}

	return iCount;
}

//	---------------------------------------------------------------------------
static void loadSlotsHelper(
    FDataStream& loadFrom,
    uint uiVersion,
    GameSpeedTypes& gameSpeed,
    WorldSizeTypes& worldSize,
    CvString& mapScriptName,
    std::vector<CivilizationTypes>& civilizations,
    std::vector<CvString>& nicknames,
    std::vector<SlotStatus>& slotStatus,
    std::vector<SlotClaim>& slotClaims,
    std::vector<TeamTypes>& teamTypes,
    std::vector<HandicapTypes>& handicapTypes,
	std::vector<CvString>& civilizationKeys,
	std::vector<CvString>& leaderKeys, 
	std::vector<KnownPlayersBitArray>& metCivs
	)
{
	loadFrom >> gameSpeed;
	loadFrom >> worldSize;
	loadFrom >> mapScriptName;

	loadFrom >> civilizations;

	loadFrom >> nicknames;

	loadFrom >> slotStatus;
	loadFrom >> slotClaims;
	loadFrom >> teamTypes;
	if(uiVersion >= 2)
		loadFrom >> handicapTypes;
	else
		handicapTypes.clear();

	if(uiVersion >= 3)
	{
		loadFrom >> civilizationKeys;
		loadFrom >> leaderKeys;
	}
	else
	{
		civilizationKeys.clear();
		leaderKeys.clear();
	}

	loadFrom >> metCivs;
}

int readActiveSlotCountFromSaveGame(FDataStream& loadFrom, bool bReadVersion)
{
	uint uiVersion = 0;
	if(bReadVersion)
		loadFrom >> uiVersion;

	GameSpeedTypes	dummyGameSpeed;
	WorldSizeTypes	dummyWorldSize;
	CvString		dummyMapScriptName;
	std::vector<CivilizationTypes>	dummyCivilizations;
	std::vector<CvString> dummyNicknames;
	std::vector<TeamTypes> dummyTeamTypes;
	std::vector<SlotStatus> slotStatus(MAX_PLAYERS);
	std::vector<SlotClaim> slotClaims(MAX_PLAYERS);
	std::vector<HandicapTypes> dummyHandicapTypes;
	std::vector<CvString> civilizationKeys;
	std::vector<CvString> leaderKeys;
	std::vector<KnownPlayersBitArray> dummyKnownPlayersTable;
	loadSlotsHelper(loadFrom, uiVersion, dummyGameSpeed, dummyWorldSize, dummyMapScriptName, dummyCivilizations, dummyNicknames, slotStatus, slotClaims, dummyTeamTypes, dummyHandicapTypes, civilizationKeys, leaderKeys, dummyKnownPlayersTable);
	return calcActiveSlotCount(slotStatus, slotClaims);
}

void loadSlotHints(FDataStream& loadFrom, bool bReadVersion)
{
	uint uiVersion = 0;
	if(bReadVersion)
		loadFrom >> uiVersion;

	GameSpeedTypes	gameSpeed;
	WorldSizeTypes	worldSize;
	CvString		mapScriptName;
	std::vector<CivilizationTypes>	civilizations;
	std::vector<CvString> nicknames;
	std::vector<TeamTypes> teamTypes;
	std::vector<SlotStatus> slotStatus;
	std::vector<SlotClaim> slotClaims;
	std::vector<HandicapTypes> handicapTypes;
	std::vector<CvString> civilizationKeys;
	std::vector<CvString> leaderKeys;
	std::vector<KnownPlayersBitArray> knownPlayersTable;
	loadSlotsHelper(loadFrom, uiVersion, gameSpeed, worldSize, mapScriptName, civilizations, nicknames, slotStatus, slotClaims, teamTypes, handicapTypes, civilizationKeys, leaderKeys, knownPlayersTable);

	s_gameSpeed = gameSpeed;
	s_worldSize = worldSize;
	s_mapScriptName = mapScriptName;
	if (uiVersion >= 3)
	{
		// Set the civilizations and leaders from the key values
		if (civilizationKeys.size() > 0)
		{
			for (uint i = 0; i < civilizationKeys.size(); ++i)	
				setCivilizationKey((PlayerTypes)i, civilizationKeys[i]);
		}
		else
			// Fall back to using the indices.  This shouldn't happen.
			s_civilizations = civilizations;

		if (leaderKeys.size() > 0)
		{
			for (uint i = 0; i < leaderKeys.size(); ++i)	
				setLeaderKey((PlayerTypes)i, leaderKeys[i]);
		}
	}
	else
	{
		// Civilization saved as indices, not a good idea.
		s_civilizations = civilizations;
	}

	s_nicknames = nicknames;
	s_teamTypes = teamTypes;
	s_slotStatus = slotStatus;
	s_slotClaims = slotClaims;

	// The slots hints handicaps may be invalid, only copy them over if they are not
	if(handicapTypes.size() == MAX_PLAYERS)
		s_handicaps = handicapTypes;

	size_t i = 0;
	for(i = 0; i < s_nicknames.size(); ++i)
	{
		PlayerTypes p = static_cast<PlayerTypes>(i);
		setNickname(p, s_nicknames[i]); // fix display names
	}

	s_knownPlayersTable = knownPlayersTable;
	ReseatConnectedPlayers();
}

PlayerTypes activePlayer()
{
	return s_activePlayer;
}

const CvString& adminPassword()
{
	return s_adminPassword;
}

const CvString& alias()
{
	return s_alias;
}

ArtStyleTypes artStyle(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_artStyles[p];
	return NO_ARTSTYLE;
}

bool autorun()
{
	return s_autorun;
}

float autorunTurnDelay()
{
	return s_autorunTurnDelay;
}

int autorunTurnLimit()
{
	return s_autorunTurnLimit;
}

BandwidthType bandwidth()
{
	return s_bandwidth;
}

CvString bandwidthDescription()
{
	if(bandwidth() == BANDWIDTH_BROADBAND)
	{
		//return CvString("broadband");
		return GetLocalizedText("broadband");
	}
	else
	{
		return GetLocalizedText("modem");
		//return CvString("modem");
	}
}

CalendarTypes calendar()
{
	return s_calendar;
}

CivilizationTypes civilization(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_civilizations[p];
	return NO_CIVILIZATION;
}

const CvString& civilizationAdjective(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		s_civAdjectivesLocalized[p] = GetLocalizedText(s_civAdjectives[p]);
		return s_civAdjectivesLocalized[p];
	}

	static const CvString empty = "";
	return empty;
}

const CvString& civilizationAdjectiveKey(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_civAdjectives[p];
	static const CvString empty = "";
	return empty;
}

const CvString& civilizationDescription(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		s_civDescriptionsLocalized[p] = GetLocalizedText(s_civDescriptions[p]);
		return s_civDescriptionsLocalized[p];
	}

	static const CvString empty = "";
	return empty;
}

const CvString& civilizationDescriptionKey(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_civDescriptions[p];
	static const CvString empty = "";
	return empty;
}

const CvString& civilizationPassword(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_civPasswords[p];

	static const CvString empty = "";
	return empty;
}

const CvString& civilizationShortDescription(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		s_civShortDescriptionsLocalized[p] = GetLocalizedText(s_civShortDescriptions[p]);
		return s_civShortDescriptionsLocalized[p];
	}

	static const CvString empty = "";
	return empty;
}

const CvString& civilizationShortDescriptionKey(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_civShortDescriptions[p];
	static const CvString empty = "";
	return empty;
}

ClimateTypes climate()
{
	return s_climate;
}

const CvClimateInfo& climateInfo()
{
	if(s_climate != s_climateInfo.GetID())
	{
		Database::SingleResult kResult;
		DB.SelectAt(kResult, "Climates", s_climate);
		s_climateInfo.CacheResult(kResult);
	}
	return s_climateInfo;
}

void closeAllSlots()
{
	for(int i = 0; i < MAX_CIV_PLAYERS; i++)
	{
		PlayerTypes eID = (PlayerTypes)i;
		setSlotStatus(eID, SS_CLOSED);
		setSlotClaim(eID, SLOTCLAIM_UNASSIGNED);
	}
}

void closeInactiveSlots()
{
	setGameStarted(true);

	// Open inactive slots mean different things to different game modes and types...
	// Let's figure out what they mean for us
	gDLL->BeginSendBundle();
	for (int i = 0; i < MAX_CIV_PLAYERS; i++)
	{
		PlayerTypes eID = (PlayerTypes)i;
		if (slotStatus(eID) == SS_OPEN)
		{
			if (gameType() == GAME_NETWORK_MULTIPLAYER && gameMapType() == GAME_SCENARIO)
			{
				// Multiplayer scenario - all "open" slots should be filled with an AI player
				setSlotStatus(eID, SS_COMPUTER);
			}
			else
			{
				// If it's a normal game, all "open" slots should be closed.
				setSlotStatus(eID, SS_CLOSED);
			}
			setSlotClaim(eID, SLOTCLAIM_UNASSIGNED);
		}
		gDLL->sendPlayerInfo(eID);
	}
	gDLL->EndSendBundle();
}

const CvString& emailAddress(PlayerTypes p)
{
	static const CvString empty("");
	if(p >= 0 && p < MAX_PLAYERS)
		return s_emailAddresses[p];
	return empty;
}

const CvString& emailAddress()
{
	return s_localPlayerEmailAddress;
}

float endTurnTimerLength()
{
	return s_endTurnTimerLength;
}

EraTypes era()
{
	return s_era;
}

PlayerTypes findPlayerByNickname(const char* const name)
{
	PlayerTypes result = NO_PLAYER;
	if(name)
	{
		int i = 0;
		for(i = 0; i < MAX_PLAYERS; ++i)
		{
			if(s_nicknames[i] == name)
			{
				result = static_cast<PlayerTypes>(i);
				break;
			}
		}
	}
	return result;
}

GameMode gameMode()
{
	return s_gameMode;
}

const CvString& gameName()
{
	return s_gameName;
}

//! This does not need to be an auto var!
//! The actual game options vector s_GameOptions is an auto var.
//! This is only here to make retrieving enum-based options fast
//! since GameCore does it way to effing often.
int s_EnumBasedGameOptions[NUM_GAMEOPTION_TYPES];

//! This method sync's up an enum-based array of game options w/ the actual
//! game options structure.  This should be called every time new options
//! are set.
void SyncGameOptionsWithEnumList()
{
	const char* str = NULL;
	for(int i = 0; i < NUM_GAMEOPTION_TYPES; i++)
	{
		int value = 0;
		str = ConvertGameOptionTypeToString((GameOptionTypes)i);
		if(str)
		{
			GetGameOption(str, value);
		}
		s_EnumBasedGameOptions[i] = value;

		switch(i)
		{
		case GAMEOPTION_QUICK_COMBAT:
			s_quickCombat = value>0;	// set directly, to avoid infinite recursion.
			break;
		default:
			break;
		}
	}
}

bool GetGameOption(const char* szOptionName, int& iValue)
{
	if (szOptionName == NULL || *szOptionName == 0)
		return false;

	for(std::vector<CustomOption>::const_iterator it = s_GameOptions.begin();
	        it != s_GameOptions.end(); ++it)
	{
		size_t bytes = 0;
		const char* szCurrentOptionName = (*it).GetName(bytes);
		if(strncmp(szCurrentOptionName, szOptionName, bytes) == 0)
		{
			iValue = (*it).GetValue();
			return true;
		}
	}

	//Try and lookup the default value.
	Database::Results kLookup;
	if(GC.GetGameDatabase()->Execute(kLookup, "Select \"Default\" from GameOptions where Type = ? LIMIT 1"))
	{
		kLookup.Bind(1, szOptionName);
		if(kLookup.Step())
		{
			iValue = kLookup.GetInt(0);

			if (GC.getGame().getGameTurn() > 0)
				s_GameOptions.push_back(CustomOption(szOptionName, iValue));

			return true;
		}
	}

	return false;
}

bool GetGameOption(GameOptionTypes eOption, int& iValue)
{
	if((uint)eOption >= 0 && (uint)eOption < (uint)NUM_GAMEOPTION_TYPES)
	{
		iValue = s_EnumBasedGameOptions[(size_t)eOption];
		return true;
	}
	else
	{
		// Hash lookup
		HashToOptionMap::const_iterator itr = s_GameOptionsHash.find((uint)eOption);
		if(itr != s_GameOptionsHash.end())
		{
			if(itr->second < s_GameOptions.size())
			{
				iValue = s_GameOptions[itr->second].GetValue();
				return true;
			}
		}

		// Must get the string from the hash
		GameOptionTypes eOptionIndex = (GameOptionTypes)GC.getInfoTypeForHash((uint)eOption);
		if(eOptionIndex >= 0)
		{
			CvGameOptionInfo* pkInfo = GC.getGameOptionInfo(eOptionIndex);
			if(pkInfo)
			{
				//Try and lookup the default value.
				Database::Results kLookup;
				if(DB.Execute(kLookup, "Select \"Default\" from GameOptions where Type = ? LIMIT 1"))
				{
					kLookup.Bind(1, pkInfo->GetType());
					if(kLookup.Step())
					{
						iValue = kLookup.GetInt(0);
						return true;
					}
				}
			}
		}

		return false;
	}
}

const std::vector<CustomOption>& GetGameOptions()
{
	return s_GameOptions;
}

bool SetGameOption(const char* szOptionName, int iValue)
{
	//Do not allow NULL entries :P
	if(szOptionName == NULL || strlen(szOptionName) == 0)
		return false;

	for(size_t i = 0; i < s_GameOptions.size(); i++)
	{
		const CustomOption& option = s_GameOptions[i];
		size_t bytes = 0;
		const char* szCurrentOptionName = option.GetName(bytes);
		if(strncmp(szCurrentOptionName, szOptionName, bytes) == 0)
		{
			//I'd like to just set the value here, but that doesn't seem possible
			//so instead, create a new CustomOption type and assign it to this index.
			s_GameOptions[i] = CustomOption(szOptionName, iValue);
			SyncGameOptionsWithEnumList();
			return true;
		}

	}

	//Didn't find the option, push it.
	s_GameOptions.push_back(CustomOption(szOptionName, iValue));
	s_GameOptionsHash[FStringHash(szOptionName)] = s_GameOptions.size() - 1;
	SyncGameOptionsWithEnumList();

	return true;
}

bool SetGameOption(GameOptionTypes eOption, int iValue)
{
	const char* str = ConvertGameOptionTypeToString(eOption);
	if(str)
	{
		return SetGameOption(str, iValue);
	}

	return false;
}

bool SetGameOptions(const std::vector<CustomOption>& gameOptions)
{
	s_GameOptions = gameOptions;
	s_GameOptionsHash.clear();
	for(size_t i = 0; i < s_GameOptions.size(); i++)
	{
		const CustomOption& kOption = s_GameOptions[i];
		s_GameOptionsHash[FStringHash(kOption.GetName())] = i;
	}

	SyncGameOptionsWithEnumList();
	return true;
}

GameSpeedTypes gameSpeed()
{
	return s_gameSpeed;
}

bool gameStarted()
{
	return s_gameStarted;
}

int gameTurn()
{
	return s_gameTurn;
}

GameTypes gameType()
{
	return s_gameType;
}

GameMapTypes gameMapType()
{
	return s_gameMapType;
}

GameStartTypes gameStartType()
{
	return s_gameStartType;
}

int gameUpdateTime()
{
	return s_gameUpdateTime;
}

HandicapTypes handicap(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_handicaps[p];
	return NO_HANDICAP;
}

HandicapTypes lastHumanHandicap(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_lastHumanHandicaps[p];
	return NO_HANDICAP;
}

bool isHotSeat()
{
	GameTypes g = gameType();
	return (g == GAME_HOTSEAT_MULTIPLAYER);
}

bool isHotSeatGame()
{
	return isHotSeat();
}

bool isHuman(PlayerTypes p)
{
	SlotStatus s = slotStatus(p);
	if(s == SS_TAKEN)
	{
		return true;
	}
	else if(s == SS_OPEN)
	{
		return (isHotSeat() || isPitBoss() || isPlayByEmail());
	}
	else if(s == SS_OBSERVER)
	{
		return slotClaim(p) == SLOTCLAIM_ASSIGNED;
	}
	else
	{
		return false;
	}

}

bool isInternetGame()
{
	return s_isInternetGame;
}

bool isMinorCiv(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_minorNationCivs[p];
	return false;
}

bool isReallyNetworkMultiPlayer()
{
	GameTypes eType = gameType();
	if (eType == GAME_NETWORK_MULTIPLAYER)
		return true;

	if (s_pushedGameType != GAME_TYPE_NONE) {
		eType = s_pushedGameType;
	}

	return eType == GAME_NETWORK_MULTIPLAYER;
}

bool isNetworkMultiplayerGame()
{
	GameTypes eType = gameType();
	return eType == GAME_NETWORK_MULTIPLAYER;
}

bool isPitBoss()
{
	int iValue = 0;
	if(GetGameOption(GAMEOPTION_PITBOSS, iValue))
		return iValue != 0;
	return false;
}

bool g_bPersistSettings = false;
bool GetPersistSettings()
{
	return g_bPersistSettings;
}

void setPersistSettings(bool bPersist)
{
	g_bPersistSettings = bPersist;
}

bool isPlayable(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		if(isWBMapScript() && !mapNoPlayers())
		{
			return s_playableCivs[p];
		}
		else
		{
			if(civilization(p) != NO_CIVILIZATION)
			{
				return s_civilizationKeysPlayable[p];
			}
			else
			{
				// Don't allow people to play the barbarian civ
				return (p < MAX_CIV_PLAYERS);
			}
		}
	}

	return false;
}

bool isPlayByEmail()
{
	GameTypes g = gameType();
	return g == GAME_EMAIL_MULTIPLAYER;
}

bool isReady(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_readyPlayers[p];
	return false;
}

bool IsCityScreenBlocked()
{
	return s_bCityScreenBlocked;
}

bool isVictory(VictoryTypes v)
{
	if(v < s_numVictoryInfos)
		return s_victories[v];
	return false;
}

bool isWBMapScript()
{
	bool bIsMapScript = false;
	const CvString& mapScript = mapScriptName();
	const size_t lenMapScript = mapScript.size();
	if(lenMapScript > 0)
	{
		size_t lenExtension = 0;
		const char* szExtension = NULL;
		for(size_t i = 0; i < lenMapScript; i++)
		{
			if(mapScript[i] == '.')
			{
				szExtension = mapScript.c_str() + i;
				lenExtension = lenMapScript - i;
			}
		}

		if(szExtension && lenExtension == strlen(CIV5_WBMAP_EXT))
		{
			bIsMapScript = (_strnicmp(szExtension, CIV5_WBMAP_EXT, lenExtension) == 0);
		}
	}

	return bIsMapScript;
}

bool isWhiteFlag(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_whiteFlags[p];
	return false;
}

bool isTurnNotifySteamInvite(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_turnNotifySteamInvite[p];
	return false;
}

bool isTurnNotifyEmail(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_turnNotifyEmail[p];
	return false;
}

const CvString& getTurnNotifyEmailAddress(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		return s_turnNotifyEmailAddress[p];
	}

	static const CvString empty = "";
	return empty;
}

LeaderHeadTypes leaderHead(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_leaderHeads[p];
	return NO_LEADER;
}

const CvString& leaderName(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		s_leaderNamesLocalized[p] = GetLocalizedText(s_leaderNames[p]);
		return s_leaderNamesLocalized[p];
	}

	static const CvString empty = "";
	return empty;
}

const CvString& leaderNameKey(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_leaderNames[p];
	static const CvString empty = "";
	return empty;
}

const CvString& leaderKey(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_leaderKeys[p];
	static const CvString empty = "";
	return empty;
}

// Get the package ID the player's leader is from.  If empty, it comes from the base installation.
const GUID& leaderKeyPackageID(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		return s_leaderPackageID[p];
	}

	return s_emptyGUID;
}

const CvString& loadFileName()
{
	return s_loadFileName;
}

StorageLocation loadFileStorage()
{
	return s_loadFileStorage;
}

void loadFromIni(FIGameIniParser& iniParser)
{
	if (gameStarted())
		return;
	FString szHolder;
	CvString tempString;

	int iHolder = 0;
	iniParser.GetKeyValue("GAME", "QuickCombat", &iHolder, "Force quick combat animations", 0);
	setQuickCombatDefault(iHolder != 0);
	setQuickCombat(quickCombatDefault());

	CvString szGameDefault = "";//FString(GC.getSetupData().getAlias().GetCString());
	if (szGameDefault.IsEmpty())
	{
		Localization::String strGameName = Localization::Lookup("TXT_KEY_DEFAULT_GAMENAME");
		szGameDefault = strGameName.toUTF8();
	}
	else
	{
		Localization::String strGameName = Localization::Lookup("TXT_KEY_USER_GAMENAME");
		szGameDefault += strGameName.toUTF8();
	}
	iniParser.GetKeyValue("GAME", "GameName", &szHolder, "Game Name", szGameDefault);
	tempString = szHolder;
	setGameName(tempString);
	// World Size
	iniParser.GetKeyValue("GAME", "WorldSize", &szHolder, "Worldsize options are WORLDSIZE_DUEL/WORLDSIZE_TINY/WORLDSIZE_SMALL/WORLDSIZE_STANDARD/WORLDSIZE_LARGE/WORLDSIZE_HUGE", "WORLDSIZE_SMALL");
	tempString = szHolder;
	setWorldSize(tempString);
	// Climate
	iniParser.GetKeyValue("GAME", "Climate", &szHolder, "Climate options are CLIMATE_ARID/CLIMATE_TEMPERATE/CLIMATE_TROPICAL", "CLIMATE_TEMPERATE");
	tempString = szHolder;
	setClimate(tempString);
	// Sea Level
	iniParser.GetKeyValue("GAME", "SeaLevel", &szHolder, "Sealevel options are SEALEVEL_LOW/SEALEVEL_MEDIUM/SEALEVEL_HIGH", "SEALEVEL_MEDIUM");
	tempString = szHolder;
	setSeaLevel(tempString);
	// Era
	iniParser.GetKeyValue("GAME", "Era", &szHolder, "Era options are ERA_ANCIENT/ERA_CLASSICAL/ERA_MEDIEVAL/ERA_RENAISSANCE/ERA_INDUSTRIAL/ERA_MODERN", "ERA_ANCIENT");
	tempString = szHolder;
	setEra(tempString);
	// Game speed
	iniParser.GetKeyValue("GAME", "GameSpeed", &szHolder, "GameSpeed options are GAMESPEED_QUICK/GAMESPEED_STANDARD/GAMESPEED_EPIC/GAMESPEED_MARATHON", "GAMESPEED_STANDARD");
	tempString = szHolder;
	setGameSpeed(tempString);
	// Quick Handicap

	int iNumBools = 0;
	bool* pbBools = NULL;

	// Victory Conditions
	iniParser.GetKeyValue("GAME", "VictoryConditions", &szHolder, "Victory Conditions", "11111111");
	if (szHolder != "EMPTY")
	{
		StringToBools(szHolder, &iNumBools, &pbBools);
		iNumBools = std::min(iNumBools, s_numVictoryInfos);
		std::vector<bool> tempVBool;
		int i = 0;
		for (i = 0; i < iNumBools; i++)
		{
			tempVBool.push_back(pbBools[i]);
		}
		setVictories(tempVBool);
		SAFE_DELETE_ARRAY(pbBools);
	}

	// Game Options
	//if (!CIV.GetModName())
	{
		iniParser.GetKeyValue("GAME", "GameOptions", &szHolder, "Game Options", "EMPTY");
		if (szHolder != "EMPTY")
		{
			StringToBools(szHolder, &iNumBools, &pbBools);
			iNumBools = std::min(iNumBools, static_cast<int>(NUM_GAMEOPTION_TYPES));
			int i = 0;
			for (i = 0; i < iNumBools; i++)
			{
				SetGameOption(((GameOptionTypes)i), pbBools[i]);
			}
			SAFE_DELETE_ARRAY(pbBools);
		}
	}

	// Max Turns
	iniParser.GetKeyValue("GAME", "MaxTurns", &iHolder, "Max number of turns (0 for no turn limit)", 0);
	setMaxTurns(iHolder);

	iniParser.GetKeyValue("GAME", "EnableMultiplayerAI", &iHolder, "Allow AI in multiplayer games", 1);
	setMultiplayerAIEnabled(iHolder != 0);

	// Pitboss SMTP info
	//iniParser.SetGroupKey("CONFIG");
	//iniParser.GetKeyValue("PitbossSMTPHost", &szHolder, "", "SMTP server for Pitboss reminder emails");
	//GC.getSetupData().setPitbossSmtpHost((szHolder.Compare("0") ? szHolder.GetCString() : ""));
	//iniParser.GetKeyValue("PitbossSMTPLogin", &szHolder, "", "SMTP server authentication login for Pitboss reminder emails");
	//GC.getSetupData().setPitbossSmtpLogin((szHolder.Compare("0") ? szHolder.GetCString() : ""));
	//iniParser.GetKeyValue("PitbossEmail", &szHolder, "", "Email address from which Pitboss reminder emails are sent");
	//GC.getSetupData().setPitbossEmail((szHolder.Compare("0") ? szHolder.GetCString() : ""));

	// Get Pitboss Turn Time
	//iniParser.SetGroupKey("GAME");
	//iniParser.GetKeyValue("PitbossTurnTime", &iHolder, 0, "Pitboss Turn Time");
	//setPitbossTurnTime(iHolder);
	// Sync Rand
#ifdef CRIPPLED_BUILD
	//Dbaker set the seed to the same thing for testing purposes
	setSyncRandomSeed(1);
	setMapRandomSeed(1);
#else
	iniParser.GetKeyValue("CONFIG", "SyncRandSeed", &iHolder, "Random seed for game sync, or '0' for default", 0);
	setSyncRandomSeed((iHolder != 0) ? iHolder : timeGetTime());
	// Map Rand
	iniParser.GetKeyValue("CONFIG", "MapRandSeed", &iHolder, "Random seed for map generation, or '0' for default", 0);
	setMapRandomSeed((iHolder != 0) ? iHolder : timeGetTime());
#endif

	//	Game Type
	iniParser.GetKeyValue("GAME", "GameType", &szHolder, "GameType options are singlePlayer/spLoad", "singlePlayer");
	tempString = szHolder;
	setGameType(tempString);

	// Map Script
	iniParser.GetKeyValue("GAME", "Map", &szHolder, "Map Script file name", "Assets/Maps/Continents.lua");
	szHolder.StandardizePath(false, false);
	tempString = szHolder;
	setMapScriptName(tempString);
}

bool mapNoPlayers()
{
	return s_mapNoPlayers;
}

bool GetMapOption(const char* szOptionName, int& iValue)
{
	for(std::vector<CustomOption>::const_iterator it = s_MapOptions.begin();
	        it != s_MapOptions.end(); ++it)
	{
		size_t bytes = 0;
		const char* szCurrentOptionName = (*it).GetName(bytes);
		if(strncmp(szCurrentOptionName, szOptionName, bytes) == 0)
		{
			iValue = (*it).GetValue();
			return true;
		}
	}

	//Try and lookup the default value.
	//Not a huge fan of this being here as it adds an additional dependency, but there was really no "clean" place to put it.
	Database::Results kLookup;
	if(DB.Execute(kLookup, "Select DefaultValue from MapScriptOptions where FileName = ? and OptionID = ? LIMIT 1"))
	{
		kLookup.Bind(1, CvPreGame::mapScriptName().c_str());
		kLookup.Bind(2, szOptionName);
		if(kLookup.Step())
		{
			iValue = kLookup.GetInt(0);
			//Didn't find the option, push it.
			s_MapOptions.push_back(CustomOption(szOptionName, iValue));
			return true;
		}
	}

	return false;
}

const std::vector<CustomOption>& GetMapOptions()
{
	return s_MapOptions;
}

bool SetMapOption(const char* szOptionName, int iValue)
{
	//DO NOT ALLOW NULL
	if(szOptionName == NULL)
		return false;

	for(size_t i = 0; i < s_MapOptions.size(); i++)
	{
		const CustomOption& option = s_MapOptions[i];
		size_t bytes = 0;
		const char* szCurrentOptionName = option.GetName(bytes);
		if(strncmp(szCurrentOptionName, szOptionName, bytes) == 0)
		{
			//I'd like to just set the value here, but that doesn't seem possible
			//so instead, create a new CustomOption type and assign it to this index.
			s_MapOptions[i] = CustomOption(szOptionName, iValue);
			return true;
		}

	}

	//Didn't find the option, push it.
	s_MapOptions.push_back(CustomOption(szOptionName, iValue));
	return true;
}

bool SetMapOptions(const std::vector<CustomOption>& mapOptions)
{
	s_MapOptions = mapOptions;
	return true;
}

unsigned int mapRandomSeed()
{
	return s_mapRandomSeed;
}

bool loadWBScenario()
{
	return s_loadWBScenario;
}

bool overrideScenarioHandicap()
{
	return s_overrideScenarioHandicap;
}

const CvString& mapScriptName()
{
	return s_mapScriptName;
}

int maxCityElimination()
{
	return s_maxCityElimination;
}

int maxTurns()
{
	return s_maxTurns;
}

MinorCivTypes minorCivType(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_minorCivTypes[p];
	return NO_MINORCIV;
}

bool multiplayerAIEnabled()
{
	return s_multiplayerAIEnabled;
}
bool multiplayerOptionEnabled(MultiplayerOptionTypes o)
{
	if(o < NUM_MPOPTION_TYPES)
		return s_multiplayerOptions[o];
	return false;
}

const std::vector<bool>& multiplayerOptions()
{
	return s_multiplayerOptions;
}

int netID(PlayerTypes p)
{
	if(p < MAX_PLAYERS)
		return s_netIDs[p];
	return -1;
}

const CvString& nickname(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_nicknames[p];
	static const CvString none("");
	return none;
}

const CvString& nicknameDisplayed(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_displayNicknames[p];
	static const CvString none("");
	return none;
}

int numDefinedPlayers()
{
	int iCount = 0;
	for(int i = 0; i < MAX_CIV_PLAYERS; ++i)
	{
		if((civilization((PlayerTypes)i) != NO_CIVILIZATION) && (leaderHead((PlayerTypes)i) != NO_LEADER))
		{
			iCount++;
		}
	}
	return iCount;
}

int numHumans()
{
	int iNumHumans = 0;
	for(int i = 0; i < MAX_CIV_PLAYERS; ++i)
	{
		if(isHuman((PlayerTypes)i))
		{
			++iNumHumans;
		}
	}
	return iNumHumans;
}

int numMinorCivs()
{
	return s_numMinorCivs;
}

int pitBossTurnTime()
{
	return s_pitBossTurnTime;
}

PlayerColorTypes playerColor(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_playerColors[p];
	return NO_PLAYERCOLOR;
}

bool privateGame()
{
	return s_privateGame;
}

HandicapTypes quickHandicap()
{
	return s_quickHandicap;
}

bool quickCombat()
{
	return s_quickCombat;
}

bool quickCombatDefault()
{
	return s_quickCombatDefault;
}

bool quickMovement()
{
	int iValue = 0;
	if(GetGameOption(GAMEOPTION_QUICK_MOVEMENT_KEY, iValue))
		return iValue != 0;
	return false;
}

bool quickstart()
{
	return s_quickstart;
}

bool randomWorldSize()
{
	return s_randomWorldSize;
}

bool randomMapScript()
{
	return s_randomMapScript;
}

void readArchive(FDataStream& loadFrom, bool bReadVersion)
{
	uint uiVersion = 0;
	if(bReadVersion)
		loadFrom >> uiVersion;

	loadFrom >> s_activePlayer;
	loadFrom >> s_adminPassword;
	loadFrom >> s_alias;
	loadFrom >> s_artStyles;
	loadFrom >> s_autorun;
	loadFrom >> s_autorunTurnDelay;
	loadFrom >> s_autorunTurnLimit;
	loadFrom >> s_bandwidth;
	loadFrom >> s_calendar;
	loadFrom >> s_calendarInfo;
	loadFrom >> s_civAdjectives;
	loadFrom >> s_civDescriptions;
	if (uiVersion <= 2)						// Read the civilization indices.  Newer versions have the keys in the header.
		loadFrom >> s_civilizations;

	loadFrom >> s_civPasswords;
	loadFrom >> s_civShortDescriptions;
	loadFrom >> s_climate;
	loadFrom >> s_climateInfo;
	loadFrom >> s_era;
	loadFrom >> s_emailAddresses;
	loadFrom >> s_endTurnTimerLength;
	loadFrom >> s_flagDecals;
	loadFrom >> s_DEPRECATEDforceControls;
	loadFrom >> s_gameMode;
	loadFrom >> s_gameName;
	loadFrom >> s_gameSpeed;
	loadFrom >> s_gameStarted;
	loadFrom >> s_gameTurn;
	if(uiVersion >= 1)
	{
		loadFrom >> s_gameType;
		loadFrom >> s_gameMapType;
	}
	else
	{
		// Old enum that was trying to cram too much info into a sequence
		enum GameType
		{
		    GAME_NONE = -1,

		    GAME_SP_NEW,
		    GAME_SP_SCENARIO,
		    GAME_SP_LOAD,
		    GAME_MP_NEW,
		    GAME_MP_SCENARIO,
		    GAME_MP_LOAD,
		    GAME_HOTSEAT_NEW,
		    GAME_HOTSEAT_SCENARIO,
		    GAME_HOTSEAT_LOAD,
		    GAME_PBEM_NEW,
		    GAME_PBEM_SCENARIO,
		    GAME_PBEM_LOAD,

		    NUM_GAMETYPES
		};

		int iDummy = 0;
		loadFrom >> iDummy;
		GameType eGameType = static_cast<GameType>(iDummy);

		// Convert the old one to the new format

		if((eGameType == GAME_HOTSEAT_NEW) || (eGameType == GAME_HOTSEAT_SCENARIO) || (eGameType == GAME_HOTSEAT_LOAD))
			s_gameType = GAME_HOTSEAT_MULTIPLAYER;
		else if((eGameType == GAME_MP_NEW) || (eGameType == GAME_MP_SCENARIO) || (eGameType == GAME_MP_LOAD))
			s_gameType = GAME_NETWORK_MULTIPLAYER;
		else if((eGameType == GAME_SP_NEW) || (eGameType == GAME_SP_SCENARIO) || (eGameType == GAME_SP_LOAD))
			s_gameType = GAME_SINGLE_PLAYER;
		else
		{
			if(s_gameType == GAME_TYPE_NONE)
				s_gameType = GAME_SINGLE_PLAYER;		// I've seen this saved as -1 (GAME_NONE)
		}

		s_gameMapType = GAME_USER_PARAMETERS;
		if((eGameType == GAME_MP_SCENARIO) || (eGameType == GAME_HOTSEAT_SCENARIO) || (eGameType == GAME_SP_SCENARIO))
			s_gameMapType = GAME_SCENARIO;

	}
	loadFrom >> s_gameUpdateTime;
	loadFrom >> s_handicaps;
	if(uiVersion >= 6){
		loadFrom >> s_lastHumanHandicaps;
	}

	loadFrom >> s_isEarthMap;
	loadFrom >> s_isInternetGame;
	if(uiVersion == 0)
	{
		bool bNetworkMultiplayerGame = false;
		loadFrom >> bNetworkMultiplayerGame;
		if(bNetworkMultiplayerGame && s_gameType != GAME_HOTSEAT_MULTIPLAYER)
		{
			s_gameType = GAME_NETWORK_MULTIPLAYER;
		}
	}
	if (uiVersion <= 2)						// Read the leader indices.  Newer versions have the keys in the header.
		loadFrom >> s_leaderHeads;
	loadFrom >> s_leaderNames;
	loadFrom >> s_loadFileName;
	loadFrom >> s_localPlayerEmailAddress;
	loadFrom >> s_mapNoPlayers;
	loadFrom >> s_mapRandomSeed;
	loadFrom >> s_loadWBScenario;
	loadFrom >> s_overrideScenarioHandicap;
	loadFrom >> s_mapScriptName;
	loadFrom >> s_maxCityElimination;
	loadFrom >> s_maxTurns;
	loadFrom >> s_numMinorCivs;
	if (uiVersion >= 3)
	{
		CvInfosSerializationHelper::ReadTypeArrayDBLookup<MinorCivTypes>(loadFrom, s_minorCivTypes, "MinorCivilizations"); 
	}
	else
		loadFrom >> s_minorCivTypes;
	loadFrom >> s_minorNationCivs;
	loadFrom >> s_dummyvalue;
	loadFrom >> s_multiplayerOptions;
	loadFrom >> s_netIDs;
	loadFrom >> s_nicknames;
	loadFrom >> s_numVictoryInfos;
	loadFrom >> s_pitBossTurnTime;
	loadFrom >> s_playableCivs;
	if (uiVersion >= 3)
	{
		CvInfosSerializationHelper::ReadTypeArrayDBLookup<PlayerColorTypes>(loadFrom, s_playerColors, "PlayerColors");
	}
	else
		loadFrom >> s_playerColors;

	loadFrom >> s_privateGame;
	loadFrom >> s_quickCombat;
	loadFrom >> s_quickCombatDefault;
	loadFrom >> s_quickHandicap;
	loadFrom >> s_quickstart;
	loadFrom >> s_randomWorldSize;
	loadFrom >> s_randomMapScript;
	loadFrom >> s_readyPlayers;
	loadFrom >> s_seaLevel;
	loadFrom >> s_seaLevelInfo;
	loadFrom >> s_dummyvalue2;
	loadFrom >> s_slotClaims;
	loadFrom >> s_slotStatus;
	loadFrom >> s_smtpHost;
	loadFrom >> s_syncRandomSeed;
	loadFrom >> s_targetScore;
	loadFrom >> s_teamTypes;
	loadFrom >> s_transferredMap;
	loadFrom >> s_turnTimer;
	loadFrom >> s_turnTimerType;
	loadFrom >> s_bCityScreenBlocked;
	loadFrom >> s_victories;
	loadFrom >> s_whiteFlags;
	if(uiVersion <= 1)
		s_worldInfo.readFromVersion0(loadFrom);
	else
		loadFrom >> s_worldInfo;

	loadFrom >> s_worldSize;
	loadFrom >> s_GameOptions;
	loadFrom >> s_MapOptions;
	loadFrom >> s_versionString;

	if (uiVersion >= 4)
		loadFrom >> s_turnNotifySteamInvite;

	if (uiVersion >= 5){
		loadFrom >> s_turnNotifyEmail;
		loadFrom >> s_turnNotifyEmailAddress;
	}

	// Rebuild the hash lookup to the options
	s_GameOptionsHash.clear();
	for(size_t i = 0; i < s_GameOptions.size(); i++)
	{
		const CustomOption& kOption = s_GameOptions[i];
		s_GameOptionsHash[FStringHash(kOption.GetName())] = i;
	}
}

void read(FDataStream& loadFrom, bool bReadVersion)
{
	loadSlotHints(loadFrom, bReadVersion);
	readArchive(loadFrom, bReadVersion);

	SyncGameOptionsWithEnumList();
	for(int i = 0; i < MAX_PLAYERS; ++i)
	{
		bindLeaderKeys((PlayerTypes)i);
		bindCivilizationKeys((PlayerTypes)i);
	}
	ReseatConnectedPlayers();
}

void resetGame()
{
	// Descriptive strings about game and map
	s_gameStartType = GAME_NEW;
	s_gameMapType = GAME_USER_PARAMETERS;

	s_gameStarted = false;
	s_gameName = "";
	s_adminPassword = "";
	s_mapScriptName = "";
	s_loadWBScenario = false;

	s_mapNoPlayers = false;

	// Standard game parameters
	s_climate = ClimateTypes(0); // NO_ option?
	s_seaLevel = SeaLevelTypes(1); // NO_ option?
	s_era = EraTypes(GD_INT_GET(STANDARD_ERA)); // NO_ option?
	s_gameSpeed = GameSpeedTypes(GD_INT_GET(STANDARD_GAMESPEED)); // NO_ option?
	s_turnTimerType = TurnTimerTypes(4); // NO_ option?
	s_calendar = CalendarTypes(0); // NO_ option?

	// Data-defined victory conditions 
	s_numVictoryInfos = DB.Count("Victories");
	s_victories.assign(s_numVictoryInfos, true);

	// Standard game options
	s_multiplayerOptions.assign(NUM_MPOPTION_TYPES, false);

	//s_statReporting = false;

	// Game turn mgmt
	s_gameTurn = 0;
	s_maxTurns = 0;
	s_pitBossTurnTime = 0;
	s_targetScore = 0;

	// City Elimination
	s_maxCityElimination = 0;

	s_numMinorCivs = -1;

	// Unsaved game data
	s_syncRandomSeed = 0;
	s_mapRandomSeed = 0;
	s_activePlayer = NO_PLAYER;

	s_quickstart = false;
	s_randomWorldSize = false;
	s_randomMapScript = false;
	s_isEarthMap = false;
	s_autorun = false;
	s_transferredMap = false;

	s_autorunTurnLimit = 0;
	s_autorunTurnDelay = 0.f;
	s_gameUpdateTime = 0;

	s_bandwidth = NO_BANDWIDTH;
	s_quickHandicap = NO_HANDICAP;

	s_alias = "";
	s_localPlayerEmailAddress = "";
	s_loadFileName = "";
	s_loadFileStorage = STORAGE_LOCAL;

	s_endTurnTimerLength = 0.0f;

	s_privateGame = false;
	s_isInternetGame = false;
	s_knownPlayersTable.clear();

	ResetMapOptions();
	ResetGameOptions();
}

void ResetGameOptions()
{
	s_GameOptions.clear();
	s_GameOptionsHash.clear();
	for(size_t i = 0; i < s_GameOptions.size(); i++)
	{
		const CustomOption& kOption = s_GameOptions[i];
		s_GameOptionsHash[FStringHash(kOption.GetName())] = i;
	}
	SyncGameOptionsWithEnumList();
}
void ResetMapOptions()
{
	s_MapOptions.clear();
}

void resetPlayer(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		// Only copy over saved data

		// Civ details
		setLeaderName(p, "");
		setCivilizationDescription(p, "");
		setCivilizationShortDescription(p, "");
		setCivilizationAdjective(p, "");
		setCivilizationPassword(p, "");
		setEmailAddress(p, "");
		setWhiteFlag(p, false);
		setCivilization(p, NO_CIVILIZATION);
		setLeaderHead(p, NO_LEADER);
		setMinorCivType(p, NO_MINORCIV);
		setTeamType(p, (TeamTypes)p); // JAR : Whisky Tango Foxtrot?

		if(isNetworkMultiplayerGame())
			setHandicap(p, (HandicapTypes)GD_INT_GET(MULTIPLAYER_HANDICAP));
		else
			setHandicap(p, (HandicapTypes)GD_INT_GET(STANDARD_HANDICAP));

		setLastHumanHandicap(p, NO_HANDICAP);
		setPlayerColor(p, NO_PLAYERCOLOR);
		setArtStyle(p, NO_ARTSTYLE);


		// Slot data
		setSlotStatus(p, SS_CLOSED);
		setSlotClaim(p, SLOTCLAIM_UNASSIGNED);

		// Civ flags
		setPlayable(p, false);
		setMinorCiv(p, false);

		// Unsaved player data
		setNetID(p, -1);
		setReady(p, false);
		setNickname(p, "");

		clearDLCAvailable(p);
	}
}

void resetPlayers()
{
	for(int i = 0; i < MAX_PLAYERS; ++i)
	{
		resetPlayer((PlayerTypes)i);
	}
}

void resetSlots()
{
	//setNumMinorCivs( CvPreGame::worldInfo().getDefaultMinorCivs() );
	// adjust player slots
	int suggestedPlayerCount = CvPreGame::worldInfo().getDefaultPlayers();
	int slotsAssigned = 0;
	int i = 0;
	for(i = 0; i < MAX_CIV_PLAYERS; ++i)
	{
		PlayerTypes p = static_cast<PlayerTypes>(i);
		if(!isNetworkMultiplayerGame())
		{
			if(slotsAssigned < suggestedPlayerCount)
			{
				// don't reset slots taken by human players
				SlotStatus s = slotStatus(p);
				if(s != SS_TAKEN)
				{
					// the player may have already assigned
					// a civ for this computer slot, don't
					// overwrite those settings.
					if(s != SS_COMPUTER)
					{
						setSlotStatus(p, SS_COMPUTER);
						setSlotClaim(p, SLOTCLAIM_ASSIGNED);
						setCivilization(p, NO_CIVILIZATION); // defaults to random civ
					}
				}

				//S.S:  In single player games, slot 1 *should* be marked as taken but it currently is not.
				if(slotStatus(p) == SS_COMPUTER && i != 0)
					setHandicap(p, (HandicapTypes)GD_INT_GET(AI_HANDICAP));
				else
				{
					if(isHotSeatGame())
						setHandicap(p, (HandicapTypes)GD_INT_GET(MULTIPLAYER_HANDICAP));
				}
				++slotsAssigned;
			}
			else
			{
				resetPlayer(p);
				// setup an observer slot
				if(p >= suggestedPlayerCount && p < MAX_MAJOR_CIVS)
					setSlotStatus(p, SS_OBSERVER);
			}
		}
		else
		{
			// don't reset slots taken by human players, even if we exceed the suggested slots
			SlotStatus s = slotStatus(p);
			if(s != SS_TAKEN && slotsAssigned >= suggestedPlayerCount)
			{
				resetPlayer(p);
				// setup an observer slot
				if(p >= suggestedPlayerCount && p < MAX_MAJOR_CIVS)
					setSlotStatus(p, SS_OBSERVER);
			}
			else
			{
				if(s != SS_TAKEN)
				{
					if(slotsAssigned < suggestedPlayerCount)
					{
						// the player may have already assigned
						// a civ for this computer slot, don't
						// overwrite those settings.
						setSlotStatus(p, SS_OPEN);
						setSlotClaim(p, SLOTCLAIM_ASSIGNED);
						setCivilization(p, NO_CIVILIZATION); // defaults to random civ
					}
				}
				if(slotStatus(p) == SS_COMPUTER)
					setHandicap(p, (HandicapTypes)GD_INT_GET(AI_HANDICAP));
				else
					setHandicap(p, (HandicapTypes)GD_INT_GET(MULTIPLAYER_HANDICAP));

				++slotsAssigned;
			}
		}
	}
}

std::vector<SlotClaim> s_savedSlotClaims;
std::vector<SlotStatus> s_savedSlotStatus;
std::vector<CivilizationTypes> s_savedCivilizations;
std::vector<LeaderHeadTypes> s_savedLeaderHeads;
PlayerTypes s_savedLocalPlayer;
std::vector<CvString> s_savedCivilizationKeys(MAX_PLAYERS);
std::vector<GUID> s_savedCivilizationPackageID(MAX_PLAYERS);
std::vector<bool> s_savedCivilizationKeysAvailable(MAX_PLAYERS);
std::vector<bool> s_savedCivilizationKeysPlayable(MAX_PLAYERS);

std::vector<CvString> s_savedLeaderKeys(MAX_PLAYERS);
std::vector<GUID> s_savedLeaderPackageID(MAX_PLAYERS);
std::vector<bool> s_savedLeaderKeysAvailable(MAX_PLAYERS);
std::vector<PackageIDList> s_savedDLCPackagesAvailable(MAX_PLAYERS);

std::vector<KnownPlayersBitArray> s_savedKnownPlayersTable;

//	------------------------------------------------------------------------------------
void restoreSlots()
{
	s_slotClaims = s_savedSlotClaims;
	s_slotStatus = s_savedSlotStatus;
	s_civilizations = s_savedCivilizations;
	s_leaderHeads = s_savedLeaderHeads;

	s_civilizationKeys = s_savedCivilizationKeys;
	s_civilizationPackageID = s_savedCivilizationPackageID;
	s_civilizationKeysAvailable = s_savedCivilizationKeysAvailable;
	s_civilizationKeysPlayable = s_savedCivilizationKeysPlayable;

	s_leaderKeys = s_savedLeaderKeys;
	s_leaderPackageID = s_savedLeaderPackageID;
	s_leaderKeysAvailable =  s_savedLeaderKeysAvailable;
	s_DLCPackagesAvailable = s_savedDLCPackagesAvailable;

	//not sure if this is necessary but won't hurt
	s_knownPlayersTable = s_savedKnownPlayersTable;
	setActivePlayer(s_savedLocalPlayer);
}

//	------------------------------------------------------------------------------------
//  Save all the information needed for the current slot setup that needs to be restored
//	after a hot-join/re-sync
void saveSlots()
{
	s_savedSlotClaims = s_slotClaims;
	s_savedSlotStatus = s_slotStatus;
	s_savedCivilizations = s_civilizations;
	s_savedLeaderHeads = s_leaderHeads;
	s_savedLocalPlayer = activePlayer();

	s_savedCivilizationKeys = s_civilizationKeys;
	s_savedCivilizationPackageID = s_civilizationPackageID;
	s_savedCivilizationKeysAvailable = s_civilizationKeysAvailable;
	s_savedCivilizationKeysPlayable = s_civilizationKeysPlayable;

	s_savedLeaderKeys = s_leaderKeys;
	s_savedLeaderPackageID = s_leaderPackageID;
	s_savedLeaderKeysAvailable = s_leaderKeysAvailable;
	s_savedDLCPackagesAvailable = s_DLCPackagesAvailable;

	//not sure if this is necessary but won't hurt
	s_savedKnownPlayersTable = s_knownPlayersTable;
}

SeaLevelTypes seaLevel()
{
	return s_seaLevel;
}

const CvSeaLevelInfo& seaLevelInfo()
{
	if(s_seaLevel != s_seaLevelInfo.GetID())
	{
		Database::SingleResult kResult;
		DB.SelectAt(kResult, "SeaLevels", s_seaLevel);
		s_seaLevelInfo.CacheResult(kResult);
	}
	return s_seaLevelInfo;
}

void setActivePlayer(PlayerTypes p)
{
	s_activePlayer = p;
}

void setAdminPassword(const CvString& p)
{
	s_adminPassword = p;
}

void setAlias(const CvString& a)
{
	s_alias = a;
}

void setArtStyle(PlayerTypes p, ArtStyleTypes a)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_artStyles[p] = a;
}

void setAutorun(bool isEnabled)
{
	s_autorun = isEnabled;
}

void setAutorunTurnDelay(float turnDelay)
{
	s_autorunTurnDelay = turnDelay;
}

void setAutorunTurnLimit(int turnLimit)
{
	s_autorunTurnLimit = turnLimit;
}

void setBandwidth(BandwidthType b)
{
	s_bandwidth = b;
}

void setBandwidth(const CvString& b)
{
	if(b== "modem")
	{
		setBandwidth(BANDWIDTH_MODEM);
	}
	else if(b== "broadband")
	{
		setBandwidth(BANDWIDTH_BROADBAND);
	}
	else
	{
		setBandwidth(BANDWIDTH_MODEM);
	}
}

void setCalendar(CalendarTypes c)
{
	s_calendar = c;
}

void setCalendar(const CvString& c)
{
	Database::SingleResult kResult;
	if(!DB.SelectAt(kResult, "Calendars", "Type", c.c_str()))
	{
		ASSERT_DEBUG(false, "Cannot find calendar info.");
	}
	s_calendarInfo.CacheResult(kResult);

	s_calendar = (CalendarTypes)s_calendarInfo.GetID();
}

void setCivilization(PlayerTypes p, CivilizationTypes c)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		s_civilizations[p] = c;

		bool bFailed = bindCivilizationKeys(p);

		if(bFailed)
		{
			s_civilizations[p] = NO_CIVILIZATION;
			s_civilizationKeys[p].clear();
			ClearGUID(s_civilizationPackageID[p]);
			s_civilizationKeysAvailable[p] = true;	// If the key is empty, we assume the selection is in the 'random' state, so it is available.
			s_civilizationKeysPlayable[p] = true;
		}
	}
}

void setCivilizationAdjective(PlayerTypes p, const CvString& a)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_civAdjectives[p] = a;
}

void setCivilizationDescription(PlayerTypes p, const CvString& d)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_civDescriptions[p] = d;
}

void setCivilizationPassword(PlayerTypes p, const CvString& pwd)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_civPasswords[p] = pwd;
}

void setCivilizationShortDescription(PlayerTypes p, const CvString& d)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_civShortDescriptions[p] = d;
}

void setClimate(ClimateTypes c)
{
	s_climate = c;
}

void setClimate(const CvString& c)
{
	//Query
	Database::SingleResult kResult;
	DB.SelectAt(kResult, "Climates", "Type", c);
	s_climateInfo.CacheResult(kResult);

	s_climate = (ClimateTypes)s_climateInfo.GetID();
}

void setCustomWorldSize(int iWidth, int iHeight, int iPlayers, int iMinorCivs)
{
	ASSERT_DEBUG(iWidth >= 20 && iHeight >= 20, "Cannot have a map that small!");

	const int iArea = iWidth * iHeight;

	CvWorldInfo kClosestSizeType;
	int iSmallestAreaDifference = 64000; // Arbitrarily large at start

	Database::Results kWorldSizes;
	DB.SelectAll(kWorldSizes, "Worlds");
	while(kWorldSizes.Step())
	{
		CvWorldInfo kInfo;
		kInfo.CacheResult(kWorldSizes);

		int iSizeTypeArea = kInfo.getGridWidth() * kInfo.getGridHeight();
		int iAreaDifference = abs(iArea - iSizeTypeArea);
		if(iAreaDifference < iSmallestAreaDifference)
		{
			iSmallestAreaDifference = iAreaDifference;
			kClosestSizeType = kInfo;
		}
	}

	s_worldSize = (WorldSizeTypes)kClosestSizeType.GetID();
	if(iPlayers > 0 || !CvPreGame::mapNoPlayers())
		s_worldInfo = CvWorldInfo::CreateCustomWorldSize(kClosestSizeType, iWidth, iHeight, iPlayers, iMinorCivs);
	else
		s_worldInfo = CvWorldInfo::CreateCustomWorldSize(kClosestSizeType, iWidth, iHeight);
}

void setEmailAddress(PlayerTypes p, const CvString& address)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_emailAddresses[p] = address;
}

void setEmailAddress(const CvString& address)
{
	s_localPlayerEmailAddress = address;
}

void setEndTurnTimerLength(float f)
{
	s_endTurnTimerLength = f;
}

void setEra(EraTypes e)
{
	s_era = e;
}

void setEra(const CvString& e)
{
	for(int i = 0; i < GC.getNumEraInfos(); i++)
	{
		const EraTypes eEra = static_cast<EraTypes>(i);
		CvEraInfo* pkEraInfo = GC.getEraInfo(eEra);
		if(pkEraInfo)
		{
			if(_stricmp(e.c_str(), pkEraInfo->GetType()) == 0)
			{
				setEra(eEra);
			}
		}

	}
}

void setGameMode(GameMode g)
{
	s_gameMode = g;
}

void setGameSpeed(GameSpeedTypes g)
{
	s_gameSpeed = g;

	switch(s_gameSpeed)
	{
	case 0: // GAMESPEED_MARATHON
	{
		// No turn timer
	}
	break;
	case 1: //  GAMESPEED_EPIC
	{
		// TURN_TIMER_SNAIL, 1
		setTurnTimer(static_cast<TurnTimerTypes>(1));
	}
	break;
	case 2: // GAMESPEED_STANDARD
	{
		// TUNRTIMER_MEDIUM, 3
		setTurnTimer(static_cast<TurnTimerTypes>(3));
	}
	break;
	case 3: // GAMESPEED_QUICK
	{
		// TURNTIMER_FAST, 4
		setTurnTimer(static_cast<TurnTimerTypes>(4));
	}
	break;
	default:
		break;
	}
}

void setGameSpeed(const CvString& g)
{
	for(int i = 0; i < GC.getNumGameSpeedInfos(); i++)
	{
		const GameSpeedTypes eGameSpeed = static_cast<GameSpeedTypes>(i);
		CvGameSpeedInfo* pkGameSpeedInfo = GC.getGameSpeedInfo(eGameSpeed);
		if(pkGameSpeedInfo)
		{
			if(_stricmp(g.GetCString(), pkGameSpeedInfo->GetType()) == 0)
			{
				setGameSpeed((GameSpeedTypes)i);
			}
		}
	}
}

void setGameName(const CvString& g)
{
	s_gameName = g;
}

void setGameStarted(bool started)
{
	if (!s_gameStarted && started)
	{
		onGameStarted();
	}
	s_gameStarted = started;
}

/// Hack to manually override the engine's random selection of civilizations and City-States
/// This is necessary for custom City-State selection logic to function
void onGameStarted()
{
	// First loop: collect all major civs already chosen and the indices of the players that still need to choose
	vector<CivilizationTypes> vCivsChosen;
	vector<PlayerTypes> vMajorsToChoose;
	for (int p = 0; p < MAX_MAJOR_CIVS; p++)
	{
		const PlayerTypes eID = static_cast<PlayerTypes>(p);
		SlotStatus s = slotStatus(eID);
		if (s == SS_TAKEN || s == SS_COMPUTER)
		{
			const CivilizationTypes eCiv = civilization(eID);
			if (eCiv == NO_CIVILIZATION)
				vMajorsToChoose.push_back(eID);
			else if (std::find(vCivsChosen.begin(), vCivsChosen.end(), eCiv) == vCivsChosen.end()) // These checks are necessary because duplicates can exist!
				vCivsChosen.push_back(eCiv);
		}
	}

	if (vMajorsToChoose.size() > 0)
	{
		// Second loop: collect all civs in the database, and see which ones aren't chosen
		vector<CivilizationTypes> vAICivs, vHumanCivs, vAICivsNotChosen, vHumanCivsNotChosen;
		for (int c = 0; c < GC.getNumCivilizationInfos(); c++)
		{
			CivilizationTypes eCiv = static_cast<CivilizationTypes>(c);
			CvCivilizationInfo* pkCivilization = GC.getCivilizationInfo(eCiv);
			if (eCiv != NO_CIVILIZATION && pkCivilization)
			{
				bool bChosen = std::find(vCivsChosen.begin(), vCivsChosen.end(), eCiv) != vCivsChosen.end();

				if (pkCivilization->isPlayable())
				{
					vHumanCivs.push_back(eCiv);
					if (!bChosen && std::find(vHumanCivsNotChosen.begin(), vHumanCivsNotChosen.end(), eCiv) == vHumanCivsNotChosen.end())
						vHumanCivsNotChosen.push_back(eCiv);
				}

				if (pkCivilization->isAIPlayable())
				{
					vAICivs.push_back(eCiv);
					if (!bChosen && std::find(vAICivsNotChosen.begin(), vAICivsNotChosen.end(), eCiv) == vAICivsNotChosen.end())
						vAICivsNotChosen.push_back(eCiv);
				}
			}
		}

		// Third loop: for each player that needs to choose a civ, choose one
		int iCounter = 1;
		for (std::vector<PlayerTypes>::iterator it = vMajorsToChoose.begin(); it != vMajorsToChoose.end(); it++)
		{
			// First, try to pick a random valid civ that wasn't chosen
			// If that fails, try to pick a random valid civ that was chosen
			CivilizationTypes eRandomCiv = NO_CIVILIZATION;
			if (isHuman(*it))
			{
				if (vHumanCivsNotChosen.size() > 0)
				{
					uint uRand = GC.getGame().urandLimitExclusive(vHumanCivsNotChosen.size(), CvSeeder::fromRaw(0x3153de36).mix(iCounter));
					eRandomCiv = static_cast<CivilizationTypes>(vHumanCivsNotChosen[uRand]);
					vHumanCivsNotChosen.erase(vHumanCivsNotChosen.begin() + uRand);
					for (uint ui = 0; ui < vAICivsNotChosen.size(); ui++)
					{
						if (vAICivsNotChosen[ui] == eRandomCiv)
						{
							vAICivsNotChosen.erase(vAICivsNotChosen.begin() + ui);
							break;
						}
					}
				}
				else if (vHumanCivs.size() > 0)
				{
					uint uRand = GC.getGame().urandLimitExclusive(vHumanCivs.size(), CvSeeder::fromRaw(0x298ac75d).mix(iCounter));
					eRandomCiv = static_cast<CivilizationTypes>(vHumanCivs[uRand]);
				}
				else
					UNREACHABLE();
			}
			else
			{
				if (vAICivsNotChosen.size() > 0)
				{
					uint uRand = GC.getGame().urandLimitExclusive(vAICivsNotChosen.size(), CvSeeder::fromRaw(0x9f2b8561).mix(iCounter));
					eRandomCiv = static_cast<CivilizationTypes>(vAICivsNotChosen[uRand]);
					vAICivsNotChosen.erase(vAICivsNotChosen.begin() + uRand);
					for (uint ui = 0; ui < vHumanCivsNotChosen.size(); ui++)
					{
						if (vHumanCivsNotChosen[ui] == eRandomCiv)
						{
							vHumanCivsNotChosen.erase(vHumanCivsNotChosen.begin() + ui);
							break;
						}
					}
				}
				else if (vAICivs.size() > 0)
				{
					uint uRand = GC.getGame().urandLimitExclusive(vAICivs.size(), CvSeeder::fromRaw(0x52094553).mix(iCounter));
					eRandomCiv = static_cast<CivilizationTypes>(vHumanCivs[uRand]);
				}
				else
					UNREACHABLE();
			}
			setCivilization(*it, eRandomCiv);
			iCounter++;
		}
	}

	// Fourth loop: Gather all the player indices for City-States that need choosing
	vector<PlayerTypes> vMinorsToChoose;
	int iCultured = 0;
	int iMilitaristic = 0;
	int iMaritime = 0;
	int iMercantile = 0;
	int iReligious = 0;
	for (int p = MAX_MAJOR_CIVS; p < MAX_CIV_PLAYERS; p++)
	{
		const PlayerTypes eID = static_cast<PlayerTypes>(p);
		const MinorCivTypes eMinorCiv = minorCivType(eID);
		if (eMinorCiv == NO_MINORCIV)
		{
			vMinorsToChoose.push_back(eID);
		}
		else if (MOD_BALANCE_CITY_STATE_TRAITS)
		{
			CvMinorCivInfo* pkCityState = GC.getMinorCivInfo(eMinorCiv);
			MinorCivTraitTypes eTrait = (MinorCivTraitTypes)pkCityState->GetMinorCivTrait();
			switch (eTrait)
			{
			case MINOR_CIV_TRAIT_CULTURED:
				iCultured++;
				break;
			case MINOR_CIV_TRAIT_MILITARISTIC:
				iMilitaristic++;
				break;
			case MINOR_CIV_TRAIT_MARITIME:
				iMaritime++;
				break;
			case MINOR_CIV_TRAIT_MERCANTILE:
				iMercantile++;
				break;
			case MINOR_CIV_TRAIT_RELIGIOUS:
				iReligious++;
				break;
			default:
				UNREACHABLE();
			}
		}
	}

	if (vMinorsToChoose.size() > 0)
	{
		// Fifth loop: Go through all City-States in the database and exclude ones which are already chosen or blocked by MajorBlocksMinor
		vector<MinorCivTypes> vCultured, vMilitaristic, vMaritime, vMercantile, vReligious;
		vector<MinorCivTypes> vAvailableCityStates = GetAvailableMinorCivTypes(vCultured, vMilitaristic, vMaritime, vMercantile, vReligious);

		// Sixth loop: Loop through all City-State slots and fill them with random valid City-States
		int iCounter = 1;
		for (std::vector<PlayerTypes>::iterator it = vMinorsToChoose.begin(); it != vMinorsToChoose.end(); it++)
		{
			if (!MOD_BALANCE_CITY_STATE_TRAITS && !MOD_BALANCE_VP)
			{
				// Not enough City-States in the database.
				if (vAvailableCityStates.empty())
					break;

				uint uRand = GC.getGame().urandLimitExclusive(vAvailableCityStates.size(), CvSeeder::fromRaw(0xd73596c3).mix(iCounter));
				MinorCivTypes eChoice = static_cast<MinorCivTypes>(vAvailableCityStates[uRand]);
				setMinorCivType(*it, eChoice);
				vAvailableCityStates.erase(vAvailableCityStates.begin() + uRand);
			}
			else
			{
				// Which categories are valid choices to maintain the ratio?
				vector<MinorCivTraitTypes> vValidCategories;
				bool bCulturedValid = vCultured.size() > 0;
				bool bMilitaristicValid = vMilitaristic.size() > 0;
				bool bMaritimeValid = vMaritime.size() > 0;
				bool bMercantileValid = vMercantile.size() > 0;
				bool bReligiousValid = vReligious.size() > 0;

				// No City-States left in any category?
				if (!bCulturedValid && !bMilitaristicValid && !bMaritimeValid && !bMercantileValid && !bReligiousValid)
					break;

				// We're balancing the different types of City-States.
				// Aim for a ratio of 1 Cultured / 1 Militaristic / 1 Maritime / 1 Mercantile / 1 Religious
				if (MOD_BALANCE_CITY_STATE_TRAITS)
				{
					// Which categories are valid to maintain the ratio?
					if (bCulturedValid)
					{
						if (iCultured > iMilitaristic && bMilitaristicValid)
							bCulturedValid = false;
						else if (iCultured > iMaritime && bMaritimeValid)
							bCulturedValid = false;
						else if (iCultured > iMercantile && bMercantileValid)
							bCulturedValid = false;
						else if (iCultured > iReligious && bReligiousValid)
							bCulturedValid = false;

						if (bCulturedValid)
							vValidCategories.push_back(MINOR_CIV_TRAIT_CULTURED);
					}
					if (bMilitaristicValid)
					{
						if (iMilitaristic > iCultured && bCulturedValid)
							bMilitaristicValid = false;
						else if (iMilitaristic > iMaritime && bMaritimeValid)
							bMilitaristicValid = false;
						else if (iMilitaristic > iMercantile && bMercantileValid)
							bMilitaristicValid = false;
						else if (iMilitaristic > iReligious && bReligiousValid)
							bMilitaristicValid = false;

						if (bMilitaristicValid)
							vValidCategories.push_back(MINOR_CIV_TRAIT_MILITARISTIC);
					}
					if (bMaritimeValid)
					{
						if (iMaritime > iCultured && bCulturedValid)
							bMaritimeValid = false;
						else if (iMaritime > iMilitaristic && bMilitaristicValid)
							bMaritimeValid = false;
						else if (iMaritime > iMercantile && bMercantileValid)
							bMaritimeValid = false;
						else if (iMaritime > iReligious && bReligiousValid)
							bMaritimeValid = false;

						if (bMaritimeValid)
							vValidCategories.push_back(MINOR_CIV_TRAIT_MARITIME);
					}
					if (bMercantileValid)
					{
						if (iMercantile > iCultured && bCulturedValid)
							bMercantileValid = false;
						else if (iMercantile > iMilitaristic && bMilitaristicValid)
							bMercantileValid = false;
						else if (iMercantile > iMaritime && bMaritimeValid)
							bMercantileValid = false;
						else if (iMercantile > iReligious && bReligiousValid)
							bMercantileValid = false;

						if (bMercantileValid)
							vValidCategories.push_back(MINOR_CIV_TRAIT_MERCANTILE);
					}
					if (bReligiousValid)
					{
						if (iReligious > iCultured && bCulturedValid)
							bReligiousValid = false;
						else if (iReligious > iMilitaristic && bMilitaristicValid)
							bReligiousValid = false;
						else if (iReligious > iMaritime && bMaritimeValid)
							bReligiousValid = false;
						else if (iReligious > iMercantile && bMercantileValid)
							bReligiousValid = false;

						if (bReligiousValid)
							vValidCategories.push_back(MINOR_CIV_TRAIT_RELIGIOUS);
					}
				}
				// We're merely equalizing the probability of each type of City-State showing up (regardless of how many are in the pool).
				else
				{
					if (bCulturedValid)
						vValidCategories.push_back(MINOR_CIV_TRAIT_CULTURED);

					if (bMilitaristicValid)
						vValidCategories.push_back(MINOR_CIV_TRAIT_MILITARISTIC);

					if (bMaritimeValid)
						vValidCategories.push_back(MINOR_CIV_TRAIT_MARITIME);

					if (bMercantileValid)
						vValidCategories.push_back(MINOR_CIV_TRAIT_MERCANTILE);

					if (bReligiousValid)
						vValidCategories.push_back(MINOR_CIV_TRAIT_RELIGIOUS);
				}

				// Pick at random from the valid categories.
				uint uRand = GC.getGame().urandLimitExclusive(vValidCategories.size(), CvSeeder::fromRaw(0x95c6b165).mix(iCounter));
				MinorCivTraitTypes eTrait = static_cast<MinorCivTraitTypes>(vValidCategories[uRand]);

				// Now pick a random City-State within this category.
				MinorCivTypes eChoice = NO_MINORCIV;
				switch (eTrait)
				{
				case MINOR_CIV_TRAIT_CULTURED:
				{
					uRand = GC.getGame().urandLimitExclusive(vCultured.size(), CvSeeder::fromRaw(0x5ac0d892).mix(iCounter));
					eChoice = static_cast<MinorCivTypes>(vCultured[uRand]);
					vCultured.erase(vCultured.begin() + uRand);
					iCultured++;
					break;
				}
				case MINOR_CIV_TRAIT_MILITARISTIC:
				{
					uRand = GC.getGame().urandLimitExclusive(vMilitaristic.size(), CvSeeder::fromRaw(0x7fde469e).mix(iCounter));
					eChoice = static_cast<MinorCivTypes>(vMilitaristic[uRand]);
					vMilitaristic.erase(vMilitaristic.begin() + uRand);
					iMilitaristic++;
					break;
				}
				case MINOR_CIV_TRAIT_MARITIME:
				{
					uRand = GC.getGame().urandLimitExclusive(vMaritime.size(), CvSeeder::fromRaw(0x9a763898).mix(iCounter));
					eChoice = static_cast<MinorCivTypes>(vMaritime[uRand]);
					vMaritime.erase(vMaritime.begin() + uRand);
					iMaritime++;
					break;
				}
				case MINOR_CIV_TRAIT_MERCANTILE:
				{
					uRand = GC.getGame().urandLimitExclusive(vMercantile.size(), CvSeeder::fromRaw(0xef6c2156).mix(iCounter));
					eChoice = static_cast<MinorCivTypes>(vMercantile[uRand]);
					vMercantile.erase(vMercantile.begin() + uRand);
					iMercantile++;
					break;
				}
				case MINOR_CIV_TRAIT_RELIGIOUS:
				{
					uRand = GC.getGame().urandLimitExclusive(vReligious.size(), CvSeeder::fromRaw(0x0d3e0560).mix(iCounter));
					eChoice = static_cast<MinorCivTypes>(vReligious[uRand]);
					vReligious.erase(vReligious.begin() + uRand);
					iReligious++;
					break;
				}
				default:
					UNREACHABLE();
				}
				setMinorCivType(*it, eChoice);
			}
			iCounter++;
		}
	}
}

vector<MinorCivTypes> GetAvailableMinorCivTypes(vector<MinorCivTypes>& vCultured, vector<MinorCivTypes>& vMilitaristic, vector<MinorCivTypes>& vMaritime, vector<MinorCivTypes>& vMercantile, vector<MinorCivTypes>& vReligious)
{
	vector<MinorCivTypes> vAvailable;
	vector<CivilizationTypes> vChosenMajors;
	vector<MinorCivTypes> vChosenMinors;
	bool bNoReligion = GC.getGame().isOption(GAMEOPTION_NO_RELIGION);
	bool bNoPolicies = GC.getGame().isOption(GAMEOPTION_NO_POLICIES);

	for (int p = 0; p < MAX_MAJOR_CIVS; p++)
	{
		const PlayerTypes eID = static_cast<PlayerTypes>(p);
		SlotStatus s = slotStatus(eID);
		if (s == SS_TAKEN || s == SS_COMPUTER)
		{
			const CivilizationTypes eCiv = civilization(eID);
			if (eCiv != NO_CIVILIZATION && std::find(vChosenMajors.begin(), vChosenMajors.end(), eCiv) == vChosenMajors.end())
				vChosenMajors.push_back(eCiv);
		}
	}
	for (int p = MAX_MAJOR_CIVS; p < MAX_CIV_PLAYERS; p++)
	{
		const PlayerTypes eID = static_cast<PlayerTypes>(p);
		const MinorCivTypes eMinorCiv = minorCivType(eID);
		if (eMinorCiv != NO_MINORCIV && std::find(vChosenMinors.begin(), vChosenMinors.end(), eMinorCiv) == vChosenMinors.end())
			vChosenMinors.push_back(eMinorCiv);
	}

	for (int i = 0; i < GC.getNumMinorCivInfos(); i++)
	{
		MinorCivTypes eAvailability = static_cast<MinorCivTypes>(i);
		CvMinorCivInfo* pkCityState = GC.getMinorCivInfo(eAvailability);
		if (pkCityState == NULL)
			continue;

		// Turned off in XML?
		if (!pkCityState->IsPlayable())
			continue;

		// Blocked by game options?
		MinorCivTraitTypes eTrait = (MinorCivTraitTypes)pkCityState->GetMinorCivTrait();
		if (eTrait == MINOR_CIV_TRAIT_RELIGIOUS && bNoReligion)
			continue;
		if (eTrait == MINOR_CIV_TRAIT_CULTURED && bNoPolicies)
			continue;

		// City-State is already ingame?
		if (std::find(vChosenMinors.begin(), vChosenMinors.end(), eAvailability) != vChosenMinors.end())
			continue;

		// Check to see if this City-State is blocked from appearing due to an ingame major civ.
		bool bBlocked = false;
		for (std::vector<CivilizationTypes>::iterator it = vChosenMajors.begin(); it != vChosenMajors.end(); it++)
		{
			CvCivilizationInfo* pkCivilization = GC.getCivilizationInfo(*it);
			if (pkCivilization->IsBlocksMinor(i))
			{
				bBlocked = true;
				break;
			}
		}
		if (bBlocked)
			continue;

		vAvailable.push_back(eAvailability);
		switch (eTrait)
		{
		case MINOR_CIV_TRAIT_CULTURED:
			vCultured.push_back(eAvailability);
			break;
		case MINOR_CIV_TRAIT_MILITARISTIC:
			vMilitaristic.push_back(eAvailability);
			break;
		case MINOR_CIV_TRAIT_MARITIME:
			vMaritime.push_back(eAvailability);
			break;
		case MINOR_CIV_TRAIT_MERCANTILE:
			vMercantile.push_back(eAvailability);
			break;
		case MINOR_CIV_TRAIT_RELIGIOUS:
			vReligious.push_back(eAvailability);
			break;
		default:
			UNREACHABLE();
		}
	}
	return vAvailable;
}

void setGameTurn(int turn)
{
	s_gameTurn = turn;
}

void setGameType(GameTypes g, GameStartTypes eStartType)
{
	setGameType(g);
	s_gameStartType = eStartType;
}

void setGameType(GameTypes g)
{
	s_gameType = g;
	if(s_gameType != GAME_NETWORK_MULTIPLAYER)
		s_isInternetGame = false;
}

void setGameType(const CvString& g)
{
	if(_stricmp(g.GetCString(), "singleplayer") == 0)
	{
		setGameType(GAME_SINGLE_PLAYER, GAME_NEW);
	}
	else if(_stricmp(g.GetCString(), "spload") == 0)
	{
		setGameType(GAME_SINGLE_PLAYER, GAME_LOADED);
	}
	else
	{
		//ASSERT_DEBUG(false, "Invalid game type in ini file!");
		setGameType(GAME_TYPE_NONE);
	}
}

void pushGameType(GameTypes g)
{
	if (s_pushedGameType == GAME_TYPE_NONE) {
		s_pushedGameType = gameType();
		setGameType(g);
	}
}

void popGameType()
{
	if (s_pushedGameType != GAME_TYPE_NONE) {
		setGameType(s_pushedGameType);
		
		s_pushedGameType = GAME_TYPE_NONE;
	}
}

void setGameStartType(GameStartTypes eStartType)
{
	s_gameStartType = eStartType;
}

void setGameUpdateTime(int updateTime)
{
	s_gameUpdateTime = updateTime;
}

void setHandicap(PlayerTypes p, HandicapTypes h)
{
	if(p >= 0 && p < MAX_PLAYERS){
		s_handicaps[p] = h;

		if(slotStatus(p) == SS_TAKEN){
			//Cache the handicap of human players.  
			//We do this so we can recall the human handicap setting if the human player happens to disconnect and get replaced by an ai.
			setLastHumanHandicap(p, h);
		}
	}
}

void setLastHumanHandicap(PlayerTypes p, HandicapTypes h)
{
	if(p >= 0 && p < MAX_PLAYERS){
		s_lastHumanHandicaps[p] = h;
	}
}

void setInternetGame(bool bIsInternetGame)
{
	s_isInternetGame = bIsInternetGame;
}

void setLeaderHead(PlayerTypes p, LeaderHeadTypes l)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		s_leaderHeads[p] = l;

		bool bFailed = bindLeaderKeys(p);

		if(bFailed)
		{
			s_leaderHeads[p] = NO_LEADER;
			s_leaderKeysAvailable[p] = true;	// If the key is empty, we assume the selection is in the 'random' state, so it is available.
		}
	}
}

void setLeaderName(PlayerTypes p, const CvString& n)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_leaderNames[p] = n;
}

void setLeaderKey(PlayerTypes p, const CvString& szKey)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		s_leaderKeys[p] = szKey;
		// Check to see if this is available, if not, set the index to NO_LEADER and the package ID to invalid
		// The key will stay valid
		bool bFailed = true;
		if(!szKey.empty())
		{
			// During the pre-game, we can't be sure the cached *Infos are current, so query the database
			Database::Connection* pDB = GC.GetGameDatabase();
			if(pDB)
			{
				Database::Results kResults;
				if(pDB->Execute(kResults, "SELECT ID, PackageID from Leaders where Type = ? LIMIT 1"))
				{
					kResults.Bind(1, szKey.c_str());
					if(kResults.Step())
					{
						s_leaderHeads[p] = (LeaderHeadTypes)kResults.GetInt(0);
						if(!ExtractGUID(kResults.GetText(1), s_leaderPackageID[p]))
							ClearGUID(s_leaderPackageID[p]);
						s_leaderKeysAvailable[p] = true;
						bFailed = false;
					}
				}
			}
		}

		if(bFailed)
		{
			s_leaderHeads[p] = NO_LEADER;
			ClearGUID(s_leaderPackageID[p]);
			s_leaderKeysAvailable[p] = (szKey.empty());	// If the key was empty, then it is the 'random' state so it is available
		}
	}
}

// Return true if the key for the players leader is available on this machine
bool leaderKeyAvailable(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		return s_civilizationKeysAvailable[p];
	}

	return false;
}

// Set the unique key value for the leaders's package.
void setLeaderKeyPackageID(PlayerTypes p, const GUID& kKey)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		s_leaderPackageID[p] = kKey;
	}
}

void setLoadFileName(const CvString& f)
{
	setLoadFileName(f, STORAGE_LOCAL);
}

void setLoadFileName(const CvString& f, StorageLocation eStorage)
{
	if(s_loadFileName != f)
	{
		s_loadFileName = f;
	}

	s_loadFileStorage = eStorage;
}

bool readPlayerSlotInfo(FDataStream& loadFrom, bool bReadVersion)
{
	if(gDLL->IsHost())	// Only do this if the player is the host.
	{
		loadSlotHints(loadFrom, bReadVersion);
	}
	return true;
}

void setMapNoPlayers(bool p)
{
	s_mapNoPlayers = p;
}

void setMapRandomSeed(unsigned int newSeed)
{
	s_mapRandomSeed = newSeed;
}

void setLoadWBScenario(bool b)
{
	s_loadWBScenario = b;
}

void setOverrideScenarioHandicap(bool b)
{
	s_overrideScenarioHandicap = b;
}

void setMapScriptName(const CvString& s)
{
	if(s_mapScriptName != s)
	{
		s_mapScriptName = s;
		ResetMapOptions();
	}
}

void setMaxCityElimination(int m)
{
	s_maxCityElimination = m;
}

void setMaxTurns(int m)
{
	s_maxTurns = m;
}

void setMinorCivType(PlayerTypes p, MinorCivTypes m)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_minorCivTypes[p] = m;
}

void setMinorCiv(PlayerTypes p, bool isMinor)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_minorNationCivs[p] = isMinor;
}

void setMultiplayerAIEnabled(bool enabled)
{
	s_multiplayerAIEnabled = enabled;
}

void setMultiplayerOption(MultiplayerOptionTypes o, bool enabled)
{
	if(o >= 0 && o < NUM_MPOPTION_TYPES)
		s_multiplayerOptions[o] = enabled;
}

void setMultiplayerOptions(const std::vector<bool>& mpOptions)
{
	s_multiplayerOptions = mpOptions;
}

void setNumMinorCivs(int n)
{
	s_numMinorCivs = n;
}

void setNetID(PlayerTypes p, int id)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_netIDs[p] = id;
}

void setNickname(PlayerTypes p, const CvString& n)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		string _szName(n);
		if(!n.IsEmpty())
		{
			size_t _pos = n.rfind('@');
			size_t _cNum = n.length() - _pos;
			// The max player name length is defined as 64 in ffirewiretypes, the max size of the string past that can only be 64 if we represent it as binary, currently representing it as HEX
			if((_pos > 0 && _pos < 64) && _cNum < 32)
			{
				_szName.erase(_pos, _cNum);
			}
		}
		s_displayNicknames[p] = (CvString)_szName;
		s_nicknames[p] = n;
	}
}

void setPitBossTurnTime(int t)
{
	s_pitBossTurnTime = t;
}

void setPlayable(PlayerTypes p, bool playable)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		s_playableCivs[p] = playable;
	}
}

void setPlayerColor(PlayerTypes p, PlayerColorTypes c)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_playerColors[p] = c;
}

void setPrivateGame(bool isPrivateGame)
{
	s_privateGame = isPrivateGame;
}

void setQuickCombat(bool isQuickCombat)
{
	s_quickCombat = isQuickCombat;
	SetGameOption(GAMEOPTION_QUICK_COMBAT_KEY, isQuickCombat?1:0);
}

void setQuickCombatDefault(bool isQuickCombat)
{
	s_quickCombatDefault = isQuickCombat;
}

void setQuickHandicap(HandicapTypes h)
{
	s_quickHandicap = h;
}

void setQuickHandicap(const CvString& h)
{
	for(int i = 0; i < GC.getNumHandicapInfos(); i++)
	{
		const HandicapTypes eHandicap = static_cast<HandicapTypes>(i);
		CvHandicapInfo* pkHandicapInfo = GC.getHandicapInfo(eHandicap);
		if(pkHandicapInfo)
		{
			if(_stricmp(h.GetCString(), pkHandicapInfo->GetType()) == 0)
			{
				setQuickHandicap(eHandicap);
			}
		}
	}
}

void setQuickMovement(bool isQuickMovement)
{
	SetGameOption(GAMEOPTION_QUICK_MOVEMENT_KEY, isQuickMovement?1:0);
}

void setQuickstart(bool isQuickstart)
{
	s_quickstart = isQuickstart;
}

void setRandomWorldSize(bool isRandomWorldSize)
{
	s_randomWorldSize = isRandomWorldSize;
}

void setRandomMapScript(bool isRandomMapScript)
{
	if(s_randomMapScript != isRandomMapScript)
	{
		s_randomMapScript = isRandomMapScript;
		ResetMapOptions();
	}
}

void setReady(PlayerTypes p, bool bIsReady)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		if(!bIsReady || p != activePlayer() || canReadyLocalPlayer())
			s_readyPlayers[p] = bIsReady;
	}
}

void setSeaLevel(SeaLevelTypes s)
{
	s_seaLevel = s;
}

void setSeaLevel(const CvString& s)
{
	//Query
	Database::SingleResult kResult;
	DB.SelectAt(kResult, "SeaLevels", "Type", s.c_str());
	s_seaLevelInfo.CacheResult(kResult);

	s_seaLevel = (SeaLevelTypes)s_seaLevelInfo.GetID();
}

void setSlotClaim(PlayerTypes p, SlotClaim s)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_slotClaims[p] = s;
}

void setSlotStatus(PlayerTypes p, SlotStatus eSlotStatus)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_slotStatus[p] = eSlotStatus;
}

void setAllSlotStatus(const std::vector<SlotStatus>& vSlotStatus)
{
	s_slotStatus = vSlotStatus;
}

void setSyncRandomSeed(unsigned int newSeed)
{
	s_syncRandomSeed = newSeed;
}

void setTargetScore(int t)
{
	s_targetScore = t;
}

void setTeamType(PlayerTypes p, TeamTypes t)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_teamTypes[p] = t;
}

void setTransferredMap(bool transferred)
{
	s_transferredMap = transferred;
}

void setTurnTimer(TurnTimerTypes t)
{
	s_turnTimerType = t;
	Database::SingleResult kResult;
	if(!DB.SelectAt(kResult, "TurnTimers", "ID", static_cast<int>(t)))
	{
		ASSERT_DEBUG(false, "Cannot find turn timer info.");
	}
	s_turnTimer.CacheResult(kResult);
}

void setTurnTimer(const CvString& t)
{
	Database::SingleResult kResult;
	if(!DB.SelectAt(kResult, "TurnTimers", "Type", t.c_str()))
	{
		ASSERT_DEBUG(false, "Cannot find turn timer info.");
	}
	s_turnTimer.CacheResult(kResult);

	s_turnTimerType = (TurnTimerTypes)s_turnTimer.GetID();
}

void SetCityScreenBlocked(bool bCityScreenBlocked)
{
	s_bCityScreenBlocked = bCityScreenBlocked;
}

void setVersionString(const std::string& v)
{
	s_versionString = v;
}

void setVictory(VictoryTypes v, bool isValid)
{
	if(v >= 0 && v < s_numVictoryInfos)
		s_victories[v] = isValid;
}

void setVictories(const std::vector<bool>& v)
{
	ASSERT_DEBUG(v.size() <= std::size_t(s_numVictoryInfos));
	for (std::size_t i = 0; i < v.size(); i++)
		s_victories[i] = v[i];
}

void setWhiteFlag(PlayerTypes p, bool flag)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_whiteFlags[p] = flag;
}

void setTurnNotifySteamInvite(PlayerTypes p, bool flag)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_turnNotifySteamInvite[p] = flag;
}

void setTurnNotifyEmail(PlayerTypes p, bool flag)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_turnNotifyEmail[p] = flag;
}

void setTurnNotifyEmailAddress(PlayerTypes p, const CvString& emailAddress)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_turnNotifyEmailAddress[p] = emailAddress;
}

void VerifyHandicap(PlayerTypes p, bool bHumanPlayerSwap)
{//Verifies that the current handicap is valid for the current player.
	//non-ai players can't use the default ai handicap and ai players MUST use it.
	if(slotStatus(p) == SS_COMPUTER){
		setHandicap(p, (HandicapTypes)GD_INT_GET(AI_HANDICAP));
	}
	else if(handicap(p) == GD_INT_GET(AI_HANDICAP))
	{
		if (lastHumanHandicap(p) != NO_HANDICAP)
		{
			setHandicap(p, lastHumanHandicap(p));
		}
		else if (GC.getGame().isNetworkMultiPlayer())
		{
			setHandicap(p, (HandicapTypes)GD_INT_GET(MULTIPLAYER_HANDICAP));
		}
		else
		{
			if (bHumanPlayerSwap)
			{
				// loop through the player to find the handicap the human player had
				for (int i = 0; i < MAX_MAJOR_CIVS; i++)
				{
					if (lastHumanHandicap((PlayerTypes)i) > NO_HANDICAP)
					{
						setHandicap(p, lastHumanHandicap((PlayerTypes)i));
						break;
					}
				}
			}
			else
			{
				setHandicap(p, (HandicapTypes)GD_INT_GET(STANDARD_HANDICAP));
			}
		}
	}
}

void setWorldSize(WorldSizeTypes w, bool bResetSlots)
{
	ASSERT_DEBUG(!gameStarted() || isNetworkMultiplayerGame() || isHotSeatGame());

	//Query
	Database::Results kQuery;
	DB.Execute(kQuery, "SELECT * from Worlds where ID = ? LIMIT 1");
	kQuery.Bind(1, w);

	if(kQuery.Step())
	{
		s_worldInfo.CacheResult(kQuery);
		s_worldSize = w;
		if(bResetSlots)
			resetSlots();
	}
	else
	{
		ASSERT_DEBUG(false, "Could not find world size entry.")
	}
}

void setWorldSize(const CvString& w)
{
	ASSERT_DEBUG(!gameStarted() || isNetworkMultiplayerGame() || isHotSeatGame());

	Database::Results kQuery;

	DB.Execute(kQuery, "SELECT * from Worlds where Type = ? LIMIT 1");
	kQuery.Bind(1, w.c_str(), w.size(), false);

	if(kQuery.Step())
	{
		s_worldInfo.CacheResult(kQuery);
		s_worldSize = (WorldSizeTypes)s_worldInfo.GetID();
		resetSlots();
	}
	else
	{
		ASSERT_DEBUG(false, "Could not find world size entry.")
	}
}

SlotClaim slotClaim(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_slotClaims[p];
	return SLOTCLAIM_UNASSIGNED;
}

SlotStatus slotStatus(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_slotStatus[p];
	return SS_CLOSED;
}

const std::vector<SlotStatus>& GetSlotStatus()
{
	return s_slotStatus;
}

void StringToBools(const char* szString, int* iNumBools, bool** ppBools)
{
	ASSERT_DEBUG(szString, "null string");
	if(szString)
	{
		*iNumBools = strlen(szString);
		*ppBools = FNEW(bool[*iNumBools], c_eCiv5GameplayDLL, 0);
		int i = 0;
		for(i=0; i<*iNumBools; i++)
		{
			(*ppBools)[i] = (szString[i]=='1');
		}
	}
}
unsigned int syncRandomSeed()
{
	return s_syncRandomSeed;
}

int targetScore()
{
	return s_targetScore;
}

bool transferredMap()
{
	return s_transferredMap;
}

TurnTimerTypes turnTimer()
{
	return s_turnTimerType;
}

const CvTurnTimerInfo& turnTimerInfo()
{
	return s_turnTimer;
}

const std::string& versionString()
{
	return s_versionString;
}

const std::vector<bool>& victories()
{
	return s_victories;
}

const CvWorldInfo& worldInfo()
{
	if(s_worldSize != s_worldInfo.GetID())
	{
		Database::SingleResult kResult;
		DB.SelectAt(kResult, "Worlds", s_worldSize);
		s_worldInfo.CacheResult(kResult);
	}
	return s_worldInfo;
}

WorldSizeTypes worldSize()
{
	return s_worldSize;
}

void writeArchive(FDataStream& saveTo)
{
	uint uiVersion = 6;
	saveTo << uiVersion;

	saveTo << s_activePlayer;
	saveTo << s_adminPassword;
	saveTo << s_alias;
	saveTo << s_artStyles;
	saveTo << s_autorun;
	saveTo << s_autorunTurnDelay;
	saveTo << s_autorunTurnLimit;
	saveTo << s_bandwidth;
	saveTo << s_calendar;
	saveTo << s_calendarInfo;
	saveTo << s_civAdjectives;
	saveTo << s_civDescriptions;
	saveTo << s_civPasswords;
	saveTo << s_civShortDescriptions;
	saveTo << s_climate;
	saveTo << s_climateInfo;
	saveTo << s_era;
	saveTo << s_emailAddresses;
	saveTo << s_endTurnTimerLength;
	saveTo << s_flagDecals;
	saveTo << s_DEPRECATEDforceControls;
	saveTo << s_gameMode;
	saveTo << s_gameName;
	saveTo << s_gameSpeed;
	saveTo << s_gameStarted;
	saveTo << s_gameTurn;
	saveTo << s_gameType;
	saveTo << s_gameMapType;
	saveTo << s_gameUpdateTime;
	saveTo << s_handicaps;
	saveTo << s_lastHumanHandicaps;
	saveTo << s_isEarthMap;
	saveTo << s_isInternetGame;
	saveTo << s_leaderNames;
	saveTo << s_loadFileName;
	saveTo << s_localPlayerEmailAddress;
	saveTo << s_mapNoPlayers;
	saveTo << s_mapRandomSeed;
	saveTo << s_loadWBScenario;
	saveTo << s_overrideScenarioHandicap;
	saveTo << s_mapScriptName;
	saveTo << s_maxCityElimination;
	saveTo << s_maxTurns;
	saveTo << s_numMinorCivs;
	CvInfosSerializationHelper::WriteTypeArray<MinorCivTypes>(saveTo, s_minorCivTypes);
	saveTo << s_minorNationCivs;
	saveTo << s_dummyvalue;
	saveTo << s_multiplayerOptions;
	saveTo << s_netIDs;
	writeNicknames(saveTo);
	saveTo << s_numVictoryInfos;
	saveTo << s_pitBossTurnTime;
	saveTo << s_playableCivs;
	CvInfosSerializationHelper::WriteTypeArray<PlayerColorTypes>(saveTo, s_playerColors);
	saveTo << s_privateGame;
	saveTo << s_quickCombat;
	saveTo << s_quickCombatDefault;
	saveTo << s_quickHandicap;
	saveTo << s_quickstart;
	saveTo << s_randomWorldSize;
	saveTo << s_randomMapScript;
	saveTo << s_readyPlayers;
	saveTo << s_seaLevel;
	saveTo << s_seaLevelInfo;
	saveTo << s_dummyvalue2;
	saveTo << s_slotClaims;
	writeSlotStatus(saveTo);
	saveTo << s_smtpHost;
	saveTo << s_syncRandomSeed;
	saveTo << s_targetScore;
	saveTo << s_teamTypes;
	saveTo << s_transferredMap;
	saveTo << s_turnTimer;
	saveTo << s_turnTimerType;
	saveTo << s_bCityScreenBlocked;
	saveTo << s_victories;
	saveTo << s_whiteFlags;
	saveTo << s_worldInfo;
	saveTo << s_worldSize;
	saveTo << s_GameOptions;
	saveTo << s_MapOptions;
	saveTo << s_versionString;
	saveTo << s_turnNotifySteamInvite;
	saveTo << s_turnNotifyEmail;
	saveTo << s_turnNotifyEmailAddress;
}

void write(FDataStream& saveTo)
{
	// header needs to include slot setup
	saveSlotHints(saveTo);
	writeArchive(saveTo);
}

// Get the number of slots that have been taken by humans and AIs
int getActiveSlotCount()
{
	return calcActiveSlotCount(s_slotStatus, s_slotClaims);
}

// Return true if the DLC is allowed in the game
bool isDLCAllowed(const GUID& kDLCID)
{
	for(PackageIDList::const_iterator itr = s_AllowedDLC.begin(); itr != s_AllowedDLC.end(); ++itr)
	{
		if(memcmp(&kDLCID, &(*itr), sizeof(GUID)) == 0)
		{
			return true;
		}
	}

	return false;
}

// Set whether or not the DLC is allowed in the game
void setDLCAllowed(const GUID& kDLCID, bool bState)
{
	if(bState)
	{
		if(!isDLCAllowed(kDLCID))
			s_AllowedDLC.push_back(kDLCID);
	}
	else
	{
		for(PackageIDList::const_iterator itr = s_AllowedDLC.begin(); itr != s_AllowedDLC.end(); ++itr)
		{
			if(memcmp(&kDLCID, &(*itr), sizeof(GUID)) == 0)
			{
				s_AllowedDLC.erase(itr);
				break;
			}
		}
	}
}

// Clear all DLC from being the allowed list
void clearDLCAllowed()
{
	s_AllowedDLC.clear();
}

// Return the number of DLCs allowed
uint getDLCAllowedCount()
{
	return s_AllowedDLC.size();
}

// Return the list of DLCs allowed
const PackageIDList& getDLCAllowed()
{
	return s_AllowedDLC;
}

// Return the list of DLCs available for a player
const PackageIDList& getDLCAvailable(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_DLCPackagesAvailable[p];
	return s_emptyGUIDList;
}

// Set the list of DLCs available to a player
void setDLCAvailable(PlayerTypes p, const PackageIDList& kList)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_DLCPackagesAvailable[p] = kList;
}

bool isDLCAvailable(PlayerTypes p, const GUID& kDLCID)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		const PackageIDList& kList = s_DLCPackagesAvailable[p];
		for(PackageIDList::const_iterator itr = kList.begin(); itr != kList.end(); ++itr)
		{
			if(*itr == kDLCID)
				return true;
		}
	}

	return false;
}

// Clear the list of DLCs available for a player
void clearDLCAvailable(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		s_DLCPackagesAvailable[p].clear();
}

// Get the unique key value for the civilization
const CvString& civilizationKey(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
		return s_civilizationKeys[p];
	static const CvString empty = "";
	return empty;
}

// Set the unique key value for the civilization
void setCivilizationKey(PlayerTypes p, const CvString& szKey)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		s_civilizationKeys[p] = szKey;

		// Check to see if this is available, if not, set the index to NO_CIVILIZATION and the package ID to invalid
		// The key will stay valid
		bool bFailed = true;
		if(!szKey.empty())
		{
			// During the pre-game, we can't be sure the cached *Infos are current, so query the database
			Database::Results kResults;
			if(DB.Execute(kResults, "SELECT ID, PackageID, Playable from Civilizations where Type = ? LIMIT 1"))
			{
				kResults.Bind(1, szKey.c_str());
				if(kResults.Step())
				{
					s_civilizations[p] = (CivilizationTypes)kResults.GetInt(0);
					if(!ExtractGUID(kResults.GetText(1), s_civilizationPackageID[p]))
						ClearGUID(s_civilizationPackageID[p]);
					s_civilizationKeysAvailable[p] = true;
					s_civilizationKeysPlayable[p] = kResults.GetBool(2);
					bFailed = false;
				}
			}
		}

		if(bFailed)
		{
			s_civilizations[p] = NO_CIVILIZATION;
			ClearGUID(s_civilizationPackageID[p]);
			s_civilizationKeysAvailable[p] = (szKey.empty());	// If the key was empty, then it is the 'random' state so it is available
			s_civilizationKeysPlayable[p] = s_civilizationKeysAvailable[p];
		}
	}
}

// Return true if the key for the players civilization is available on this machine
// ***ALSO*** hijacked to allow checking if a player is known to the local human player. Seems difficult to expose stuff to the pregame Lua stuff (like the StagingRoom), sorry. This function was being used in exactly one place and has limited utility at least. 
bool civilizationKeyAvailable(PlayerTypes p)
{
	///////////////////////////////////////
	/////////////// HIJACKED //////////////
	///////////////////////////////////////
	if (isKnownPlayerReq(p))
	{
		return handleKnownPlayerReq(p);
	}
	///////////////////////////////////////
	//////////////////END//////////////////
	///////////////////////////////////////

	if(p >= 0 && p < MAX_PLAYERS)
	{
		return s_civilizationKeysAvailable[p];
	}

	return false;
}

// Get the package ID the player's civilization is from.  If empty, it comes from the base installation.
const GUID& civilizationKeyPackageID(PlayerTypes p)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		return s_civilizationPackageID[p];
	}

	return s_emptyGUID;
}

// Set the unique key value for the civilization's package.
void setCivilizationKeyPackageID(PlayerTypes p, const GUID& kKey)
{
	if(p >= 0 && p < MAX_PLAYERS)
	{
		s_civilizationPackageID[p] = kKey;
	}
}

// Can the player be readied?
bool canReadyLocalPlayer()
{
	for(int itr = 0; itr < MAX_PLAYERS; ++itr)
	{
		if(slotStatus((PlayerTypes)itr) != SS_CLOSED && !CvPreGame::civilizationKeyAvailable((PlayerTypes)itr))
			return false;
	}

	// Also check to see if they have the requested map script
	if (mapScriptName().GetLength() > 0)
	{
		if (!FFILESYSTEM.Exist(mapScriptName()))
			return false;
	}
	return true;
}

const std::vector<KnownPlayersBitArray>& GetKnownPlayersTable() {
	return s_knownPlayersTable;
}

void SetKnownPlayersTable(const std::vector<KnownPlayersBitArray>& aiKnownPlayersTable) {
	s_knownPlayersTable = aiKnownPlayersTable;
}

void updateKnownPlayersTable()
{
	// Playing it safe here, don't want to introduce a bug due to not knowing all the code paths and trying to save a few cycles/bytes
	s_knownPlayersTable.clear();
	s_knownPlayersTable.resize(MAX_MAJOR_CIVS);
	for (int i = 0; i < MAX_MAJOR_CIVS; i++) {
		KnownPlayersBitArray bitarray = 0;
		const CvTeam& kTeam = GET_TEAM((TeamTypes)i);

		for (int j = 0; j < MAX_MAJOR_CIVS; j++) {
			if (i == j || kTeam.isHasMet((TeamTypes)j))

				bitarray |= KnownPlayersBitArray(1) << j;
		}
		s_knownPlayersTable[i] = bitarray;
	}
}

bool isKnownPlayerReq(PlayerTypes ePlayer)
{
	// to differentiate I am passing in the player id as -(p+2), so player 0 would become player -2 which is an otherwise non-occuring input to the function (along with all other negative values). 2 was used to that accident passing of NO_PLAYER wouldn't get picked up and become my problem.
	return ePlayer < -1;
}

bool handleKnownPlayerReq(PlayerTypes ePlayer)
{
	// Only makes sense for Network MP
	ASSERT_DEBUG(isNetworkMultiplayerGame(), "Checking known players table does not make sense outside of Network MP games!");
	
	// decode actual player ID - it was encoded as -(ePlayer+2), probably in StagingRoom.lua
	ePlayer = static_cast<PlayerTypes>(-ePlayer - 2);

	return isKnownPlayer(ePlayer, activePlayer());
}

bool isKnownPlayer(PlayerTypes eA, PlayerTypes eB)
{
	// table will be empty if GAMEOPTION_KEEP_UNMET_PLAYERS_UNKNOWN, i.e. not keeping track of unmet players and consider all to be met for the purposes of PreGame UI.
	if (s_knownPlayersTable.empty()) return true;
	if (eA < 0 || eB < 0) return true; // erring on the side of caution
	if ((size_t)eA >= s_knownPlayersTable.size() || (size_t)eB >= s_knownPlayersTable.size()) return true; // erring on the side of caution

	return (s_knownPlayersTable[eA] & (KnownPlayersBitArray(1) << eB)) != 0;
}
}
