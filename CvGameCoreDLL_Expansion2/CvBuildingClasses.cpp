/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#include "CvGameCoreDLLPCH.h"
#include "ICvDLLUserInterface.h"
#include "CvGameCoreUtils.h"
#include "CvInternalGameCoreUtils.h"
#include "FStlContainerSerialization.h"
#include "CvEnumSerialization.h"
#include "CvDLLUtilDefines.h"
#include "CvDllCity.h"
#include "CvDllPlot.h"
#include "CvInfosSerializationHelper.h"

#include "CvTypes.h"
#include "CvSpanSerialization.h"

#include <algorithm>

// include after all other headers
#include "LintFree.h"

/// Constructor
CvBuildingEntry::CvBuildingEntry(void):

	m_iBuildingClassType(NO_BUILDINGCLASS),
	m_pkBuildingClassInfo(NULL),
	m_iNearbyTerrainRequired(NO_VICTORY),
	m_iProhibitedCityTerrain(NO_VICTORY),
	m_iVictoryPrereq(NO_VICTORY),
	m_iFreeStartEra(NO_ERA),
	m_iMaxStartEra(NO_ERA),
	m_iObsoleteTech(NO_TECH),
	m_iGoldMaintenance(0),
	m_iMutuallyExclusiveGroup(0),
	m_iReplacementBuildingClass(NO_BUILDINGCLASS),
	m_iPrereqAndTech(NO_TECH),
	m_iPolicyBranchType(NO_POLICY_BRANCH_TYPE),
#if defined(MOD_BALANCE_CORE_POLICIES)
	m_iPolicyType(NO_POLICY),
	m_eCivType(NO_CIVILIZATION),
#endif
#if defined(MOD_BALANCE_CORE)
	m_iResourceType(NO_RESOURCE),
	m_iNumPoliciesNeeded(0),
	m_iGrantsRandomResourceTerritory(0),
	m_bPuppetPurchaseOverride(false),
	m_bAllowsPuppetPurchase(false),
	m_bNoStarvationNonSpecialist(false),
	m_iGetCooldown(0),
	m_bTradeRouteInvulnerable(false),
	m_iTRSpeedBoost(0),
	m_iTRVisionBoost(0),
	m_iTRTurnModGlobal(0),
	m_iTRTurnModLocal(0),
	m_iVotesPerGPT(0),
	m_bRequiresIndustrialCityConnection(false),
	m_bDummy(false),
	m_iLandmarksTourismPercentGlobal(0),
	m_iGreatWorksTourismModifierGlobal(0),
#endif
	m_iSpecialistType(NO_SPECIALIST),
	m_iSpecialistCount(0),
	m_iSpecialistExtraCulture(0),
	m_iGreatPeopleRateChange(0),
	m_eGreatWorkSlotType(NO_GREAT_WORK_SLOT),
#if defined(MOD_GLOBAL_GREATWORK_YIELDTYPES)
	m_eGreatWorkYieldType(YIELD_CULTURE),
#endif
	m_iGreatWorkCount(0),
	m_eFreeGreatWork(NO_GREAT_WORK),
	m_iFreeBuildingClass(NO_BUILDINGCLASS),
	m_iFreeBuildingThisCity(NO_BUILDINGCLASS),
	m_iFreePromotion(NO_PROMOTION),
	m_iTrainedFreePromotion(NO_PROMOTION),
	m_iFreePromotionRemoved(NO_PROMOTION),
	m_iProductionCost(0),
	m_iFaithCost(0),
	m_iLeagueCost(0),
	m_iNumCityCostMod(0),
	m_iHurryCostModifier(0),
	m_iNumCitiesPrereq(0),
	m_iUnitLevelPrereq(0),
	m_iCultureRateModifier(0),
	m_iGlobalCultureRateModifier(0),
	m_iGreatPeopleRateModifier(0),
	m_iGlobalGreatPeopleRateModifier(0),
	m_iGPRateModifierPerMarriage(0),
	m_iGPRateModifierPerLocalTheme(0),
	m_iGPPOnCitizenBirth(0),
	m_iGreatGeneralRateModifier(0),
	m_iGreatPersonExpendGold(0),
	m_iUnitUpgradeCostMod(0),
	m_iGoldenAgeModifier(0),
	m_iFreeExperience(0),
	m_iGlobalFreeExperience(0),
	m_iGlobalHappinessPerMajorWar(0),
	m_iGlobalMilitaryProductionModPerMajorWar(0),
	m_iFoodKept(0),
	m_bAirlift(false),
	m_iAirModifier(0),
	m_iAirModifierGlobal(0),
	m_iNukeModifier(0),
	m_iNukeExplosionRand(0),
	m_iWorkerSpeedModifier(0),
	m_iMilitaryProductionModifier(0),
	m_iSpaceProductionModifier(0),
	m_iGlobalSpaceProductionModifier(0),
	m_iMinAreaSize(0),
	m_iConquestProbability(0),
	m_iHealRateChange(0),
	m_iHappiness(0),
	m_iUnmoddedHappiness(0),
	m_iUnhappinessModifier(0),
#if defined(MOD_BALANCE_CORE)
	m_iLocalUnhappinessModifier(0),
	m_iGlobalBuildingGoldMaintenanceMod(0),
	m_iBuildingDefenseModifier(0),
	m_iDamageReductionFlat(0),
	m_iCitySupplyModifier(0),
	m_iCitySupplyModifierGlobal(0),
	m_iCitySupplyFlat(0),
	m_iCitySupplyFlatGlobal(0),
	m_iCityRangedStrikeRange(0),
	m_iCityIndirectFire(0),
	m_iRangedStrikeModifier(0),
	m_iGarrisonRangedAttackModifier(0),
#endif
	m_iHappinessPerCity(0),
	m_iHappinessPerXPolicies(0),
	m_iCityCountUnhappinessMod(0),
	m_bNoOccupiedUnhappiness(false),
	m_iGlobalPopulationChange(0),
	m_iPopulationChange(0),
	m_iTechShare(0),
	m_iFreeTechs(0),
	m_iFreePolicies(0),
	m_iFreeGreatPeople(0),
	m_iMedianTechPercentChange(0),
	m_iGold(0),
	m_bNearbyMountainRequired(false),
	m_bAllowsRangeStrike(false),
	m_iDefenseModifier(0),
	m_iGlobalDefenseModifier(0),
	m_iExtraCityHitPoints(0),
	m_iMissionType(NO_MISSION),
	m_iMinorFriendshipChange(0),
	m_iVictoryPoints(0),
	m_iExtraMissionarySpreads(0),
	m_iExtraMissionaryStrength(0),
	m_iExtraMissionarySpreadsGlobal(0),
	m_iReligiousPressureModifier(0),
	m_iEspionageModifier(0),
	m_iGlobalEspionageModifier(0),
	m_iSpySecurityModifier(0),
	m_iSpySecurityModifierPerXPop(0),
	m_iGlobalSpySecurityModifier(0),
	m_iExtraSpies(0),
	m_iSpyRankChange(0),
	m_iTradeRouteRecipientBonus(0),
	m_iTradeRouteTargetBonus(0),
	m_iNumTradeRouteBonus(0),
	m_iTradeRouteSeaDistanceModifier(0),
	m_iTradeRouteSeaGoldBonus(0),
	m_iTradeRouteLandDistanceModifier(0),
	m_iTradeRouteLandGoldBonus(0),
	m_iCityConnectionTradeRouteGoldModifier(0),
	m_iCityStateTradeRouteProductionModifier(0),
#if defined(MOD_RELIGION_CONVERSION_MODIFIERS)
	m_iConversionModifier(0),
	m_iGlobalConversionModifier(0),
#endif
	m_iInstantSpyRankChange(0),
	m_iLandmarksTourismPercent(0),
	m_iInstantMilitaryIncrease(0),
	m_iGreatWorksTourismModifier(0),
	m_iXBuiltTriggersIdeologyChoice(0),
	m_iGreatScientistBeakerModifier(0),
	m_iExtraLeagueVotes(0),
	m_iSingleLeagueVotes(0),
	m_iFaithToVotesBase(0),
	m_iCapitalsToVotesBase(0),
	m_iDoFToVotesBase(0),
	m_iRAToVotesBase(0),
	m_iDPToVotesBase(0),
	m_bIgnoreDefensivePactLimit(false),
	m_iGPExpendInfluenceBase(0),
	m_iEmpireSizeModifierReduction(0),
	m_iEmpireSizeModifierReductionGlobal(0),
	m_iBasicNeedsMedianModifier(0),
	m_iGoldMedianModifier(0),
	m_iScienceMedianModifier(0),
	m_iCultureMedianModifier(0),
	m_iReligiousUnrestModifier(0),
	m_iBasicNeedsMedianModifierGlobal(0),
	m_iGoldMedianModifierGlobal(0),
	m_iScienceMedianModifierGlobal(0),
	m_iCultureMedianModifierGlobal(0),
	m_iReligiousUnrestModifierGlobal(0),
	m_iDistressFlatReduction(0),
	m_iPovertyFlatReduction(0),
	m_iIlliteracyFlatReduction(0),
	m_iBoredomFlatReduction(0),
	m_iReligiousUnrestFlatReduction(0),
	m_iDistressFlatReductionGlobal(0),
	m_iPovertyFlatReductionGlobal(0),
	m_iIlliteracyFlatReductionGlobal(0),
	m_iBoredomFlatReductionGlobal(0),
	m_iReligiousUnrestFlatReductionGlobal(0),
	m_iExperiencePerGoldenAge(0),
	m_iExperiencePerGoldenAgeCap(0),
	m_iInstantReligionPressure(0),
	m_iBasePressureModGlobal(0),
	m_iDefensePerXWonder(0),
	m_iPreferredDisplayPosition(0),
	m_iPortraitIndex(-1),
	m_bTeamShare(false),
	m_bWater(false),
	m_bRiver(false),
	m_bFreshWater(false),
	m_bAnyWater(false),
#if defined(MOD_BALANCE_CORE_EVENTS)
	m_iEventRequiredActive(NO_EVENT_CHOICE),
	m_iCityEventRequiredActive(NO_EVENT_CHOICE_CITY),
#endif
	m_bAddsFreshWater(false),
	m_bPurchaseOnly(false),
	m_bSecondaryPantheon(false),
	m_piGreatWorkYieldChange(NULL),
	m_piGreatWorkYieldChangeLocal(NULL),
#if defined(MOD_BALANCE_CORE)
	m_iNumRequiredTier3Tenets(0),
	m_bIsNoWater(false),
	m_bIsNoRiver(false),
	m_bIsNoCoast(false),
	m_bIsCapitalOnly(false),
	m_bIsReformation(false),
	m_bBuildAnywhere(false),
	m_iTradeReligionModifier(-1),
	m_iReformationFollowerReduction(0),
	m_iFreeArtifacts(0),
	m_iResourceDiversityModifier(0),
	m_iNoUnhappfromXSpecialists(0),
	m_iNoUnhappfromXSpecialistsGlobal(0),
	m_iPurchaseCooldownReduction(0),
	m_iPurchaseCooldownReductionCivilian(0),
#endif
	m_iVassalLevyEra(0),
#if defined(MOD_BALANCE_CORE_POP_REQ_BUILDINGS)
	m_iNationalPopRequired(-1),
	m_iLocalPopRequired(-1),
#endif
#if defined(MOD_BALANCE_CORE_FOLLOWER_POP_WONDER)
	m_iNationalFollowerPopRequired(-1),
	m_iGlobalFollowerPopRequired(-1),
#endif
	m_bMountain(false),
	m_bHill(false),
	m_bFlat(false),
	m_bBorderObstacle(false),
#if defined(MOD_BALANCE_CORE)
	m_iCityAirStrikeDefense(0),
	m_iBorderObstacleCity(-1),
	m_iBorderObstacleWater(-1),
	m_iDeepWaterTileDamage(0),
	m_iWLTKDTurns(-1),
	m_iEventTourism(0),
	m_iLandTourism(0),
	m_iSeaTourism(0),
	m_iAlwaysHeal(0),
	m_iNukeInterceptionChance(0),
	m_bIsCorp(false),
#endif
	m_iFoodBonusPerCityMajorityFollower(0),
#if defined(HH_MOD_BUILDINGS_FRUITLESS_PILLAGE)
	m_bPlayerBorderGainlessPillage(false),
	m_bCityGainlessPillage(false),
#endif
	m_bPlayerBorderObstacle(false),
	m_bCapital(false),
	m_bGoldenAge(false),
	m_bMapCentering(false),
	m_bNeverCapture(false),
	m_bNukeImmune(false),
	m_bExtraLuxuries(false),
	m_bDiplomaticVoting(false),
	m_bAllowsWaterRoutes(false),
	m_bAllowsIndustrialWaterRoutes(false),
	m_bAllowsAirRoutes(false),
	m_bCityWall(false),
	m_bUnlockedByBelief(false),
	m_bUnlockedByLeague(false),
	m_bRequiresHolyCity(false),
	m_bAffectSpiesNow(false),
	m_bEspionage(false),
	m_bAllowsFoodTradeRoutes(false),
	m_bAllowsFoodTradeRoutesGlobal(false),
	m_bAllowsProductionTradeRoutes(false),
	m_bAllowsProductionTradeRoutesGlobal(false),
	m_bNullifyInfluenceModifier(false),
	m_piLockedBuildingClasses(NULL),
	m_piPrereqAndTechs(NULL),
	m_piResourceQuantityRequirements(NULL),
	m_piResourceQuantity(NULL),
	m_piResourceCultureChanges(NULL),
	m_piResourceFaithChanges(NULL),
	m_piProductionTraits(NULL),
	m_piSeaPlotYieldChange(NULL),
	m_piRiverPlotYieldChange(NULL),
	m_piLakePlotYieldChange(NULL),
	m_piLakePlotYieldChangeGlobal(NULL),
	m_piSeaResourceYieldChange(NULL),
	m_piGrowthExtraYield(NULL),
#if defined(MOD_BALANCE_CORE_POLICIES)
	m_piYieldFromDeath(NULL),
#endif
#if defined(MOD_BALANCE_CORE)
	m_piYieldFromVictory(NULL),
	m_piYieldFromVictoryGlobal(NULL),
	m_piYieldFromVictoryGlobalEraScaling(NULL),
	m_piYieldFromVictoryGlobalInGoldenAge(NULL),
	m_piYieldFromVictoryGlobalInGoldenAgeEraScaling(NULL),
	m_piYieldFromVictoryGlobalPlayer(NULL),
	m_piYieldFromPillage(NULL),
	m_piYieldFromPillageGlobal(NULL),
	m_piYieldFromPillageGlobalPlayer(NULL),
	m_iNeedBuildingThisCity(NO_BUILDING),
	m_piYieldFromGoldenAgeStart(NULL),
	m_piYieldChangePerGoldenAge(NULL),
	m_piYieldChangePerGoldenAgeCap(NULL),
	m_piGoldenAgeYieldMod(NULL),
	m_piYieldChangesPerLocalTheme(NULL),
	m_piYieldFromUnitGiftGlobal(NULL),
	m_piYieldFromWLTKD(NULL),
	m_piInstantYieldFromWLTKDStart(NULL),
	m_piYieldFromGPExpend(NULL),
	m_piThemingYieldBonus(NULL),
	m_piYieldFromSpyAttack(NULL),
	m_piYieldFromSpyDefense(NULL),
	m_piYieldFromSpyIdentify(NULL),
	m_piYieldFromSpyDefenseOrID(NULL),
	m_piYieldFromSpyRigElection(NULL),
	m_piYieldChangesPerCityStrengthTimes100(NULL),
	m_piYieldFromTech(NULL),
	m_piYieldFromConstruction(NULL),
	m_piYieldFromInternalTREnd(NULL),
	m_piYieldFromInternationalTREnd(NULL),
	m_piYieldFromInternal(NULL),
	m_piYieldFromProcessModifier(NULL),
	m_piYieldFromLongCount(NULL),
	m_piYieldFromGPBirthScaledWithWriterBulb(NULL),
	m_piYieldFromGPBirthScaledWithArtistBulb(NULL),
	m_miYieldFromGPBirthScaledWithPerTurnYield(),
	m_piYieldFromBirth(NULL),
	m_piYieldFromBirthEraScaling(NULL),
	m_piYieldFromBirthRetroactive(NULL),
	m_piYieldFromUnitProduction(NULL),
	m_piYieldFromBorderGrowth(NULL),
	m_piYieldFromPolicyUnlock(NULL),
	m_piYieldFromUnitLevelUp(NULL),
	m_piYieldFromCombatExperienceTimes100(NULL),
	m_piYieldFromPurchase(NULL),
	m_piYieldFromPurchaseGlobal(NULL),
	m_piYieldFromFaithPurchase(NULL),
	m_piYieldFromUnitLevelUpGlobal(NULL),
#endif
	m_piYieldChange(NULL),
	m_piYieldChangeEraScalingTimes100(NULL),
	m_pfYieldChangePerBuilding(NULL),
	m_pfYieldChangePerTile(NULL),
	m_pfYieldChangePerCityStateStrategicResource(NULL),
	m_piYieldChangePerPop(NULL),
	m_piYieldChangePerPopInEmpire(),
	m_miExtraPlayerInstancesFromAccomplishments(),
	m_siUnitClassTrainingAllowed(),
	m_sibResourceClaim(),
	m_miWLTKDFromProject(),
	m_piYieldChangePerReligion(NULL),
	m_piYieldModifier(NULL),
	m_piAreaYieldModifier(NULL),
	m_piGlobalYieldModifier(NULL),
	m_piUnitCombatFreeExperience(NULL),
	m_piUnitCombatProductionModifiers(NULL),
	m_piUnitCombatProductionModifiersGlobal(NULL),
	m_piDomainFreeExperience(NULL),
	m_piDomainFreeExperiencePerGreatWork(NULL),
#if defined(MOD_BALANCE_CORE)
	m_piDomainFreeExperiencePerGreatWorkGlobal(NULL),
	m_piDomainFreeExperienceGlobal(),
#endif
	m_piDomainProductionModifier(NULL),
	m_piPrereqNumOfBuildingClass(NULL),
	m_piFlavorValue(NULL),
	m_viLocalResourceAnds(),
	m_viLocalResourceOrs(),
#if defined(MOD_BALANCE_CORE_RESOURCE_MONOPOLIES)
	m_piLocalFeatureOrs(NULL),
	m_piLocalFeatureAnds(NULL),
	m_viResourceMonopolyAnds(),
	m_viResourceMonopolyOrs(),
	m_iGPRateModifierPerXFranchises(0),
	m_piResourceQuantityPerXFranchises(NULL),
	m_piYieldChangePerMonopoly(NULL),
	m_piYieldChangeFromPassingTR(NULL),
	m_piYieldPerFranchise(NULL),
#endif
	m_piResourceQuantityFromPOP(NULL),
	m_paiHurryModifier(NULL),
	m_pbBuildingClassNeededInCity(NULL),
#if defined(MOD_BALANCE_CORE)
	m_paiHurryModifierLocal(NULL),
	m_pbBuildingClassNeededAnywhere(NULL),
	m_pbBuildingClassNeededNowhere(NULL),
	m_piNumSpecFreeUnits(NULL),
	m_ppiResourcePlotsToPlace(),
	m_piYieldPerFriend(NULL),
	m_piYieldPerAlly(NULL),
	m_piYieldChangeWorldWonder(NULL),
	m_piYieldChangeWorldWonderGlobal(NULL),
	m_piLuxuryYieldChanges(NULL),
#endif
	m_piNumFreeUnits(NULL),
	m_bArtInfoEraVariation(false),
	m_bArtInfoCulturalVariation(false),
	m_bArtInfoRandomVariation(false),
	m_ppaiResourceYieldChange(NULL),
	m_ppaiFeatureYieldChange(NULL),
#if defined(MOD_BALANCE_CORE)
	m_ppiResourceYieldChangeGlobal(),
	m_miTechEnhancedYields(),
	m_miBonusFromAccomplishments(),
	m_miYieldChangesFromAccomplishments(),
	m_miGreatPersonPointFromConstruction(),
	m_ppaiImprovementYieldChange(NULL),
	m_ppaiImprovementYieldChangeGlobal(NULL),
	m_ppaiSpecialistYieldChangeLocal(NULL),
	m_paiResourceHappinessChange(NULL),
#endif
	m_ppaiSpecialistYieldChange(NULL),
	m_ppaiResourceYieldModifier(NULL),
	m_ppaiTerrainYieldChange(NULL),
	m_ppaiYieldPerXTerrain(NULL),
	m_ppaiYieldPerXFeature(NULL),
	m_ppYieldPerXImprovementLocal(),
	m_ppYieldPerXImprovementGlobal(),
	m_ppaiPlotYieldChange(NULL),
	m_ppiBuildingClassYieldChanges(NULL),
	m_ppiBuildingClassYieldModifiers(NULL),
#if defined(MOD_BALANCE_CORE)
	m_ppiBuildingClassLocalYieldChanges(NULL),
#endif
	m_paiBuildingClassHappiness(NULL),
	m_paYieldFromYield(NULL),
	m_paYieldFromYieldGlobal(NULL),
	m_paThemingBonusInfo(NULL),
#if defined(MOD_BALANCE_CORE_BUILDING_INSTANT_YIELD)
	m_piInstantYield(NULL),
	m_paiBuildingClassLocalHappiness(NULL),
	m_paiSpecificGreatPersonRateModifier(NULL),
#endif
#if defined(MOD_BALANCE_CORE)
	m_piiGreatPersonProgressFromConstruction(),
#endif
	m_iNumThemingBonuses(0)
{
}

/// Destructor
CvBuildingEntry::~CvBuildingEntry(void)
{
	SAFE_DELETE_ARRAY(m_piLockedBuildingClasses);
	SAFE_DELETE_ARRAY(m_piPrereqAndTechs);
	SAFE_DELETE_ARRAY(m_piResourceQuantityRequirements);
	SAFE_DELETE_ARRAY(m_piResourceQuantity);
	SAFE_DELETE_ARRAY(m_piResourceCultureChanges);
	SAFE_DELETE_ARRAY(m_piResourceFaithChanges);
	SAFE_DELETE_ARRAY(m_piProductionTraits);
	SAFE_DELETE_ARRAY(m_piSeaPlotYieldChange);
	SAFE_DELETE_ARRAY(m_piRiverPlotYieldChange);
	SAFE_DELETE_ARRAY(m_piLakePlotYieldChange);
	SAFE_DELETE_ARRAY(m_piLakePlotYieldChangeGlobal);
	SAFE_DELETE_ARRAY(m_piSeaResourceYieldChange);
	SAFE_DELETE_ARRAY(m_piGrowthExtraYield);
#if defined(MOD_BALANCE_CORE_POLICIES)
	SAFE_DELETE_ARRAY(m_piYieldFromDeath);
#endif
#if defined(MOD_BALANCE_CORE)
	SAFE_DELETE_ARRAY(m_piYieldFromVictory);
	SAFE_DELETE_ARRAY(m_piYieldFromVictoryGlobal);
	SAFE_DELETE_ARRAY(m_piYieldFromVictoryGlobalEraScaling);
	SAFE_DELETE_ARRAY(m_piYieldFromVictoryGlobalInGoldenAge);
	SAFE_DELETE_ARRAY(m_piYieldFromVictoryGlobalInGoldenAgeEraScaling);
	SAFE_DELETE_ARRAY(m_piYieldFromVictoryGlobalPlayer);
	SAFE_DELETE_ARRAY(m_piYieldFromPillage);
	SAFE_DELETE_ARRAY(m_piYieldFromPillageGlobal);
	SAFE_DELETE_ARRAY(m_piYieldFromPillageGlobalPlayer);
	SAFE_DELETE_ARRAY(m_piYieldFromGoldenAgeStart);
	SAFE_DELETE_ARRAY(m_piYieldChangePerGoldenAge);
	SAFE_DELETE_ARRAY(m_piYieldChangePerGoldenAgeCap);
	SAFE_DELETE_ARRAY(m_piGoldenAgeYieldMod);
	SAFE_DELETE_ARRAY(m_piYieldChangesPerLocalTheme);
	SAFE_DELETE_ARRAY(m_piYieldFromUnitGiftGlobal);
	SAFE_DELETE_ARRAY(m_piYieldFromWLTKD);
	SAFE_DELETE_ARRAY(m_piInstantYieldFromWLTKDStart);
	SAFE_DELETE_ARRAY(m_piYieldFromGPExpend);
	SAFE_DELETE_ARRAY(m_piThemingYieldBonus);
	SAFE_DELETE_ARRAY(m_piYieldFromSpyAttack);
	SAFE_DELETE_ARRAY(m_piYieldFromSpyDefense);
	SAFE_DELETE_ARRAY(m_piYieldFromSpyIdentify);
	SAFE_DELETE_ARRAY(m_piYieldFromSpyDefenseOrID);
	SAFE_DELETE_ARRAY(m_piYieldFromSpyRigElection);
	SAFE_DELETE_ARRAY(m_piYieldChangesPerCityStrengthTimes100);
	SAFE_DELETE_ARRAY(m_piGreatWorkYieldChange);
	SAFE_DELETE_ARRAY(m_piGreatWorkYieldChangeLocal);
	SAFE_DELETE_ARRAY(m_piYieldFromTech);
	SAFE_DELETE_ARRAY(m_piYieldFromConstruction);
	SAFE_DELETE_ARRAY(m_piYieldFromInternalTREnd);
	SAFE_DELETE_ARRAY(m_piYieldFromInternationalTREnd);
	SAFE_DELETE_ARRAY(m_piYieldFromInternal);
	SAFE_DELETE_ARRAY(m_piYieldFromProcessModifier);
	SAFE_DELETE_ARRAY(m_piYieldFromLongCount);
	SAFE_DELETE_ARRAY(m_piYieldFromGPBirthScaledWithWriterBulb);
	SAFE_DELETE_ARRAY(m_piYieldFromGPBirthScaledWithArtistBulb);
	m_miYieldFromGPBirthScaledWithPerTurnYield.clear();
	SAFE_DELETE_ARRAY(m_piYieldFromBirth);
	SAFE_DELETE_ARRAY(m_piYieldFromBirthEraScaling);
	SAFE_DELETE_ARRAY(m_piYieldFromBirthRetroactive);
	SAFE_DELETE_ARRAY(m_piYieldFromUnitProduction);
	SAFE_DELETE_ARRAY(m_piYieldFromBorderGrowth);
	SAFE_DELETE_ARRAY(m_piYieldFromPolicyUnlock);
	SAFE_DELETE_ARRAY(m_piYieldFromUnitLevelUp);
	SAFE_DELETE_ARRAY(m_piYieldFromCombatExperienceTimes100);
	SAFE_DELETE_ARRAY(m_piYieldFromPurchase);
	SAFE_DELETE_ARRAY(m_piYieldFromPurchaseGlobal);
	SAFE_DELETE_ARRAY(m_piYieldFromFaithPurchase);
	SAFE_DELETE_ARRAY(m_piYieldFromUnitLevelUpGlobal);
#endif
	SAFE_DELETE_ARRAY(m_piYieldChange);
	SAFE_DELETE_ARRAY(m_piYieldChangeEraScalingTimes100);
	SAFE_DELETE_ARRAY(m_pfYieldChangePerBuilding);
	SAFE_DELETE_ARRAY(m_pfYieldChangePerTile);
	SAFE_DELETE_ARRAY(m_pfYieldChangePerCityStateStrategicResource);
	SAFE_DELETE_ARRAY(m_piYieldChangePerPop);
	m_piYieldChangePerPopInEmpire.clear();
	m_miExtraPlayerInstancesFromAccomplishments.clear();
	m_siUnitClassTrainingAllowed.clear();
	m_sibResourceClaim.clear();
	m_miWLTKDFromProject.clear();
	SAFE_DELETE_ARRAY(m_piYieldChangePerReligion);
	SAFE_DELETE_ARRAY(m_piYieldModifier);
	SAFE_DELETE_ARRAY(m_piAreaYieldModifier);
	SAFE_DELETE_ARRAY(m_piGlobalYieldModifier);
	SAFE_DELETE_ARRAY(m_piUnitCombatFreeExperience);
	SAFE_DELETE_ARRAY(m_piUnitCombatProductionModifiers);
	SAFE_DELETE_ARRAY(m_piUnitCombatProductionModifiersGlobal);
	SAFE_DELETE_ARRAY(m_piDomainFreeExperience);
	SAFE_DELETE_ARRAY(m_piDomainFreeExperiencePerGreatWork);
#if defined(MOD_BALANCE_CORE)
	SAFE_DELETE_ARRAY(m_piDomainFreeExperiencePerGreatWorkGlobal);
	m_piDomainFreeExperienceGlobal.clear();
#endif
	SAFE_DELETE_ARRAY(m_piDomainProductionModifier);
	SAFE_DELETE_ARRAY(m_piPrereqNumOfBuildingClass);
	SAFE_DELETE_ARRAY(m_piFlavorValue);
	m_viLocalResourceAnds.clear();
	m_viLocalResourceOrs.clear();
#if defined(MOD_BALANCE_CORE_RESOURCE_MONOPOLIES)
	SAFE_DELETE_ARRAY(m_piLocalFeatureOrs);
	SAFE_DELETE_ARRAY(m_piLocalFeatureAnds);
	m_viResourceMonopolyAnds.clear();
	m_viResourceMonopolyOrs.clear();
	SAFE_DELETE_ARRAY(m_piYieldChangePerMonopoly);
	SAFE_DELETE_ARRAY(m_piYieldChangeFromPassingTR);
	SAFE_DELETE_ARRAY(m_piYieldPerFranchise);
	SAFE_DELETE_ARRAY(m_piResourceQuantityPerXFranchises);
#endif
	SAFE_DELETE_ARRAY(m_piResourceQuantityFromPOP);
	SAFE_DELETE_ARRAY(m_paiHurryModifier);
	SAFE_DELETE_ARRAY(m_pbBuildingClassNeededInCity);
#if defined(MOD_BALANCE_CORE)
	SAFE_DELETE_ARRAY(m_paiHurryModifierLocal);
	SAFE_DELETE_ARRAY(m_pbBuildingClassNeededAnywhere);
	SAFE_DELETE_ARRAY(m_pbBuildingClassNeededNowhere);
	SAFE_DELETE_ARRAY(m_piNumSpecFreeUnits);
	m_ppiResourcePlotsToPlace.clear();
	SAFE_DELETE_ARRAY(m_paiResourceHappinessChange);
	SAFE_DELETE_ARRAY(m_piYieldPerFriend);
	SAFE_DELETE_ARRAY(m_piYieldPerAlly);
	SAFE_DELETE_ARRAY(m_piYieldChangeWorldWonder);
	SAFE_DELETE_ARRAY(m_piYieldChangeWorldWonderGlobal);
	SAFE_DELETE_ARRAY(m_piLuxuryYieldChanges);
#endif
	SAFE_DELETE_ARRAY(m_piNumFreeUnits);
	SAFE_DELETE_ARRAY(m_paiBuildingClassHappiness);
	SAFE_DELETE_ARRAY(m_paThemingBonusInfo);
	SAFE_DELETE_ARRAY(m_paYieldFromYield);
	SAFE_DELETE_ARRAY(m_paYieldFromYieldGlobal);
#if defined(MOD_BALANCE_CORE_BUILDING_INSTANT_YIELD)
	SAFE_DELETE_ARRAY(m_piInstantYield);

#endif
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiResourceYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiFeatureYieldChange);
#if defined(MOD_BALANCE_CORE)
	m_ppiResourceYieldChangeGlobal.clear();
	m_miTechEnhancedYields.clear();
	m_miBonusFromAccomplishments.clear();
	m_miYieldChangesFromAccomplishments.clear();
	m_miGreatPersonPointFromConstruction.clear();
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiImprovementYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiImprovementYieldChangeGlobal);
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiSpecialistYieldChangeLocal);
#endif
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiSpecialistYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiResourceYieldModifier);
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiTerrainYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiYieldPerXTerrain);
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiYieldPerXFeature);
	m_ppYieldPerXImprovementLocal.clear();
	m_ppYieldPerXImprovementGlobal.clear();
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiPlotYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiBuildingClassYieldChanges);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiBuildingClassYieldModifiers);
#if defined(MOD_BALANCE_CORE)
	SAFE_DELETE_ARRAY(m_paiBuildingClassLocalHappiness);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiBuildingClassLocalYieldChanges);
	SAFE_DELETE_ARRAY(m_paiSpecificGreatPersonRateModifier);
#endif
#if defined(MOD_BALANCE_CORE)
	m_piiGreatPersonProgressFromConstruction.clear();
#endif
}

/// Read from XML file
bool CvBuildingEntry::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if(!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	//Basic Properties
	m_iGoldMaintenance = kResults.GetInt("GoldMaintenance");
	m_iMutuallyExclusiveGroup = kResults.GetInt("MutuallyExclusiveGroup");
	m_bTeamShare = kResults.GetBool("TeamShare");
	m_bWater = kResults.GetBool("Water");
	m_bRiver = kResults.GetBool("River");
	m_bFreshWater = kResults.GetBool("FreshWater");
	m_bAnyWater = kResults.GetBool("AnyWater");
	m_bAddsFreshWater = kResults.GetBool("AddsFreshWater");
	m_bPurchaseOnly = kResults.GetBool("PurchaseOnly");
	m_bSecondaryPantheon = kResults.GetBool("SecondaryPantheon");
#if defined(MOD_BALANCE_CORE)
	m_iNumRequiredTier3Tenets = kResults.GetInt("NumRequiredTier3Tenets");
	m_bIsNoWater = kResults.GetBool("IsNoWater");
	m_bIsNoRiver = kResults.GetBool("IsNoRiver");
	m_bIsNoCoast = kResults.GetBool("IsNoCoast");
	m_bIsCapitalOnly = kResults.GetBool("CapitalOnly");
	m_bIsReformation = kResults.GetBool("IsReformation");
	m_bBuildAnywhere = kResults.GetBool("BuildAnywhere");
	m_iTradeReligionModifier = kResults.GetInt("TradeReligionModifier");
	m_iReformationFollowerReduction = kResults.GetInt("ReformationFollowerReduction");
	m_iFreeArtifacts = kResults.GetInt("FreeArtifacts");
	m_iResourceDiversityModifier = kResults.GetInt("ResourceDiversityModifier");
	m_iNoUnhappfromXSpecialists = kResults.GetInt("NoUnhappfromXSpecialists");
	m_iNoUnhappfromXSpecialistsGlobal = kResults.GetInt("NoUnhappfromXSpecialistsGlobal");
	m_iPurchaseCooldownReduction = kResults.GetInt("PurchaseCooldownReduction");
	m_iPurchaseCooldownReductionCivilian = kResults.GetInt("PurchaseCooldownReductionCivilian");
#endif
	m_iVassalLevyEra = kResults.GetInt("VassalLevyEra");
#if defined(MOD_BALANCE_CORE_POP_REQ_BUILDINGS)
	m_iNationalPopRequired = kResults.GetInt("NationalPopRequired");
	m_iLocalPopRequired = kResults.GetInt("LocalPopRequired");
#endif
#if defined(MOD_BALANCE_CORE_FOLLOWER_POP_WONDER)
	m_iNationalFollowerPopRequired = kResults.GetInt("NationalFollowerPopRequired");
	m_iGlobalFollowerPopRequired = kResults.GetInt("GlobalFollowerPopRequired");
#endif
	m_bMountain = kResults.GetBool("Mountain");
	m_bHill = kResults.GetBool("Hill");
	m_bFlat = kResults.GetBool("Flat");
	m_bBorderObstacle = kResults.GetBool("BorderObstacle");
#if defined(MOD_BALANCE_CORE)
	m_iCityAirStrikeDefense = kResults.GetInt("CityAirStrikeDefense");
	m_iBorderObstacleCity = kResults.GetInt("BorderObstacleCity");
	m_iBorderObstacleWater = kResults.GetInt("BorderObstacleWater");
	m_iDeepWaterTileDamage = kResults.GetInt("DeepWaterTileDamage");
	m_iWLTKDTurns = kResults.GetInt("WLTKDTurns");
	m_iEventTourism = kResults.GetInt("EventTourism");
	m_iLandTourism = kResults.GetInt("FinishLandTRTourism");
	m_iSeaTourism = kResults.GetInt("FinishSeaTRTourism");
	m_iAlwaysHeal = kResults.GetInt("AlwaysHeal");
	m_bIsCorp = kResults.GetBool("IsCorporation");
	m_iNukeInterceptionChance = kResults.GetInt("NukeInterceptionChance");
#endif
	m_iFoodBonusPerCityMajorityFollower = kResults.GetInt("FoodBonusPerCityMajorityFollower");
#if defined(HH_MOD_BUILDINGS_FRUITLESS_PILLAGE)
	m_bPlayerBorderGainlessPillage = kResults.GetBool("PlayerBorderGainlessPillage");
	m_bCityGainlessPillage = kResults.GetBool("CityGainlessPillage");
#endif
	m_bPlayerBorderObstacle = kResults.GetBool("PlayerBorderObstacle");
	m_bCapital = kResults.GetBool("Capital");
	m_bGoldenAge = kResults.GetBool("GoldenAge");
	m_bMapCentering = kResults.GetBool("MapCentering");
	m_bNeverCapture = kResults.GetBool("NeverCapture");
	m_bNukeImmune = kResults.GetBool("NukeImmune");
	m_bCityWall = kResults.GetBool("CityWall");
	m_bExtraLuxuries = kResults.GetBool("ExtraLuxuries");
	m_bDiplomaticVoting = kResults.GetBool("DiplomaticVoting");
	m_bAllowsWaterRoutes = kResults.GetBool("AllowsWaterRoutes");
	m_bAllowsIndustrialWaterRoutes = kResults.GetBool("AllowsIndustrialWaterRoutes");
	m_bAllowsAirRoutes = kResults.GetBool("AllowsAirRoutes");
	m_iProductionCost = kResults.GetInt("Cost");
	m_iFaithCost = kResults.GetInt("FaithCost");
	m_iLeagueCost = kResults.GetInt("LeagueCost");
	m_bUnlockedByBelief = kResults.GetBool("UnlockedByBelief");
	m_bUnlockedByLeague = kResults.GetBool("UnlockedByLeague");
	m_bRequiresHolyCity = kResults.GetBool("HolyCity");
	m_bAffectSpiesNow = kResults.GetBool("AffectSpiesNow");
	m_bEspionage = kResults.GetBool("Espionage");
	m_bAllowsFoodTradeRoutes = kResults.GetBool("AllowsFoodTradeRoutes");
	m_bAllowsFoodTradeRoutesGlobal = kResults.GetBool("AllowsFoodTradeRoutesGlobal");
	m_bAllowsProductionTradeRoutes = kResults.GetBool("AllowsProductionTradeRoutes");
	m_bAllowsProductionTradeRoutesGlobal = kResults.GetBool("AllowsProductionTradeRoutesGlobal");
	m_bNullifyInfluenceModifier = kResults.GetBool("NullifyInfluenceModifier");
	m_iNumCityCostMod = kResults.GetInt("NumCityCostMod");
	m_iHurryCostModifier = kResults.GetInt("HurryCostModifier");
	m_iMinAreaSize = kResults.GetInt("MinAreaSize");
	m_iConquestProbability = kResults.GetInt("ConquestProb");
	m_iNumCitiesPrereq = kResults.GetInt("CitiesPrereq");
	m_iUnitLevelPrereq = kResults.GetInt("LevelPrereq");
	m_iCultureRateModifier = kResults.GetInt("CultureRateModifier");
	m_iGlobalCultureRateModifier = kResults.GetInt("GlobalCultureRateModifier");
	m_iGreatPeopleRateModifier = kResults.GetInt("GreatPeopleRateModifier");
	m_iGlobalGreatPeopleRateModifier = kResults.GetInt("GlobalGreatPeopleRateModifier");
	m_iGPRateModifierPerMarriage = kResults.GetInt("GPRateModifierPerMarriage");
	m_iGPRateModifierPerLocalTheme = kResults.GetInt("GPRateModifierPerLocalTheme");
	m_iGPPOnCitizenBirth = kResults.GetInt("GPPOnCitizenBirth");
	m_iGreatGeneralRateModifier = kResults.GetInt("GreatGeneralRateModifier");
	m_iGreatPersonExpendGold = kResults.GetInt("GreatPersonExpendGold");
	m_iUnitUpgradeCostMod = kResults.GetInt("UnitUpgradeCostMod");
	m_iGoldenAgeModifier = kResults.GetInt("GoldenAgeModifier");
	m_iFreeExperience = kResults.GetInt("Experience");
	m_iGlobalFreeExperience = kResults.GetInt("GlobalExperience");
	m_iGlobalHappinessPerMajorWar = kResults.GetInt("GlobalHappinessPerMajorWar");
	m_iGlobalMilitaryProductionModPerMajorWar = kResults.GetInt("GlobalMilitaryProductionModPerMajorWar");
	m_iFoodKept = kResults.GetInt("FoodKept");
	m_bAirlift = kResults.GetBool("Airlift");
	m_iAirModifier = kResults.GetInt("AirModifier");
	m_iAirModifierGlobal = kResults.GetInt("AirModifierGlobal");
	m_iNukeModifier = kResults.GetInt("NukeModifier");
	m_iNukeExplosionRand = kResults.GetInt("NukeExplosionRand");
	m_iHealRateChange = kResults.GetInt("HealRateChange");
	m_iHappiness = kResults.GetInt("Happiness");
	m_iUnmoddedHappiness = kResults.GetInt("UnmoddedHappiness");
	m_iUnhappinessModifier = kResults.GetInt("UnhappinessModifier");
#if defined(MOD_BALANCE_CORE)
	m_iLocalUnhappinessModifier = kResults.GetInt("LocalUnhappinessModifier");
	m_iGlobalBuildingGoldMaintenanceMod = kResults.GetInt("GlobalBuildingGoldMaintenanceMod");
	m_iBuildingDefenseModifier = kResults.GetInt("BuildingDefenseModifier");
	m_iDamageReductionFlat = kResults.GetInt("DamageReductionFlat");
	m_iCitySupplyModifier = kResults.GetInt("CitySupplyModifier");
	m_iCitySupplyModifierGlobal = kResults.GetInt("CitySupplyModifierGlobal");
	m_iCitySupplyFlat = kResults.GetInt("CitySupplyFlat");
	m_iCitySupplyFlatGlobal = kResults.GetInt("CitySupplyFlatGlobal");
	m_iCityRangedStrikeRange = kResults.GetInt("CityRangedStrikeRange");
	m_iCityIndirectFire = kResults.GetInt("CityIndirectFire");
	m_iRangedStrikeModifier = kResults.GetInt("RangedStrikeModifier");
	m_iGarrisonRangedAttackModifier = kResults.GetInt("GarrisonRangedAttackModifier");
#endif
	m_iHappinessPerCity = kResults.GetInt("HappinessPerCity");
	m_iHappinessPerXPolicies = kResults.GetInt("HappinessPerXPolicies");
	m_iCityCountUnhappinessMod = kResults.GetInt("CityCountUnhappinessMod");
	m_bNoOccupiedUnhappiness = kResults.GetBool("NoOccupiedUnhappiness");
	m_iWorkerSpeedModifier = kResults.GetInt("WorkerSpeedModifier");
	m_iMilitaryProductionModifier = kResults.GetInt("MilitaryProductionModifier");
	m_iSpaceProductionModifier = kResults.GetInt("SpaceProductionModifier");
	m_iGlobalSpaceProductionModifier = kResults.GetInt("GlobalSpaceProductionModifier");
	m_iBuildingProductionModifier = kResults.GetInt("BuildingProductionModifier");
	m_iWonderProductionModifier = kResults.GetInt("WonderProductionModifier");
	m_iCityConnectionTradeRouteModifier = kResults.GetInt("CityConnectionTradeRouteModifier");
	m_iCapturePlunderModifier = kResults.GetInt("CapturePlunderModifier");
	m_iPolicyCostModifier = kResults.GetInt("PolicyCostModifier");
	m_iDiplomatInfluenceBoost = kResults.GetInt("DiplomatInfluenceBoost");
	m_iBorderGrowthRateIncrease = kResults.GetInt("BorderGrowthRateIncrease");
	m_iBorderGrowthRateIncreaseGlobal = kResults.GetInt("BorderGrowthRateIncreaseGlobal");
	m_iPlotCultureCostModifier = kResults.GetInt("PlotCultureCostModifier");
	m_iGlobalPlotCultureCostModifier = kResults.GetInt("GlobalPlotCultureCostModifier");
	m_iPlotBuyCostModifier = kResults.GetInt("PlotBuyCostModifier");
	m_iGlobalPlotBuyCostModifier = kResults.GetInt("GlobalPlotBuyCostModifier");
#if defined(MOD_BUILDINGS_CITY_WORKING)
	m_iCityWorkingChange = kResults.GetInt("CityWorkingChange");
	m_iGlobalCityWorkingChange = kResults.GetInt("GlobalCityWorkingChange");
#endif
#if defined(MOD_BUILDINGS_CITY_AUTOMATON_WORKERS)
	m_iCityAutomatonWorkersChange = kResults.GetInt("CityAutomatonWorkersChange");
	m_iGlobalCityAutomatonWorkersChange = kResults.GetInt("GlobalCityAutomatonWorkersChange");
#endif
	m_iGlobalPopulationChange = kResults.GetInt("GlobalPopulationChange");
	m_iPopulationChange = kResults.GetInt("PopulationChange");
	m_iTechShare = kResults.GetInt("TechShare");
	m_iFreeTechs = kResults.GetInt("FreeTechs");
	m_iFreePolicies = kResults.GetInt("FreePolicies");
	m_iFreeGreatPeople = kResults.GetInt("FreeGreatPeople");
	m_iMedianTechPercentChange = kResults.GetInt("MedianTechPercentChange");
	m_iGold = kResults.GetInt("Gold");
	m_bNearbyMountainRequired = kResults.GetInt("NearbyMountainRequired")>0;
	m_bAllowsRangeStrike = kResults.GetInt("AllowsRangeStrike")>0;
	m_iDefenseModifier = kResults.GetInt("Defense");
	m_iGlobalDefenseModifier = kResults.GetInt("GlobalDefenseMod");
	m_iExtraCityHitPoints = kResults.GetInt("ExtraCityHitPoints");
	m_iMinorFriendshipChange = kResults.GetInt("MinorFriendshipChange");
	m_iVictoryPoints = kResults.GetInt("VictoryPoints");
	m_iExtraMissionarySpreads = kResults.GetInt("ExtraMissionarySpreads");
	m_iExtraMissionaryStrength = kResults.GetInt("ExtraMissionaryStrengthGlobal");
	m_iExtraMissionarySpreadsGlobal = kResults.GetInt("ExtraMissionarySpreadsGlobal");
	m_iReligiousPressureModifier = kResults.GetInt("ReligiousPressureModifier");
	m_iEspionageModifier = kResults.GetInt("EspionageModifier");
	m_iGlobalEspionageModifier = kResults.GetInt("GlobalEspionageModifier");
	m_iSpySecurityModifier = kResults.GetInt("SpySecurityModifier");
	m_iSpySecurityModifierPerXPop = kResults.GetInt("SpySecurityModifierPerXPop");
	m_iGlobalSpySecurityModifier = kResults.GetInt("GlobalSpySecurityModifier");
	m_iExtraSpies = kResults.GetInt("ExtraSpies");
	m_iSpyRankChange = kResults.GetInt("SpyRankChange");
	m_iTradeRouteRecipientBonus = kResults.GetInt("TradeRouteRecipientBonus");
	m_iTradeRouteTargetBonus = kResults.GetInt("TradeRouteTargetBonus");
	m_iNumTradeRouteBonus = kResults.GetInt("NumTradeRouteBonus");
	m_iTradeRouteSeaDistanceModifier = kResults.GetInt("TradeRouteSeaDistanceModifier");
	m_iTradeRouteSeaGoldBonus = kResults.GetInt("TradeRouteSeaGoldBonus");
	m_iTradeRouteLandDistanceModifier = kResults.GetInt("TradeRouteLandDistanceModifier");
	m_iTradeRouteLandGoldBonus = kResults.GetInt("TradeRouteLandGoldBonus");
	m_iCityConnectionTradeRouteGoldModifier = kResults.GetInt("CityConnectionGoldModifier");
	m_iCityStateTradeRouteProductionModifier = kResults.GetInt("CityStateTradeRouteProductionModifier");
#if defined(MOD_RELIGION_CONVERSION_MODIFIERS)
	m_iConversionModifier = kResults.GetInt("ConversionModifier");
	m_iGlobalConversionModifier = kResults.GetInt("GlobalConversionModifier");
#endif
	m_iInstantSpyRankChange = kResults.GetInt("InstantSpyRankChange");
	m_iLandmarksTourismPercent = kResults.GetInt("LandmarksTourismPercent");
	m_iInstantMilitaryIncrease = kResults.GetInt("InstantMilitaryIncrease");
	m_iGreatWorksTourismModifier = kResults.GetInt("GreatWorksTourismModifier");
	m_iXBuiltTriggersIdeologyChoice = kResults.GetInt("XBuiltTriggersIdeologyChoice");
	m_iGreatScientistBeakerModifier = kResults.GetInt("GreatScientistBeakerModifier");
	m_iExtraLeagueVotes = kResults.GetInt("ExtraLeagueVotes");
	m_iSingleLeagueVotes = kResults.GetInt("SingleLeagueVotes");
	m_iFaithToVotesBase = kResults.GetInt("FaithToVotes");
	m_iCapitalsToVotesBase = kResults.GetInt("CapitalsToVotes");
	m_iDoFToVotesBase = kResults.GetInt("DoFToVotes");
	m_iRAToVotesBase = kResults.GetInt("RAToVotes");
	m_iDPToVotesBase = kResults.GetInt("DPToVotes");
	m_bIgnoreDefensivePactLimit = kResults.GetBool("IgnoreDefensivePactLimit");
	m_iGPExpendInfluenceBase = kResults.GetInt("GPExpendInfluence");
	m_iEmpireSizeModifierReduction = kResults.GetInt("EmpireSizeModifierReduction");
	m_iEmpireSizeModifierReductionGlobal = kResults.GetInt("EmpireSizeModifierReductionGlobal");
	m_iBasicNeedsMedianModifier = kResults.GetInt("BasicNeedsMedianModifier");
	m_iGoldMedianModifier = kResults.GetInt("GoldMedianModifier");
	m_iScienceMedianModifier = kResults.GetInt("ScienceMedianModifier");
	m_iCultureMedianModifier = kResults.GetInt("CultureMedianModifier");
	m_iReligiousUnrestModifier = kResults.GetInt("ReligiousUnrestModifier");
	m_iBasicNeedsMedianModifierGlobal = kResults.GetInt("BasicNeedsMedianModifierGlobal");
	m_iGoldMedianModifierGlobal = kResults.GetInt("GoldMedianModifierGlobal");
	m_iScienceMedianModifierGlobal = kResults.GetInt("ScienceMedianModifierGlobal");
	m_iCultureMedianModifierGlobal = kResults.GetInt("CultureMedianModifierGlobal");
	m_iReligiousUnrestModifierGlobal = kResults.GetInt("ReligiousUnrestModifierGlobal");
	m_iPreferredDisplayPosition = kResults.GetInt("DisplayPosition");
	m_iPortraitIndex = kResults.GetInt("PortraitIndex");

	m_bArtInfoCulturalVariation = kResults.GetBool("ArtInfoCulturalVariation");
	m_bArtInfoEraVariation = kResults.GetBool("ArtInfoEraVariation");
	m_bArtInfoRandomVariation = kResults.GetBool("ArtInfoRandomVariation");

	//References
	const char* szTextVal = NULL;
	szTextVal = kResults.GetText("BuildingClass");
	m_iBuildingClassType = GC.getInfoTypeForString(szTextVal, true);

	//This may need to be deferred to a routine that is called AFTER pre-fetch has been called for all infos.
	m_pkBuildingClassInfo = GC.getBuildingClassInfo(static_cast<BuildingClassTypes>(m_iBuildingClassType));
	ASSERT_DEBUG(m_pkBuildingClassInfo, "Could not find BuildingClassInfo for BuildingType. Have BuildingClasses been prefetched yet?");

	szTextVal = kResults.GetText("ArtDefineTag");
	SetArtDefineTag(szTextVal);

	szTextVal = kResults.GetText("WonderSplashAudio");
	m_strWonderSplashAudio = szTextVal;

	szTextVal = kResults.GetText("ThemingBonusHelp");
	m_strThemingBonusHelp = szTextVal;

	szTextVal = kResults.GetText("NearbyTerrainRequired");
	m_iNearbyTerrainRequired = GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("ProhibitedCityTerrain");
	m_iProhibitedCityTerrain = GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("VictoryPrereq");
	m_iVictoryPrereq = GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("FreeStartEra");
	m_iFreeStartEra = GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("MaxStartEra");
	m_iMaxStartEra = GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("ObsoleteTech");
	m_iObsoleteTech = GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("FreeBuilding");
	m_iFreeBuildingClass = GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("FreeBuildingThisCity");
	m_iFreeBuildingThisCity = GC.getInfoTypeForString(szTextVal, true);

#if defined(MOD_BALANCE_CORE)
	szTextVal = kResults.GetText("FreeBuildingTradeTargetCity");
	m_bTradeRouteInvulnerable = kResults.GetBool("TradeRouteInvulnerable");
	m_iTRSpeedBoost = kResults.GetInt("TRSpeedBoost");
	m_iTRVisionBoost = kResults.GetInt("TRVisionBoost");
	m_iTRTurnModGlobal = kResults.GetInt("TRTurnModGlobal");
	m_iTRTurnModLocal = kResults.GetInt("TRTurnModLocal");
	m_iVotesPerGPT = kResults.GetInt("VotesPerGPT");
	m_bRequiresIndustrialCityConnection = kResults.GetBool("RequiresIndustrialCityConnection");
	m_bDummy = kResults.GetBool("IsDummy");
	m_iLandmarksTourismPercentGlobal = kResults.GetInt("GlobalLandmarksTourismPercent");
	m_iGreatWorksTourismModifierGlobal = kResults.GetInt("GlobalGreatWorksTourismModifier");
#endif

#if defined(MOD_BALANCE_CORE_EVENTS)
	szTextVal = kResults.GetText("EventChoiceRequiredActive");
	m_iEventRequiredActive = GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("CityEventChoiceRequiredActive");
	m_iCityEventRequiredActive = GC.getInfoTypeForString(szTextVal, true);
#endif

	szTextVal = kResults.GetText("FreePromotion");
	m_iFreePromotion = GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("TrainedFreePromotion");
	m_iTrainedFreePromotion = GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("FreePromotionRemoved");
	m_iFreePromotionRemoved = GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("ReplacementBuildingClass");
	m_iReplacementBuildingClass= GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("PrereqTech");
	m_iPrereqAndTech = GC.getInfoTypeForString(szTextVal, true);

	szTextVal = kResults.GetText("PolicyBranchType");
	m_iPolicyBranchType = GC.getInfoTypeForString(szTextVal, true);

#if defined(MOD_BALANCE_CORE_POLICIES)
	szTextVal = kResults.GetText("PolicyType");
	m_iPolicyType = static_cast<PolicyTypes>(GC.getInfoTypeForString(szTextVal, true));

	const char* szSpecificCivilizationType = kResults.GetText("CivilizationRequired");
	m_eCivType = (CivilizationTypes)GC.getInfoTypeForString(szSpecificCivilizationType, true);
#endif
#if defined(MOD_BALANCE_CORE)
	szTextVal = kResults.GetText("ResourceType");
	m_iResourceType = GC.getInfoTypeForString(szTextVal, true);
	szTextVal = kResults.GetText("NeedBuildingThisCity");
	m_iNeedBuildingThisCity = GC.getInfoTypeForString(szTextVal, true);
	m_iGrantsRandomResourceTerritory = kResults.GetInt("GrantsRandomResourceTerritory");
	m_bPuppetPurchaseOverride = kResults.GetBool("PuppetPurchaseOverride");
	m_bAllowsPuppetPurchase = kResults.GetBool("AllowsPuppetPurchase");
	m_bNoStarvationNonSpecialist = kResults.GetBool("NoStarvationNonSpecialist");
	m_iGetCooldown = kResults.GetInt("PurchaseCooldown");
	m_iNumPoliciesNeeded = kResults.GetInt("NumPoliciesNeeded");
#endif

	szTextVal = kResults.GetText("SpecialistType");
	m_iSpecialistType = GC.getInfoTypeForString(szTextVal, true);
	m_iSpecialistCount = kResults.GetInt("SpecialistCount");
	m_iSpecialistExtraCulture = kResults.GetInt("SpecialistExtraCulture");
	m_iGreatPeopleRateChange= kResults.GetInt("GreatPeopleRateChange");

	szTextVal = kResults.GetText("GreatWorkSlotType");
	m_eGreatWorkSlotType = (GreatWorkSlotType)GC.getInfoTypeForString(szTextVal, true);
#if defined(MOD_GLOBAL_GREATWORK_YIELDTYPES)
	szTextVal = kResults.GetText("GreatWorkYieldType");
	m_eGreatWorkYieldType = (YieldTypes)GC.getInfoTypeForString(szTextVal, true);
#endif
	m_iGreatWorkCount = kResults.GetInt("GreatWorkCount");
	szTextVal = kResults.GetText("FreeGreatWork");
	m_eFreeGreatWork = (GreatWorkType)GC.getInfoTypeForString(szTextVal, true);

	m_iDistressFlatReduction = kResults.GetInt("DistressFlatReduction");
	m_iPovertyFlatReduction = kResults.GetInt("PovertyFlatReduction");
	m_iIlliteracyFlatReduction = kResults.GetInt("IlliteracyFlatReduction");
	m_iBoredomFlatReduction = kResults.GetInt("BoredomFlatReduction");
	m_iReligiousUnrestFlatReduction = kResults.GetInt("ReligiousUnrestFlatReduction");

	m_iDistressFlatReductionGlobal = kResults.GetInt("DistressFlatReductionGlobal");
	m_iPovertyFlatReductionGlobal = kResults.GetInt("PovertyFlatReductionGlobal");
	m_iIlliteracyFlatReductionGlobal = kResults.GetInt("IlliteracyFlatReductionGlobal");
	m_iBoredomFlatReductionGlobal = kResults.GetInt("BoredomFlatReductionGlobal");
	m_iReligiousUnrestFlatReductionGlobal = kResults.GetInt("ReligiousUnrestFlatReductionGlobal");
	
	m_iInstantReligionPressure = kResults.GetInt("InstantReligiousPressure");
	m_iExperiencePerGoldenAge = kResults.GetInt("ExperiencePerGoldenAge");
	m_iExperiencePerGoldenAgeCap = kResults.GetInt("ExperiencePerGoldenAgeCap");

	m_iBasePressureModGlobal = kResults.GetInt("BasePressureModifierGlobal");
	m_iDefensePerXWonder = kResults.GetInt("DefensePerXWonder");

	//Arrays
	const char* szBuildingType = GetType();

	kUtility.SetFlavors(m_piFlavorValue, "Building_Flavors", "BuildingType", szBuildingType);

	kUtility.SetYields(m_piSeaPlotYieldChange, "Building_SeaPlotYieldChanges", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piRiverPlotYieldChange, "Building_RiverPlotYieldChanges", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piLakePlotYieldChange, "Building_LakePlotYieldChanges", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piLakePlotYieldChangeGlobal, "Building_LakePlotYieldChangesGlobal", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piSeaResourceYieldChange, "Building_SeaResourceYieldChanges", "BuildingType", szBuildingType);
#if defined(MOD_BALANCE_CORE)
	kUtility.SetYields(m_piGrowthExtraYield, "Building_GrowthExtraYield", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromDeath, "Building_YieldFromDeath", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromVictory, "Building_YieldFromVictory", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromVictoryGlobal, "Building_YieldFromVictoryGlobal", "BuildingType", szBuildingType, "(IsEraScaling='false' or IsEraScaling='0') and (GoldenAgeOnly='false' or GoldenAgeOnly='0')");
	kUtility.SetYields(m_piYieldFromVictoryGlobalEraScaling, "Building_YieldFromVictoryGlobal", "BuildingType", szBuildingType, "(IsEraScaling='true' or IsEraScaling='1') and (GoldenAgeOnly='false' or GoldenAgeOnly='0')");
	kUtility.SetYields(m_piYieldFromVictoryGlobalInGoldenAge, "Building_YieldFromVictoryGlobal", "BuildingType", szBuildingType, "(IsEraScaling='false' or IsEraScaling='0') and (GoldenAgeOnly='true' or GoldenAgeOnly='1')");
	kUtility.SetYields(m_piYieldFromVictoryGlobalInGoldenAgeEraScaling, "Building_YieldFromVictoryGlobal", "BuildingType", szBuildingType, "(IsEraScaling='true' or IsEraScaling='1') and (GoldenAgeOnly='true' or GoldenAgeOnly='1')");
	kUtility.SetYields(m_piYieldFromVictoryGlobalPlayer, "Building_YieldFromVictoryGlobalPlayer", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromPillage, "Building_YieldFromPillage", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromPillageGlobal, "Building_YieldFromPillageGlobal", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromPillageGlobalPlayer, "Building_YieldFromPillageGlobalPlayer", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromGoldenAgeStart, "Building_YieldFromGoldenAgeStart", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldChangePerGoldenAge, "Building_YieldChangesPerGoldenAge", "BuildingType", szBuildingType);
	kUtility.PopulateArrayByValue(m_piYieldChangePerGoldenAgeCap, "Yields", "Building_YieldChangesPerGoldenAge", "YieldType", "BuildingType", szBuildingType, "YieldCap");
	kUtility.SetYields(m_piGoldenAgeYieldMod, "Building_GoldenAgeYieldMod", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldChangesPerLocalTheme, "Building_YieldChangesPerLocalTheme", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromUnitGiftGlobal, "Building_YieldFromUnitGiftGlobal", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromWLTKD, "Building_WLTKDYieldMod", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piInstantYieldFromWLTKDStart, "Building_InstantYieldFromWLTKDStart", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piInstantYieldFromWLTKDStart, "Building_InstantYieldFromWLTKDStart", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromGPExpend, "Building_YieldFromGPExpend", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piThemingYieldBonus, "Building_ThemingYieldBonus", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromSpyAttack, "Building_YieldFromSpyAttack", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromSpyDefense, "Building_YieldFromSpyDefense", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromSpyIdentify, "Building_YieldFromSpyIdentify", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromSpyDefenseOrID, "Building_YieldFromSpyDefenseOrID", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromSpyRigElection, "Building_YieldFromSpyRigElection", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldChangesPerCityStrengthTimes100, "Building_YieldChangesPerCityStrengthTimes100", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piGreatWorkYieldChange, "Building_GreatWorkYieldChanges", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piGreatWorkYieldChangeLocal, "Building_GreatWorkYieldChangesLocal", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromTech, "Building_YieldFromTech", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromConstruction, "Building_YieldFromConstruction", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromInternalTREnd, "Building_YieldFromInternalTREnd", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromInternationalTREnd, "Building_YieldFromInternationalTREnd", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromInternal, "Building_YieldFromInternalTR", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromProcessModifier, "Building_YieldFromProcessModifier", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromLongCount, "Building_YieldFromLongCount", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromGPBirthScaledWithWriterBulb, "Building_YieldFromGPBirthScaledWithWriterBulb", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromGPBirthScaledWithArtistBulb, "Building_YieldFromGPBirthScaledWithArtistBulb", "BuildingType", szBuildingType);
	{
		std::string strKey("Building_YieldFromGPBirthScaledWithPerTurnYield");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select GreatPersons.ID as GreatPersonID, YieldsIn.ID as YieldInID, YieldsOut.ID as YieldOutID, Value from Building_YieldFromGPBirthScaledWithPerTurnYield inner join GreatPersons on GreatPersons.Type = GreatPersonType inner join Yields as YieldsIn on YieldsIn.Type = YieldIn inner join Yields as YieldsOut on YieldsOut.Type = YieldOut where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int iGreatPerson = pResults->GetInt(0);
			const int iYieldIn = pResults->GetInt(1);
			const int iYieldOut= pResults->GetInt(2);
			const int iYield = pResults->GetInt(3);

			m_miYieldFromGPBirthScaledWithPerTurnYield[(GreatPersonTypes)iGreatPerson][std::make_pair((YieldTypes)iYieldIn, (YieldTypes)iYieldOut)] += iYield;
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		map<GreatPersonTypes, map<pair<YieldTypes, YieldTypes>, int>>(m_miYieldFromGPBirthScaledWithPerTurnYield).swap(m_miYieldFromGPBirthScaledWithPerTurnYield);
	}
	{
		std::string strKey("Building_WLTKDFromProject");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Projects.ID as ProjectID, Turns from Building_WLTKDFromProject inner join Projects on Projects.Type = ProjectType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int iProjectID = pResults->GetInt(0);
			const int iTurns = pResults->GetInt(1);

			m_miWLTKDFromProject[(ProjectTypes)iProjectID] += iTurns;
		}

		pResults->Reset();
	}
	kUtility.SetYields(m_piYieldFromBirth, "Building_YieldFromBirth", "BuildingType", szBuildingType, "(IsEraScaling='false' or IsEraScaling='0')");
	kUtility.SetYields(m_piYieldFromBirthEraScaling, "Building_YieldFromBirth", "BuildingType", szBuildingType, "(IsEraScaling='true' or IsEraScaling='1')");
	kUtility.SetYields(m_piYieldFromBirthRetroactive, "Building_YieldFromBirthRetroactive", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromUnitProduction, "Building_YieldFromUnitProduction", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromBorderGrowth, "Building_YieldFromBorderGrowth", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromPolicyUnlock, "Building_YieldFromPolicyUnlock", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromUnitLevelUp, "Building_YieldFromUnitLevelUp", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromCombatExperienceTimes100, "Building_YieldFromCombatExperienceTimes100", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromPurchase, "Building_YieldFromPurchase", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromPurchaseGlobal, "Building_YieldFromPurchaseGlobal", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromFaithPurchase, "Building_YieldFromFaithPurchase", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldFromUnitLevelUpGlobal, "Building_YieldFromUnitLevelUpGlobal", "BuildingType", szBuildingType);
#endif
	kUtility.SetYields(m_piYieldChange, "Building_YieldChanges", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldChangeEraScalingTimes100, "Building_YieldChangesEraScalingTimes100", "BuildingType", szBuildingType);
	kUtility.SetFractionYields(m_pfYieldChangePerBuilding, "Building_YieldChangesPerXBuilding", "BuildingType", szBuildingType, "NumRequired");
	kUtility.SetFractionYields(m_pfYieldChangePerTile, "Building_YieldChangesPerXTiles", "BuildingType", szBuildingType, "NumRequired");
	kUtility.SetFractionYields(m_pfYieldChangePerCityStateStrategicResource, "Building_YieldChangesFromXCityStateStrategicResource", "BuildingType", szBuildingType, "NumRequired");
	kUtility.SetYields(m_piYieldChangePerPop, "Building_YieldChangesPerPop", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldChangePerReligion, "Building_YieldChangesPerReligion", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldModifier, "Building_YieldModifiers", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piAreaYieldModifier, "Building_AreaYieldModifiers", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piGlobalYieldModifier, "Building_GlobalYieldModifiers", "BuildingType", szBuildingType);
#if defined(MOD_BALANCE_CORE_BUILDING_INSTANT_YIELD)
	kUtility.SetYields(m_piInstantYield, "Building_InstantYield", "BuildingType", szBuildingType);
#endif

	kUtility.PopulateArrayByValue(m_piResourceQuantityRequirements, "Resources", "Building_ResourceQuantityRequirements", "ResourceType", "BuildingType", szBuildingType, "Cost");
	kUtility.PopulateArrayByValue(m_piResourceQuantity, "Resources", "Building_ResourceQuantity", "ResourceType", "BuildingType", szBuildingType, "Quantity");
	kUtility.PopulateArrayByValue(m_piResourceCultureChanges, "Resources", "Building_ResourceCultureChanges", "ResourceType", "BuildingType", szBuildingType, "CultureChange");
	kUtility.PopulateArrayByValue(m_piResourceFaithChanges, "Resources", "Building_ResourceFaithChanges", "ResourceType", "BuildingType", szBuildingType, "FaithChange");

	kUtility.PopulateArrayByValue(m_paiHurryModifier, "HurryInfos", "Building_HurryModifiers", "HurryType", "BuildingType", szBuildingType, "HurryCostModifier");
#if defined(MOD_BALANCE_CORE)
	kUtility.PopulateArrayByValue(m_paiHurryModifierLocal, "HurryInfos", "Building_HurryModifiersLocal", "HurryType", "BuildingType", szBuildingType, "HurryCostModifier");
	kUtility.PopulateArrayByValue(m_paiResourceHappinessChange, "Resources", "Building_ResourceHappinessChange", "ResourceType", "BuildingType", szBuildingType, "HappinessChange");
#endif

	//kUtility.PopulateArrayByValue(m_piProductionTraits, "Traits", "Building_ProductionTraits", "TraitType", "BuildingType", szBuildingType, "Trait");

	kUtility.PopulateArrayByValue(m_piUnitCombatFreeExperience, "UnitCombatInfos", "Building_UnitCombatFreeExperiences", "UnitCombatType", "BuildingType", szBuildingType, "Experience");
	kUtility.PopulateArrayByValue(m_piUnitCombatProductionModifiers, "UnitCombatInfos", "Building_UnitCombatProductionModifiers", "UnitCombatType", "BuildingType", szBuildingType, "Modifier");
	kUtility.PopulateArrayByValue(m_piUnitCombatProductionModifiersGlobal, "UnitCombatInfos", "Building_UnitCombatProductionModifiersGlobal", "UnitCombatType", "BuildingType", szBuildingType, "Modifier");
	kUtility.PopulateArrayByValue(m_piDomainFreeExperience, "Domains", "Building_DomainFreeExperiences", "DomainType", "BuildingType", szBuildingType, "Experience", 0, NUM_DOMAIN_TYPES);
	kUtility.PopulateArrayByValue(m_piDomainFreeExperiencePerGreatWork, "Domains", "Building_DomainFreeExperiencePerGreatWork", "DomainType", "BuildingType", szBuildingType, "Experience", 0, NUM_DOMAIN_TYPES);
#if defined(MOD_BALANCE_CORE)
	kUtility.PopulateArrayByValue(m_piDomainFreeExperiencePerGreatWorkGlobal, "Domains", "Building_DomainFreeExperiencePerGreatWorkGlobal", "DomainType", "BuildingType", szBuildingType, "Experience", 0, NUM_DOMAIN_TYPES);
#endif
	kUtility.PopulateArrayByValue(m_piDomainProductionModifier, "Domains", "Building_DomainProductionModifiers", "DomainType", "BuildingType", szBuildingType, "Modifier", 0, NUM_DOMAIN_TYPES);

	kUtility.PopulateArrayByValue(m_piPrereqNumOfBuildingClass, "BuildingClasses", "Building_PrereqBuildingClasses", "BuildingClassType", "BuildingType", szBuildingType, "NumBuildingNeeded");
	kUtility.PopulateArrayByExistence(m_pbBuildingClassNeededInCity, "BuildingClasses", "Building_ClassesNeededInCity", "BuildingClassType", "BuildingType", szBuildingType);
#if defined(MOD_BALANCE_CORE)
	kUtility.PopulateArrayByExistence(m_pbBuildingClassNeededAnywhere, "BuildingClasses", "Building_ClassNeededAnywhere", "BuildingClassType", "BuildingType", szBuildingType);
	kUtility.PopulateArrayByExistence(m_pbBuildingClassNeededNowhere, "BuildingClasses", "Building_ClassNeededNowhere", "BuildingClassType", "BuildingType", szBuildingType);
	kUtility.PopulateArrayByValue(m_paiBuildingClassLocalHappiness, "BuildingClasses", "Building_BuildingClassLocalHappiness", "BuildingClassType", "BuildingType", szBuildingType, "Happiness");
	kUtility.PopulateArrayByValue(m_paiSpecificGreatPersonRateModifier, "Specialists", "Building_SpecificGreatPersonRateModifier", "SpecialistType", "BuildingType", szBuildingType, "Modifier");
	kUtility.PopulateArrayByValue(m_piNumSpecFreeUnits, "Units", "Building_FreeSpecUnits", "UnitType", "BuildingType", szBuildingType, "NumUnits");
#endif
	//kUtility.PopulateArrayByExistence(m_piNumFreeUnits, "Units", "Building_FreeUnits", "UnitType", "BuildingType", szBuildingType);
	kUtility.PopulateArrayByValue(m_piNumFreeUnits, "Units", "Building_FreeUnits", "UnitType", "BuildingType", szBuildingType, "NumUnits");
	kUtility.PopulateArrayByValue(m_paiBuildingClassHappiness, "BuildingClasses", "Building_BuildingClassHappiness", "BuildingClassType", "BuildingType", szBuildingType, "Happiness");

	kUtility.PopulateArrayByExistence(m_piLockedBuildingClasses, "BuildingClasses", "Building_LockedBuildingClasses", "BuildingClassType", "BuildingType", szBuildingType);
	kUtility.PopulateArrayByExistence(m_piPrereqAndTechs, "Technologies", "Building_TechAndPrereqs", "TechType", "BuildingType", szBuildingType);
	kUtility.PopulateVector(m_viLocalResourceAnds, "Resources", "Building_LocalResourceAnds", "ResourceType", "BuildingType", szBuildingType);
	kUtility.PopulateVector(m_viLocalResourceOrs, "Resources", "Building_LocalResourceOrs", "ResourceType", "BuildingType", szBuildingType);

#if defined(MOD_BALANCE_CORE_RESOURCE_MONOPOLIES)
	kUtility.PopulateArrayByExistence(m_piLocalFeatureOrs, "Features", "Building_LocalFeatureOrs", "FeatureType", "BuildingType", szBuildingType);
	kUtility.PopulateArrayByExistence(m_piLocalFeatureAnds, "Features", "Building_LocalFeatureAnds", "FeatureType", "BuildingType", szBuildingType);

	kUtility.PopulateVector(m_viResourceMonopolyOrs, "Resources", "Building_ResourceMonopolyOrs", "ResourceType", "BuildingType", szBuildingType);
	kUtility.PopulateVector(m_viResourceMonopolyAnds, "Resources", "Building_ResourceMonopolyAnds", "ResourceType", "BuildingType", szBuildingType);

	kUtility.PopulateArrayByValue(m_piResourceQuantityPerXFranchises, "Resources", "Building_ResourceQuantityPerXFranchises", "ResourceType", "BuildingType", szBuildingType, "NumFranchises");
	kUtility.SetYields(m_piYieldChangePerMonopoly, "Building_YieldChangesPerMonopoly", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldChangeFromPassingTR, "Building_YieldChangesFromPassingTR", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldPerFranchise, "Building_YieldPerFranchise", "BuildingType", szBuildingType);

	kUtility.SetYields(m_piYieldPerFriend, "Building_YieldPerFriend", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldPerAlly, "Building_YieldPerAlly", "BuildingType", szBuildingType);

	kUtility.SetYields(m_piYieldChangeWorldWonder, "Building_YieldChangeWorldWonder", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piYieldChangeWorldWonderGlobal, "Building_YieldChangeWorldWonderGlobal", "BuildingType", szBuildingType);
	kUtility.SetYields(m_piLuxuryYieldChanges, "Building_LuxuryYieldChanges", "BuildingType", szBuildingType);
	
	m_iGPRateModifierPerXFranchises = kResults.GetInt("GPRateModifierPerXFranchises");
#endif
	kUtility.PopulateArrayByValue(m_piResourceQuantityFromPOP, "Resources", "Building_ResourceQuantityFromPOP", "ResourceType", "BuildingType", szBuildingType, "Modifier");
	//YieldFromYieldYieldChanges
	{
		//Initialize Theming Bonuses
		m_paYieldFromYield = FNEW(CvDoubleYieldInfo[NUM_YIELD_TYPES], c_eCiv5GameplayDLL, 0);
		int idx = 0;

		std::string strResourceTypesKey = "Building_YieldFromYieldPercent";
		Database::Results* pResourceTypes = kUtility.GetResults(strResourceTypesKey);
		if (pResourceTypes == NULL)
		{
			pResourceTypes = kUtility.PrepareResults(strResourceTypesKey, "select YieldIn, YieldOut, Value from Building_YieldFromYieldPercent where BuildingType = ?");
		}

		const size_t lenBuildingType = strlen(szBuildingType);
		pResourceTypes->Bind(1, szBuildingType, lenBuildingType, false);

		while (pResourceTypes->Step())
		{
			CvDoubleYieldInfo& pDoubleYieldInfo = m_paYieldFromYield[idx];

			const char* szYield = pResourceTypes->GetText("YieldIn");
			pDoubleYieldInfo.m_iYieldIn = (YieldTypes)GC.getInfoTypeForString(szYield, true);

			szYield = pResourceTypes->GetText("YieldOut");
			pDoubleYieldInfo.m_iYieldOut = (YieldTypes)GC.getInfoTypeForString(szYield, true);

			pDoubleYieldInfo.m_iValue = pResourceTypes->GetInt("Value");

			idx++;
		}

		pResourceTypes->Reset();
	}

	//YieldFromYieldYieldChangesGlobal
	{
		m_paYieldFromYieldGlobal = FNEW(CvDoubleYieldInfo[NUM_YIELD_TYPES], c_eCiv5GameplayDLL, 0);
		int idx = 0;

		std::string strResourceTypesKey = "Building_YieldFromYieldPercentGlobal";
		Database::Results* pResults = kUtility.GetResults(strResourceTypesKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strResourceTypesKey, "select YieldIn, YieldOut, Value from Building_YieldFromYieldPercentGlobal where BuildingType = ?");
		}

		const size_t lenBuildingType = strlen(szBuildingType);
		pResults->Bind(1, szBuildingType, lenBuildingType, false);

		while (pResults->Step())
		{
			CvDoubleYieldInfo& pDoubleYieldInfo = m_paYieldFromYieldGlobal[idx];

			const char* szYield = pResults->GetText("YieldIn");
			pDoubleYieldInfo.m_iYieldIn = (YieldTypes)GC.getInfoTypeForString(szYield, true);

			szYield = pResults->GetText("YieldOut");
			pDoubleYieldInfo.m_iYieldOut = (YieldTypes)GC.getInfoTypeForString(szYield, true);

			pDoubleYieldInfo.m_iValue = pResults->GetInt("Value");

			idx++;
		}

		pResults->Reset();
	}

	//ResourceYieldChanges
	{
		kUtility.Initialize2DArray(m_ppaiResourceYieldChange, "Resources", "Yields");

		std::string strKey("Building_ResourceYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Resources.ID as ResourceID, Yields.ID as YieldID, Yield from Building_ResourceYieldChanges inner join Resources on Resources.Type = ResourceType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while(pResults->Step())
		{
			const int ResourceID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppaiResourceYieldChange[ResourceID][YieldID] = yield;
		}
	}

#if defined(MOD_BALANCE_CORE)
	//Building_ResourceYieldChangesGlobal
	{
		std::string strKey("Building_ResourceYieldChangesGlobal");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Resources.ID as ResourceID, Yields.ID as YieldID, Yield from Building_ResourceYieldChangesGlobal inner join Resources on Resources.Type = ResourceType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int iResource = pResults->GetInt(0);
			const int iYieldType = pResults->GetInt(1);
			const int iYield = pResults->GetInt(2);

			m_ppiResourceYieldChangeGlobal[iResource][iYieldType] += iYield;
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::map<int, std::map<int, int>>(m_ppiResourceYieldChangeGlobal).swap(m_ppiResourceYieldChangeGlobal);
	}

	// Building_TechEnhancedYieldChanges
	// Table structure (BuildingType, YieldType, Yield, TechType)
	// The building produces additional yields per turn once the corresponding technology has been researched
	{
		// In vanilla, this table didn't have the column TechType, the technology was stored in Buildings.EnhancedYieldTech instead.
		// That means that a building couldn't get additional yields for more than one tech.
		// To keep compatibility with existing mods the new column TechType is nullable, if it is empty, Buildings.EnhancedYieldTech is used as fallback value.
		// In vanilla, the column Buildings.TechEnhancedTourism can also be used together with Buildings.EnhancedYieldTech to give tourism once a particular tech is researched, this is now also processed here
		std::string strKey("Building_TechEnhancedYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select coalesce(Technologies.ID, -1) as TechID, Yields.ID as YieldID, Yield from Building_TechEnhancedYieldChanges left join Technologies on Technologies.Type = TechType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		szTextVal = kResults.GetText("EnhancedYieldTech");
		int iFallbackTech = GC.getInfoTypeForString(szTextVal, true);

		while (pResults->Step())
		{
			const int iTech = pResults->GetInt(0) >= 0 ? pResults->GetInt(0) : iFallbackTech;
			const int iYieldType = pResults->GetInt(1);
			const int iYield = pResults->GetInt(2);

			if (iTech >= 0)
			{
				m_miTechEnhancedYields[iTech][iYieldType] += iYield;
			}
		}

		int iTechEnhancedTourism = kResults.GetInt("TechEnhancedTourism");
		if (iTechEnhancedTourism > 0 && iFallbackTech >= 0)
		{
			m_miTechEnhancedYields[iFallbackTech][YIELD_TOURISM] += iTechEnhancedTourism;
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::map<int, std::map<int, int>>(m_miTechEnhancedYields).swap(m_miTechEnhancedYields);
	}
	// Building_YieldChangesFromAccomplishments
	// Table structure (BuildingType, YieldType, Yield, AccomplishmentType)
	// The building produces additional yields per turn once the corresponding accomplishment has been achieved
	{
		std::string strKey("Building_YieldChangesFromAccomplishments");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Accomplishments.ID, Yields.ID as YieldID, Yield from Building_YieldChangesFromAccomplishments left join Accomplishments on Accomplishments.Type = AccomplishmentType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int iAccomplishment = pResults->GetInt(0);
			const int iYieldType = pResults->GetInt(1);
			const int iYield = pResults->GetInt(2);

			m_miYieldChangesFromAccomplishments[iAccomplishment][iYieldType] += iYield;
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::map<int, std::map<int, int>>(m_miYieldChangesFromAccomplishments).swap(m_miYieldChangesFromAccomplishments);
	}
	// Building_BonusFromAccomplishments
	// Table structure (BuildingType, AccomplishmentType, Happiness, DomainType, DomainXP, UnitCombatType, UnitProductionModifier)
	// The building gives additional bonuses once the corresponding accomplishment has been achieved
	{
		std::string strKey("Building_BonusFromAccomplishments");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Accomplishments.ID, Happiness, coalesce(Domains.ID, -1), DomainXP, coalesce(UnitCombatInfos.ID, -1), UnitProductionModifier from Building_BonusFromAccomplishments inner join Accomplishments on Accomplishments.Type = AccomplishmentType left join Domains on Domains.Type = DomainType left join UnitCombatInfos on UnitCombatInfos.Type = UnitCombatType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			AccomplishmentBonusInfo bonusInfo;

			const int iAccomplishment = pResults->GetInt(0);
			bonusInfo.iHappiness = pResults->GetInt(1);
			bonusInfo.eDomainType = (DomainTypes)pResults->GetInt(2);
			bonusInfo.iDomainXP = pResults->GetInt(3);
			bonusInfo.eUnitCombatType = (UnitCombatTypes)pResults->GetInt(4);
			bonusInfo.iUnitProductionModifier = pResults->GetInt(5);

			m_miBonusFromAccomplishments[iAccomplishment] = bonusInfo;
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::map<int, AccomplishmentBonusInfo>(m_miBonusFromAccomplishments).swap(m_miBonusFromAccomplishments);
	}
	// Building_ExtraPlayerInstancesFromAccomplishments 
	// Table structure (BuildingType, AccomplishmentType, ExtraInstances)
	// The building can be built more often if an accomplishment has been achieved
	{
		std::string strKey("Building_ExtraPlayerInstancesFromAccomplishments");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Accomplishments.ID, ExtraInstances from Building_ExtraPlayerInstancesFromAccomplishments inner join Accomplishments on Accomplishments.Type = AccomplishmentType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int iAccomplishment = pResults->GetInt(0);
			const int iExtraInstances = pResults->GetInt(1);

			m_miExtraPlayerInstancesFromAccomplishments[iAccomplishment] = iExtraInstances;
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::map<int, int>(m_miExtraPlayerInstancesFromAccomplishments).swap(m_miExtraPlayerInstancesFromAccomplishments);
	}

	// Building_GreatPersonPointFromConstruction
	// Table structure (BuildingType, GreatPersonType, EraType, Value)
	// The building gives GPP to the specified GreatPersonType whenever a building unlocked in EraType or later is constructed. The number of GPP is (BuildingCost) * Value / 100
	{
		std::string strKey("Building_GreatPersonPointFromConstruction");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select GreatPersons.ID as GreatPersonID, Eras.ID as EraID, Value from Building_GreatPersonPointFromConstruction inner join GreatPersons on GreatPersons.Type = GreatPersonType inner join Eras on Eras.Type = EraType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int iGreatPerson = pResults->GetInt(0);
			const int iEra = pResults->GetInt(1);
			const int iYield = pResults->GetInt(2);

			m_miGreatPersonPointFromConstruction[std::make_pair((GreatPersonTypes)iGreatPerson, (EraTypes)iEra)] += iYield;
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::map<pair<GreatPersonTypes, EraTypes>, int>(m_miGreatPersonPointFromConstruction).swap(m_miGreatPersonPointFromConstruction);
	}

	//Building_YieldChangesPerPopInEmpire
	{
		std::string strKey("Building_YieldChangesPerPopInEmpire");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Yields.ID as YieldID, Yield from Building_YieldChangesPerPopInEmpire inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int iYieldType = pResults->GetInt(0);
			const int iYield = pResults->GetInt(1);

			m_piYieldChangePerPopInEmpire[iYieldType] += iYield;
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::map<int, int>(m_piYieldChangePerPopInEmpire).swap(m_piYieldChangePerPopInEmpire);
	}

	//Building_UnitClassTrainingAllowed
	{
		std::string strKey("Building_UnitClassTrainingAllowed");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select UnitClasses.ID as UnitClassID from Building_UnitClassTrainingAllowed inner join UnitClasses on UnitClasses.Type = UnitClassType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int iUnitClass = pResults->GetInt(0);

			m_siUnitClassTrainingAllowed.insert(iUnitClass);
		}

		pResults->Reset();
	}

	//Building_ResourceClaim
	{
		std::string strKey("Building_ResourceClaim");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Resources.ID as ResourceID, IncludeOwnedByOtherPlayer from Building_ResourceClaim inner join Resources on Resources.Type = ResourceType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int iResourceID = pResults->GetInt(0);
			const bool bIncludeOwnedByOtherPlayer = pResults->GetBool(1);

			m_sibResourceClaim.insert(make_pair(iResourceID, bIncludeOwnedByOtherPlayer));
		}

		pResults->Reset();
	}
#endif

	//FeatureYieldChanges
	{
		kUtility.Initialize2DArray(m_ppaiFeatureYieldChange, "Features", "Yields");

		std::string strKey("Building_FeatureYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Features.ID as FeatureID, Yields.ID as YieldID, Yield from Building_FeatureYieldChanges inner join Features on Features.Type = FeatureType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while(pResults->Step())
		{
			const int FeatureID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppaiFeatureYieldChange[FeatureID][YieldID] = yield;
		}
	}
#if defined(MOD_BALANCE_CORE)
	//ImprovementYieldChanges
	{
		kUtility.Initialize2DArray(m_ppaiImprovementYieldChange, "Improvements", "Yields");

		std::string strKey("Building_ImprovementYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Improvements.ID as ImprovementID, Yields.ID as YieldID, Yield from Building_ImprovementYieldChanges inner join Improvements on Improvements.Type = ImprovementType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while(pResults->Step())
		{
			const int ImprovementID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppaiImprovementYieldChange[ImprovementID][YieldID] = yield;
		}
	}
	//ImprovementYieldChangesGlobal
	{
		kUtility.Initialize2DArray(m_ppaiImprovementYieldChangeGlobal, "Improvements", "Yields");

		std::string strKey("Building_ImprovementYieldChangesGlobal");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Improvements.ID as ImprovementID, Yields.ID as YieldID, Yield from Building_ImprovementYieldChangesGlobal inner join Improvements on Improvements.Type = ImprovementType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while(pResults->Step())
		{
			const int ImprovementID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppaiImprovementYieldChangeGlobal[ImprovementID][YieldID] = yield;
		}
	}
	//TerrainYieldChanges
	{
		kUtility.Initialize2DArray(m_ppaiYieldPerXTerrain, "Terrains", "Yields");

		std::string strKey("Building_YieldPerXTerrainTimes100");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Terrains.ID as TerrainID, Yields.ID as YieldID, Yield from Building_YieldPerXTerrainTimes100 inner join Terrains on Terrains.Type = TerrainType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while(pResults->Step())
		{
			const int TerrainID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppaiYieldPerXTerrain[TerrainID][YieldID] = yield;
		}
	}
	//FeatureYieldChanges
	{
		kUtility.Initialize2DArray(m_ppaiYieldPerXFeature, "Features", "Yields");

		std::string strKey("Building_YieldPerXFeatureTimes100");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Features.ID as FeatureID, Yields.ID as YieldID, Yield from Building_YieldPerXFeatureTimes100 inner join Features on Features.Type = FeatureType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int FeatureID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppaiYieldPerXFeature[FeatureID][YieldID] = yield;
		}
	}
	//YieldPerXImprovementLocal
	{
		std::string strKey("Building_YieldPerXImprovementLocal");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Improvements.ID as ImprovementID, Yields.ID as YieldID, Yield, NumRequired from Building_YieldPerXImprovementLocal inner join Improvements on Improvements.Type = ImprovementType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const ImprovementTypes eImprovementType = ImprovementTypes(pResults->GetInt(0));
			const YieldTypes eYieldType = YieldTypes(pResults->GetInt(1));
			const int iYield = pResults->GetInt(2);
			const int iNumReq = pResults->GetInt(3);

			m_ppYieldPerXImprovementLocal[eImprovementType][eYieldType] += fraction(iYield,iNumReq);
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::map<ImprovementTypes, std::map<YieldTypes, fraction>>(m_ppYieldPerXImprovementLocal).swap(m_ppYieldPerXImprovementLocal);
	}
	//YieldPerXImprovementGlobal
	{
		std::string strKey("Building_YieldPerXImprovementGlobal");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Improvements.ID as ImprovementID, Yields.ID as YieldID, Yield, NumRequired from Building_YieldPerXImprovementGlobal inner join Improvements on Improvements.Type = ImprovementType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const ImprovementTypes eImprovementType = ImprovementTypes(pResults->GetInt(0));
			const YieldTypes eYieldType = YieldTypes(pResults->GetInt(1));
			const int iYield = pResults->GetInt(2);
			const int iNumReq = pResults->GetInt(3);

			m_ppYieldPerXImprovementGlobal[eImprovementType][eYieldType] += fraction(iYield,iNumReq);
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::map<ImprovementTypes, std::map<YieldTypes, fraction>>(m_ppYieldPerXImprovementGlobal).swap(m_ppYieldPerXImprovementGlobal);
	}
	//Building_DomainFreeExperiencesGlobal
	{
		std::string strKey("Building_DomainFreeExperiencesGlobal");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Domains.ID as DomainID, Experience from Building_DomainFreeExperiencesGlobal inner join Domains on Domains.Type = DomainType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int iDomain = pResults->GetInt(0);
			const int iExperience = pResults->GetInt(1);

			m_piDomainFreeExperienceGlobal[iDomain] += iExperience;
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::map<int, int>(m_piDomainFreeExperienceGlobal).swap(m_piDomainFreeExperienceGlobal);
	}
#endif

	//TerrainYieldChanges
	{
		kUtility.Initialize2DArray(m_ppaiTerrainYieldChange, "Terrains", "Yields");

		std::string strKey("Building_TerrainYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Terrains.ID as TerrainID, Yields.ID as YieldID, Yield from Building_TerrainYieldChanges inner join Terrains on Terrains.Type = TerrainType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while(pResults->Step())
		{
			const int TerrainID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppaiTerrainYieldChange[TerrainID][YieldID] = yield;
		}
	}

	//PlotYieldChanges
	{
		kUtility.Initialize2DArray(m_ppaiPlotYieldChange, "Plots", "Yields");

		std::string strKey("Building_PlotYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Plots.ID as PlotID, Yields.ID as YieldID, Yield from Building_PlotYieldChanges inner join Plots on Plots.Type = PlotType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while(pResults->Step())
		{
			const int PlotID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppaiPlotYieldChange[PlotID][YieldID] = yield;
		}
	}

	//SpecialistYieldChangesLocal
	{
		kUtility.Initialize2DArray(m_ppaiSpecialistYieldChangeLocal, "Specialists", "Yields");

		std::string strKey("Building_SpecialistYieldChangesLocal");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Specialists.ID as SpecialistID, Yields.ID as YieldID, Yield from Building_SpecialistYieldChangesLocal inner join Specialists on Specialists.Type = SpecialistType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while(pResults->Step())
		{
			const int SpecialistID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppaiSpecialistYieldChangeLocal[SpecialistID][YieldID] = yield;
		}
	}

	//SpecialistYieldChanges
	{
		kUtility.Initialize2DArray(m_ppaiSpecialistYieldChange, "Specialists", "Yields");

		std::string strKey("Building_SpecialistYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Specialists.ID as SpecialistID, Yields.ID as YieldID, Yield from Building_SpecialistYieldChanges inner join Specialists on Specialists.Type = SpecialistType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while(pResults->Step())
		{
			const int SpecialistID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppaiSpecialistYieldChange[SpecialistID][YieldID] = yield;
		}
	}

	//ResourceYieldModifiers
	{
		kUtility.Initialize2DArray(m_ppaiResourceYieldModifier, "Resources", "Yields");

		std::string strKey("Building_ResourceYieldModifiers");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Resources.ID as ResourceID, Yields.ID as YieldID, Yield from Building_ResourceYieldModifiers inner join Resources on Resources.Type = ResourceType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while(pResults->Step())
		{
			const int ResourceID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppaiResourceYieldModifier[ResourceID][YieldID] = yield;
		}
	}

	//BuildingClassYieldChanges
	{
		kUtility.Initialize2DArray(m_ppiBuildingClassYieldChanges, "BuildingClasses", "Yields");

		std::string strKey("Building_BuildingClassYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select BuildingClasses.ID as BuildingClassID, Yields.ID as YieldID, YieldChange from Building_BuildingClassYieldChanges inner join BuildingClasses on BuildingClasses.Type = BuildingClassType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while(pResults->Step())
		{
			const int BuildingClassID = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYieldChange = pResults->GetInt(2);

			m_ppiBuildingClassYieldChanges[BuildingClassID][iYieldID] = iYieldChange;
		}
	}
	//BuildingClassYieldModifiers
	{
		kUtility.Initialize2DArray(m_ppiBuildingClassYieldModifiers, "BuildingClasses", "Yields");

		std::string strKey("Building_BuildingClassYieldModifiers");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select BuildingClasses.ID as BuildingClassID, Yields.ID as YieldID, Modifier from Building_BuildingClassYieldModifiers inner join BuildingClasses on BuildingClasses.Type = BuildingClassType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int BuildingClassID = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYieldModifier = pResults->GetInt(2);

			m_ppiBuildingClassYieldModifiers[BuildingClassID][iYieldID] = iYieldModifier;
		}
	}
#if defined(MOD_BALANCE_CORE)
	//BuildingClassLocalYieldChanges
	{
		kUtility.Initialize2DArray(m_ppiBuildingClassLocalYieldChanges, "BuildingClasses", "Yields");

		std::string strKey("Building_BuildingClassLocalYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select BuildingClasses.ID as BuildingClassID, Yields.ID as YieldID, YieldChange from Building_BuildingClassLocalYieldChanges inner join BuildingClasses on BuildingClasses.Type = BuildingClassType inner join Yields on Yields.Type = YieldType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while(pResults->Step())
		{
			const int BuildingClassID = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYieldChange = pResults->GetInt(2);

			m_ppiBuildingClassLocalYieldChanges[BuildingClassID][iYieldID] = iYieldChange;
		}
	}

	//Building_ResourcePlotsToPlace
	{
		std::string strKey("Building_ResourcePlotsToPlace");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Resources.ID as ResourceID, NumPlots, ResourceQuantityToPlace from Building_ResourcePlotsToPlace inner join Resources on Resources.Type = ResourceType where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int iResource = pResults->GetInt(0);
			const int iNumPlots = pResults->GetInt(1);
			const int iResourceQuantity = pResults->GetInt(2);

			m_ppiResourcePlotsToPlace[iResource][iResourceQuantity] += iNumPlots;
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::map<int, std::map<int, int>>(m_ppiResourcePlotsToPlace).swap(m_ppiResourcePlotsToPlace);
	}
#endif
#if defined(MOD_BALANCE_CORE)
	//Building_GreatPersonProgressFromConstruction
	{
		std::string strKey("Building_GreatPersonProgressFromConstruction");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select GreatPersons.ID as GreatPersonID, Eras.ID as ErasID, Value from Building_GreatPersonProgressFromConstruction inner join GreatPersons on GreatPersons.Type = GreatPersonType inner join Eras on EraType = Eras.Type where BuildingType = ?");
		}

		pResults->Bind(1, szBuildingType);

		while (pResults->Step())
		{
			const int iGreatPerson = pResults->GetInt(0);
			const int iEra = pResults->GetInt(1);
			const int iValue = pResults->GetInt(2);

			m_piiGreatPersonProgressFromConstruction.insert(std::make_pair(iGreatPerson, std::make_pair(iEra, iValue)));
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::multimap<int, std::pair<int, int>>(m_piiGreatPersonProgressFromConstruction).swap(m_piiGreatPersonProgressFromConstruction);
	}
#endif
	{
		//Initialize Theming Bonuses
		const int iNumThemes = MAX_THEMING_BONUSES; /* 12 */
		m_paThemingBonusInfo = FNEW(CvThemingBonusInfo[iNumThemes], c_eCiv5GameplayDLL, 0);
		int idx = 0;

		std::string strResourceTypesKey = "Building_ThemingBonuses";
		Database::Results* pResourceTypes = kUtility.GetResults(strResourceTypesKey);
		if(pResourceTypes == NULL)
		{
			pResourceTypes = kUtility.PrepareResults(strResourceTypesKey, "select Bonus, Description, SameEra, UniqueEras, ConsecutiveEras, MustBeArt, MustBeArtifact, MustBeEqualArtArtifact, RequiresOwner, RequiresAnyButOwner, RequiresSamePlayer, RequiresUniquePlayers, AIPriority from Building_ThemingBonuses where BuildingType = ?");
		}

		const size_t lenBuildingType = strlen(szBuildingType);
		pResourceTypes->Bind(1, szBuildingType, lenBuildingType, false);

		while(pResourceTypes->Step())
		{
			CvThemingBonusInfo& pThemingInfo = m_paThemingBonusInfo[idx];

			pThemingInfo.m_iBonus = pResourceTypes->GetInt("Bonus");
			pThemingInfo.m_strDescription = pResourceTypes->GetText("Description");
			pThemingInfo.m_bSameEra = pResourceTypes->GetBool("SameEra");
			pThemingInfo.m_bUniqueEras = pResourceTypes->GetBool("UniqueEras");
			pThemingInfo.m_bConsecutiveEras = pResourceTypes->GetBool("ConsecutiveEras");
			pThemingInfo.m_bMustBeArt = pResourceTypes->GetBool("MustBeArt");
			pThemingInfo.m_bMustBeArtifact = pResourceTypes->GetBool("MustBeArtifact");
			pThemingInfo.m_bMustBeEqualArtArtifact = pResourceTypes->GetBool("MustBeEqualArtArtifact");
			pThemingInfo.m_bRequiresOwner = pResourceTypes->GetBool("RequiresOwner");
			pThemingInfo.m_bRequiresAnyButOwner = pResourceTypes->GetBool("RequiresAnyButOwner");
			pThemingInfo.m_bRequiresSamePlayer = pResourceTypes->GetBool("RequiresSamePlayer");
			pThemingInfo.m_bRequiresUniquePlayers = pResourceTypes->GetBool("RequiresUniquePlayers");
			pThemingInfo.m_iAIPriority = pResourceTypes->GetInt("AIPriority");

			idx++;
		}

		m_iNumThemingBonuses = idx;
		pResourceTypes->Reset();
	}

	return true;
}

/// Class of this building
BuildingClassTypes CvBuildingEntry::GetBuildingClassType() const
{
	return (BuildingClassTypes)m_iBuildingClassType;
}

const CvBuildingClassInfo& CvBuildingEntry::GetBuildingClassInfo() const
{
	static CvBuildingClassInfo emptyResult;
	if(m_pkBuildingClassInfo == NULL)
	{
		const char* szError = "ERROR: Building does not contain valid BuildingClass type!!";
		GC.LogMessage(szError);
		ASSERT_DEBUG(false, szError);
		return emptyResult;
	}
	else
		return *m_pkBuildingClassInfo;
}

/// Does this building require a city built on or next to a specific terrain type?
int CvBuildingEntry::GetNearbyTerrainRequired() const
{
	return m_iNearbyTerrainRequired;
}

/// Does this building need the absence of a terrain under the city?
int CvBuildingEntry::GetProhibitedCityTerrain() const
{
	return m_iProhibitedCityTerrain;
}

/// Does a Victory need to be active for this building to be buildable?
int CvBuildingEntry::GetVictoryPrereq() const
{
	return m_iVictoryPrereq;
}

/// Do you get this building for free if start in a later era?
int CvBuildingEntry::GetFreeStartEra() const
{
	return m_iFreeStartEra;
}

/// Is this building unbuildable if start in a later era?
int CvBuildingEntry::GetMaxStartEra() const
{
	return m_iMaxStartEra;
}

/// Tech that makes this building obsolete
int CvBuildingEntry::GetObsoleteTech() const
{
	return m_iObsoleteTech;
}

/// How much GPT does this Building cost?
int CvBuildingEntry::GetGoldMaintenance() const
{
	return m_iGoldMaintenance;
}

/// Only one Building from each Group may be constructed in a City
int CvBuildingEntry::GetMutuallyExclusiveGroup() const
{
	return m_iMutuallyExclusiveGroup;
}

/// Upgraded version of this building
int CvBuildingEntry::GetReplacementBuildingClass() const
{
	return m_iReplacementBuildingClass;
}

/// Techs required for this building
int CvBuildingEntry::GetPrereqAndTech() const
{
	return m_iPrereqAndTech;
}

#if defined(MOD_BALANCE_CORE)
/// Era this building belongs to
int CvBuildingEntry::GetEra() const
{
	TechTypes eTech = (TechTypes)GetPrereqAndTech();
	if (eTech != NO_TECH)
	{
		CvTechEntry* pTech = GC.getTechInfo((TechTypes)GetPrereqAndTech());
		return pTech->GetEra();
	}

	return -1;
}
#endif

/// Policy branch required for this building
int CvBuildingEntry::GetPolicyBranchType() const
{
	return m_iPolicyBranchType;
}
#if defined(MOD_BALANCE_CORE_POLICIES)
/// Policy required for this building
PolicyTypes CvBuildingEntry::GetPolicyType() const
{
	return m_iPolicyType;
}
CivilizationTypes CvBuildingEntry::GetCivType() const
{
	return m_eCivType;
}
#endif
#if defined(MOD_BALANCE_CORE)
/// Resource required for this building
int CvBuildingEntry::GetResourceType() const
{
	return m_iResourceType;
}
//Num Policies required for this building
int CvBuildingEntry::GetNumPoliciesNeeded() const
{
	return m_iNumPoliciesNeeded;
}
/// Does this building grant a random resource around the territory?
int CvBuildingEntry::GrantsRandomResourceTerritory() const
{
	return m_iGrantsRandomResourceTerritory;
}
/// Is this building purchaseable in any city?
bool CvBuildingEntry::IsPuppetPurchaseOverride() const
{
	return m_bPuppetPurchaseOverride;
}
/// Does this building unlock purchasing in any city?
bool CvBuildingEntry::IsAllowsPuppetPurchase() const
{
	return m_bAllowsPuppetPurchase;
}
/// Does this building prevent non-specialists to consume more food than the city's food production?
bool CvBuildingEntry::IsNoStarvationNonSpecialist() const
{
	return m_bNoStarvationNonSpecialist;
}
/// Does this building have a cooldown cost when purchased?
int CvBuildingEntry::GetCooldown() const
{
	return m_iGetCooldown;
}
#endif
/// What SpecialistType is allowed by this Building
int CvBuildingEntry::GetSpecialistType() const
{
	return m_iSpecialistType;
}

/// How many SpecialistTypes are allowed by this Building
int CvBuildingEntry::GetSpecialistCount() const
{
	return m_iSpecialistCount;
}

/// Extra culture from every specialist
int CvBuildingEntry::GetSpecialistExtraCulture() const
{
	return m_iSpecialistExtraCulture;
}

/// How many GPP does this Building provide (linked to the SpecialistType)
int CvBuildingEntry::GetGreatPeopleRateChange() const
{
	return m_iGreatPeopleRateChange;
}

/// What GreatWorkType is allowed by this Building
GreatWorkSlotType CvBuildingEntry::GetGreatWorkSlotType() const
{
	return m_eGreatWorkSlotType;
}

#if defined(MOD_GLOBAL_GREATWORK_YIELDTYPES)
/// What YieldType is generated by Great Works in this Building
YieldTypes CvBuildingEntry::GetGreatWorkYieldType() const
{
	return m_eGreatWorkYieldType;
}
#endif

/// How many great works are allowed by this Building
int CvBuildingEntry::GetGreatWorkCount() const
{
	return max(0, m_iGreatWorkCount); // protect against modders using negative values
}

/// Does this building come with a built-in Great Work?
GreatWorkType CvBuildingEntry::GetFreeGreatWork() const
{
	return m_eFreeGreatWork;
}

/// Free building in each city from this building/wonder
int CvBuildingEntry::GetFreeBuildingClass() const
{
	return m_iFreeBuildingClass;
}

/// Free building in the city that builds this building/wonder
int CvBuildingEntry::GetFreeBuildingThisCity() const
{
	return m_iFreeBuildingThisCity;
}
#if defined(MOD_BALANCE_CORE)
/// TRs are invulnerable!
bool CvBuildingEntry::IsTradeRouteInvulnerable() const
{
	return m_bTradeRouteInvulnerable;
}
/// TRs have speed!
int CvBuildingEntry::GetTRSpeedBoost() const
{
	return m_iTRSpeedBoost;
}
/// TRs have vision!
int CvBuildingEntry::GetTRVisionBoost() const
{
	return m_iTRVisionBoost;
}
// TRs take less time on empire
int CvBuildingEntry::GetTRTurnModGlobal() const
{
	return m_iTRTurnModGlobal;
}
// TRs take less time from the city
int CvBuildingEntry::GetTRTurnModLocal() const
{
	return m_iTRTurnModLocal;
}

int CvBuildingEntry::GetVotesPerGPT() const
{
	return m_iVotesPerGPT;
}
bool CvBuildingEntry::IsRequiresIndustrialCityConnection() const
{
	return m_bRequiresIndustrialCityConnection;
}
bool CvBuildingEntry::IsDummy() const
{
	return m_bDummy;
}
int CvBuildingEntry::GetLandmarksTourismPercentGlobal() const
{
	return m_iLandmarksTourismPercentGlobal;
}
int CvBuildingEntry::GetGreatWorksTourismModifierGlobal() const
{
	return m_iGreatWorksTourismModifierGlobal;
}

#if defined(MOD_BALANCE_CORE_EVENTS)
int CvBuildingEntry::GetEventRequiredActive() const
{
	return m_iEventRequiredActive;
}

int CvBuildingEntry::GetCityEventRequiredActive() const
{
	return m_iCityEventRequiredActive;
}
#endif
#endif

/// Does this building give all units a promotion for free instantly?
int CvBuildingEntry::GetFreePromotion() const
{
	return m_iFreePromotion;
}

/// Does this building give units a promotion when trained from this city?
int CvBuildingEntry::GetTrainedFreePromotion() const
{
	return m_iTrainedFreePromotion;
}

/// Does this building get rid of an undesirable promotion?
int CvBuildingEntry::GetFreePromotionRemoved() const
{
	return m_iFreePromotionRemoved;
}

/// Shields to construct the building
int CvBuildingEntry::GetProductionCost() const
{
	return m_iProductionCost;
}

/// Faith to construct the unit (as a percentage of cost of next Great Prophet)
int CvBuildingEntry::GetFaithCost() const
{
	return m_iFaithCost;
}

/// Production value per League member to construct the building
int CvBuildingEntry::GetLeagueCost() const
{
	return m_iLeagueCost;
}

/// Additional cost based on the number of cities in the empire
int CvBuildingEntry::GetNumCityCostMod() const
{
	return m_iNumCityCostMod;
}

/// Does this Building modify any hurry costs
int CvBuildingEntry::GetHurryCostModifier() const
{
	return m_iHurryCostModifier;
}

/// Number of cities required to build this?
int CvBuildingEntry::GetNumCitiesPrereq() const
{
	return m_iNumCitiesPrereq;
}

/// Do we need a unit at a certain level to build this?
int CvBuildingEntry::GetUnitLevelPrereq() const
{
	return m_iUnitLevelPrereq;
}

/// Multiplier to the rate of accumulating culture for policies
int CvBuildingEntry::GetCultureRateModifier() const
{
	return m_iCultureRateModifier;
}

/// Multiplier to the rate of accumulating culture for policies in all Cities
int CvBuildingEntry::GetGlobalCultureRateModifier() const
{
	return m_iGlobalCultureRateModifier;
}

/// Change in spawn rate for great people
int CvBuildingEntry::GetGreatPeopleRateModifier() const
{
	return m_iGreatPeopleRateModifier;
}

/// Change global spawn rate for great people
int CvBuildingEntry::GetGlobalGreatPeopleRateModifier() const
{
	return m_iGlobalGreatPeopleRateModifier;
}

/// Modifier to great people per active marriage with a CS
int CvBuildingEntry::GetGPRateModifierPerMarriage() const
{
	return m_iGPRateModifierPerMarriage;
}

/// Modifier to great people rate per active theming bonus in the city
int CvBuildingEntry::GetGPRateModifierPerLocalTheme() const
{
	return m_iGPRateModifierPerLocalTheme;
}

/// Instant GPP for the specialist with the most points currently, scaling with Era
int CvBuildingEntry::GetGPPOnCitizenBirth() const
{
	return m_iGPPOnCitizenBirth;
}

/// Change in spawn rate for great generals
int CvBuildingEntry::GetGreatGeneralRateModifier() const
{
	return m_iGreatGeneralRateModifier;
}

/// Gold received when great person expended
int CvBuildingEntry::GetGreatPersonExpendGold() const
{
	return m_iGreatPersonExpendGold;
}

/// Reduces cost of unit upgrades?
int CvBuildingEntry::GetUnitUpgradeCostMod() const
{
	return m_iUnitUpgradeCostMod;
}

/// Percentage increase in the length of Golden Ages
int CvBuildingEntry::GetGoldenAgeModifier() const
{
	return m_iGoldenAgeModifier;
}

/// Free experience for units built in this city
int CvBuildingEntry::GetFreeExperience() const
{
	return m_iFreeExperience;
}

/// Free experience for all player units
int CvBuildingEntry::GetGlobalFreeExperience() const
{
	return m_iGlobalFreeExperience;
}

/// Global happiness per major civ currently at war with the player
int CvBuildingEntry::GetGlobalHappinessPerMajorWar() const
{
	return m_iGlobalHappinessPerMajorWar;
}

/// Global Production modifier towards military units per major civ currently at war with the player
int CvBuildingEntry::GetGlobalMilitaryProductionModPerMajorWar() const
{
	return m_iGlobalMilitaryProductionModPerMajorWar;
}

/// Percentage of food retained after city growth
int CvBuildingEntry::GetFoodKept() const
{
	return m_iFoodKept;
}

/// Does this building allow airlifts?
bool CvBuildingEntry::IsAirlift() const
{
	return m_bAirlift;
}

/// Modifier to city air defense
int CvBuildingEntry::GetAirModifier() const
{
	return m_iAirModifier;
}

int CvBuildingEntry::GetAirModifierGlobal() const
{
	return m_iAirModifierGlobal;
}

/// Modifier to city nuke defense
int CvBuildingEntry::GetNukeModifier() const
{
	return m_iNukeModifier;
}

/// Will this building cause a big problem (meltdown) if the city is hit with a nuke?
int CvBuildingEntry::GetNukeExplosionRand() const
{
	return m_iNukeExplosionRand;
}

/// Improvement in worker speed
int CvBuildingEntry::GetWorkerSpeedModifier() const
{
	return m_iWorkerSpeedModifier;
}

/// Improvement in military unit production
int CvBuildingEntry::GetMilitaryProductionModifier() const
{
	return m_iMilitaryProductionModifier;
}

/// Improvement in space race component production
int CvBuildingEntry::GetSpaceProductionModifier() const
{
	return m_iSpaceProductionModifier;
}

/// Improvement in space race component production in all cities
int CvBuildingEntry::GetGlobalSpaceProductionModifier() const
{
	return m_iGlobalSpaceProductionModifier;
}

/// Improvement in building production
int CvBuildingEntry::GetBuildingProductionModifier() const
{
	return m_iBuildingProductionModifier;
}

/// Improvement in wonder production
int CvBuildingEntry::GetWonderProductionModifier() const
{
	return m_iWonderProductionModifier;
}

/// Trade route gold modifier
int CvBuildingEntry::GetCityConnectionTradeRouteModifier() const
{
	return m_iCityConnectionTradeRouteModifier;
}

/// Increased plunder if city captured
int CvBuildingEntry::GetCapturePlunderModifier() const
{
	return m_iCapturePlunderModifier;
}

/// Change in culture cost to earn a new policy
int CvBuildingEntry::GetPolicyCostModifier() const
{
	return m_iPolicyCostModifier;
}

/// +X Influence from Diplomatic Missions for units that originate from this city
int CvBuildingEntry::GetDiplomatInfluenceBoost() const
{
	return m_iDiplomatInfluenceBoost;
}

/// Increase to rate of border growth in city
int CvBuildingEntry::GetBorderGrowthRateIncrease() const
{
	return m_iBorderGrowthRateIncrease;
}

/// Increase to rate of border growth in all cities
int CvBuildingEntry::GetBorderGrowthRateIncreaseGlobal() const
{
	return m_iBorderGrowthRateIncreaseGlobal;
}

/// Change in culture cost to earn a new tile
int CvBuildingEntry::GetPlotCultureCostModifier() const
{
	return m_iPlotCultureCostModifier;
}

/// Change in culture cost to earn a new tile
int CvBuildingEntry::GetGlobalPlotCultureCostModifier() const
{
	return m_iGlobalPlotCultureCostModifier;
}

/// Change in gold cost to earn a new tile
int CvBuildingEntry::GetPlotBuyCostModifier() const
{
	return m_iPlotBuyCostModifier;
}

/// Change in gold cost to earn a new tile across the empire
int CvBuildingEntry::GetGlobalPlotBuyCostModifier() const
{
	return m_iGlobalPlotBuyCostModifier;
}

#if defined(MOD_BUILDINGS_CITY_WORKING)
/// Change in number of rings this city can work
int CvBuildingEntry::GetCityWorkingChange() const
{
	return m_iCityWorkingChange;
}

/// Change in number of rings any city can work
int CvBuildingEntry::GetGlobalCityWorkingChange() const
{
	return m_iGlobalCityWorkingChange;
}
#endif

#if defined(MOD_BUILDINGS_CITY_AUTOMATON_WORKERS)
/// Change in number of automatons for this city
int CvBuildingEntry::GetCityAutomatonWorkersChange() const
{
	return m_iCityAutomatonWorkersChange;
}

/// Change in number of automatons for every city
int CvBuildingEntry::GetGlobalCityAutomatonWorkersChange() const
{
	return m_iGlobalCityAutomatonWorkersChange;
}
#endif

/// Required Plot count of the CvArea this City belongs to (Usually used for Water Buildings to prevent Harbors in tiny lakes and such)
int CvBuildingEntry::GetMinAreaSize() const
{
	return m_iMinAreaSize;
}

/// Chance of building surviving after conquest
int CvBuildingEntry::GetConquestProbability() const
{
	return m_iConquestProbability;
}

/// Improvement in unit heal rate from this building
int CvBuildingEntry::GetHealRateChange() const
{
	return m_iHealRateChange;
}

/// Happiness provided by this building
int CvBuildingEntry::GetHappiness() const
{
	return m_iHappiness;
}

/// UnmoddedHappiness provided by this building - NOT affected by a city's pop
int CvBuildingEntry::GetUnmoddedHappiness() const
{
	return m_iUnmoddedHappiness;
}

/// Get percentage modifier to overall player happiness
int CvBuildingEntry::GetUnhappinessModifier() const
{
	return m_iUnhappinessModifier;
}
#if defined(MOD_BALANCE_CORE)
/// Get percentage modifier to overall player happiness
int CvBuildingEntry::GetLocalUnhappinessModifier() const
{
	return m_iLocalUnhappinessModifier;
}
/// Get percentage modifier to overall player happiness
int CvBuildingEntry::GetGlobalBuildingGoldMaintenanceMod() const
{
	return m_iGlobalBuildingGoldMaintenanceMod;
}
#endif
/// HappinessPerCity provided by this building
int CvBuildingEntry::GetHappinessPerCity() const
{
	return m_iHappinessPerCity;
}

/// Happiness per X number of Policies provided by this building
int CvBuildingEntry::GetHappinessPerXPolicies() const
{
	return m_iHappinessPerXPolicies;
}

/// CityCountUnhappinessMod provided by this building
int CvBuildingEntry::GetCityCountUnhappinessMod() const
{
	return m_iCityCountUnhappinessMod;
}

/// NoOccupiedUnhappiness
bool CvBuildingEntry::IsNoOccupiedUnhappiness() const
{
	return m_bNoOccupiedUnhappiness;
}

/// Population added to every City in the player's empire
int CvBuildingEntry::GetGlobalPopulationChange() const
{
	return m_iGlobalPopulationChange;
}

int CvBuildingEntry::GetPopulationChange() const
{
	return m_iPopulationChange;
}

/// If this # of players have a Tech then the owner of this Building gets that Tech as well
int CvBuildingEntry::GetTechShare() const
{
	return m_iTechShare;
}

/// Number of free techs granted by this building
int CvBuildingEntry::GetFreeTechs() const
{
	return m_iFreeTechs;
}

/// Number of free Policies granted by this building
int CvBuildingEntry::GetFreePolicies() const
{
	return m_iFreePolicies;
}

/// Number of free Great People granted by this building
int CvBuildingEntry::GetFreeGreatPeople() const
{
	return m_iFreeGreatPeople;
}

/// Boost to median tech received from research agreements
int CvBuildingEntry::GetMedianTechPercentChange() const
{
	return m_iMedianTechPercentChange;
}

/// Gold generated by this building
int CvBuildingEntry::GetGold() const
{
	return m_iGold;
}

/// Does a city need to be near a mountain to build this?
bool CvBuildingEntry::IsNearbyMountainRequired() const
{
	return m_bNearbyMountainRequired;
}

/// Does this Building allow us to Range Strike?
bool CvBuildingEntry::IsAllowsRangeStrike() const
{
	return m_bAllowsRangeStrike;
}

/// Does this Building allow us to Range Strike?
int CvBuildingEntry::CityRangedStrikeRange() const
{
	return m_iCityRangedStrikeRange;
}
/// Does this Building allow us to Range Strike?
int CvBuildingEntry::CityIndirectFire() const
{
	return m_iCityIndirectFire;
}
/// Modifier to the ranged attack strength of garrison in cities with this building.
int CvBuildingEntry::GetGarrisonRangedAttackModifier() const
{
	return m_iGarrisonRangedAttackModifier;
}
/// Does this Building allow us to Range Strike?
int CvBuildingEntry::CityRangedStrikeModifier() const
{
	return m_iRangedStrikeModifier;
}
#if defined(MOD_BALANCE_CORE)
// This is an actual Modifier where as GetDefenseModifier is just building Hit Points
int CvBuildingEntry::GetBuildingDefenseModifier() const
{
	return m_iBuildingDefenseModifier;
}
int CvBuildingEntry::GetDamageReductionFlat() const
{
	return m_iDamageReductionFlat;
}
int CvBuildingEntry::GetCitySupplyModifier() const
{
	return m_iCitySupplyModifier;
}
int CvBuildingEntry::GetCitySupplyModifierGlobal() const
{
	return m_iCitySupplyModifierGlobal;
}
int CvBuildingEntry::GetCitySupplyFlat() const
{
	return m_iCitySupplyFlat;

}
int CvBuildingEntry::GetCitySupplyFlatGlobal() const
{
	return m_iCitySupplyFlatGlobal;
}
#endif

/// Modifier to city defense
int CvBuildingEntry::GetDefenseModifier() const
{
	return m_iDefenseModifier;
}

/// Modifier to every City's Building defense
int CvBuildingEntry::GetGlobalDefenseModifier() const
{
	return m_iGlobalDefenseModifier;
}

/// Modifier to city's hit points
int CvBuildingEntry::GetExtraCityHitPoints() const
{
	return m_iExtraCityHitPoints;
}

/// Instant Friendship mod change with City States
int CvBuildingEntry::GetMinorFriendshipChange() const
{
	return m_iMinorFriendshipChange;
}

/// VPs added to overall Team score
int CvBuildingEntry::GetVictoryPoints() const
{
	return m_iVictoryPoints;
}

/// Extra religion spreads from missionaries built in this city
int CvBuildingEntry::GetExtraMissionarySpreads() const
{
	return m_iExtraMissionarySpreads;
}

/// Extra religion spreads from missionaries global
int CvBuildingEntry::GetExtraMissionarySpreadsGlobal() const
{
	return m_iExtraMissionarySpreadsGlobal;
}

/// Extra religion spreads from missionaries built in this city
int CvBuildingEntry::GetExtraMissionaryStrength() const
{
	return m_iExtraMissionaryStrength;
}

/// Extra religion pressure emanating from this city
int CvBuildingEntry::GetReligiousPressureModifier() const
{
	return m_iReligiousPressureModifier;
}

/// Modifier to chance of espionage against this city
int CvBuildingEntry::GetEspionageModifier() const
{
	return m_iEspionageModifier;
}

/// Modifier to chance of espionage against all cities
int CvBuildingEntry::GetGlobalEspionageModifier() const
{
	return m_iGlobalEspionageModifier;
}

/// Modifier to Security against espionage in this city
int CvBuildingEntry::GetSpySecurityModifier() const
{
	return m_iSpySecurityModifier;
}

/// Modifier to Security against espionage based on ESPIONAGE_SECURITY_PER_POPULATION_BUILDING_SCALER
int CvBuildingEntry::GetSpySecurityModifierPerXPop() const
{
	return m_iSpySecurityModifierPerXPop;
}

/// Modifier to Security against espionage in all cities
int CvBuildingEntry::GetGlobalSpySecurityModifier() const
{
	return m_iGlobalSpySecurityModifier;
}

/// Extra spies after this is built
int CvBuildingEntry::GetExtraSpies() const
{
	return m_iExtraSpies;
}

/// Increase in rank of all starting spies
int CvBuildingEntry::GetSpyRankChange() const
{
	return m_iSpyRankChange;
}

/// How much the trade recipient gets for a trade route being establish with the city
int CvBuildingEntry::GetTradeRouteRecipientBonus() const
{
	return m_iTradeRouteRecipientBonus;
}

/// How much the trade target gets for a trade route being established with the city
int CvBuildingEntry::GetTradeRouteTargetBonus() const
{
	return m_iTradeRouteTargetBonus;
}

int CvBuildingEntry::GetNumTradeRouteBonus() const
{
	return m_iNumTradeRouteBonus;
}

int CvBuildingEntry::GetTradeRouteSeaDistanceModifier() const
{
	return m_iTradeRouteSeaDistanceModifier;
}

int CvBuildingEntry::GetTradeRouteSeaGoldBonus() const
{
	return m_iTradeRouteSeaGoldBonus;
}

int CvBuildingEntry::GetTradeRouteLandDistanceModifier() const
{
	return m_iTradeRouteLandDistanceModifier;
}

int CvBuildingEntry::GetTradeRouteLandGoldBonus() const
{
	return m_iTradeRouteLandGoldBonus;
}

int CvBuildingEntry::GetCityConnectionTradeRouteGoldModifier() const
{
	return m_iCityConnectionTradeRouteGoldModifier;
}

int CvBuildingEntry::GetCityStateTradeRouteProductionModifier() const
{
	return m_iCityStateTradeRouteProductionModifier;
}

int CvBuildingEntry::GetGreatScientistBeakerModifier() const
{
	return m_iGreatScientistBeakerModifier;
}

/// One-time boost for all existing spies
int CvBuildingEntry::GetInstantSpyRankChange() const
{
	return m_iInstantSpyRankChange;
}

/// Tourism output from Landmarks and Wonders
int CvBuildingEntry::GetLandmarksTourismPercent() const
{
	return m_iLandmarksTourismPercent;
}

/// For the terra cotta army. DOUBLE THE SIZE OF YOUR ARMY
int CvBuildingEntry::GetInstantMilitaryIncrease() const
{
	return m_iInstantMilitaryIncrease;
}

/// Boost to tourism output from Great Works
int CvBuildingEntry::GetGreatWorksTourismModifier() const
{
	return m_iGreatWorksTourismModifier;
}

/// Is an Ideology choice brought on by constructing this building in X cities?
int CvBuildingEntry::GetXBuiltTriggersIdeologyChoice() const
{
	return m_iXBuiltTriggersIdeologyChoice;
}

/// Extra votes to use in leagues
int CvBuildingEntry::GetExtraLeagueVotes() const
{
	return m_iExtraLeagueVotes;
}

int CvBuildingEntry::GetSingleLeagueVotes() const
{
	return m_iSingleLeagueVotes;
}
/// Extra votes from faith generation
int CvBuildingEntry::GetFaithToVotes() const
{
	return m_iFaithToVotesBase;
}
/// Extra votes from captured capitals
int CvBuildingEntry::GetCapitalsToVotes() const
{
	return m_iCapitalsToVotesBase;
}
/// Extra votes from DoFs
int CvBuildingEntry::GetDoFToVotes() const
{
	return m_iDoFToVotesBase;
}

/// Extra votes from Research Agreements
int CvBuildingEntry::GetRAToVotes() const
{
	return m_iRAToVotesBase;
}
/// Extra votes from Defense Pacts
int CvBuildingEntry::GetDPToVotes() const
{
	return m_iDPToVotesBase;
}

/// Allows the player to ignore the global Defensive Pact limit
bool CvBuildingEntry::IsIgnoreDefensivePactLimit() const
{
	return m_bIgnoreDefensivePactLimit;
}

/// Extra votes from Research Agreements
int CvBuildingEntry::GetGPExpendInfluence() const
{
	return m_iGPExpendInfluenceBase;
}

int CvBuildingEntry::GetEmpireSizeModifierReduction() const
{
	return m_iEmpireSizeModifierReduction;
}
int CvBuildingEntry::GetEmpireSizeModifierReductionGlobal() const
{
	return m_iEmpireSizeModifierReductionGlobal;
}
int CvBuildingEntry::GetBasicNeedsMedianModifier() const
{
	return m_iBasicNeedsMedianModifier;
}
int CvBuildingEntry::GetGoldMedianModifier() const
{
	return m_iGoldMedianModifier;
}
int CvBuildingEntry::GetScienceMedianModifier() const
{
	return m_iScienceMedianModifier;
}
int CvBuildingEntry::GetCultureMedianModifier() const
{
	return m_iCultureMedianModifier;
}
int CvBuildingEntry::GetReligiousUnrestModifier() const
{
	return m_iReligiousUnrestModifier;
}
int CvBuildingEntry::GetBasicNeedsMedianModifierGlobal() const
{
	return m_iBasicNeedsMedianModifierGlobal;
}
int CvBuildingEntry::GetGoldMedianModifierGlobal() const
{
	return m_iGoldMedianModifierGlobal;
}
int CvBuildingEntry::GetScienceMedianModifierGlobal() const
{
	return m_iScienceMedianModifierGlobal;
}
int CvBuildingEntry::GetCultureMedianModifierGlobal() const
{
	return m_iCultureMedianModifierGlobal;
}
int CvBuildingEntry::GetReligiousUnrestModifierGlobal() const
{
	return m_iReligiousUnrestModifierGlobal;
}

#if defined(MOD_RELIGION_CONVERSION_MODIFIERS)
/// Modifier to chance of conversion against this city
int CvBuildingEntry::GetConversionModifier() const
{
	return m_iConversionModifier;
}

/// Modifier to chance of conversion against all cities
int CvBuildingEntry::GetGlobalConversionModifier() const
{
	return m_iGlobalConversionModifier;
}
#endif

/// What ring the engine will try to display this building
int CvBuildingEntry::GetPreferredDisplayPosition() const
{
	return m_iPreferredDisplayPosition;
}

/// index of portrait in the texture sheet
int CvBuildingEntry::GetPortraitIndex() const
{
	return m_iPortraitIndex;
}

/// Is the presence of this building shared with team allies?
bool CvBuildingEntry::IsTeamShare() const
{
	return m_bTeamShare;
}

/// Must this be built in a coastal city?
bool CvBuildingEntry::IsWater() const
{
	return m_bWater;
}

/// Must this be built in a river city?
bool CvBuildingEntry::IsRiver() const
{
	return m_bRiver;
}

/// Must this be built in a city next to FreshWater?
bool CvBuildingEntry::IsFreshWater() const
{
	return m_bFreshWater;
}

/// Does this building add FreshWater?
bool CvBuildingEntry::IsAddsFreshWater() const
{
	return m_bAddsFreshWater;
}

/// Do we need to purchase this building (i.e. can't be built)?
bool CvBuildingEntry::IsPurchaseOnly() const
{
	return m_bPurchaseOnly;
}
//Does this give us the secondary pantheon effect?
bool CvBuildingEntry::IsSecondaryPantheon() const
{
	return m_bSecondaryPantheon;
}
/// Change to Great Work yield by type
int CvBuildingEntry::GetGreatWorkYieldChange(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piGreatWorkYieldChange ? m_piGreatWorkYieldChange[i] : -1;
}
/// Array of yield changes to Great Works
int* CvBuildingEntry::GetGreatWorkYieldChangeArray() const
{
	return m_piGreatWorkYieldChange;
}

/// Change to Great Work yield by type
int CvBuildingEntry::GetGreatWorkYieldChangeLocal(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piGreatWorkYieldChangeLocal ? m_piGreatWorkYieldChangeLocal[i] : -1;
}
/// Array of yield changes to Great Works
int* CvBuildingEntry::GetGreatWorkYieldChangeLocalArray() const
{
	return m_piGreatWorkYieldChangeLocal;
}

/// Must this be built in a city next to Mountain?
bool CvBuildingEntry::IsMountain() const
{
	return m_bMountain;
}

/// Must this be built in a city on a hill?
bool CvBuildingEntry::IsHill() const
{
	return m_bHill;
}

/// Must this be built in a city on Flat ground?
bool CvBuildingEntry::IsFlat() const
{
	return m_bFlat;
}

/// Is this an obstacle at the edge of your empire (e.g. Great Wall) -- for you AND your teammates
bool CvBuildingEntry::IsBorderObstacle() const
{
	return m_bBorderObstacle;
}
#if defined(MOD_BALANCE_CORE)
bool CvBuildingEntry::IsAnyBodyOfWater() const
{
	return m_bAnyWater;
}
/// How much damage does this building contribute against attacking air unit?
int CvBuildingEntry::GetCityAirStrikeDefense() const
{
	return m_iCityAirStrikeDefense;
}
/// Is this an obstacle for just the tiles around your city?
int CvBuildingEntry::GetBorderObstacleLand() const
{
	return m_iBorderObstacleCity;
}

int CvBuildingEntry::GetDeepWaterTileDamage() const
{
	return m_iDeepWaterTileDamage;
}

//Is this an obstacle for the water tiles around your city?
int CvBuildingEntry::GetBorderObstacleWater() const
{
	return m_iBorderObstacleWater;
}
int CvBuildingEntry::GetWLTKDTurns() const
{
	return m_iWLTKDTurns;
}
int CvBuildingEntry::GetEventTourism() const
{
	return m_iEventTourism;
}
int CvBuildingEntry::GetLandTourismEnd() const
{
	return m_iLandTourism;
}
int CvBuildingEntry::GetSeaTourismEnd() const
{
	return m_iSeaTourism;
}
int CvBuildingEntry::GetAlwaysHeal() const
{
	return m_iAlwaysHeal;
}
bool CvBuildingEntry::IsCorp() const
{
	return m_bIsCorp;
}
int CvBuildingEntry::GetNukeInterceptionChance() const
{
	return m_iNukeInterceptionChance;
}
#endif
/// +x% Food for each follower of the city's majority religion
int CvBuildingEntry::GetFoodBonusPerCityMajorityFollower() const
{
	return m_iFoodBonusPerCityMajorityFollower;
}
#if defined(HH_MOD_BUILDINGS_FRUITLESS_PILLAGE)
/// Is a border-wide nullification of the heal and gold benefits from pillaging
bool CvBuildingEntry::IsPlayerBorderGainlessPillage() const
{
	return m_bPlayerBorderGainlessPillage;
}

/// Is a nullification of heal and gold benefits from pillaging this city's tiles
bool CvBuildingEntry::IsCityGainlessPillage() const
{
	return m_bCityGainlessPillage;
}
#endif
/// Is this an obstacle at the edge of your empire (e.g. Great Wall) -- for just the owning player
bool CvBuildingEntry::IsPlayerBorderObstacle() const
{
	return m_bPlayerBorderObstacle;
}

/// Does this trigger drawing a wall around the city
bool CvBuildingEntry::IsCityWall() const
{
	return m_bCityWall;
}

/// Is this building unlocked through religion?
bool CvBuildingEntry::IsUnlockedByBelief() const
{
	return m_bUnlockedByBelief;
}

/// Is this building unlocked through League actions?
bool CvBuildingEntry::IsUnlockedByLeague() const
{
	return m_bUnlockedByLeague;
}

/// Does it have to be built in the Holy City?
bool CvBuildingEntry::IsRequiresHolyCity() const
{
	return m_bRequiresHolyCity;
}

/// Does this building affect spy rates when it is built?
bool CvBuildingEntry::AffectSpiesNow() const
{
	return m_bAffectSpiesNow;
}

// Is this an espionage building that should be disabled when espionage is disabled?
bool CvBuildingEntry::IsEspionage() const
{
	return m_bEspionage;
}

bool CvBuildingEntry::AllowsFoodTradeRoutes() const
{
	return m_bAllowsFoodTradeRoutes;
}
bool CvBuildingEntry::AllowsFoodTradeRoutesGlobal() const
{
	return m_bAllowsFoodTradeRoutesGlobal;
}

bool CvBuildingEntry::AllowsProductionTradeRoutes() const
{
	return m_bAllowsProductionTradeRoutes;
}
bool CvBuildingEntry::AllowsProductionTradeRoutesGlobal() const
{
	return m_bAllowsProductionTradeRoutesGlobal;
}

bool CvBuildingEntry::NullifyInfluenceModifier() const
{
	return m_bNullifyInfluenceModifier;
}

/// Does this building define the capital?
bool CvBuildingEntry::IsCapital() const
{
	return m_bCapital;
}

/// Does this building spawn a golden age?
bool CvBuildingEntry::IsGoldenAge() const
{
	return m_bGoldenAge;
}

/// Is the map centered after this building is constructed?
bool CvBuildingEntry::IsMapCentering() const
{
	return m_bMapCentering;
}

/// Can this building never be captured?
bool CvBuildingEntry::IsNeverCapture() const
{
	return m_bNeverCapture;
}

/// Is the building immune to nukes?
bool CvBuildingEntry::IsNukeImmune() const
{
	return m_bNukeImmune;
}

/// Does the building add an additional of each luxury in city radius
bool CvBuildingEntry::IsExtraLuxuries() const
{
	return m_bExtraLuxuries;
}

/// Begins voting for the diplo victory?
bool CvBuildingEntry::IsDiplomaticVoting() const
{
	return m_bDiplomaticVoting;
}

/// Does the building allow routes over the water
bool CvBuildingEntry::AllowsWaterRoutes() const
{
	return m_bAllowsWaterRoutes;
}

/// Does the building allow industrial routes over the water
bool CvBuildingEntry::AllowsIndustrialWaterRoutes() const
{
	return m_bAllowsIndustrialWaterRoutes;
}

/// Does the building allow routes through the air
bool CvBuildingEntry::AllowsAirRoutes() const
{
	return m_bAllowsAirRoutes;
}

/// Derive property: is this considered a science building?
bool CvBuildingEntry::IsScienceBuilding() const
{
	bool bRtnValue = false;

	if(IsCapital())
	{
		bRtnValue = false;
	}
	else if(GetYieldChange(YIELD_SCIENCE) > 0)
	{
		bRtnValue = true;
	}
	else if(GetYieldChangePerPop(YIELD_SCIENCE) > 0)
	{
		bRtnValue = true;
	}
	else if(GetYieldChangePerReligion(YIELD_SCIENCE) > 0)
	{
		bRtnValue = true;
	}
	else if(GetYieldModifier(YIELD_SCIENCE) > 0)
	{
		bRtnValue = true;
	}
#if defined(MOD_BALANCE_CORE)
	else if (GetYieldChangePerPopInEmpire(YIELD_SCIENCE) > 0)
	{
		bRtnValue = true;
	}
	else if(GetMedianTechPercentChange() > 0)
	{
		bRtnValue = true;
	}
#endif

	return bRtnValue;
}

/// Retrieve art tag
const char* CvBuildingEntry::GetArtDefineTag() const
{
	return m_strArtDefineTag.c_str();
}

/// Set art tag
void CvBuildingEntry::SetArtDefineTag(const char* szVal)
{
	m_strArtDefineTag = szVal;
}

/// Return whether we should try to find a culture specific variant art tag
const bool CvBuildingEntry::GetArtInfoCulturalVariation() const
{
	return m_bArtInfoCulturalVariation;
}

/// Return whether we should try to find an era specific variant art tag
const bool CvBuildingEntry::GetArtInfoEraVariation() const
{
	return m_bArtInfoEraVariation;
}

/// Return whether we should try to find an era specific variant art tag
const bool CvBuildingEntry::GetArtInfoRandomVariation() const
{
	return m_bArtInfoRandomVariation;
}

const char* CvBuildingEntry::GetWonderSplashAudio() const
{
	return m_strWonderSplashAudio.c_str();
}

CvString CvBuildingEntry::GetThemingBonusHelp() const
{
	return m_strThemingBonusHelp;
}

int CvBuildingEntry::GetDistressFlatReduction() const
{
	return m_iDistressFlatReduction;
}

int CvBuildingEntry::GetPovertyFlatReduction() const
{
	return m_iPovertyFlatReduction;
}

int CvBuildingEntry::GetIlliteracyFlatReduction() const
{
	return m_iIlliteracyFlatReduction;
}

int CvBuildingEntry::GetBoredomFlatReduction() const
{
	return m_iBoredomFlatReduction;
}

int CvBuildingEntry::GetReligiousUnrestFlatReduction() const
{
	return m_iReligiousUnrestFlatReduction;
}

int CvBuildingEntry::GetDistressFlatReductionGlobal() const
{
	return m_iDistressFlatReductionGlobal;
}

int CvBuildingEntry::GetPovertyFlatReductionGlobal() const
{
	return m_iPovertyFlatReductionGlobal;
}

int CvBuildingEntry::GetIlliteracyFlatReductionGlobal() const
{
	return m_iIlliteracyFlatReductionGlobal;
}

int CvBuildingEntry::GetBoredomFlatReductionGlobal() const
{
	return m_iBoredomFlatReductionGlobal;
}

int CvBuildingEntry::GetReligiousUnrestFlatReductionGlobal() const
{
	return m_iReligiousUnrestFlatReductionGlobal;
}

int CvBuildingEntry::GetExperiencePerGoldenAge() const
{
	return m_iExperiencePerGoldenAge;
}

int CvBuildingEntry::GetExperiencePerGoldenAgeCap() const
{
	return m_iExperiencePerGoldenAgeCap;
}

/// Instant Boost of religious pressure in the city when built
int CvBuildingEntry::GetInstantReligionPressure() const
{
	return m_iInstantReligionPressure;
}

int CvBuildingEntry::GetDefensePerXWonder() const
{
	return m_iDefensePerXWonder;
}

int CvBuildingEntry::GetBasePressureModGlobal() const
{
	return m_iBasePressureModGlobal;
}

// ARRAYS
/// Change to yield by type
int CvBuildingEntry::GetGrowthExtraYield(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piGrowthExtraYield ? m_piGrowthExtraYield[i] : -1;
}

/// Array of yield changes
int* CvBuildingEntry::GetGrowthExtraYieldArray() const
{
	return m_piGrowthExtraYield;
}

/// Need building in the city
int CvBuildingEntry::GetNeedBuildingThisCity() const
{
	return m_iNeedBuildingThisCity;
}

#if defined(MOD_BALANCE_CORE_POLICIES)
/// Change to yield by type if unit defeated in battle
int CvBuildingEntry::GetYieldFromDeath(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromDeath ? m_piYieldFromDeath[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromDeathArray() const
{
	return m_piYieldFromDeath;
}
#endif
#if defined(MOD_BALANCE_CORE)
/// Change to yield if victorious in battle.
int CvBuildingEntry::GetYieldFromVictory(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromVictory ? m_piYieldFromVictory[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromVictoryArray() const
{
	return m_piYieldFromVictory;
}

/// Change to yield if victorious in battle.
int CvBuildingEntry::GetYieldFromVictoryGlobal(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromVictoryGlobal ? m_piYieldFromVictoryGlobal[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromVictoryGlobalArray() const
{
	return m_piYieldFromVictoryGlobal;
}

/// Change to yield if victorious in battle, scaling with era.
int CvBuildingEntry::GetYieldFromVictoryGlobalEraScaling(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromVictoryGlobalEraScaling ? m_piYieldFromVictoryGlobalEraScaling[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromVictoryGlobalEraScalingArray() const
{
	return m_piYieldFromVictoryGlobalEraScaling;
}

/// Change to yield if victorious in battle while in a golden age
int CvBuildingEntry::GetYieldFromVictoryGlobalInGoldenAge(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromVictoryGlobalInGoldenAge ? m_piYieldFromVictoryGlobalInGoldenAge[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromVictoryGlobalInGoldenAgeArray() const
{
	return m_piYieldFromVictoryGlobalInGoldenAge;
}

/// Change to yield if victorious in battle while in a golden age, scaling with era.
int CvBuildingEntry::GetYieldFromVictoryGlobalInGoldenAgeEraScaling(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromVictoryGlobalInGoldenAgeEraScaling ? m_piYieldFromVictoryGlobalInGoldenAgeEraScaling[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromVictoryGlobalInGoldenAgeEraScalingArray() const
{
	return m_piYieldFromVictoryGlobalInGoldenAgeEraScaling;
}



/// Change to yield if victorious in battle.
int CvBuildingEntry::GetYieldFromVictoryGlobalPlayer(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromVictoryGlobalPlayer ? m_piYieldFromVictoryGlobalPlayer[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromVictoryGlobalPlayerArray() const
{
	return m_piYieldFromVictoryGlobalPlayer;
}

/// Change to yield if pillaging
int CvBuildingEntry::GetYieldFromPillage(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromPillage ? m_piYieldFromPillage[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromPillageArray() const
{
	return m_piYieldFromPillage;
}

/// Change to yield if pillaging
int CvBuildingEntry::GetYieldFromPillageGlobal(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromPillageGlobal ? m_piYieldFromPillageGlobal[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromPillageGlobalArray() const
{
	return m_piYieldFromPillageGlobal;
}

/// Change to yield if pillaging
int CvBuildingEntry::GetYieldFromPillageGlobalPlayer(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromPillageGlobalPlayer ? m_piYieldFromPillageGlobalPlayer[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromPillageGlobalPlayerArray() const
{
	return m_piYieldFromPillageGlobalPlayer;
}


/// Instant yield when starting a golden age
int CvBuildingEntry::GetYieldFromGoldenAgeStart(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromGoldenAgeStart ? m_piYieldFromGoldenAgeStart[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromGoldenAgeStartArray() const
{
	return m_piYieldFromGoldenAgeStart;
}

/// Yields permamently added whenever a golden age starts
int CvBuildingEntry::GetYieldChangePerGoldenAge(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldChangePerGoldenAge ? m_piYieldChangePerGoldenAge[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldChangePerGoldenAgeArray() const
{
	return m_piYieldChangePerGoldenAge;
}

/// cap to the values in GetYieldChangesPerGoldenAge
int CvBuildingEntry::GetYieldChangePerGoldenAgeCap(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldChangePerGoldenAgeCap ? m_piYieldChangePerGoldenAgeCap[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldChangePerGoldenAgeCapArray() const
{
	return m_piYieldChangePerGoldenAgeCap;
}

/// Change to yield during golden ages
int CvBuildingEntry::GetYieldChangesPerLocalTheme(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldChangesPerLocalTheme ? m_piYieldChangesPerLocalTheme[i] : -1;
}

/// Instant yields from gifting a unit
int CvBuildingEntry::GetYieldFromUnitGiftGlobal(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromUnitGiftGlobal ? m_piYieldFromUnitGiftGlobal[i] : -1;
}

/// Change to yield during golden ages
int CvBuildingEntry::GetGoldenAgeYieldMod(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piGoldenAgeYieldMod ? m_piGoldenAgeYieldMod[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetGoldenAgeYieldModArray() const
{
	return m_piGoldenAgeYieldMod;
}

/// Instant yield when WLTKD starts
int CvBuildingEntry::GetInstantYieldFromWLTKDStart(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piInstantYieldFromWLTKDStart[i];
}

/// Change to yield during WLTKD
int CvBuildingEntry::GetYieldFromWLTKD(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromWLTKD ? m_piYieldFromWLTKD[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromWLTKDArray() const
{
	return m_piYieldFromWLTKD;
}

/// Free Yield from GP Expend
int CvBuildingEntry::GetYieldFromGPExpend(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromGPExpend ? m_piYieldFromGPExpend[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromGPExpendArray() const
{
	return m_piYieldFromGPExpend;
}
/// Free Yield from Tech unlocked
int CvBuildingEntry::GetYieldFromTech(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromTech ? m_piYieldFromTech[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromTechArray() const
{
	return m_piYieldFromTech;
}
/// Does this Policy grant yields from constructing buildings?
int CvBuildingEntry::GetYieldFromConstruction(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromConstruction[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromConstructionArray() const
{
	return m_piYieldFromConstruction;
}

int CvBuildingEntry::GetYieldFromBirth(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromBirth[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromBirthArray() const
{
	return m_piYieldFromBirth;
}
int CvBuildingEntry::GetYieldFromBirthEraScaling(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromBirthEraScaling[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromBirthEraScalingArray() const
{
	return m_piYieldFromBirthEraScaling;
}
int CvBuildingEntry::GetYieldFromBirthRetroactive(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromBirthRetroactive[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromBirthRetroactiveArray() const
{
	return m_piYieldFromBirthRetroactive;
}
int CvBuildingEntry::GetYieldFromUnitProduction(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromUnitProduction[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromUnitProductionArray() const
{
	return m_piYieldFromUnitProduction;
}

int CvBuildingEntry::GetYieldFromBorderGrowth(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromBorderGrowth[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromBorderGrowthArray() const
{
	return m_piYieldFromBorderGrowth;
}

int CvBuildingEntry::GetYieldFromPolicyUnlock(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromPolicyUnlock[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromPolicyUnlockArray() const
{
	return m_piYieldFromPolicyUnlock;
}

int CvBuildingEntry::GetYieldFromUnitLevelUp(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromUnitLevelUp[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromUnitLevelUpArray() const
{
	return m_piYieldFromUnitLevelUp;
}

int CvBuildingEntry::GetYieldFromCombatExperienceTimes100(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromCombatExperienceTimes100[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromCombatExperienceTimes100Array() const
{
	return m_piYieldFromCombatExperienceTimes100;
}


int CvBuildingEntry::GetYieldFromPurchase(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromPurchase[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromPurchaseArray() const
{
	return m_piYieldFromPurchase;
}

int CvBuildingEntry::GetYieldFromPurchaseGlobal(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromPurchaseGlobal[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromPurchaseGlobalArray() const
{
	return m_piYieldFromPurchaseGlobal;
}

int CvBuildingEntry::GetYieldFromFaithPurchase(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromFaithPurchase[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromFaithPurchaseArray() const
{
	return m_piYieldFromFaithPurchase;
}

int CvBuildingEntry::GetYieldFromUnitLevelUpGlobal(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromUnitLevelUpGlobal[i];
}

/// Instant yield granted when an international trade route ends
int CvBuildingEntry::GetYieldFromInternationalTREnd(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromInternationalTREnd[i];
}

/// Instant yield granted when an international trade route ends
int CvBuildingEntry::GetYieldFromInternalTREnd(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromInternalTREnd[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromInternalTREndArray() const
{
	return m_piYieldFromInternalTREnd;
}


/// Does this Policy grant yields from constructing buildings?
int CvBuildingEntry::GetYieldFromInternal(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromInternal[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromInternalArray() const
{
	return m_piYieldFromInternal;
}


/// Does this Building grant yields when a b'ak'tun ends?
int CvBuildingEntry::GetYieldFromLongCount(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromLongCount[i];
}

/// Does this Building grant yields when a Great Writer is born?
int CvBuildingEntry::GetYieldFromGPBirthScaledWithWriterBulb(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromGPBirthScaledWithWriterBulb[i];
}

/// Does this Building grant yields when a Great Artist is born?
int CvBuildingEntry::GetYieldFromGPBirthScaledWithArtistBulb(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromGPBirthScaledWithArtistBulb[i];
}

/// Does this Building grant yields from other yields when a Great Person is born?
map<GreatPersonTypes, map<pair<YieldTypes, YieldTypes>, int>> CvBuildingEntry::GetYieldFromGPBirthScaledWithPerTurnYieldMap() const
{
	return m_miYieldFromGPBirthScaledWithPerTurnYield;
}
//	--------------------------------------------------------------------------------
/// Instant yield when a Great Person is born based on the yield output of the city
int CvBuildingEntry::GetYieldFromGPBirthScaledWithPerTurnYield(GreatPersonTypes eGreatPerson, YieldTypes eYieldIn, YieldTypes eYieldOut) const
{
	VALIDATE_OBJECT();
	ASSERT_DEBUG(eGreatPerson >= 0, "eYieldIn expected to be >= 0");
	ASSERT_DEBUG(eGreatPerson < GC.getNumGreatPersonInfos(), "eYieldIn expected to be < getNumGreatPersonInfos()");
	ASSERT_DEBUG(eYieldIn >= 0, "eYieldIn expected to be >= 0");
	ASSERT_DEBUG(eYieldIn < NUM_YIELD_TYPES, "eYieldIn expected to be < NUM_YIELD_TYPES");
	ASSERT_DEBUG(eYieldOut >= 0, "eYieldOut expected to be >= 0");
	ASSERT_DEBUG(eYieldOut < NUM_YIELD_TYPES, "eYieldOut expected to be < NUM_YIELD_TYPES");

	map<GreatPersonTypes, map<std::pair<YieldTypes, YieldTypes>, int>>::const_iterator it = m_miYieldFromGPBirthScaledWithPerTurnYield.find(eGreatPerson);
	if (it != m_miYieldFromGPBirthScaledWithPerTurnYield.end()) // find returns the iterator to map::end if the key i is not present in the map
	{
		map<std::pair<YieldTypes, YieldTypes>, int> miYieldMap = it->second;
		map<std::pair<YieldTypes, YieldTypes>, int>::const_iterator it2 = miYieldMap.find(std::make_pair(eYieldIn, eYieldOut));
		if (it2 != miYieldMap.end())
		{
			return it2->second;
		}
	}

	return 0;
}

/// Does this Building grant yields from constructing buildings?
int CvBuildingEntry::GetYieldFromProcessModifier(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromProcessModifier[i];
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromProcessModifierArray() const
{
	return m_piYieldFromProcessModifier;
}

/// Array of yield changes
int CvBuildingEntry::GetThemingYieldBonus(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piThemingYieldBonus ? m_piThemingYieldBonus[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetThemingYieldBonusArray() const
{
	return m_piThemingYieldBonus;
}

/// Array of yield changes
int CvBuildingEntry::GetYieldFromSpyAttack(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromSpyAttack ? m_piYieldFromSpyAttack[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromSpyAttackArray() const
{
	return m_piYieldFromSpyAttack;
}

/// Array of yield changes
int CvBuildingEntry::GetYieldFromSpyDefense(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromSpyDefense ? m_piYieldFromSpyDefense[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromSpyDefenseArray() const
{
	return m_piYieldFromSpyDefense;
}

/// Array of yield changes
int CvBuildingEntry::GetYieldFromSpyIdentify(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromSpyIdentify ? m_piYieldFromSpyIdentify[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromSpyIdentifyArray() const
{
	return m_piYieldFromSpyIdentify;
}

/// Array of yield changes
int CvBuildingEntry::GetYieldFromSpyDefenseOrID(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromSpyDefenseOrID ? m_piYieldFromSpyDefenseOrID[i] : -1;
}
/// Array of yield changes
int* CvBuildingEntry::GetYieldFromSpyDefenseOrIDArray() const
{
	return m_piYieldFromSpyDefenseOrID;
}

int CvBuildingEntry::GetYieldChangesPerCityStrengthTimes100(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldChangesPerCityStrengthTimes100 ? m_piYieldChangesPerCityStrengthTimes100[i] : -1;
}

/// Array of yield changes
int CvBuildingEntry::GetYieldFromSpyRigElection(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldFromSpyRigElection ? m_piYieldFromSpyRigElection[i] : -1;
}

/// Array of yield changes
int* CvBuildingEntry::GetYieldFromSpyRigElectionArray() const
{
	return m_piYieldFromSpyRigElection;
}


#endif
/// Change to yield by type
int CvBuildingEntry::GetYieldChange(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldChange ? m_piYieldChange[i] : -1;
}

/// Array of yield changes
int* CvBuildingEntry::GetYieldChangeArray() const
{
	return m_piYieldChange;
}

/// Change to yield per turn, scaling with era
int CvBuildingEntry::GetYieldChangeEraScalingTimes100(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldChangeEraScalingTimes100[i];
}

/// Change to yield per turn per non-dummy building in the city
fraction CvBuildingEntry::GetYieldChangePerBuilding(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_pfYieldChangePerBuilding[i];
}

/// Change to yield per turn per city tile
fraction CvBuildingEntry::GetYieldChangePerTile(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_pfYieldChangePerTile[i];
}

/// Change to yield per turn per strategic resource from city-states
fraction CvBuildingEntry::GetYieldChangePerCityStateStrategicResource(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_pfYieldChangePerCityStateStrategicResource[i];
}

/// Change to yield by type
int CvBuildingEntry::GetYieldChangePerPop(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldChangePerPop ? m_piYieldChangePerPop[i] : -1;
}

/// Array of yield changes
int* CvBuildingEntry::GetYieldChangePerPopArray() const
{
	return m_piYieldChangePerPop;
}

/// Change to yield by type
int CvBuildingEntry::GetYieldChangePerPopInEmpire(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");

	std::map<int, int>::const_iterator it = m_piYieldChangePerPopInEmpire.find(i);
	if (it != m_piYieldChangePerPopInEmpire.end()) // find returns the iterator to map::end if the key i is not present in the map
	{
		return it->second;
	}

	return 0;
}

std::map<int, int> CvBuildingEntry::GetExtraPlayerInstancesFromAccomplishments() const
{
	return m_miExtraPlayerInstancesFromAccomplishments;
}

/// Change to yield by type
int CvBuildingEntry::GetYieldChangePerReligion(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldChangePerReligion ? m_piYieldChangePerReligion[i] : -1;
}

/// Array of yield changes
int* CvBuildingEntry::GetYieldChangePerReligionArray() const
{
	return m_piYieldChangePerReligion;
}

set<int> CvBuildingEntry::GetUnitClassTrainingAllowed() const
{
	return m_siUnitClassTrainingAllowed;
}

set<std::pair<int, bool>> CvBuildingEntry::GetResourceClaim() const
{
	return m_sibResourceClaim;
}

map<ProjectTypes, int> CvBuildingEntry::GetWLTKDFromProject() const
{
	return m_miWLTKDFromProject;
}

/// Modifier to yield by type
int CvBuildingEntry::GetYieldModifier(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldModifier ? m_piYieldModifier[i] : -1;
}

/// Array of yield modifiers
int* CvBuildingEntry::GetYieldModifierArray() const
{
	return m_piYieldModifier;
}

/// Modifier to yield by type in area
int CvBuildingEntry::GetAreaYieldModifier(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piAreaYieldModifier ? m_piAreaYieldModifier[i] : -1;
}

/// Array of yield modifiers in area
int* CvBuildingEntry::GetAreaYieldModifierArray() const
{
	return m_piAreaYieldModifier;
}

/// Global modifier to yield by type
int CvBuildingEntry::GetGlobalYieldModifier(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piGlobalYieldModifier ? m_piGlobalYieldModifier[i] : -1;
}

/// Array of global yield modifiers
int* CvBuildingEntry::GetGlobalYieldModifierArray() const
{
	return m_piGlobalYieldModifier;
}

/// Sea plot yield changes by type
int CvBuildingEntry::GetSeaPlotYieldChange(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piSeaPlotYieldChange ? m_piSeaPlotYieldChange[i] : -1;
}

/// Array of sea plot yield changes
int* CvBuildingEntry::GetSeaPlotYieldChangeArray() const
{
	return m_piSeaPlotYieldChange;
}

/// River plot yield changes by type
int CvBuildingEntry::GetRiverPlotYieldChange(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piRiverPlotYieldChange ? m_piRiverPlotYieldChange[i] : -1;
}

/// Array of river plot yield changes
int* CvBuildingEntry::GetRiverPlotYieldChangeArray() const
{
	return m_piRiverPlotYieldChange;
}

/// Lake plot yield changes by type
int CvBuildingEntry::GetLakePlotYieldChange(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piLakePlotYieldChange ? m_piLakePlotYieldChange[i] : -1;
}

/// Array of lake plot yield changes
int* CvBuildingEntry::GetLakePlotYieldChangeArray() const
{
	return m_piLakePlotYieldChange;
}

/// Global lake plot yield changes by type
int CvBuildingEntry::GetLakePlotYieldChangeGlobal(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piLakePlotYieldChangeGlobal ? m_piLakePlotYieldChangeGlobal[i] : -1;
}

/// Array of global lake plot yield changes
int* CvBuildingEntry::GetLakePlotYieldChangeGlobalArray() const
{
	return m_piLakePlotYieldChangeGlobal;
}

/// Sea resource yield changes by type
int CvBuildingEntry::GetSeaResourceYieldChange(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piSeaResourceYieldChange ? m_piSeaResourceYieldChange[i] : -1;
}

/// Array of sea resource yield changes
int* CvBuildingEntry::GetSeaResourceYieldChangeArray() const
{
	return m_piSeaResourceYieldChange;
}

/// Free combat experience by unit combat type
int CvBuildingEntry::GetUnitCombatFreeExperience(int i) const
{
	ASSERT_DEBUG(i < GC.getNumUnitCombatClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piUnitCombatFreeExperience ? m_piUnitCombatFreeExperience[i] : -1;
}

/// Extra Production towards a unit combat type in this city
int CvBuildingEntry::GetUnitCombatProductionModifier(int i) const
{
	ASSERT_DEBUG(i < GC.getNumUnitCombatClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piUnitCombatProductionModifiers ? m_piUnitCombatProductionModifiers[i] : -1;
}

/// Extra Production towards a unit combat type in all cities
int CvBuildingEntry::GetUnitCombatProductionModifierGlobal(int i) const
{
	ASSERT_DEBUG(i < GC.getNumUnitCombatClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piUnitCombatProductionModifiersGlobal ? m_piUnitCombatProductionModifiersGlobal[i] : -1;
}

/// Free experience gained for units in this domain
int CvBuildingEntry::GetDomainFreeExperience(int i) const
{
	ASSERT_DEBUG(i < NUM_DOMAIN_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piDomainFreeExperience ? m_piDomainFreeExperience[i] : -1;
}

/// Free experience gained for units in this domain for each Great Work in this building
int CvBuildingEntry::GetDomainFreeExperiencePerGreatWork(int i) const
{
	ASSERT_DEBUG(i < NUM_DOMAIN_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piDomainFreeExperiencePerGreatWork ? m_piDomainFreeExperiencePerGreatWork[i] : -1;
}
#if defined(MOD_BALANCE_CORE)
/// Free experience gained for units in this domain for each Great Work in this building
int CvBuildingEntry::GetDomainFreeExperiencePerGreatWorkGlobal(int i) const
{
	ASSERT_DEBUG(i < NUM_DOMAIN_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piDomainFreeExperiencePerGreatWorkGlobal ? m_piDomainFreeExperiencePerGreatWorkGlobal[i] : -1;
}

/// Free experience gained for units in this domain (global)
int CvBuildingEntry::GetDomainFreeExperienceGlobal(int i) const
{
	ASSERT_DEBUG(i < NUM_DOMAIN_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	
	std::map<int, int>::const_iterator it = m_piDomainFreeExperienceGlobal.find(i);
	if (it != m_piDomainFreeExperienceGlobal.end()) // find returns the iterator to map::end if the key i is not present in the map
	{
		return it->second;
	}

	return 0;
}
#endif

/// Production modifier in this domain
int CvBuildingEntry::GetDomainProductionModifier(int i) const
{
	ASSERT_DEBUG(i < NUM_DOMAIN_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piDomainProductionModifier ? m_piDomainProductionModifier[i] : -1;
}

/// BuildingClasses that may no longer be constructed after this Building is built in a City
int CvBuildingEntry::GetLockedBuildingClasses(int i) const
{
	ASSERT_DEBUG(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piLockedBuildingClasses ? m_piLockedBuildingClasses[i] : -1;
}

/// Prerequisite techs with AND
int CvBuildingEntry::GetPrereqAndTechs(int i) const
{
	ASSERT_DEBUG(i < /*3*/ GD_INT_GET(NUM_BUILDING_AND_TECH_PREREQS), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piPrereqAndTechs ? m_piPrereqAndTechs[i] : -1;
}

/// Resources consumed to construct
int CvBuildingEntry::GetResourceQuantityRequirement(int i) const
{
	ASSERT_DEBUG(i < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piResourceQuantityRequirements ? m_piResourceQuantityRequirements[i] : -1;
}

/// Resources provided once constructed
int CvBuildingEntry::GetResourceQuantity(int i) const
{
	ASSERT_DEBUG(i < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piResourceQuantity ? m_piResourceQuantity[i] : -1;
}

/// Boost in Culture for each of these Resources
int CvBuildingEntry::GetResourceCultureChange(int i) const
{
	ASSERT_DEBUG(i < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piResourceCultureChanges ? m_piResourceCultureChanges[i] : -1;
}

/// Boost in Faith for each of these Resources
int CvBuildingEntry::GetResourceFaithChange(int i) const
{
	ASSERT_DEBUG(i < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piResourceFaithChanges ? m_piResourceFaithChanges[i] : -1;
}

/// Boost in production for leader with this trait
int CvBuildingEntry::GetProductionTraits(int i) const
{
	ASSERT_DEBUG(i < GC.getNumTraitInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piProductionTraits ? m_piProductionTraits[i] : 0;
}

/// Number of prerequisite buildings of a particular class
int CvBuildingEntry::GetPrereqNumOfBuildingClass(int i) const
{
	ASSERT_DEBUG(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piPrereqNumOfBuildingClass ? m_piPrereqNumOfBuildingClass[i] : -1;
}

/// Find value of flavors associated with this building
int CvBuildingEntry::GetFlavorValue(int i) const
{
	ASSERT_DEBUG(i < GC.getNumFlavorTypes(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piFlavorValue ? m_piFlavorValue[i] : 0;
}

/// Prerequisite resources with AND
uint CvBuildingEntry::GetLocalResourceAndSize() const
{
	return m_viLocalResourceAnds.size();
}
int CvBuildingEntry::GetLocalResourceAnd(uint ui) const
{
	ASSERT_DEBUG(ui < m_viLocalResourceAnds.size(), "Index out of bounds");
	return m_viLocalResourceAnds[ui];
}

/// Prerequisite resources with OR
uint CvBuildingEntry::GetLocalResourceOrSize() const
{
	return m_viLocalResourceOrs.size();
}
int CvBuildingEntry::GetLocalResourceOr(uint ui) const
{
	ASSERT_DEBUG(ui < m_viLocalResourceOrs.size(), "Index out of bounds");
	return m_viLocalResourceOrs[ui];
}

#if defined(MOD_BALANCE_CORE_RESOURCE_MONOPOLIES)
/// Prerequisite resources with AND
int CvBuildingEntry::GetFeatureAnd(int i) const
{
	ASSERT_DEBUG(i < GC.getNumFeatureInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piLocalFeatureAnds ? m_piLocalFeatureAnds[i] : -1;
}
/// Prerequisite resources with AND
int CvBuildingEntry::GetFeatureOr(int i) const
{
	ASSERT_DEBUG(i < GC.getNumFeatureInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piLocalFeatureOrs ? m_piLocalFeatureOrs[i] : -1;
}
/// Prerequisite resources with AND
uint CvBuildingEntry::GetResourceMonopolyAndSize() const
{
	return m_viResourceMonopolyAnds.size();
}
int CvBuildingEntry::GetResourceMonopolyAnd(uint ui) const
{
	ASSERT_DEBUG(ui < m_viResourceMonopolyAnds.size(), "Index out of bounds");
	return m_viResourceMonopolyAnds[ui];
}

/// Prerequisite resources with OR
uint CvBuildingEntry::GetResourceMonopolyOrSize() const
{
	return m_viResourceMonopolyOrs.size();
}
int CvBuildingEntry::GetResourceMonopolyOr(uint ui) const
{
	ASSERT_DEBUG(ui < m_viResourceMonopolyOrs.size(), "Index out of bounds");
	return m_viResourceMonopolyOrs[ui];
}
//Coroporation Stuff
int CvBuildingEntry::GetGPRateModifierPerXFranchises() const
{
	return m_iGPRateModifierPerXFranchises;
}
/// Resources provided once constructed
int CvBuildingEntry::GetResourceQuantityPerXFranchises(int i) const
{
	ASSERT_DEBUG(i < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piResourceQuantityPerXFranchises ? m_piResourceQuantityPerXFranchises[i] : -1;
}
int CvBuildingEntry::GetYieldChangePerMonopoly(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldChangePerMonopoly[i];
}
int CvBuildingEntry::GetYieldChangeFromPassingTR(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldChangeFromPassingTR[i];
}
int CvBuildingEntry::GetYieldPerFranchise(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldPerFranchise ? m_piYieldPerFranchise[i] : -1;
}
#endif
// Resource provided by Population
int CvBuildingEntry::GetResourceQuantityFromPOP(int i) const
{
	ASSERT_DEBUG(i < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piResourceQuantityFromPOP ? m_piResourceQuantityFromPOP[i] : -1;
}
/// Modifier to Hurry cost
int CvBuildingEntry::GetHurryModifier(int i) const
{
	ASSERT_DEBUG(i < GC.getNumHurryInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_paiHurryModifier ? m_paiHurryModifier[i] : -1;
}
#if defined(MOD_BALANCE_CORE)
/// Local Modifier to Hurry cost
int CvBuildingEntry::GetHurryModifierLocal(int i) const
{
	ASSERT_DEBUG(i < GC.getNumHurryInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_paiHurryModifierLocal ? m_paiHurryModifierLocal[i] : -1;
}
#endif

/// Can it only built if there is a building of this class in the city?
bool CvBuildingEntry::IsBuildingClassNeededInCity(int i) const
{
	ASSERT_DEBUG(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_pbBuildingClassNeededInCity ? m_pbBuildingClassNeededInCity[i] : false;
}
#if defined(MOD_BALANCE_CORE)
/// Can it only built if there is a building of this class in any owned city?
bool CvBuildingEntry::IsBuildingClassNeededAnywhere(int i) const
{
	ASSERT_DEBUG(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_pbBuildingClassNeededAnywhere ? m_pbBuildingClassNeededAnywhere[i] : false;
}
/// Can it only built if there is NOT a building of this class in any owned city?
bool CvBuildingEntry::IsBuildingClassNeededNowhere(int i) const
{
	ASSERT_DEBUG(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_pbBuildingClassNeededNowhere ? m_pbBuildingClassNeededNowhere[i] : false;
}
// Free units which appear near the capital, can be any UnitType of other civs or not
int CvBuildingEntry::GetNumFreeSpecialUnits(int i) const
{
	ASSERT_DEBUG(i < GC.getNumUnitInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piNumSpecFreeUnits ? m_piNumSpecFreeUnits[i] : -1;
}
/// Building grants resource plots when built
std::map<int, int> CvBuildingEntry::GetResourcePlotsToPlace(int i) const
{
	ASSERT_DEBUG(i < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");

	std::map<int, std::map<int, int>>::const_iterator it = m_ppiResourcePlotsToPlace.find(i);
	if (it != m_ppiResourcePlotsToPlace.end()) // find returns the iterator to map::end if the key bInternationalOnly is not present
	{
		return it->second;
	}

	std::map<int, int> piDefault;

	return piDefault;
}
/// Check if this building grants resource plots
bool CvBuildingEntry::IsResourcePlotsToPlace() const
{
	return !m_ppiResourcePlotsToPlace.empty();
}
int CvBuildingEntry::GetYieldPerFriend(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldPerFriend ? m_piYieldPerFriend[i] : -1;
}

int CvBuildingEntry::GetYieldPerAlly(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldPerAlly ? m_piYieldPerAlly[i] : -1;
}

int CvBuildingEntry::GetYieldChangeWorldWonder(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldChangeWorldWonder ? m_piYieldChangeWorldWonder[i] : 0;
}
int CvBuildingEntry::GetYieldChangeWorldWonderGlobal(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piYieldChangeWorldWonderGlobal ? m_piYieldChangeWorldWonderGlobal[i] : 0;
}
int CvBuildingEntry::GetLuxuryYieldChanges(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piLuxuryYieldChanges ? m_piLuxuryYieldChanges[i] : 0;
}
#endif
/// Free units which appear near the capital
int CvBuildingEntry::GetNumFreeUnits(int i) const
{
	ASSERT_DEBUG(i < GC.getNumUnitInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piNumFreeUnits ? m_piNumFreeUnits[i] : -1;
}

/// Does this building generate yields from other yields?
int CvBuildingEntry::GetYieldFromYield(int i, int j) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");

	if (m_paYieldFromYield == NULL || m_paYieldFromYield[0].GetValue() == 0)
	{
		return 0;
	}
	else
	{
		for (int iI = 0; iI < NUM_YIELD_TYPES; iI++)
		{
			for (int iK = 0; iK < NUM_YIELD_TYPES; iK++)
			{
				if (m_paYieldFromYield[iI].GetYieldIn() != i)
					continue;
				if (m_paYieldFromYield[iI].GetYieldOut() != j)
					continue;

				return m_paYieldFromYield[iI].GetValue();
			}
		}
	}

	return 0;
}

/// Does this building generate yields from other yields globally?
int CvBuildingEntry::GetYieldFromYieldGlobal(int i, int j) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");

	if (m_paYieldFromYieldGlobal == NULL || m_paYieldFromYieldGlobal[0].GetValue() == 0)
	{
		return 0;
	}
	else
	{
		for (int iI = 0; iI < NUM_YIELD_TYPES; iI++)
		{
			for (int iK = 0; iK < NUM_YIELD_TYPES; iK++)
			{
				if (m_paYieldFromYieldGlobal[iI].GetYieldIn() != i)
					continue;
				if (m_paYieldFromYieldGlobal[iI].GetYieldOut() != j)
					continue;

				return m_paYieldFromYieldGlobal[iI].GetValue();
			}
		}
	}

	return 0;
}

/// Change to Resource yield by type
int CvBuildingEntry::GetResourceYieldChange(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppaiResourceYieldChange ? m_ppaiResourceYieldChange[i][j] : -1;
}

/// Array of changes to Resource yield
int* CvBuildingEntry::GetResourceYieldChangeArray(int i) const
{
	ASSERT_DEBUG(i < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_ppaiResourceYieldChange[i];
}

#if defined(MOD_BALANCE_CORE)
/// Change to Resource yield by type
int CvBuildingEntry::GetResourceYieldChangeGlobal(int iResource, int iYieldType) const
{
	ASSERT_DEBUG(iResource < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(iResource > -1, "Index out of bounds");
	ASSERT_DEBUG(iYieldType < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(iYieldType > -1, "Index out of bounds");
	std::map<int, std::map<int, int>>::const_iterator itResource = m_ppiResourceYieldChangeGlobal.find(iResource);
	if (itResource != m_ppiResourceYieldChangeGlobal.end()) // find returns the iterator to map::end if the key iResource is not present in the map
	{
		std::map<int, int>::const_iterator itYield = itResource->second.find(iYieldType);
		if (itYield != itResource->second.end()) // find returns the iterator to map::end if the key iYield is not present in the map
		{
			return itYield->second;
		}
	}

	return 0;
}
#endif

std::map<int, std::map<int, int>> CvBuildingEntry::GetTechEnhancedYields() const
{
	return m_miTechEnhancedYields;
}

std::map<int, AccomplishmentBonusInfo> CvBuildingEntry::GetBonusFromAccomplishments() const
{
	return m_miBonusFromAccomplishments;
}

std::map<int, std::map<int, int>> CvBuildingEntry::GetYieldChangesFromAccomplishments() const
{
	return m_miYieldChangesFromAccomplishments;
}

std::map<pair<GreatPersonTypes, EraTypes>, int> CvBuildingEntry::GetGreatPersonPointFromConstruction() const
{
	return m_miGreatPersonPointFromConstruction;
}

/// Change to Feature yield by type
int CvBuildingEntry::GetFeatureYieldChange(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumFeatureInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppaiFeatureYieldChange ? m_ppaiFeatureYieldChange[i][j] : -1;
}

/// Array of changes to Feature yield
int* CvBuildingEntry::GetFeatureYieldChangeArray(int i) const
{
	ASSERT_DEBUG(i < GC.getNumFeatureInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_ppaiFeatureYieldChange[i];
}

#if defined(MOD_BALANCE_CORE)
/// Change to Improvement yield by type
int CvBuildingEntry::GetImprovementYieldChange(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumImprovementInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppaiImprovementYieldChange ? m_ppaiImprovementYieldChange[i][j] : -1;
}

/// Array of changes to Improvement yield
int* CvBuildingEntry::GetImprovementYieldChangeArray(int i) const
{
	ASSERT_DEBUG(i < GC.getNumImprovementInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_ppaiImprovementYieldChange[i];
}
/// Change to Improvement yield by type
int CvBuildingEntry::GetImprovementYieldChangeGlobal(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumImprovementInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppaiImprovementYieldChangeGlobal ? m_ppaiImprovementYieldChangeGlobal[i][j] : -1;
}

/// Array of changes to Improvement yield
int* CvBuildingEntry::GetImprovementYieldChangeGlobalArray(int i) const
{
	ASSERT_DEBUG(i < GC.getNumFeatureInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_ppaiImprovementYieldChangeGlobal[i];
}
/// Change to specialist yield by type
int CvBuildingEntry::GetSpecialistYieldChangeLocal(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumSpecialistInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppaiSpecialistYieldChangeLocal ? m_ppaiSpecialistYieldChangeLocal[i][j] : -1;
}
/// Array of changes to specialist yield
int* CvBuildingEntry::GetSpecialistYieldChangeLocalArray(int i) const
{
	ASSERT_DEBUG(i < GC.getNumSpecialistInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_ppaiSpecialistYieldChangeLocal[i];
}
#endif

/// Change to specialist yield by type
int CvBuildingEntry::GetSpecialistYieldChange(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumSpecialistInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppaiSpecialistYieldChange ? m_ppaiSpecialistYieldChange[i][j] : -1;
}

/// Array of changes to specialist yield
int* CvBuildingEntry::GetSpecialistYieldChangeArray(int i) const
{
	ASSERT_DEBUG(i < GC.getNumSpecialistInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_ppaiSpecialistYieldChange[i];
}

/// Modifier to resource yield
int CvBuildingEntry::GetResourceYieldModifier(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppaiResourceYieldModifier ? m_ppaiResourceYieldModifier[i][j] : -1;
}

/// Array of modifiers to resource yield
int* CvBuildingEntry::GetResourceYieldModifierArray(int i) const
{
	ASSERT_DEBUG(i < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_ppaiResourceYieldModifier[i];
}

/// Change to Terrain yield by type
int CvBuildingEntry::GetTerrainYieldChange(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumTerrainInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppaiTerrainYieldChange ? m_ppaiTerrainYieldChange[i][j] : -1;
}

/// Array of changes to Feature yield
int* CvBuildingEntry::GetTerrainYieldChangeArray(int i) const
{
	ASSERT_DEBUG(i < GC.getNumTerrainInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_ppaiTerrainYieldChange[i];
}

/// Change to Terrain yield by type
int CvBuildingEntry::GetYieldPerXTerrain(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumTerrainInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppaiYieldPerXTerrain ? m_ppaiYieldPerXTerrain[i][j] : -1;
}

/// Array of changes to Feature yield
int* CvBuildingEntry::GetYieldPerXTerrainArray(int i) const
{
	ASSERT_DEBUG(i < GC.getNumTerrainInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_ppaiYieldPerXTerrain[i];
}

int CvBuildingEntry::GetYieldPerXFeature(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumFeatureInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppaiYieldPerXFeature ? m_ppaiYieldPerXFeature[i][j] : -1;
}

/// Array of changes to Feature yield
int* CvBuildingEntry::GetYieldPerXFeatureArray(int i) const
{
	ASSERT_DEBUG(i < GC.getNumFeatureInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_ppaiYieldPerXFeature[i];
}

/// Change to Building Yield by Improvement Type
fraction CvBuildingEntry::GetYieldPerXImprovementLocal(ImprovementTypes eImprovementType, YieldTypes eYieldType) const
{
	ASSERT_DEBUG(eImprovementType < GC.getNumImprovementInfos(), "Index out of bounds");
	ASSERT_DEBUG(eImprovementType > -1, "Index out of bounds");
	ASSERT_DEBUG(eYieldType < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(eYieldType > -1, "Index out of bounds");
	std::map<ImprovementTypes, std::map<YieldTypes, fraction>>::const_iterator itImprovement = m_ppYieldPerXImprovementLocal.find(eImprovementType);
	if (itImprovement != m_ppYieldPerXImprovementLocal.end()) // find returns the iterator to map::end if the key eImprovementType is not present in the map
	{
		std::map<YieldTypes, fraction>::const_iterator itYield = itImprovement->second.find(eYieldType);
		if (itYield != itImprovement->second.end()) // find returns the iterator to map::end if the key eYieldType is not present in the map
		{
			return itYield->second;
		}
	}

	return fraction(0);
}
/// Change to Building Yield by Improvement Type (Global)
fraction CvBuildingEntry::GetYieldPerXImprovementGlobal(ImprovementTypes eImprovementType, YieldTypes eYieldType) const
{
	ASSERT_DEBUG(eImprovementType < GC.getNumImprovementInfos(), "Index out of bounds");
	ASSERT_DEBUG(eImprovementType > -1, "Index out of bounds");
	ASSERT_DEBUG(eYieldType < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(eYieldType > -1, "Index out of bounds");
	std::map<ImprovementTypes, std::map<YieldTypes, fraction>>::const_iterator itImprovement = m_ppYieldPerXImprovementGlobal.find(eImprovementType);
	if (itImprovement != m_ppYieldPerXImprovementGlobal.end()) // find returns the iterator to map::end if the key eImprovementType is not present in the map
	{
		std::map<YieldTypes, fraction>::const_iterator itYield = itImprovement->second.find(eYieldType);
		if (itYield != itImprovement->second.end()) // find returns the iterator to map::end if the key eYieldType is not present in the map
		{
			return itYield->second;
		}
	}

	return fraction(0);
}

/// Change to Plot yield by type
int CvBuildingEntry::GetPlotYieldChange(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumPlotInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppaiPlotYieldChange ? m_ppaiPlotYieldChange[i][j] : -1;
}

/// Array of changes to Plot yield
int* CvBuildingEntry::GetPlotYieldChangeArray(int i) const
{
	ASSERT_DEBUG(i < GC.getNumPlotInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_ppaiPlotYieldChange[i];
}

/// Yield change for a specific BuildingClass by yield type
int CvBuildingEntry::GetBuildingClassYieldChange(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppiBuildingClassYieldChanges[i][j];
}

/// Yield change for a specific BuildingClass by yield type
int CvBuildingEntry::GetBuildingClassYieldModifier(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppiBuildingClassYieldModifiers[i][j];
}
#if defined(MOD_BALANCE_CORE)
/// Yield change for a specific BuildingClass by yield type
int CvBuildingEntry::GetBuildingClassLocalYieldChange(int i, int j) const
{
	ASSERT_DEBUG(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	ASSERT_DEBUG(j < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(j > -1, "Index out of bounds");
	return m_ppiBuildingClassLocalYieldChanges[i][j];
}
/// Amount of extra Happiness per turn a BuildingClass provides
int CvBuildingEntry::GetBuildingClassLocalHappiness(int i) const
{
	ASSERT_DEBUG(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_paiBuildingClassLocalHappiness ? m_paiBuildingClassLocalHappiness[i] : -1;
}

int CvBuildingEntry::GetSpecificGreatPersonRateModifier(int i) const
{
	ASSERT_DEBUG(i < GC.getNumSpecialistInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_paiSpecificGreatPersonRateModifier ? m_paiSpecificGreatPersonRateModifier[i] : -1;
}
int CvBuildingEntry::GetResourceHappiness(int i) const
{
	ASSERT_DEBUG(i < GC.getNumResourceInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_paiResourceHappinessChange ? m_paiResourceHappinessChange[i] : -1;
}
#endif
/// Amount of extra Happiness per turn a BuildingClass provides
int CvBuildingEntry::GetBuildingClassHappiness(int i) const
{
	ASSERT_DEBUG(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_paiBuildingClassHappiness ? m_paiBuildingClassHappiness[i] : -1;
}
#if defined(MOD_BALANCE_CORE)
std::multimap<int, std::pair<int, int>> CvBuildingEntry::GetGreatPersonProgressFromConstructionArray() const
{
	return m_piiGreatPersonProgressFromConstruction;
}
#endif
#if defined(MOD_BALANCE_CORE)
int CvBuildingEntry::GetNumRequiredTier3Tenets() const
{
	return m_iNumRequiredTier3Tenets;
}

/// Does a city need to lack fresh water?
bool CvBuildingEntry::IsNoWater() const
{
	return m_bIsNoWater;
}
bool CvBuildingEntry::IsNoRiver() const
{
	return m_bIsNoRiver;
}
/// Does a city need to be away from the coast?
bool CvBuildingEntry::IsNoCoast() const
{
	return m_bIsNoCoast;
}
/// Does a city need to be the Capital?
bool CvBuildingEntry::IsCapitalOnly() const
{
	return m_bIsCapitalOnly;
}
/// Does this building enable a reformation?
bool CvBuildingEntry::IsReformation() const
{
	return m_bIsReformation;
}
/// Does this building boost religious spread via trade?
int CvBuildingEntry::GetReformationFollowerReduction() const
{
	return m_iReformationFollowerReduction;
}

/// Can this building ignore most restrictions?
bool CvBuildingEntry::IsBuildAnywhere() const
{
	return m_bBuildAnywhere;
}
/// Does this building boost religious spread via trade?
int CvBuildingEntry::GetTradeReligionModifier() const
{
	return m_iTradeReligionModifier;
}
// Creates free Artifacts for building, up to # indicated (or artifact slots in building).
int CvBuildingEntry::GetNumFreeArtifacts() const
{
	return m_iFreeArtifacts;
}

int CvBuildingEntry::GetResourceDiversityModifier() const
{
	return m_iResourceDiversityModifier;
}

int CvBuildingEntry::GetNoUnhappfromXSpecialists() const
{
	return m_iNoUnhappfromXSpecialists;
}
int CvBuildingEntry::GetNoUnhappfromXSpecialistsGlobal() const
{
	return m_iNoUnhappfromXSpecialistsGlobal;
}

int CvBuildingEntry::GetPurchaseCooldownReduction(bool bCivilian) const
{
	if (bCivilian)
		return m_iPurchaseCooldownReductionCivilian;

	return m_iPurchaseCooldownReduction;
}
#endif

int CvBuildingEntry::GetVassalLevyEra() const
{
	return m_iVassalLevyEra;
}

#if defined(MOD_BALANCE_CORE_POP_REQ_BUILDINGS)
int CvBuildingEntry::GetNationalPopulationRequired() const
{
	return m_iNationalPopRequired;
}
int CvBuildingEntry::GetLocalPopulationRequired() const
{
	return m_iLocalPopRequired;
}
#endif
#if defined(MOD_BALANCE_CORE_FOLLOWER_POP_WONDER)
int CvBuildingEntry::GetNationalFollowerPopRequired() const
{
	return m_iNationalFollowerPopRequired;
}
int CvBuildingEntry::GetGlobalFollowerPopRequired() const
{
	return m_iGlobalFollowerPopRequired;
}
#endif
#if defined(MOD_BALANCE_CORE_BUILDING_INSTANT_YIELD)
/// Instant yield
int CvBuildingEntry::GetInstantYield(int i) const
{
	ASSERT_DEBUG(i < NUM_YIELD_TYPES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");
	return m_piInstantYield ? m_piInstantYield[i] : -1;
}

/// Array of instant yields
int* CvBuildingEntry::GetInstantYieldArray() const
{
	return m_piInstantYield;
}
#endif

CvThemingBonusInfo *CvBuildingEntry::GetThemingBonusInfo(int i) const
{
	ASSERT_DEBUG(i < MAX_THEMING_BONUSES, "Index out of bounds");
	ASSERT_DEBUG(i > -1, "Index out of bounds");

	if (m_paThemingBonusInfo[0].m_iBonus == 0)
	{
		return NULL;
	}
	else
	{
		return &m_paThemingBonusInfo[i];
	}
}

void CvBuildingEntry::UpdateUnitTypesUnlocked()
{
	m_vUnitTypesUnlocked.clear();
	for (int i = 0; i < GC.getNumUnitInfos(); i++)
	{
		const UnitTypes eUnit = static_cast<UnitTypes>(i);
		CvUnitEntry* pkUnitInfo = GC.getUnitInfo(eUnit);
		if (pkUnitInfo && pkUnitInfo->GetBuildingClassRequireds(GetBuildingClassType()))
			m_vUnitTypesUnlocked.push_back(eUnit);
	}
}

bool CvBuildingEntry::IsFaithPurchaseOnly() const
{
	if (GetFaithCost() > 0 && GetProductionCost() <= -1)
	{
		if (IsUnlockedByBelief() || GetPolicyType() != NO_POLICY)
			return true;
	}

	return false;
}

//=====================================
// CvBuildingXMLEntries
//=====================================
/// Constructor
CvBuildingXMLEntries::CvBuildingXMLEntries(void)
{

}

/// Destructor
CvBuildingXMLEntries::~CvBuildingXMLEntries(void)
{
	DeleteArray();
}

/// Returns vector of policy entries
std::vector<CvBuildingEntry*>& CvBuildingXMLEntries::GetBuildingEntries()
{
	return m_paBuildingEntries;
}

/// Number of defined policies
int CvBuildingXMLEntries::GetNumBuildings()
{
	return m_paBuildingEntries.size();
}

/// Clear policy entries
void CvBuildingXMLEntries::DeleteArray()
{
	for(std::vector<CvBuildingEntry*>::iterator it = m_paBuildingEntries.begin(); it != m_paBuildingEntries.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	m_paBuildingEntries.clear();
}

/// Get a specific entry
CvBuildingEntry* CvBuildingXMLEntries::GetEntry(int index) const
{
	return (index!=NO_BUILDING) ? m_paBuildingEntries[index] : NULL;
}

//=====================================
// CvCityBuildings
//=====================================
/// Constructor
CvCityBuildings::CvCityBuildings():
	m_paiBuildingProduction(NULL),
	m_paiBuildingProductionTime(NULL),
	m_paiBuildingOriginalOwner(NULL),
	m_paiBuildingOriginalTime(NULL),
	m_paiNumRealBuilding(NULL),
	m_paiNumFreeBuilding(NULL),
#if defined(MOD_BALANCE_CORE)
	m_paiFirstTimeBuilding(NULL),
	m_paiThemingBonusIndex(NULL),
#endif
	m_iNumBuildings(0),
	m_iBuildingProductionModifier(0),
	m_iBuildingProductionModifierPotentialFromMinorTrade(0),
	m_iBuildingDefense(0),
	m_iBuildingDefenseMod(0),
	m_iMissionaryExtraSpreads(0),
	m_iLandmarksTourismPercent(0),
	m_iGreatWorksTourismModifier(0),
	m_bSoldBuildingThisTurn(false),
	m_pPossibleBuildings(NULL),
	m_pCity(NULL)
{
}

/// Destructor
CvCityBuildings::~CvCityBuildings(void)
{
}

/// Initialize
void CvCityBuildings::Init(CvBuildingXMLEntries* pPossibleBuildings, CvCity* pCity)
{
	// Store off the pointers to objects we'll need later
	m_pPossibleBuildings = pPossibleBuildings;
	m_pCity = pCity;

	// Initialize status arrays

	int iNumBuildings = pPossibleBuildings->GetNumBuildings();

	ASSERT_DEBUG((0 < iNumBuildings),  "m_pBuildings->GetNumBuildings() is not greater than zero but an array is being allocated in CvCityBuildings::Init");

	ASSERT_DEBUG(m_paiBuildingProduction==NULL, "about to leak memory, CvCityBuildings::m_paiBuildingProduction");
	m_paiBuildingProduction = FNEW(int[iNumBuildings], c_eCiv5GameplayDLL, 0);

	ASSERT_DEBUG(m_paiBuildingProductionTime==NULL, "about to leak memory, CvCityBuildings::m_paiBuildingProductionTime");
	m_paiBuildingProductionTime = FNEW(int[iNumBuildings], c_eCiv5GameplayDLL, 0);

	ASSERT_DEBUG(m_paiBuildingOriginalOwner==NULL, "about to leak memory, CvCityBuildings::m_paiBuildingOriginalOwner");
	m_paiBuildingOriginalOwner = FNEW(int[iNumBuildings], c_eCiv5GameplayDLL, 0);

	ASSERT_DEBUG(m_paiBuildingOriginalTime==NULL, "about to leak memory, CvCityBuildings::m_paiBuildingOriginalTime");
	m_paiBuildingOriginalTime = FNEW(int[iNumBuildings], c_eCiv5GameplayDLL, 0);

	ASSERT_DEBUG(m_paiNumRealBuilding==NULL, "about to leak memory, CvCityBuildings::m_paiNumRealBuilding");
	m_paiNumRealBuilding = FNEW(int[iNumBuildings], c_eCiv5GameplayDLL, 0);

	ASSERT_DEBUG(m_paiNumFreeBuilding==NULL, "about to leak memory, CvCityBuildings::m_paiNumFreeBuilding");
	m_paiNumFreeBuilding = FNEW(int[iNumBuildings], c_eCiv5GameplayDLL, 0);

#if defined(MOD_BALANCE_CORE)
	ASSERT_DEBUG(m_paiFirstTimeBuilding==NULL, "about to leak memory, CvCityBuildings::m_paiFirstTimeBuilding");
	m_paiFirstTimeBuilding = FNEW(int[iNumBuildings], c_eCiv5GameplayDLL, 0);

	ASSERT_DEBUG(m_paiThemingBonusIndex == NULL, "about to leak memory, CvCityBuildings::m_paiThemingBonusIndex");
	m_paiThemingBonusIndex = FNEW(int[iNumBuildings], c_eCiv5GameplayDLL, 0);
#endif

	m_aBuildingYieldChange.clear();
	m_aBuildingGreatWork.clear();

	Reset();
}

void CvCityBuildings::SetBuildingTypeByClassDirty(bool dirty) const
{
	b_existBuildingsAreDirty = dirty;
}

void CvCityBuildings::CacheBuildingTypeByClass() const
{
	if (!b_existBuildingsAreDirty)
		return;

	m_buildingTypeByClass.clear();

	for (int i = 0; i < GC.getNumBuildingClassInfos(); i++)
	{
		for (std::vector<BuildingTypes>::const_iterator iI = m_buildingsThatExistAtLeastOnce.begin(); iI != m_buildingsThatExistAtLeastOnce.end(); ++iI)
		{
			CvBuildingEntry* pkInfo = GC.getBuildingInfo(*iI);
			if (pkInfo && pkInfo->GetBuildingClassType() == i && GetNumBuilding(*iI) > 0)
			{
				m_buildingTypeByClass.insert(std::make_pair((BuildingClassTypes) i, (BuildingTypes) *iI));
			}
		}
	}

	SetBuildingTypeByClassDirty(false);
}

/// Deallocate memory created in initialize
void CvCityBuildings::Uninit()
{
	SAFE_DELETE_ARRAY(m_paiBuildingProduction);
	SAFE_DELETE_ARRAY(m_paiBuildingProductionTime);
	SAFE_DELETE_ARRAY(m_paiBuildingOriginalOwner);
	SAFE_DELETE_ARRAY(m_paiBuildingOriginalTime);
	SAFE_DELETE_ARRAY(m_paiNumRealBuilding);
	SAFE_DELETE_ARRAY(m_paiNumFreeBuilding);
#if defined(MOD_BALANCE_CORE)
	SAFE_DELETE_ARRAY(m_paiFirstTimeBuilding);
	SAFE_DELETE_ARRAY(m_paiThemingBonusIndex);
#endif
}

/// Reset status arrays to all false
void CvCityBuildings::Reset()
{
	int iI = 0;

	// Initialize non-arrays
	m_iNumBuildings = 0;
	m_iBuildingProductionModifier = 0;
	m_iBuildingProductionModifierPotentialFromMinorTrade = 0;
	m_iBuildingDefense = 0;
	m_iBuildingDefenseMod = 0;
	m_iMissionaryExtraSpreads = 0;
	m_iLandmarksTourismPercent = 0;
	m_iGreatWorksTourismModifier = 0;

	m_bSoldBuildingThisTurn = false;

	for(iI = 0; iI < m_pPossibleBuildings->GetNumBuildings(); iI++)
	{
		m_paiBuildingProduction[iI] = 0;
		m_paiBuildingProductionTime[iI] = 0;
		m_paiBuildingOriginalOwner[iI] = NO_PLAYER;
		m_paiBuildingOriginalTime[iI] = MIN_INT;
		m_paiNumRealBuilding[iI] = 0;
		m_paiNumFreeBuilding[iI] = 0;
#if defined(MOD_BALANCE_CORE)
		m_paiFirstTimeBuilding[iI] = 0;
		m_paiThemingBonusIndex[iI] = -1;
#endif
	}

#if defined(MOD_BALANCE_CORE)
	m_buildingsThatExistAtLeastOnce.clear();
	m_buildingTypeByClass.clear();
#endif

}

template<typename CityBuildings, typename Visitor>
void CvCityBuildings::Serialize(CityBuildings& cityBuildings, Visitor& visitor)
{
	visitor(cityBuildings.m_iNumBuildings);
	visitor(cityBuildings.m_iBuildingProductionModifier);
	visitor(cityBuildings.m_iBuildingProductionModifierPotentialFromMinorTrade);
	visitor(cityBuildings.m_iBuildingDefense);
	visitor(cityBuildings.m_iBuildingDefenseMod);
	visitor(cityBuildings.m_iMissionaryExtraSpreads);
	visitor(cityBuildings.m_iLandmarksTourismPercent);
	visitor(cityBuildings.m_iGreatWorksTourismModifier);

	visitor(cityBuildings.m_bSoldBuildingThisTurn);

	int iNumBuildings = cityBuildings.m_pPossibleBuildings->GetNumBuildings();
	visitor(MakeConstSpan(cityBuildings.m_paiBuildingProduction, iNumBuildings));
	visitor(MakeConstSpan(cityBuildings.m_paiBuildingProductionTime, iNumBuildings));
	visitor(MakeConstSpan(cityBuildings.m_paiBuildingOriginalOwner, iNumBuildings));
	visitor(MakeConstSpan(cityBuildings.m_paiBuildingOriginalTime, iNumBuildings));
	visitor(MakeConstSpan(cityBuildings.m_paiNumRealBuilding, iNumBuildings));
	visitor(MakeConstSpan(cityBuildings.m_paiNumFreeBuilding, iNumBuildings));
	visitor(MakeConstSpan(cityBuildings.m_paiFirstTimeBuilding, iNumBuildings));
	visitor(MakeConstSpan(cityBuildings.m_paiThemingBonusIndex, iNumBuildings));

	visitor(cityBuildings.m_aBuildingYieldChange);
	visitor(cityBuildings.m_aBuildingGreatWork);
}

/// Serialization read
void CvCityBuildings::Read(FDataStream& kStream)
{
	ASSERT_DEBUG(GC.getNumBuildingInfos() > 0, "Number of buildings to serialize is expected to greater than 0");

	CvStreamLoadVisitor serialVisitor(kStream);
	Serialize(*this, serialVisitor);

	for (int i=0; i<m_pPossibleBuildings->GetNumBuildings(); i++)
		if (m_paiNumRealBuilding[i]>0 || m_paiNumFreeBuilding[i]>0)
			m_buildingsThatExistAtLeastOnce.push_back((BuildingTypes)i);

	SetBuildingTypeByClassDirty(true);
}

/// Serialization write
void CvCityBuildings::Write(FDataStream& kStream) const
{
	ASSERT_DEBUG(GC.getNumBuildingInfos() > 0, "Number of buildings to serialize is expected to greater than 0");

	CvStreamSaveVisitor serialVisitor(kStream);
	Serialize(*this, serialVisitor);
}

FDataStream& operator>>(FDataStream& stream, CvCityBuildings& cityBuildings)
{
	cityBuildings.Read(stream);
	return stream;
}
FDataStream& operator<<(FDataStream& stream, const CvCityBuildings& cityBuildings)
{
	cityBuildings.Write(stream);
	return stream;
}

/// Accessor: Get full array of all building XML data
CvBuildingXMLEntries* CvCityBuildings::GetPossibleBuildings() const
{
	return m_pPossibleBuildings;
}

/// Accessor: Total number of buildings in the city
int CvCityBuildings::GetNumBuildings() const
{
	return m_iNumBuildings;
}

/// Accessor: Update total number of buildings in the city
void CvCityBuildings::ChangeNumBuildings(int iChange)
{
	m_iNumBuildings = (m_iNumBuildings + iChange);
	ASSERT_DEBUG(GetNumBuildings() >= 0);

//	GET_PLAYER(m_pCity->getOwner()).updateNumResourceUsed();
}

/// Accessor: How many of these buildings in the city?
int CvCityBuildings::GetNumBuilding(BuildingTypes eIndex) const
{
	ASSERT_DEBUG(eIndex != NO_BUILDING, "BuildingType eIndex is expected to not be NO_BUILDING");

	if (/*1*/ GD_INT_GET(CITY_MAX_NUM_BUILDINGS) <= 1)
	{
		return std::max(GetNumRealBuilding(eIndex), GetNumFreeBuilding(eIndex));
	}
	else
	{
		return (GetNumRealBuilding(eIndex) + GetNumFreeBuilding(eIndex));
	}
}

/// Accessor: How many of these building classes in the city?
int CvCityBuildings::GetNumBuildingClass(BuildingClassTypes eIndex) const
{
	ASSERT_DEBUG(eIndex != NO_BUILDINGCLASS, "BuildingClassTypes eIndex is expected to not be NO_BUILDINGCLASS");

	int iValue = 0;
	for (std::vector<BuildingTypes>::const_iterator iI=m_buildingsThatExistAtLeastOnce.begin(); iI!=m_buildingsThatExistAtLeastOnce.end(); ++iI)
	{
		CvBuildingEntry *pkInfo = GC.getBuildingInfo(*iI);
		if(pkInfo && pkInfo->GetBuildingClassType() == eIndex)
		{
			int iCount = MAX(GetNumBuilding(*iI), 0);
			iValue += iCount;
		}
	}
	return iValue;
}

/// Accessor: Is there at least one building of the class in the city? Potentially faster function than the above.
bool CvCityBuildings::HasBuildingClass(BuildingClassTypes eIndex) const
{
	ASSERT_DEBUG(eIndex != NO_BUILDINGCLASS, "BuildingClassTypes eIndex is expected to not be NO_BUILDINGCLASS");

	for (std::vector<BuildingTypes>::const_iterator iI = m_buildingsThatExistAtLeastOnce.begin(); iI != m_buildingsThatExistAtLeastOnce.end(); ++iI)
	{
		CvBuildingEntry* pkInfo = GC.getBuildingInfo(*iI);
		if (pkInfo && pkInfo->GetBuildingClassType() == eIndex && GetNumBuilding(*iI) > 0)
		{
			return true;
		}
	}
	return false;
}

/// Accessor: What BuildingType do we have in our city for the specified class? This function assumes at most 1 building type will exist in a city for each class.
BuildingTypes CvCityBuildings::GetBuildingTypeFromClass(BuildingClassTypes eIndex) const
{
	ASSERT_DEBUG(eIndex != NO_BUILDINGCLASS, "BuildingClassTypes eIndex is expected to not be NO_BUILDINGCLASS");

	CacheBuildingTypeByClass();
	std::map<BuildingClassTypes, BuildingTypes>::iterator it = m_buildingTypeByClass.find(eIndex);
	if (it != m_buildingTypeByClass.end()) {
		return it->second;
	}
	else {
		return NO_BUILDING;
	}
}

/// Accessor: Removes all real buildings which belong to the specified building class.
void CvCityBuildings::RemoveAllRealBuildingsOfClass(BuildingClassTypes eIndex)
{
	ASSERT_DEBUG(eIndex != NO_BUILDINGCLASS, "BuildingClassTypes eIndex is expected to not be NO_BUILDINGCLASS");

	// Shortcut
	if (!MOD_BUILDINGS_THOROUGH_PREREQUISITES)
	{
		BuildingTypes eBuilding = static_cast<BuildingTypes>(GET_PLAYER(m_pCity->getOwner()).getCivilizationInfo().getCivilizationBuildings(eIndex));
		if (eBuilding != NO_BUILDING)
			SetNumRealBuilding(eBuilding, 0);

		return;
	}

	std::vector<BuildingTypes> aBuildingsToDelete;
	for (std::vector<BuildingTypes>::const_iterator it = m_buildingsThatExistAtLeastOnce.begin(); it != m_buildingsThatExistAtLeastOnce.end(); ++it)
	{
		CvBuildingEntry* pkInfo = GC.getBuildingInfo(*it);
		if (pkInfo && pkInfo->GetBuildingClassType() == eIndex && GetNumRealBuilding(*it) > 0)
		{
			aBuildingsToDelete.push_back(*it);
		}
	}

	for (std::vector<BuildingTypes>::const_iterator it = aBuildingsToDelete.begin(); it != aBuildingsToDelete.end(); ++it)
	{
		SetNumRealBuilding(*it, 0);
	}
}

/// Accessor: How many of these buildings are not obsolete?
int CvCityBuildings::GetNumActiveBuilding(BuildingTypes eIndex) const
{
	ASSERT_DEBUG(eIndex != NO_BUILDING, "BuildingType eIndex is expected to not be NO_BUILDING");

	if(GET_TEAM(m_pCity->getTeam()).isObsoleteBuilding(eIndex))
	{
		return 0;
	}

	return (GetNumBuilding(eIndex));
}

/// Is the player allowed to sell building eIndex in this city?
bool CvCityBuildings::IsBuildingSellable(const CvBuildingEntry& kBuilding) const
{
	if (m_pCity->IsResistance())
		return false;

	// Can't sell more than one building per turn
	if(IsSoldBuildingThisTurn())
		return false;

	// Venice can't sell any buildings except in their capital (check is needed because Venice can raze cities)
	if (!m_pCity->isCapital() && GET_PLAYER(m_pCity->getOwner()).GetPlayerTraits()->IsNoAnnexing())
		return false;

	// Can't sell in puppet cities
	if (m_pCity->IsPuppet())
		return false;

	// Can't sell a building if it doesn't cost us anything (no exploits)
	if(kBuilding.GetGoldMaintenance() <= 0)
		return false;

	// Is this a free building?
	if(GetNumFreeBuilding((BuildingTypes)kBuilding.GetID()) > 0)
		return false;

#if defined(MOD_BALANCE_CORE)
	// prevent exploits - can't sell in damaged cities
	if (m_pCity->getDamage() > 0)
		return false;

	//Spawns a permanent resource? Can't sell.
	if(kBuilding.GrantsRandomResourceTerritory())
	{
		return false;
	}
	if (kBuilding.IsResourcePlotsToPlace())
	{
		return false;
	}
	// Is a Corporation building?
	if (kBuilding.GetBuildingClassInfo().getCorporationType() != NO_CORPORATION)
	{
		return false;
	}
	if (kBuilding.GetWLTKDTurns() > 0 || kBuilding.IsGoldenAge() || kBuilding.GetPopulationChange() != 0)
		return false;

	for (int iYieldLoop = 0; iYieldLoop < NUM_YIELD_TYPES; iYieldLoop++)
	{
		if ((YieldTypes)iYieldLoop == NO_YIELD)
			continue;

		if (kBuilding.GetInstantYield((YieldTypes)iYieldLoop) > 0)
			return false;

		if (kBuilding.GetYieldFromBirthRetroactive((YieldTypes)iYieldLoop) > 0)
			return false;
	}
#endif

	// Science building in capital that has given us a tech boost?
	if(m_pCity->isCapital() && kBuilding.IsScienceBuilding())
	{
		return !(GET_PLAYER(m_pCity->getOwner()).GetPlayerTraits()->IsTechBoostFromCapitalScienceBuildings());
	}

	// Great Work present in this one?
	const BuildingClassTypes buildingClassType = kBuilding.GetBuildingClassType();
	if (IsHoldingGreatWork(buildingClassType))
	{
		return false;
	}

	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if(pkScriptSystem)
	{
		CvLuaArgsHandle args;
		args->Push(m_pCity->getOwner());
		args->Push(kBuilding.GetID());

		// Attempt to execute the game events.
		// Will return false if there are no registered listeners.
		bool bResult = false;
		if(LuaSupport::CallTestAll(pkScriptSystem, "CityBuildingsIsBuildingSellable", args.get(), bResult))
		{
			// Check the result.
			if(!bResult)
			{
				return false;
			}
		}
	}

	return true;
}

/// Sell eIndex~!
void CvCityBuildings::DoSellBuilding(BuildingTypes eIndex)
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");

	CvBuildingEntry* pkBuildingEntry = GC.getBuildingInfo(eIndex);
	if(!pkBuildingEntry)
		return;

	// Can we actually do this?
	if(!IsBuildingSellable(*pkBuildingEntry))
		return;

	// Gold refund
	int iRefund = GetSellBuildingRefund(eIndex);
	GET_PLAYER(m_pCity->getOwner()).GetTreasury()->ChangeGold(iRefund);

	// Kick everyone out
	m_pCity->GetCityCitizens()->DoRemoveAllSpecialistsFromBuilding(eIndex, CvCity::YIELD_UPDATE_GLOBAL);

	SetNumRealBuilding(eIndex, 0);

	SetSoldBuildingThisTurn(true);
}

/// How much of a refund will the player get from selling eIndex?
int CvCityBuildings::GetSellBuildingRefund(BuildingTypes eIndex) const
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");

	int iRefund = GET_PLAYER(m_pCity->getOwner()).getProductionNeeded(eIndex);
	iRefund /= /*4*/ GD_INT_GET(BUILDING_SALE_DIVISOR);

	return iRefund;
}

/// Has a building already been sold this turn?
bool CvCityBuildings::IsSoldBuildingThisTurn() const
{
	return m_bSoldBuildingThisTurn;
}

/// Has a building already been sold this turn?
void CvCityBuildings::SetSoldBuildingThisTurn(bool bValue)
{
	if(IsSoldBuildingThisTurn() != bValue)
		m_bSoldBuildingThisTurn = bValue;
}

/// What is the total maintenance? (no modifiers)
int CvCityBuildings::GetTotalBaseBuildingMaintenance() const
{
	int iTotalCost = 0;

	for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
	{
		const BuildingTypes eBuilding = static_cast<BuildingTypes>(iBuildingLoop);
		CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);

		if(pkBuildingInfo)
		{
			if(GetNumBuilding(eBuilding))
				iTotalCost += (pkBuildingInfo->GetGoldMaintenance() * GetNumBuilding(eBuilding));
		}
	}

	for (int iProjectLoop = 0; iProjectLoop < GC.getNumProjectInfos(); iProjectLoop++)
	{
		const ProjectTypes eProject = static_cast<ProjectTypes>(iProjectLoop);
		CvProjectEntry* pkProject = GC.getProjectInfo(eProject);

		if (pkProject && pkProject->GetGoldMaintenance() > 0)
		{
			iTotalCost += (m_pCity->getProjectCount(eProject) * pkProject->GetGoldMaintenance());
		}
	}

	return iTotalCost;
}

/// Accessor: How far is construction of this building?
int CvCityBuildings::GetBuildingProduction(BuildingTypes eIndex)	const
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");
	return m_paiBuildingProduction[eIndex] / 100;
}

/// Accessor: How far is construction of this building? (in hundredths)
int CvCityBuildings::GetBuildingProductionTimes100(BuildingTypes eIndex)	const
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");
	return m_paiBuildingProduction[eIndex];
}

/// Accessor: Set how much construction is complete for this building
void CvCityBuildings::SetBuildingProduction(BuildingTypes eIndex, int iNewValue)
{
	SetBuildingProductionTimes100(eIndex, iNewValue*100);
}

/// Accessor: Set how much construction is complete for this building (in hundredths)
void CvCityBuildings::SetBuildingProductionTimes100(BuildingTypes eIndex, int iNewValue)
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(), "eIndex expected to be < getNumBuildingInfos())");

	if(GetBuildingProductionTimes100(eIndex) != iNewValue)
	{
		if(GetBuildingProductionTimes100(eIndex) == 0)
		{
			NotifyNewBuildingStarted(eIndex);
		}

		m_paiBuildingProduction[eIndex] = max(0,iNewValue);
		ASSERT_DEBUG(GetBuildingProductionTimes100(eIndex) >= 0);

		if((m_pCity->getOwner() == GC.getGame().getActivePlayer()) && m_pCity->isCitySelected())
		{
			GC.GetEngineUserInterface()->setDirty(CityScreen_DIRTY_BIT, true);
		}

		CvInterfacePtr<ICvCity1> pCity = GC.WrapCityPointer(m_pCity);

		GC.GetEngineUserInterface()->SetSpecificCityInfoDirty(pCity.get(), CITY_UPDATE_TYPE_BANNER);
	}
}

/// Accessor: Update construction progress for this building
void CvCityBuildings::ChangeBuildingProduction(BuildingTypes eIndex, int iChange)
{
	ChangeBuildingProductionTimes100(eIndex, iChange*100);
}

/// Accessor: Update construction progress for this building (in hundredths)
void CvCityBuildings::ChangeBuildingProductionTimes100(BuildingTypes eIndex, int iChange)
{
	SetBuildingProductionTimes100(eIndex, (GetBuildingProductionTimes100(eIndex) + iChange));
}

/// Accessor: How many turns has this building been under production?
int CvCityBuildings::GetBuildingProductionTime(BuildingTypes eIndex)	const
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");
	return m_paiBuildingProductionTime[eIndex];
}

/// Accessor: Set number of turns this building been under production
void CvCityBuildings::SetBuildingProductionTime(BuildingTypes eIndex, int iNewValue)
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");
	m_paiBuildingProductionTime[eIndex] = iNewValue;
	ASSERT_DEBUG(GetBuildingProductionTime(eIndex) >= 0);
}

/// Accessor: Change number of turns this building been under production
void CvCityBuildings::ChangeBuildingProductionTime(BuildingTypes eIndex, int iChange)
{
	SetBuildingProductionTime(eIndex, (GetBuildingProductionTime(eIndex) + iChange));
}

/// Accessor: Who owned the city when this building was built?
int CvCityBuildings::GetBuildingOriginalOwner(BuildingTypes eIndex) const
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");
	return m_paiBuildingOriginalOwner[eIndex];
}

/// Accessor: Set who owned the city when this building was built
void CvCityBuildings::SetBuildingOriginalOwner(BuildingTypes eIndex, int iNewValue)
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");
	m_paiBuildingOriginalOwner[eIndex] = iNewValue;
}

/// Accessor: What year was this building built?
int CvCityBuildings::GetBuildingOriginalTime(BuildingTypes eIndex) const
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");
	return m_paiBuildingOriginalTime[eIndex];
}

/// Accessor: Set year building was built
void CvCityBuildings::SetBuildingOriginalTime(BuildingTypes eIndex, int iNewValue)
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");
	m_paiBuildingOriginalTime[eIndex] = iNewValue;
}

/// Accessor: How many of these buildings have been constructed in the city?
int CvCityBuildings::GetNumRealBuilding(BuildingTypes eIndex) const
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");
	return m_paiNumRealBuilding[eIndex];
}

/// Accessor: Set number of these buildings that have been constructed in the city
void CvCityBuildings::SetNumRealBuilding(BuildingTypes eIndex, int iNewValue, bool bNoBonus)
{
	SetNumRealBuildingTimed(eIndex, iNewValue, true, m_pCity->getOwner(), GC.getGame().getGameTurnYear(), bNoBonus);
}

/// Accessor: Set number of these buildings that have been constructed in the city (with date)
void CvCityBuildings::SetNumRealBuildingTimed(BuildingTypes eIndex, int iNewValue, bool bFirst, PlayerTypes eOriginalOwner, int iOriginalTime, bool bNoBonus)
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");

	CvPlayer& kPlayer = GET_PLAYER(m_pCity->getOwner());

	int iChangeNumRealBuilding = iNewValue - GetNumRealBuilding(eIndex);
	if (iChangeNumRealBuilding == 0)
		return;

	CvBuildingEntry* buildingEntry = GC.getBuildingInfo(eIndex);
	if (!buildingEntry)
		return;

	const BuildingClassTypes buildingClassType = buildingEntry->GetBuildingClassType();
	const CvBuildingClassInfo& kBuildingClassInfo = buildingEntry->GetBuildingClassInfo();

	int iOldNumBuilding = GetNumBuilding(eIndex);

	if (iNewValue == 0)
	{
		m_pCity->SetBuildingInvestment(buildingClassType, false);
		m_pCity->GetCityCitizens()->DoRemoveAllSpecialistsFromBuilding(eIndex, CvCity::YIELD_UPDATE_GLOBAL);
	}

	m_paiNumRealBuilding[eIndex] = iNewValue;

	if (iNewValue > 0)
	{
		if (std::find(m_buildingsThatExistAtLeastOnce.begin(), m_buildingsThatExistAtLeastOnce.end(), eIndex) == m_buildingsThatExistAtLeastOnce.end())
			m_buildingsThatExistAtLeastOnce.push_back(eIndex);

		SetBuildingTypeByClassDirty(true);
	}
	else if (GetNumFreeBuilding(eIndex)==0)
	{
		//we care about iteration speed, so erasing something can be cumbersome
		std::vector<BuildingTypes>::iterator pos = std::find(m_buildingsThatExistAtLeastOnce.begin(), m_buildingsThatExistAtLeastOnce.end(), eIndex );
		if (pos != m_buildingsThatExistAtLeastOnce.end())
			m_buildingsThatExistAtLeastOnce.erase(pos);

		SetBuildingTypeByClassDirty(true);
	}

	if(GetNumRealBuilding(eIndex) > 0)
	{
		SetBuildingOriginalOwner(eIndex, eOriginalOwner);
		SetBuildingOriginalTime(eIndex, iOriginalTime);
	}
	else
	{
		SetBuildingOriginalOwner(eIndex, NO_PLAYER);
		SetBuildingOriginalTime(eIndex, MIN_INT);
	}

	// Process building effects
	if(iOldNumBuilding != GetNumBuilding(eIndex))
	{
		m_pCity->processBuilding(eIndex, iChangeNumRealBuilding, bFirst, false, false, bNoBonus);
	}

	// Maintenance cost
	if(buildingEntry->GetGoldMaintenance() != 0)
	{
		kPlayer.GetTreasury()->ChangeBaseBuildingGoldMaintenance(buildingEntry->GetGoldMaintenance() * iChangeNumRealBuilding);
	}

	//Achievement for Temples
	if (MOD_API_ACHIEVEMENTS)
	{
		const char* szBuildingTypeC = buildingEntry->GetType();
		CvString szBuildingType = szBuildingTypeC;
		if (szBuildingType == "BUILDING_TEMPLE")
		{
			if (m_pCity->getOwner() == GC.getGame().getActivePlayer())
			{
				gDLL->IncrementSteamStatAndUnlock(ESTEAMSTAT_TEMPLES, 1000, ACHIEVEMENT_1000TEMPLES);
			}
		}
	}

	if(buildingEntry->GetPreferredDisplayPosition() > 0)
	{
		CvInterfacePtr<ICvCity1> pDllCity(new CvDllCity(m_pCity));

		if(iNewValue > 0)
		{
			// if this is a WW that (likely has a half-built state)
			if(isWorldWonderClass(kBuildingClassInfo))
			{
				if(GetBuildingProduction(eIndex))
				{
					GC.GetEngineUserInterface()->AddDeferredWonderCommand(WONDER_EDITED, pDllCity.get(), eIndex, 1);
				}
				else
				{
					GC.GetEngineUserInterface()->AddDeferredWonderCommand(WONDER_CREATED, pDllCity.get(), eIndex, 1);
				}
			}
			else
			{
				GC.GetEngineUserInterface()->AddDeferredWonderCommand(WONDER_CREATED, pDllCity.get(), eIndex, 1);
			}
		}
		else
		{
			GC.GetEngineUserInterface()->AddDeferredWonderCommand(WONDER_REMOVED, pDllCity.get(), eIndex, 0);
		}
	}

	if(!(kBuildingClassInfo.isNoLimit()))
	{
		if(isWorldWonderClass(kBuildingClassInfo))
		{
			m_pCity->changeNumWorldWonders(iChangeNumRealBuilding);
			kPlayer.ChangeNumWonders(iChangeNumRealBuilding);
		}
		else if(isTeamWonderClass(kBuildingClassInfo))
		{
			m_pCity->changeNumTeamWonders(iChangeNumRealBuilding);
		}
		else if(isNationalWonderClass(kBuildingClassInfo))
		{
			m_pCity->changeNumNationalWonders(iChangeNumRealBuilding);
			if(m_pCity->isHuman() && !GC.getGame().isGameMultiPlayer())
			{
				IncrementWonderStats(buildingClassType);
			}
		}
		else
		{
			if(!buildingEntry->IsDummy())
			{
				ChangeNumBuildings(iChangeNumRealBuilding);
			}
		}
	}

	if(buildingEntry->IsCityWall())
	{
		CvInterfacePtr<ICvPlot1> pDllPlot(new CvDllPlot(m_pCity->plot()));
		gDLL->GameplayWallCreated(pDllPlot.get());
	}

	// Update the amount of a Resource used up by this Building
	int iNumResources = GC.getNumResourceInfos();
	for(int iResourceLoop = 0; iResourceLoop < iNumResources; iResourceLoop++)
	{
		if(buildingEntry->GetResourceQuantityRequirement(iResourceLoop) > 0)
		{
			kPlayer.changeNumResourceUsed((ResourceTypes) iResourceLoop, iChangeNumRealBuilding * buildingEntry->GetResourceQuantityRequirement(iResourceLoop));
		}
	}

	if(iChangeNumRealBuilding > 0)
	{
		if(bFirst)
		{
			if(GC.getGame().isFinalInitialized()/* && !(gDLL->GetWorldBuilderMode() )*/)
			{
				// World Wonder Notification
				if(isWorldWonderClass(kBuildingClassInfo))
				{
					Localization::String localizedText = Localization::Lookup("TXT_KEY_MISC_COMPLETES_WONDER");
					localizedText << kPlayer.getNameKey() << buildingEntry->GetTextKey();
					GC.getGame().addReplayMessage(REPLAY_MESSAGE_MAJOR_EVENT, m_pCity->getOwner(), localizedText.toUTF8(), m_pCity->getX(), m_pCity->getY());

					bool bDontShowRewardPopup = GC.GetEngineUserInterface()->IsOptionNoRewardPopups();

					// Notification in MP games
					if(bDontShowRewardPopup || GC.getGame().isReallyNetworkMultiPlayer())
					{
						CvNotifications* pNotifications = GET_PLAYER(m_pCity->getOwner()).GetNotifications();
						if(pNotifications)
						{
							localizedText = Localization::Lookup("TXT_KEY_MISC_WONDER_COMPLETED");
							localizedText << kPlayer.getNameKey() << buildingEntry->GetTextKey();
							pNotifications->Add(NOTIFICATION_WONDER_COMPLETED_ACTIVE_PLAYER, localizedText.toUTF8(), localizedText.toUTF8(), m_pCity->getX(), m_pCity->getY(), eIndex, kPlayer.GetID());
						}
					}
					// Popup in SP games
					else
					{
						if(m_pCity->getOwner() == GC.getGame().getActivePlayer())
						{
							CvPopupInfo kPopup(BUTTONPOPUP_WONDER_COMPLETED_ACTIVE_PLAYER, eIndex);
							GC.GetEngineUserInterface()->AddPopup(kPopup);

							if (MOD_API_ACHIEVEMENTS && GET_PLAYER(GC.getGame().getActivePlayer()).isHuman())
							{
								gDLL->UnlockAchievement(ACHIEVEMENT_BUILD_WONDER);

								//look to see if all wonders have been built to unlock the other one
								IncrementWonderStats(buildingClassType);
							}
						}
					}

					// Wonder notification for all other players
					for(int iI = 0; iI < MAX_MAJOR_CIVS; iI++)
					{
						CvPlayerAI& thisPlayer = GET_PLAYER((PlayerTypes)iI);
						if(thisPlayer.isAlive() || thisPlayer.isObserver())
						{
							// Owner already got his messaging
							if(iI != m_pCity->getOwner())
							{
								// If the builder is met, and the city is revealed
								// Special case for DLC_06 Scenario: Always show the more informative notification
								if((m_pCity->plot()->isRevealed(thisPlayer.getTeam()) && GET_TEAM(thisPlayer.getTeam()).isHasMet(m_pCity->getTeam())) || gDLL->IsModActivated(CIV5_DLC_06_SCENARIO_MODID) || thisPlayer.isObserver())
								{
									CvNotifications* pNotifications = thisPlayer.GetNotifications();
									if(pNotifications)
									{
										localizedText = Localization::Lookup("TXT_KEY_MISC_WONDER_COMPLETED");
										localizedText << kPlayer.getNameKey() << buildingEntry->GetTextKey();
										pNotifications->Add(NOTIFICATION_WONDER_COMPLETED, localizedText.toUTF8(), localizedText.toUTF8(), m_pCity->getX(), m_pCity->getY(), eIndex, kPlayer.GetID());
									}
								}
								else
								{
									CvNotifications* pNotifications = thisPlayer.GetNotifications();
									if(pNotifications)
									{
										localizedText = Localization::Lookup("TXT_KEY_MISC_WONDER_COMPLETED_UNKNOWN");
										localizedText <<  buildingEntry->GetTextKey();
										pNotifications->Add(NOTIFICATION_WONDER_COMPLETED, localizedText.toUTF8(), localizedText.toUTF8(), -1, -1, eIndex, -1);
									}
								}
							}
						}

						//Achievements!
						if (MOD_API_ACHIEVEMENTS && kPlayer.GetID() == GC.getGame().getActivePlayer() && strcmp(buildingEntry->GetType(), "BUILDING_GREAT_FIREWALL") == 0)
						{
							gDLL->UnlockAchievement(ACHIEVEMENT_XP1_16);
						}
					}
				}
			}

			GC.getGame().incrementBuildingClassCreatedCount(buildingClassType);
		}
	}

	m_pCity->updateStrengthValue();

	// Building might affect City Banner stats
	CvInterfacePtr<ICvCity1> pCity = GC.WrapCityPointer(m_pCity);
	GC.GetEngineUserInterface()->SetSpecificCityInfoDirty(pCity.get(), CITY_UPDATE_TYPE_BANNER);

	//Test for any achievements being unlocked.
	if (MOD_API_ACHIEVEMENTS)
		kPlayer.GetPlayerAchievements().FinishedBuilding(m_pCity, eIndex);
}

/// Accessor: Get number of free buildings of this type in city
int CvCityBuildings::GetNumFreeBuilding(BuildingTypes eIndex) const
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");
	return m_paiNumFreeBuilding[eIndex];
}

/// Accessor: Set number of free buildings of this type in city
void CvCityBuildings::SetNumFreeBuilding(BuildingTypes eIndex, int iNewValue)
{
	ASSERT_DEBUG(eIndex >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eIndex < GC.getNumBuildingInfos(),  "eIndex expected to be < getNumBuildingInfos()");

	if(GetNumFreeBuilding(eIndex) != iNewValue)
	{
		if (iNewValue>0)
		{
			if (std::find(m_buildingsThatExistAtLeastOnce.begin(), m_buildingsThatExistAtLeastOnce.end(), eIndex) == m_buildingsThatExistAtLeastOnce.end())
				m_buildingsThatExistAtLeastOnce.push_back(eIndex);

			SetBuildingTypeByClassDirty(true);
		}
		else if (GetNumRealBuilding(eIndex)==0)
		{
			//we care about iteration speed, so erasing something can be cumbersome
			std::vector<BuildingTypes>::iterator pos = std::find( m_buildingsThatExistAtLeastOnce.begin(), m_buildingsThatExistAtLeastOnce.end(), eIndex);
			if (pos != m_buildingsThatExistAtLeastOnce.end())
				m_buildingsThatExistAtLeastOnce.erase(pos);

			SetBuildingTypeByClassDirty(true);
		}

		// This condensed logic comes from SetNumRealBuilding()
		int iChangeNumFreeBuilding = iNewValue - GetNumFreeBuilding(eIndex);
		
		m_paiNumFreeBuilding[eIndex] = iNewValue;

		// Process building effects
		m_pCity->processBuilding(eIndex, iChangeNumFreeBuilding, true, false, false, true);

		CvBuildingEntry* buildingEntry = GC.getBuildingInfo(eIndex);
		if (buildingEntry && buildingEntry->IsCityWall())
		{
			CvInterfacePtr<ICvPlot1> pDllPlot(new CvDllPlot(m_pCity->plot()));
			gDLL->GameplayWallCreated(pDllPlot.get());
		}

		m_pCity->updateStrengthValue();

		// Building might affect City Banner stats
		CvInterfacePtr<ICvCity1> pCity = GC.WrapCityPointer(m_pCity);
		GC.GetEngineUserInterface()->SetSpecificCityInfoDirty(pCity.get(), CITY_UPDATE_TYPE_BANNER);
	}
}
#if defined(MOD_BALANCE_CORE)
int CvCityBuildings::IsFirstTimeBuilding(BuildingTypes eBuilding)
{
	ASSERT_DEBUG(eBuilding >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eBuilding < GC.getNumBuildingInfos(), "eIndex expected to be < getNumBuildingInfos()");
	return m_paiFirstTimeBuilding[eBuilding];
}
void CvCityBuildings::SetFirstTimeBuilding(BuildingTypes eBuilding, int iValue)
{
	ASSERT_DEBUG(eBuilding >= 0, "eIndex expected to be >= 0");
	ASSERT_DEBUG(eBuilding < GC.getNumBuildingInfos(), "eIndex expected to be < getNumBuildingInfos()");
	if(IsFirstTimeBuilding(eBuilding) != iValue)
	{
		m_paiFirstTimeBuilding[eBuilding] = iValue;
	}
}
#endif
/// Accessor: Get yield boost for a specific building by yield type
int CvCityBuildings::GetBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield) const
{
	for(std::vector<BuildingYieldChange>::const_iterator it = m_aBuildingYieldChange.begin(); it != m_aBuildingYieldChange.end(); ++it)
	{
		if(it->eBuildingClass == eBuildingClass && it->eYield == eYield)
		{
			return it->iChange;
		}
	}

	return 0;
}

/// Accessor: Set yield boost for a specific building by yield type
void CvCityBuildings::SetBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield, int iChange)
{
	for (std::vector<BuildingYieldChange>::iterator it = m_aBuildingYieldChange.begin(); it != m_aBuildingYieldChange.end(); ++it)
	{
		if (it->eBuildingClass == eBuildingClass && it->eYield == eYield)
		{
			int iOldChange = it->iChange;
			if (iOldChange != iChange)
			{

				if (iChange == 0)
				{
					m_aBuildingYieldChange.erase(it);
				}
				else
				{
					it->iChange = iChange;
				}

				BuildingTypes eBuilding = m_pCity->GetBuildingTypeFromClass(eBuildingClass);
				if (eBuilding != NO_BUILDING && GetNumActiveBuilding(eBuilding) > 0)
				{
					m_pCity->ChangeBaseYieldRateFromBuildings(eYield, (iChange - iOldChange) * GetNumActiveBuilding(eBuilding));
				}
			}
			return;
		}
	}

	if (iChange != 0)
	{
		BuildingYieldChange kChange;
		kChange.eBuildingClass = eBuildingClass;
		kChange.eYield = eYield;
		kChange.iChange = iChange;
		m_aBuildingYieldChange.push_back(kChange);

		BuildingTypes eBuilding = m_pCity->GetBuildingTypeFromClass(eBuildingClass);
		if (eBuilding != NO_BUILDING && GetNumActiveBuilding(eBuilding) > 0)
		{
			m_pCity->ChangeBaseYieldRateFromBuildings(eYield, iChange * GetNumActiveBuilding(eBuilding));
		}
	}
}

/// Accessor: Change yield boost for a specific building by yield type
void CvCityBuildings::ChangeBuildingYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield, int iChange)
{
	SetBuildingYieldChange(eBuildingClass, eYield, GetBuildingYieldChange(eBuildingClass, eYield) + iChange);
}

/// Accessor: Get Great Work in a specific building by slot index
int CvCityBuildings::GetBuildingGreatWork(BuildingClassTypes eBuildingClass, int iSlot) const
{
	for (std::vector<BuildingGreatWork>::const_iterator it = m_aBuildingGreatWork.begin(); it != m_aBuildingGreatWork.end(); ++it)
	{
		if (it->eBuildingClass == eBuildingClass && it->iSlot == iSlot)
		{
			return it->iGreatWorkIndex;
		}
	}

	return -1;
}

/// Accessor: Set yield boost for a specific building by yield type
void CvCityBuildings::SetBuildingGreatWork(BuildingClassTypes eBuildingClass, int iSlot, int iGreatWorkIndex)
{
	m_pCity->ResetGreatWorkYieldCache();

	for(std::vector<BuildingGreatWork>::iterator it = m_aBuildingGreatWork.begin(); it != m_aBuildingGreatWork.end(); ++it)
	{
		if((*it).eBuildingClass == eBuildingClass && (*it).iSlot == iSlot)
		{
			int iOldGreatWorkIndex = (*it).iGreatWorkIndex;
			if (iOldGreatWorkIndex != iGreatWorkIndex)
			{
				if (iGreatWorkIndex == -1)
				{
					m_aBuildingGreatWork.erase(it);
				}
				else
				{
					(*it).iGreatWorkIndex = iGreatWorkIndex;
				}

				if (eBuildingClass != NO_BUILDINGCLASS)
				{
					m_pCity->GetCityCulture()->UpdateThemingBonusIndex(eBuildingClass);
				}
			}

			GC.GetEngineUserInterface()->setDirty(CityInfo_DIRTY_BIT, true);
			return;
		}
	}

	if (iGreatWorkIndex != -1)
	{
		BuildingGreatWork kWork;
		kWork.eBuildingClass = eBuildingClass;
		kWork.iSlot = iSlot;
		kWork.iGreatWorkIndex = iGreatWorkIndex;
		m_aBuildingGreatWork.push_back(kWork);

		if (eBuildingClass != NO_BUILDINGCLASS)
		{
			m_pCity->GetCityCulture()->UpdateThemingBonusIndex(eBuildingClass);
		}
	}

	GC.GetEngineUserInterface()->setDirty(CityInfo_DIRTY_BIT, true);
}

/// Accessor: Is there a Great Work inside this building?
bool CvCityBuildings::IsHoldingGreatWork(BuildingClassTypes eBuildingClass) const
{
	for(std::vector<BuildingGreatWork>::const_iterator it = m_aBuildingGreatWork.begin(); it != m_aBuildingGreatWork.end(); ++it)
	{
		if((*it).eBuildingClass == eBuildingClass)
		{
			return true;
		}
	}
	return false;
}

/// Accessor: How many Great Works are inside this building?
int CvCityBuildings::GetNumGreatWorksInBuilding(BuildingClassTypes eBuildingClass) const
{
	int iCount = 0;
	for(std::vector<BuildingGreatWork>::const_iterator it = m_aBuildingGreatWork.begin(); it != m_aBuildingGreatWork.end(); ++it)
	{
		if((*it).eBuildingClass == eBuildingClass)
		{
			iCount++;
		}
	}
	return iCount;
}

/// Accessor: Is there a Great Work slot somewhere in the city?
bool CvCityBuildings::HasAnyAvailableGreatWorkSlot() const
{
	BuildingClassTypes eBuildingClass = NO_BUILDINGCLASS; // Passed by reference below
	int iSlot = -1; // Passed by reference below

	return GetNextAvailableGreatWorkSlot(eBuildingClass, iSlot);
}

/// Accessor: Is there a Great Work slot of this type somewhere in the city?
bool CvCityBuildings::HasAvailableGreatWorkSlot(GreatWorkSlotType eSlotType) const
{
	BuildingClassTypes eBuildingClass = NO_BUILDINGCLASS; // Passed by reference below
	int iSlot = -1; // Passed by reference below

	return GetNextAvailableGreatWorkSlot(eSlotType, eBuildingClass, iSlot);
}

/// Accessor: How many Great Work slots of this type are in the city?
int CvCityBuildings::GetNumAvailableGreatWorkSlots(GreatWorkSlotType eSlotType) const
{
	int iCount = 0;
	for(size_t iI = 0; iI < m_buildingsThatExistAtLeastOnce.size(); iI++)
	{
		CvBuildingEntry *pkBuilding = GC.getBuildingInfo(m_buildingsThatExistAtLeastOnce[iI]);
		if (pkBuilding)
		{
			if (eSlotType == NO_GREAT_WORK_SLOT || pkBuilding->GetGreatWorkSlotType() == eSlotType)
			{
				int iNumSlots = pkBuilding->GetGreatWorkCount();
				int iNumOpenSlots = iNumSlots - GetNumGreatWorksInBuilding(pkBuilding->GetBuildingClassType());
				if(iNumOpenSlots > 0)
				{
					iCount += iNumOpenSlots;
				}
			}
		}
	}

	return iCount;
}

int CvCityBuildings::GetNumFilledGreatWorkSlots(GreatWorkSlotType eSlotType) const
{
	int iCount = 0;
	for(size_t iI = 0; iI < m_buildingsThatExistAtLeastOnce.size(); iI++)
	{
		CvBuildingEntry *pkBuilding = GC.getBuildingInfo(m_buildingsThatExistAtLeastOnce[iI]);
		if (pkBuilding)
		{
			if (eSlotType == NO_GREAT_WORK_SLOT || pkBuilding->GetGreatWorkSlotType() == eSlotType)
			{
				int iNumFilledSlots = GetNumGreatWorksInBuilding(pkBuilding->GetBuildingClassType());
				if (iNumFilledSlots > 0)
				{
					iCount += iNumFilledSlots;
				}
			}
		}
	}

	return iCount;
}

/// Accessor: Is there a Great Work slot of this type somewhere in the city?
bool CvCityBuildings::GetNextAvailableGreatWorkSlot(BuildingClassTypes& eBuildingClass, int& iSlot) const
{
	if (eBuildingClass && iSlot)
	{
		for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
		{
			BuildingClassTypes eLoopBuildingClass = static_cast<BuildingClassTypes>(iI);
			BuildingTypes eBuilding = m_pCity->GetBuildingTypeFromClass(eLoopBuildingClass);
			if (eBuilding == NO_BUILDING)
				continue;

			CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
			if (!pkBuildingInfo)
				continue;

			if (GetNumBuilding(eBuilding) > 0)
			{
				int iNumSlots = pkBuildingInfo->GetGreatWorkCount();
				for (int iJ = 0; iJ < iNumSlots; iJ++)
				{
					if (GetBuildingGreatWork(eLoopBuildingClass, iJ) == NO_GREAT_WORK)
					{
						eBuildingClass = eLoopBuildingClass;
						iSlot = iJ;
						return true;
					}
				}
			}
		}
	}

	return false;
}

/// Accessor: Is there a Great Work slot of this type somewhere in the city?
bool CvCityBuildings::GetNextAvailableGreatWorkSlot(GreatWorkSlotType eGreatWorkSlot, BuildingClassTypes& eBuildingClass, int& iSlot) const
{
	if (eBuildingClass && iSlot)
	{
		for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
		{
			BuildingClassTypes eLoopBuildingClass = static_cast<BuildingClassTypes>(iI);
			BuildingTypes eBuilding = m_pCity->GetBuildingTypeFromClass(eLoopBuildingClass);
			if (eBuilding == NO_BUILDING)
				continue;

			CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
			if (!pkBuildingInfo)
				continue;

			if (GetNumBuilding(eBuilding) > 0)
			{
				if (pkBuildingInfo->GetGreatWorkSlotType() == eGreatWorkSlot)
				{
					int iNumSlots = pkBuildingInfo->GetGreatWorkCount();
					for (int iJ = 0; iJ < iNumSlots; iJ++)
					{
						if (GetBuildingGreatWork(eLoopBuildingClass, iJ) == NO_GREAT_WORK)
						{
							eBuildingClass = eLoopBuildingClass;
							iSlot = iJ;
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

/// Accessor: How much of this yield are we generating from Great Works in our buildings?
/// If you change anything in the calculations here, don't forget to update CvGameCulture::GetGreatWorkTooltip
int CvCityBuildings::GetYieldFromGreatWorksTimes100(YieldTypes eYield) const
{
	//Simplification - errata yields not worth considering.
	if(eYield > YIELD_CULTURE_LOCAL && !MOD_BALANCE_CORE_JFD)
	{
		return 0;
	}

	int iNumGreatWorks = 0;
	int iNumGreatWorksWithBaseYieldsOfThisType = 0;
	int iThemingBonusTotal = 0;
	int iTypeBonuses = 0;

	CvPlayer& kPlayer = GET_PLAYER(m_pCity->getOwner());

	
	for(std::vector<BuildingTypes>::const_iterator iI=m_buildingsThatExistAtLeastOnce.begin(); iI!=m_buildingsThatExistAtLeastOnce.end(); ++iI)
	{
		CvBuildingEntry *pkInfo = GC.getBuildingInfo(*iI);
		if (pkInfo && pkInfo->GetGreatWorkCount() > 0)
		{
			int iThisWork = GetNumGreatWorksInBuilding(pkInfo->GetBuildingClassType());
			iNumGreatWorks += iThisWork;

			int iThemingBonus = m_pCity->GetCityCulture()->GetThemingBonus(pkInfo->GetBuildingClassType());
			if (iThemingBonus > 0)
			{
				iThemingBonusTotal += pkInfo->GetThemingYieldBonus(eYield);
			}
			
			if ((MOD_GLOBAL_GREATWORK_YIELDTYPES && eYield == pkInfo->GetGreatWorkYieldType()) || (!MOD_GLOBAL_GREATWORK_YIELDTYPES && eYield == YIELD_CULTURE))
			{
				iNumGreatWorksWithBaseYieldsOfThisType += iThisWork;
			}
		}
	}

	iThemingBonusTotal += m_pCity->GetYieldChangesPerLocalTheme(eYield) * GetTotalNumThemedBuildings();
	
	//No works? Abort!
	if(iNumGreatWorks <= 0)
	{
		return 0;
	}
	
	int iYieldPerGreatWorkWithBaseYieldsOfThisType = /*2 in CP, 3 in VP*/ GD_INT_GET(BASE_CULTURE_PER_GREAT_WORK);

	int iYieldPerGreatWork = kPlayer.GetGreatWorkYieldChange(eYield);
	iYieldPerGreatWork += kPlayer.GetPlayerTraits()->GetGreatWorkYieldChanges(eYield);
	iYieldPerGreatWork += m_pCity->GetGreatWorkYieldChange(eYield);
	if (eYield == YIELD_TOURISM)
	{
		iYieldPerGreatWork += /*2*/ GD_INT_GET(BASE_TOURISM_PER_GREAT_WORK);
	}

	ReligionTypes eMajority = m_pCity->GetCityReligions()->GetReligiousMajority();
	BeliefTypes eSecondaryPantheon = NO_BELIEF;
	if(eMajority >= RELIGION_PANTHEON)
	{
		const CvReligion* pReligion = GC.getGame().GetGameReligions()->GetReligion(eMajority, m_pCity->getOwner());
		if (pReligion)
		{
			iYieldPerGreatWork += pReligion->m_Beliefs.GetGreatWorkYieldChange(m_pCity->getPopulation(), eYield, m_pCity->getOwner(), m_pCity);
			
			eSecondaryPantheon = m_pCity->GetCityReligions()->GetSecondaryReligionPantheonBelief();
			if (eSecondaryPantheon != NO_BELIEF)
			{
				iYieldPerGreatWork += GC.GetGameBeliefs()->GetEntry(eSecondaryPantheon)->GetGreatWorkYieldChange(eYield);
			}
		}
	}

	// Mod for civs keeping their pantheon belief forever
	if (MOD_RELIGION_PERMANENT_PANTHEON)
	{
		if (GC.getGame().GetGameReligions()->HasCreatedPantheon(m_pCity->getOwner()))
		{
			const CvReligion* pPantheon = GC.getGame().GetGameReligions()->GetReligion(RELIGION_PANTHEON, m_pCity->getOwner());
			BeliefTypes ePantheonBelief = GC.getGame().GetGameReligions()->GetBeliefInPantheon(m_pCity->getOwner());
			if (pPantheon != NULL && ePantheonBelief != NO_BELIEF && ePantheonBelief != eSecondaryPantheon)
			{
				const CvReligion* pReligion = GC.getGame().GetGameReligions()->GetReligion(eMajority, m_pCity->getOwner());
				if (pReligion == NULL || (pReligion != NULL && !pReligion->m_Beliefs.IsPantheonBeliefInReligion(ePantheonBelief, eMajority, m_pCity->getOwner()))) // check that the our religion does not have our belief, to prevent double counting
				{
					iYieldPerGreatWork += GC.GetGameBeliefs()->GetEntry(ePantheonBelief)->GetGreatWorkYieldChange(eYield);
				}
			}
		}
	}

	//First add up yields x works in city.
	int iRtnValue = (iNumGreatWorksWithBaseYieldsOfThisType * iYieldPerGreatWorkWithBaseYieldsOfThisType) * 100;
	iRtnValue += (iNumGreatWorks * iYieldPerGreatWork) * 100;

	//Tourism modifiers
	if (eYield == YIELD_TOURISM)
	{
		iRtnValue += ((m_pCity->GetCityBuildings()->GetGreatWorksTourismModifier() + kPlayer.GetGreatWorksTourismModifierGlobal()) * iRtnValue / 100);
	}

	// ---- YIELDS NOT AFFECTED BY TOURISM MODIFIER TO GREAT WORKS ----

	//Then theming bonuses for the yield.
	iRtnValue += GetCurrentThemingBonuses(eYield) * 100;

	int iArt = GetNumGreatWorks(CvTypes::getGREAT_WORK_SLOT_ART_ARTIFACT(), false, true);
	if (iArt > 0)
	{
		iTypeBonuses += (kPlayer.GetPlayerTraits()->GetArtYieldChanges(eYield) * iArt);
		iTypeBonuses += (kPlayer.getArtYieldBonus(eYield) * iArt);
	}
	int iArtifact = GetNumGreatWorks(CvTypes::getGREAT_WORK_SLOT_ART_ARTIFACT(), true);
	if (iArtifact > 0)
	{
		iTypeBonuses += (kPlayer.GetPlayerTraits()->GetArtifactYieldChanges(eYield) * iArtifact);
		iTypeBonuses += (kPlayer.getArtifactYieldBonus(eYield) * iArtifact);
	}
	int iLit = GetNumGreatWorks(CvTypes::getGREAT_WORK_SLOT_LITERATURE());
	if (iLit > 0)
	{
		iTypeBonuses += (kPlayer.GetPlayerTraits()->GetLitYieldChanges(eYield) * iLit);
		iTypeBonuses += (kPlayer.getLitYieldBonus(eYield) * iLit);
	}
	int iMusic = GetNumGreatWorks(CvTypes::getGREAT_WORK_SLOT_MUSIC());
	if (iMusic > 0)
	{
		iTypeBonuses += (kPlayer.GetPlayerTraits()->GetMusicYieldChanges(eYield) * iMusic);
		iTypeBonuses += (kPlayer.getMusicYieldBonus(eYield) * iMusic);
	}
	int iFilm = GetNumGreatWorks(CvTypes::getGREAT_WORK_SLOT_FILM());
	if (iFilm > 0)
	{
		iTypeBonuses += (kPlayer.getFilmYieldBonus(eYield) * iFilm);
	}
	int iRelic = GetNumGreatWorks(CvTypes::getGREAT_WORK_SLOT_RELIC());
	if (iRelic > 0)
	{
		iTypeBonuses += (kPlayer.getRelicYieldBonus(eYield) * iRelic);
	}

	//Next add in any UA or extra theming bonuses.
	iRtnValue += (iTypeBonuses + iThemingBonusTotal) * 100;

	return iRtnValue;
}

/// Accessor: How many Great Works of specific slot type present in this city?
int CvCityBuildings::GetNumGreatWorks(bool bIgnoreYield) const
{
	int iRtnValue = 0;

	for (std::vector<BuildingGreatWork>::const_iterator it = m_aBuildingGreatWork.begin(); it != m_aBuildingGreatWork.end(); ++it)
	{
		BuildingClassTypes eBuildingClass = it->eBuildingClass;
		BuildingTypes eBuilding = m_pCity->GetBuildingTypeFromClass(eBuildingClass);
		CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
		if (pkBuildingInfo)
		{
			if (bIgnoreYield || pkBuildingInfo->GetGreatWorkYieldType() != NO_YIELD)
			{
				iRtnValue++;
			}
		}
	}

	return iRtnValue;
}

/// Accessor: How many Great Works of specific slot type present in this city?
int CvCityBuildings::GetNumGreatWorks(GreatWorkSlotType eGreatWorkSlot, bool bArtifact, bool bArt) const
{
	if (eGreatWorkSlot == NO_GREAT_WORK_SLOT)
		return 0;

	int iRtnValue = 0;
	GreatWorkClass eArtifactsClass = static_cast<GreatWorkClass>(GC.getInfoTypeForString("GREAT_WORK_ARTIFACT"));
	GreatWorkClass eArtClass = static_cast<GreatWorkClass>(GC.getInfoTypeForString("GREAT_WORK_ART"));
	for (std::vector<BuildingGreatWork>::const_iterator it = m_aBuildingGreatWork.begin(); it != m_aBuildingGreatWork.end(); ++it)
	{
		BuildingClassTypes eBuildingClass = it->eBuildingClass;
		BuildingTypes eBuilding = m_pCity->GetBuildingTypeFromClass(eBuildingClass);
		CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
		if (!pkBuildingInfo)
			continue;

		// Art/Artifact need distinction here, because they occupy the same slot!
		if (bArtifact)
		{
			CvGreatWork work = GC.getGame().GetGameCulture()->m_CurrentGreatWorks[it->iGreatWorkIndex];

			// Check Great Work class
			if (work.m_eClassType == eArtifactsClass)
			{
				iRtnValue++;
			}
		}
		else if (bArt)
		{
			CvGreatWork work = GC.getGame().GetGameCulture()->m_CurrentGreatWorks[it->iGreatWorkIndex];

			// Check Great Work class
			if (work.m_eClassType == eArtClass)
			{
				iRtnValue++;
			}
		}
		else if (pkBuildingInfo->GetGreatWorkSlotType() == eGreatWorkSlot)
		{
			iRtnValue++;
		}
	}

	return iRtnValue;
}

int CvCityBuildings::GetThemingBonusIndex(BuildingTypes eBuilding) const
{
	return m_paiThemingBonusIndex[eBuilding];
}

void CvCityBuildings::SetThemingBonusIndex(BuildingTypes eBuilding, int iIndex)
{
	ASSERT_DEBUG(eBuilding >= 0, "eBuilding expected to be >= 0");
	ASSERT_DEBUG(eBuilding < GC.getNumBuildingInfos(), "eIndex expected to be < getNumBuildingInfos()");
	m_paiThemingBonusIndex[eBuilding] = iIndex;
}

/// Accessor: Get tourism converted from culture from Improvements and Wonders
int CvCityBuildings::GetLandmarksTourismPercent() const
{
	return m_iLandmarksTourismPercent;
}

/// Accessor: Change tourism converted from culture from Improvements and Wonders
void CvCityBuildings::ChangeLandmarksTourismPercent(int iChange)
{
	if(iChange != 0)
	{
		m_iLandmarksTourismPercent = (m_iLandmarksTourismPercent + iChange);
		ASSERT_DEBUG(m_iLandmarksTourismPercent >= 0);
	}
}

/// Accessor: Get extra times to spread religion for missionaries from this city
int CvCityBuildings::GetGreatWorksTourismModifier() const
{
	return m_iGreatWorksTourismModifier;
}

/// Accessor: Change extra times to spread religion for missionaries from this city
void CvCityBuildings::ChangeGreatWorksTourismModifier(int iChange)
{
	if(iChange != 0)
	{
		m_iGreatWorksTourismModifier = (m_iGreatWorksTourismModifier + iChange);
		ASSERT_DEBUG(m_iGreatWorksTourismModifier >= 0);
	}
}

/// Accessor: Total theming bonus from all buildings in the city
int CvCityBuildings::GetCurrentThemingBonuses(YieldTypes eYield) const
{
	int iTotal = 0;
	for(std::vector<BuildingTypes>::const_iterator iI=m_buildingsThatExistAtLeastOnce.begin(); iI!=m_buildingsThatExistAtLeastOnce.end(); ++iI)
	{
		CvBuildingEntry *pkBuilding = GC.getBuildingInfo(*iI);
		if (pkBuilding)
		{
			if (pkBuilding->GetGreatWorkYieldType() == eYield || eYield == YIELD_TOURISM)
			{
				int iIndex = GetThemingBonusIndex(*iI);
				if (iIndex < 0)
					continue;
				
				CvThemingBonusInfo* pkThemingBonus = pkBuilding->GetThemingBonusInfo(iIndex);
				ASSERT_DEBUG(pkThemingBonus);
				int iBonus = pkThemingBonus->GetBonus();
				int iModifier = m_pCity->GetPlayer()->GetPlayerPolicies()->GetNumericModifier(POLICYMOD_THEMING_BONUS);

				if (m_pCity->isCapital())
					iModifier += m_pCity->GetPlayer()->GetPlayerTraits()->GetCapitalThemingBonusModifier();

				iBonus = iBonus * (100 + iModifier) / 100;

				if (MOD_API_ACHIEVEMENTS && m_pCity->GetPlayer()->isHuman() && !GC.getGame().isGameMultiPlayer() && iBonus >= 16)
					gDLL->UnlockAchievement(ACHIEVEMENT_XP2_40);

				iTotal += iBonus;
			}
		}
	}

	return iTotal;
}

/// Accessor: Total theming bonus from all buildings in the city
int CvCityBuildings::GetTotalNumThemedBuildings() const
{
	int iBonus = 0;

	for (std::vector<BuildingTypes>::const_iterator iI = m_buildingsThatExistAtLeastOnce.begin(); iI != m_buildingsThatExistAtLeastOnce.end(); ++iI)
	{
		CvBuildingEntry *pkInfo = GC.getBuildingInfo(*iI);
		if (pkInfo)
		{
			if (GetThemingBonusIndex((BuildingTypes)*iI) != -1)
			{
				iBonus++;
			}
		}
	}

	return iBonus;
}

/// Accessor: How many buildings in this city are ones that are built through Faith?
int CvCityBuildings::GetNumBuildingsFromFaith() const
{
	int iRtnValue = 0;

	for (size_t iI = 0; iI < m_buildingsThatExistAtLeastOnce.size(); iI++)
	{
		CvBuildingEntry *pkBuilding = GC.getBuildingInfo(m_buildingsThatExistAtLeastOnce[iI]);
		if (pkBuilding && pkBuilding->IsFaithPurchaseOnly())
		{
			iRtnValue++;
		}
	}

	return iRtnValue;
}

/// Accessor: What is the production modifier for each city state trade route?
int CvCityBuildings::GetCityStateTradeRouteProductionModifier() const
{
	int iCityStates = GET_PLAYER(m_pCity->getOwner()).GetTrade()->GetNumberOfCityStateTradeRoutes();
	return iCityStates * GetBuildingProductionModifierPotentialFromMinorTrade();
}

/// Accessor: Get current production modifier from buildings
int CvCityBuildings::GetBuildingProductionModifierPotentialFromMinorTrade() const
{
	return m_iBuildingProductionModifierPotentialFromMinorTrade;
}

/// Accessor: Change current production modifier from buildings
void CvCityBuildings::ChangeBuildingProductionModifierPotentialFromMinorTrade(int iChange)
{
	m_iBuildingProductionModifierPotentialFromMinorTrade += iChange;
}

/// Accessor: Get current production modifier from buildings
int CvCityBuildings::GetBuildingProductionModifier() const
{
	return m_iBuildingProductionModifier;
}

/// Accessor: Change current production modifier from buildings
void CvCityBuildings::ChangeBuildingProductionModifier(int iChange)
{
	m_iBuildingProductionModifier += iChange;
	ASSERT_DEBUG(GetBuildingProductionModifier() >= 0);
}

/// Accessor: Get current defense boost from buildings
int CvCityBuildings::GetBuildingDefense() const
{
	return m_iBuildingDefense;
}

/// Accessor: Change current defense boost from buildings
void CvCityBuildings::ChangeBuildingDefense(int iChange)
{
	if(iChange != 0)
	{
		m_iBuildingDefense = (m_iBuildingDefense + iChange);
		ASSERT_DEBUG(GetBuildingDefense() >= 0);

		m_pCity->plot()->plotAction(PUF_makeInfoBarDirty);
	}
}

/// Accessor: Get current defense boost Mod from buildings
int CvCityBuildings::GetBuildingDefenseMod() const
{
	return m_iBuildingDefenseMod;
}

/// Accessor: Change current defense boost mod from buildings
void CvCityBuildings::ChangeBuildingDefenseMod(int iChange)
{
	if(iChange != 0)
	{
		m_iBuildingDefenseMod = (m_iBuildingDefenseMod + iChange);
		ASSERT_DEBUG(m_iBuildingDefenseMod >= 0);

		m_pCity->plot()->plotAction(PUF_makeInfoBarDirty);
	}
}

/// Accessor: Get extra times to spread religion for missionaries from this city
int CvCityBuildings::GetMissionaryExtraSpreads() const
{
	return m_iMissionaryExtraSpreads;
}

/// Accessor: Change extra times to spread religion for missionaries from this city
void CvCityBuildings::ChangeMissionaryExtraSpreads(int iChange)
{
	m_iMissionaryExtraSpreads += iChange;
}

void CvCityBuildings::IncrementWonderStats(BuildingClassTypes eIndex)
{
	if (!MOD_API_ACHIEVEMENTS)
		return;

	CvBuildingClassInfo* pkBuildingClassInfo = GC.getBuildingClassInfo(eIndex);
	if (pkBuildingClassInfo == NULL)
		return;

	const char* szWonderTypeChar = pkBuildingClassInfo->GetType();
	CvString szWonderType = szWonderTypeChar;

	if(szWonderType == "BUILDINGCLASS_HEROIC_EPIC")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_HEROICEPIC);
	}
	else if(szWonderType == "BUILDINGCLASS_NATIONAL_COLLEGE")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_NATIONALCOLLEGE);
	}
	else if(szWonderType == "BUILDINGCLASS_NATIONAL_EPIC")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_NATIONALEPIC);
	}
	else if(szWonderType == "BUILDINGCLASS_IRONWORKS")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_IRONWORKS);
	}
	else if(szWonderType == "BUILDINGCLASS_OXFORD_UNIVERSITY")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_OXFORDUNIVERSITY);
	}
	else if(szWonderType == "BUILDINGCLASS_HERMITAGE")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_HERMITAGE);
	}
	else if(szWonderType == "BUILDINGCLASS_GREAT_LIGHTHOUSE")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_GREATLIGHTHOUSE);
	}
	else if(szWonderType == "BUILDINGCLASS_STONEHENGE")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_STONEHENGE);
	}
	else if(szWonderType == "BUILDINGCLASS_GREAT_LIBRARY")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_GREATLIBRARY);
	}
	else if(szWonderType == "BUILDINGCLASS_PYRAMID")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_PYRAMIDS);
	}
	else if(szWonderType == "BUILDINGCLASS_COLOSSUS")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_COLOSSUS);
	}
	else if(szWonderType == "BUILDINGCLASS_ORACLE")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_ORACLE);
	}
	else if(szWonderType == "BUILDINGCLASS_HANGING_GARDEN")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_HANGINGGARDENS);
	}
	else if(szWonderType == "BUILDINGCLASS_GREAT_WALL")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_GREATWALL);
	}
	else if(szWonderType == "BUILDINGCLASS_ANGKOR_WAT")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_ANGKORWAT);
	}
	else if(szWonderType == "BUILDINGCLASS_HAGIA_SOPHIA")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_HAGIASOPHIA);
	}
	else if(szWonderType == "BUILDINGCLASS_CHICHEN_ITZA")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_CHICHENITZA);
	}
	else if(szWonderType == "BUILDINGCLASS_MACHU_PICHU")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_MACHUPICCHU);
	}
	else if(szWonderType == "BUILDINGCLASS_NOTRE_DAME")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_NOTREDAME);
	}
	else if(szWonderType == "BUILDINGCLASS_PORCELAIN_TOWER")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_PORCELAINTOWER);
	}
	else if(szWonderType == "BUILDINGCLASS_HIMEJI_CASTLE")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_HIMEJICASTLE);
	}
	else if(szWonderType == "BUILDINGCLASS_SISTINE_CHAPEL")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_SISTINECHAPEL);
	}
	else if(szWonderType == "BUILDINGCLASS_KREMLIN")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_KREMLIN);
	}
	else if(szWonderType == "BUILDINGCLASS_FORBIDDEN_PALACE")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_FORBIDDENPALACE);
	}
	else if(szWonderType == "BUILDINGCLASS_TAJ_MAHAL")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_TAJMAHAL);
	}
	else if(szWonderType == "BUILDINGCLASS_BIG_BEN")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_BIGBEN);
	}
	else if(szWonderType == "BUILDINGCLASS_LOUVRE")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_LOUVRE);
	}
	else if(szWonderType == "BUILDINGCLASS_BRANDENBURG_GATE")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_BRANDENBURGGATE);
	}
	else if(szWonderType == "BUILDINGCLASS_STATUE_OF_LIBERTY")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_STATUEOFLIBERTY);
	}
	else if(szWonderType == "BUILDINGCLASS_CRISTO_REDENTOR")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_CRISTOREDENTOR);
	}
	else if(szWonderType == "BUILDINGCLASS_EIFFEL_TOWER")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_EIFFELTOWER);
	}
	else if(szWonderType == "BUILDINGCLASS_PENTAGON")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_PENTAGON);
	}
	else if(szWonderType == "BUILDINGCLASS_UNITED_NATIONS")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_UNITEDNATION);
	}
	else if(szWonderType == "BUILDINGCLASS_SYDNEY_OPERA_HOUSE")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_SYDNEYOPERAHOUSE);
	}
	else if(szWonderType == "BUILDINGCLASS_STATUE_ZEUS")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_STATUEOFZEUS);
	}
	else if(szWonderType == "BUILDINGCLASS_TEMPLE_ARTEMIS")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_TEMPLEOFARTEMIS);
	}
	else if(szWonderType == "BUILDINGCLASS_MAUSOLEUM_HALICARNASSUS")
	{
		gDLL->IncrementSteamStat(ESTEAMSTAT_MAUSOLEUMOFHALICARNASSUS);
	}
	else
	{
		//CUSTOMLOG("No Stat for selected Wonder %s.",szWonderType.c_str());
	}

	bool bCheckForWonders = CheckForAllWondersBuilt();
	if (bCheckForWonders)
		gDLL->UnlockAchievement(ACHIEVEMENT_ALL_WONDERS);

	//DLC_06
	bool bCheckForAncientWonders = CheckForSevenAncientWondersBuilt();
	if (bCheckForAncientWonders)
		gDLL->UnlockAchievement(ACHIEVEMENT_SPECIAL_ANCIENT_WONDERS);
}

bool CvCityBuildings::CheckForAllWondersBuilt()
{
	int iI = 0;
	int iStartStatWonder = ESTEAMSTAT_ANGKORWAT;
	int iEndStatWonder = ESTEAMSTAT_PYRAMIDS;		//Don't include the united nations because it was removed in BNW.
	int32 nStat = 0;

	for(iI = iStartStatWonder; iI < iEndStatWonder; iI++)
	{
		if(gDLL->GetSteamStat((ESteamStat)iI, &nStat))
		{
			if(nStat <= 0)
			{
				return false;
			}
		}
	}
	return true;
}

bool CvCityBuildings::CheckForSevenAncientWondersBuilt()
{
	GUID guid;
	ExtractGUID(CIV5_DLC_06_PACKAGEID, guid);

	if(gDLL->IsDLCValid(guid))
	{
		ESteamStat arrWonderStats[7] =
		{
			ESTEAMSTAT_COLOSSUS,
			ESTEAMSTAT_GREATLIGHTHOUSE,
			ESTEAMSTAT_HANGINGGARDENS,
			ESTEAMSTAT_PYRAMIDS,
			ESTEAMSTAT_STATUEOFZEUS,
			ESTEAMSTAT_TEMPLEOFARTEMIS,
			ESTEAMSTAT_MAUSOLEUMOFHALICARNASSUS
		};
		int32 nStat = 0;
		for(int iI = 0; iI < 7; iI++)
		{
			if(gDLL->GetSteamStat(arrWonderStats[iI], &nStat))
			{
				if(nStat <= 0)
				{
					return false;
				}
			}
			else
			{
				// Couldn't get one of the SteamStats for some reason
				return false;
			}
		}
		return true;
	}
	return false;
}
/// Uses the notification system to send information out when other players need to know a building has been started
void CvCityBuildings::NotifyNewBuildingStarted(BuildingTypes /*eIndex*/)
{
	// JON: Disabling this notification
	// is this city starting a wonder? If so, send a notification
	//CvBuildingEntry* buildingEntry = GC.getBuildingInfo(eIndex);
	//if (isLimitedWonderClass((BuildingClassTypes)(buildingEntry->GetBuildingClassType())) && GetBuildingProductionTimes100(eIndex) == 0)
	//{
	//	Localization::String locString;
	//	Localization::String locSummaryString;

	//	for (uint ui = 0; ui < MAX_MAJOR_CIVS; ui++)
	//	{
	//		PlayerTypes ePlayer = (PlayerTypes)ui;

	//		if (ePlayer == m_pCity->getOwner() || !GET_PLAYER(ePlayer).isAlive())
	//		{
	//			continue;
	//		}

	//		int iX = -1;
	//		int iY = -1;
	//		int iPlayerID = -1;

	//		if (GET_TEAM(m_pCity->getTeam()).isHasMet(GET_PLAYER(ePlayer).getTeam()))
	//		{
	//			if (m_pCity->isRevealed(GET_PLAYER(ePlayer).getTeam(), false))
	//			{
	//				locString = Localization::Lookup("TXT_KEY_NOTIFICATION_WONDER_STARTED");
	//				locString << GET_PLAYER(m_pCity->getOwner()).getNameKey() << buildingEntry->GetTextKey() << m_pCity->getNameKey();
	//			}
	//			else
	//			{
	//				locString = Localization::Lookup("TXT_KEY_NOTIFICATION_WONDER_STARTED_UNKNOWN_LOCATION");
	//				locString << GET_PLAYER(m_pCity->getOwner()).getNameKey() << buildingEntry->GetTextKey();
	//			}


	//			locSummaryString = Localization::Lookup("TXT_KEY_NOTIFICATION_SUMMARY_WONDER_STARTED");
	//			locSummaryString << GET_PLAYER(m_pCity->getOwner()).getNameKey() << buildingEntry->GetTextKey();

	//		}
	//		else
	//		{
	//			locString = Localization::Lookup("TXT_KEY_NOTIFICATION_WONDER_STARTED_UNMET");
	//			locString << buildingEntry->GetTextKey();
	//			locSummaryString = Localization::Lookup("TXT_KEY_NOTIFICATION_SUMMARY_WONDER_STARTED_UNKNOWN");
	//			locSummaryString << buildingEntry->GetTextKey();
	//		}

	//		CvNotifications* pNotifications = GET_PLAYER(ePlayer).GetNotifications();
	//		if (pNotifications)
	//		{
	//			pNotifications->Add(NOTIFICATION_WONDER_STARTED, locString.toUTF8(), locSummaryString.toUTF8(), iX, iY, eIndex);
	//		}
	//	}
	//}
}

/// Helper function to read in an integer array of data sized according to number of building types
void BuildingArrayHelpers::Read(FDataStream& kStream, int* paiBuildingArray)
{
	int iNumEntries = 0;
	int iType = 0;

	kStream >> iNumEntries;

	for(int iI = 0; iI < iNumEntries; iI++)
	{
		bool bValid = true;
		iType = CvInfosSerializationHelper::ReadHashed(kStream, &bValid);
		if(iType != -1 || !bValid)
		{
			if(iType != -1)
			{
				kStream >> paiBuildingArray[iType];
			}
			else
			{
				CvString szError;
				szError.Format("LOAD ERROR: Building Type not found");
				GC.LogMessage(szError.GetCString());
				ASSERT_DEBUG(false, szError);
				int iDummy = 0;
				kStream >> iDummy; // Skip it.
			}
		}
	}
}

/// Helper function to write out an integer array of data sized according to number of building types
void BuildingArrayHelpers::Write(FDataStream& kStream, int* paiBuildingArray, int iArraySize)
{
	kStream << iArraySize;

	for(int iI = 0; iI < iArraySize; iI++)
	{
		const BuildingTypes eBuilding = static_cast<BuildingTypes>(iI);
		CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
		if(pkBuildingInfo)
		{
			CvInfosSerializationHelper::WriteHashed(kStream, pkBuildingInfo);
			kStream << paiBuildingArray[iI];
		}
		else
		{
			kStream << 0;
		}
	}
}
