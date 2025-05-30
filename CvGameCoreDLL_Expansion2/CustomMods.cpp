// CustomMods.cpp
#include "CvGameCoreDLLPCH.h"

// must be included after all other headers
#include "LintFree.h"

CustomMods gCustomMods;

CustomMods::CustomMods() :
	m_bInit(false)
{
}

// This function hooks an event with a variable number of arguments.
int CustomMods::eventHook(const char* szName, const char* p, ...) {
	// Initialize an argument handle for Lua.
	CvLuaArgsHandle args;

	// Initialize the variable argument list.
	va_list vl;
	va_start(vl, p);

	// Iterate through the format string to handle each argument.
	for (const char* it = p; *it; ++it) {
		if (*it == 'b') {
			// Handle boolean arguments.
			args->Push(!!va_arg(vl, int));
		} else if (*it == 'i') {
			// Handle integer arguments.
			args->Push(va_arg(vl, int));
		} else {
			// Assume it's a string argument.
			char* s = va_arg(vl, char*);
			args->Push(s, strlen(s));
			break; // Exit after processing the first string argument.
		}
	}

	// Clean up the variable argument list.
	va_end(vl);

	// Call the event hook with the constructed arguments.
	return eventHook(szName, args);
}

// This function tests all conditions for an event with a variable number of arguments.
int CustomMods::eventTestAll(const char* szName, const char* p, ...) {
	// Initialize an argument handle for Lua.
	CvLuaArgsHandle args;

	// Initialize the variable argument list.
    va_list vl;
    va_start(vl, p);

	// Iterate through the format string to handle each argument.
	for (const char* it = p; *it; ++it) {
		if (*it == 'b') {
			// Handle boolean arguments.
			args->Push(!!va_arg(vl, int));
		} else if (*it == 'i') {
			// Handle integer arguments.
			args->Push(va_arg(vl, int));
		} else {
			// Assume it's a string argument.
			char* s = va_arg(vl, char*);
			args->Push(s, strlen(s));
			break; // Exit after processing the first string argument.
		}
	}

	// Clean up the variable argument list.
	va_end(vl);

	// Call the event test function with the constructed arguments.
	return eventTestAll(szName, args);
}

// This function tests any condition for an event with a variable number of arguments.
int CustomMods::eventTestAny(const char* szName, const char* p, ...) {
	// Initialize an argument handle for Lua.
	CvLuaArgsHandle args;

	// Initialize the variable argument list.
	va_list vl;
	va_start(vl, p);

	// Iterate through the format string to handle each argument.
	for (const char* it = p; *it; ++it) {
		if (*it == 'b') {
			// Handle boolean arguments.
			args->Push(!!va_arg(vl, int));
		} else if (*it == 'i') {
			// Handle integer arguments.
			args->Push(va_arg(vl, int));
		} else {
			// Assume it's a string argument.
			char* s = va_arg(vl, char*);
			args->Push(s, strlen(s));
			break; // Exit after processing the first string argument.
		}
	}

	// Clean up the variable argument list.
	va_end(vl);

	// Call the event test function with the constructed arguments.
	return eventTestAny(szName, args);
}

// This function accumulates results for an event with a variable number of arguments.
int CustomMods::eventAccumulator(int &iValue, const char* szName, const char* p, ...) {
	// Initialize an argument handle for Lua.
	CvLuaArgsHandle args;

	// Initialize the variable argument list.
	va_list vl;
	va_start(vl, p);

	// Iterate through the format string to handle each argument.
	for (const char* it = p; *it; ++it) {
		if (*it == 'b') {
			// Handle boolean arguments.
			args->Push(!!va_arg(vl, int));
		} else if (*it == 'i') {
			// Handle integer arguments.
			args->Push(va_arg(vl, int));
		} else {
			// Assume it's a string argument.
			char* s = va_arg(vl, char*);
			args->Push(s, strlen(s));
			break; // Exit after processing the first string argument.
		}
	}

	// Clean up the variable argument list.
	va_end(vl);

	// Call the event accumulator with the constructed arguments.
	return eventAccumulator(iValue, szName, args);
}

int CustomMods::eventHook(const char* szName, CvLuaArgsHandle &args) {
	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if (pkScriptSystem) {
		bool bResult = false;
		if (LuaSupport::CallHook(pkScriptSystem, szName, args.get(), bResult)) {
			return GAMEEVENTRETURN_HOOK;
		}
	}

	return GAMEEVENTRETURN_NONE;
}

int CustomMods::eventTestAll(const char* szName, CvLuaArgsHandle &args) {
	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if (pkScriptSystem) {
		bool bResult = false;
		if (LuaSupport::CallTestAll(pkScriptSystem, szName, args.get(), bResult)) {
			if (bResult) {
				return GAMEEVENTRETURN_TRUE;
			} else {
				return GAMEEVENTRETURN_FALSE;
			}
		}
	}

	return GAMEEVENTRETURN_NONE;
}

int CustomMods::eventTestAny(const char* szName, CvLuaArgsHandle &args) {
	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if (pkScriptSystem) {
		bool bResult = false;
		if (LuaSupport::CallTestAny(pkScriptSystem, szName, args.get(), bResult)) {
			if (bResult) {
				return GAMEEVENTRETURN_TRUE;
			} else {
				return GAMEEVENTRETURN_FALSE;
			}
		}
	}

	return GAMEEVENTRETURN_NONE;
}

int CustomMods::eventAccumulator(int &iValue, const char* szName, CvLuaArgsHandle &args) {
	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if (pkScriptSystem) {
		if (LuaSupport::CallAccumulator(pkScriptSystem, szName, args.get(), iValue)) {
			return GAMEEVENTRETURN_TRUE;
		}
	}

	return GAMEEVENTRETURN_NONE;
}


// Update CustomModOptions table from references in CustomModPostDefines
// Based on code in CvDllDatabaseUtility::PerformDatabasePostProcessing()
void CustomMods::prefetchCache() {
	Database::Connection* db = GC.GetGameDatabase();
	db->BeginTransaction();

	Database::Results kInsert;
	db->Execute(kInsert, "INSERT OR REPLACE INTO CustomModOptions(Name, Value) VALUES(?, ?)");

	Database::Results kPostDefines;
	db->SelectAll(kPostDefines, "CustomModPostDefines");
	while (kPostDefines.Step()) {
		Database::Results kLookup;
		char szSQL[512];
		sprintf_s(szSQL, "select ROWID from %s where Type = '%s' LIMIT 1", kPostDefines.GetText("Table"), kPostDefines.GetText("Type"));

		if (db->Execute(kLookup, szSQL)) {
			if (kLookup.Step()) {
				kInsert.Bind(1, kPostDefines.GetText("Name"));
				kInsert.Bind(2, kLookup.GetInt(0));
				kInsert.Step();
				kInsert.Reset();
			}
		}
	}

	db->EndTransaction();
}

void CustomMods::preloadCache() {
	prefetchCache();

	(void) getOption("EVENTS_CIRCUMNAVIGATION");
}

void CustomMods::reloadCache() {
	m_bInit = false;

	preloadCache();
}

int CustomMods::getOption(const char* szOption, int defValue) {
	return getOption(string(szOption), defValue);
}

int CustomMods::getOption(const string& sOption, int defValue) {
	if (!m_bInit) {
		const char* szBadPrefix = "MOD_";

		Database::Results kResults;
		DB.SelectAll(kResults, MOD_DB_TABLE);

		while (kResults.Step()) {
			const char* szName = kResults.GetText(MOD_DB_COL_NAME);
			const int iDbUpdates = kResults.GetInt(MOD_DB_COL_DBUPDATES);
			int iValue = kResults.GetInt(MOD_DB_COL_VALUE);

			bool bPrefixError = (strncmp(szName, szBadPrefix, strlen(szBadPrefix)) == 0);

			if (iValue && iDbUpdates) {
				Database::Results kUpdates;
				char szQuery[512] = {0};

				// Did the required mods to the database occur?  We'll assume they didn't, unless proven otherwise!
				bool bOK = false;

				sprintf_s(szQuery, "Name='%s' AND Value=1", szName);
				if (DB.SelectWhere(kUpdates, MOD_DB_UPDATES, szQuery)) {
					if (kUpdates.Step()) {
						// BINGO!  We have our updates
						bOK = true;
					} else {
						// All is not lost as there could be BOTH xml and sql updates
						sprintf_s(szQuery, "Name='%s_SQL' AND Value=1", szName);
						if (DB.SelectWhere(kUpdates, MOD_DB_UPDATES, szQuery)) {
							if (kUpdates.Step()) {
								sprintf_s(szQuery, "Name='%s_XML' AND Value=1", szName);
								if (DB.SelectWhere(kUpdates, MOD_DB_UPDATES, szQuery)) {
									if (kUpdates.Step()) {
										// BINGO!  We have BOTH our updates
										bOK = true;
									}
								}
							}
						}
					}
				}

				if (bOK) {
					CUSTOMLOG("%s: %s appears to have the required database updates", (bPrefixError ? "PREFIX ERROR" : "Cache"), szName);
				} else {
					CUSTOMLOG("%s: %s has missing database updates!", (bPrefixError ? "PREFIX ERROR" : "Cache"), szName);
					iValue = 0;
				}
			}

			CUSTOMLOG("%s: %s = %d", (bPrefixError ? "PREFIX ERROR" : "Cache"), szName, iValue);
			m_options[string(szName)] = iValue;
		}
		MOD_OPT_CACHE(BALANCE_VP);
		MOD_OPT_CACHE(CORE_DEBUGGING);
		MOD_OPT_CACHE(BALANCE_CITY_STATE_TRAITS);
		MOD_OPT_CACHE(BALANCE_CITY_STATE_PERSONALITIES);
		MOD_OPT_CACHE(BALANCE_ENCAMPMENTS_SPAWN_ON_VISIBLE_TILES);
		MOD_OPT_CACHE(BALANCE_PERMANENT_VOTE_COMMITMENTS);
		MOD_OPT_CACHE(UI_DISPLAY_PRECISE_MOVEMENT_POINTS);
		MOD_OPT_CACHE(UI_QUICK_ANIMATIONS);
		MOD_OPT_CACHE(ALTERNATIVE_DIFFICULTY);
		MOD_OPT_CACHE(GLOBAL_STACKING_RULES);
		MOD_OPT_CACHE(GLOBAL_LOCAL_GENERALS);
		MOD_OPT_CACHE(LOCAL_GENERALS_NEAREST_CITY);
		MOD_OPT_CACHE(GLOBAL_SEPARATE_GREAT_ADMIRAL);
		MOD_OPT_CACHE(RIVER_CITY_CONNECTIONS);
		MOD_OPT_CACHE(GLOBAL_PROMOTION_CLASSES);
		MOD_OPT_CACHE(GLOBAL_PASSABLE_FORTS);
		MOD_OPT_CACHE(GLOBAL_ANYTIME_GOODY_GOLD);
		MOD_OPT_CACHE(GLOBAL_CITY_FOREST_BONUS);
		MOD_OPT_CACHE(GLOBAL_CITY_JUNGLE_BONUS);
		MOD_OPT_CACHE(GLOBAL_CITY_WORKING);
		MOD_OPT_CACHE(GLOBAL_CITY_AUTOMATON_WORKERS);
		MOD_OPT_CACHE(GLOBAL_RELOCATION);
		MOD_OPT_CACHE(GLOBAL_ALPINE_PASSES);
		MOD_OPT_CACHE(GLOBAL_CS_GIFT_SHIPS);
		MOD_OPT_CACHE(GLOBAL_CS_UPGRADES);
		MOD_OPT_CACHE(GLOBAL_CS_LIBERATE_AFTER_BUYOUT);
		MOD_OPT_CACHE(GLOBAL_CS_GIFTS);
		MOD_OPT_CACHE(GLOBAL_CS_GIFTS_LOCAL_XP);
		MOD_OPT_CACHE(GLOBAL_CS_OVERSEAS_TERRITORY);
		MOD_OPT_CACHE(GLOBAL_NO_FOLLOWUP_FROM_CITIES);
		MOD_OPT_CACHE(GLOBAL_CAPTURE_AFTER_ATTACKING);
		MOD_OPT_CACHE(GLOBAL_NO_OCEAN_PLUNDERING);
		MOD_OPT_CACHE(GLOBAL_NO_CONQUERED_SPACESHIPS);
		MOD_OPT_CACHE(GLOBAL_GRATEFUL_SETTLERS);
		MOD_OPT_CACHE(GLOBAL_RELIGIOUS_SETTLERS);
		MOD_OPT_CACHE(GLOBAL_QUICK_ROUTES);
		MOD_OPT_CACHE(GLOBAL_SUBS_UNDER_ICE_IMMUNITY);
		MOD_OPT_CACHE(GLOBAL_PARATROOPS_AA_DAMAGE);
		MOD_OPT_CACHE(GLOBAL_NUKES_MELT_ICE); 
		MOD_OPT_CACHE(GLOBAL_GREATWORK_YIELDTYPES); 
		MOD_OPT_CACHE(GLOBAL_NO_LOST_GREATWORKS); 
		MOD_OPT_CACHE(GLOBAL_EXCLUDE_FROM_GIFTS);
		MOD_OPT_CACHE(GLOBAL_MOVE_AFTER_UPGRADE);
		MOD_OPT_CACHE(GLOBAL_CANNOT_EMBARK);
		MOD_OPT_CACHE(GLOBAL_SEPARATE_GP_COUNTERS);
		MOD_OPT_CACHE(GLOBAL_TRULY_FREE_GP);
		MOD_OPT_CACHE(GLOBAL_PURCHASE_FAITH_BUILDINGS_IN_PUPPETS);
		MOD_OPT_CACHE(IMPROVEMENTS_EXTENSIONS);
		MOD_OPT_CACHE(PLOTS_EXTENSIONS);
		MOD_OPT_CACHE(NO_AUTO_SPAWN_PROPHET);
		MOD_OPT_CACHE(ALTERNATE_ASSYRIA_TRAIT);
		MOD_OPT_CACHE(ALTERNATE_SIAM_TRAIT);
		MOD_OPT_CACHE(NO_REPAIR_FOREIGN_LANDS);
		MOD_OPT_CACHE(NO_YIELD_ICE);
		MOD_OPT_CACHE(NO_MAJORCIV_GIFTING);
		MOD_OPT_CACHE(NO_HEALING_ON_MOUNTAINS);
		MOD_OPT_CACHE(PILLAGE_PERMANENT_IMPROVEMENTS);
		MOD_OPT_CACHE(DIPLOMACY_TECH_BONUSES);
		MOD_OPT_CACHE(DIPLOMACY_NO_LEADERHEADS);
		MOD_OPT_CACHE(SHIPS_FIRE_IN_CITIES_IMPROVEMENTS);
		MOD_OPT_CACHE(PSEUDO_NATURAL_WONDER);
		MOD_OPT_CACHE(COMMUNITY_PATCH);
		MOD_OPT_CACHE(BALANCE_CORE);
		MOD_OPT_CACHE(BALANCE_CORE_YIELDS);
		MOD_OPT_CACHE(BALANCE_CORE_SPIES);
		MOD_OPT_CACHE(BALANCE_CORE_MILITARY);
		MOD_OPT_CACHE(BALANCE_CORE_SETTLER_ADVANCED);
		MOD_OPT_CACHE(BALANCE_CORE_MINORS);
		MOD_OPT_CACHE(BALANCE_CORE_UNCAPPED_UNHAPPINESS);
		MOD_OPT_CACHE(BALANCE_CORE_POP_REQ_BUILDINGS);
		MOD_OPT_CACHE(BALANCE_CORE_SETTLER_RESET_FOOD);
		MOD_OPT_CACHE(BALANCE_CORE_WONDERS_VARIABLE_REWARD);
		MOD_OPT_CACHE(BALANCE_CORE_BELIEFS_RESOURCE);
		MOD_OPT_CACHE(BALANCE_CORE_AFRAID_ANNEX);
		MOD_OPT_CACHE(BALANCE_CORE_BUILDING_INSTANT_YIELD);
		MOD_OPT_CACHE(BALANCE_CORE_BELIEFS);
		MOD_OPT_CACHE(BALANCE_CORE_RANDOMIZED_GREAT_PROPHET_SPAWNS);
		MOD_OPT_CACHE(BALANCE_CORE_FOLLOWER_POP_WONDER);
		MOD_OPT_CACHE(BALANCE_CORE_POLICIES);
		MOD_OPT_CACHE(BALANCE_CORE_BARBARIAN_THEFT);
		MOD_OPT_CACHE(BALANCE_CORE_RESOURCE_FLAVORS);
		MOD_OPT_CACHE(BALANCE_CORE_PURCHASE_COST_INCREASE);
		MOD_OPT_CACHE(BALANCE_CORE_PUPPET_PURCHASE);
		MOD_OPT_CACHE(BALANCE_CORE_EMBARK_CITY_NO_COST);
		MOD_OPT_CACHE(BALANCE_CORE_MINOR_PROTECTION);
		MOD_OPT_CACHE(BALANCE_CORE_IDEOLOGY_START);
		MOD_OPT_CACHE(BALANCE_CORE_WONDER_COST_INCREASE);
		MOD_OPT_CACHE(BALANCE_CORE_MINOR_CIV_GIFT);
		MOD_OPT_CACHE(BALANCE_CORE_DIPLOMACY_ERA_INFLUENCE);
		MOD_OPT_CACHE(BALANCE_CORE_LUXURIES_TRAIT);
		MOD_OPT_CACHE(BALANCE_CORE_MILITARY_PROMOTION_ADVANCED);
		MOD_OPT_CACHE(BALANCE_CORE_MILITARY_LOGGING);
		MOD_OPT_CACHE(BALANCE_CORE_UNIT_CREATION_DAMAGED);
		MOD_OPT_CACHE(UNIT_SUPPLY_MINORS_USE_HANDICAP);
		MOD_OPT_CACHE(BALANCE_CORE_RESOURCE_MONOPOLIES);
		MOD_OPT_CACHE(BALANCE_CORE_RESOURCE_MONOPOLIES_STRATEGIC);
		MOD_OPT_CACHE(BALANCE_CORE_BUILDING_INVESTMENTS);
		MOD_OPT_CACHE(BALANCE_CORE_UNIT_INVESTMENTS);
		MOD_OPT_CACHE(BALANCE_CORE_BUILDING_RESOURCE_MAINTENANCE);
		MOD_OPT_CACHE(BALANCE_CORE_ENGINEER_HURRY);
		MOD_OPT_CACHE(BALANCE_CORE_MAYA_CHANGE);
		MOD_OPT_CACHE(BALANCE_CORE_PORTUGAL_CHANGE);
		MOD_OPT_CACHE(BALANCE_CORE_MINOR_VARIABLE_BULLYING);
		MOD_OPT_CACHE(BALANCE_CORE_MINOR_PTP_MINIMUM_VALUE);
		MOD_OPT_CACHE(BALANCE_YIELD_SCALE_ERA);
		MOD_OPT_CACHE(BALANCE_CORE_NEW_GP_ATTRIBUTES);
		MOD_OPT_CACHE(BALANCE_CORE_JFD);
		MOD_OPT_CACHE(BALANCE_CORE_MILITARY_RESISTANCE);
		MOD_OPT_CACHE(BALANCE_CORE_MILITARY_RESOURCES);
		MOD_OPT_CACHE(BALANCE_CORE_PANTHEON_RESET_FOUND);
		MOD_OPT_CACHE(BALANCE_CORE_VICTORY_GAME_CHANGES);
		MOD_OPT_CACHE(BALANCE_CORE_EVENTS);
		MOD_OPT_CACHE(NO_RANDOM_TEXT_CIVS);
		MOD_OPT_CACHE(BALANCE_RETROACTIVE_PROMOS);
		MOD_OPT_CACHE(BALANCE_NO_GAP_DURING_GA);
		MOD_OPT_CACHE(BALANCE_CORE_SETTLERS_CONSUME_POP);
		MOD_OPT_CACHE(BALANCE_CORE_UNIQUE_BELIEFS_ONLY_FOR_CIV);
		MOD_OPT_CACHE(BALANCE_DEFENSIVE_PACTS_AGGRESSION_ONLY);
		MOD_OPT_CACHE(BALANCE_CORE_SCALING_TRADE_DISTANCE);
		MOD_OPT_CACHE(BALANCE_CORE_SCALING_XP);
		MOD_OPT_CACHE(BALANCE_CORE_HALF_XP_PURCHASE);
		MOD_OPT_CACHE(BALANCE_CORE_XP_ON_FIRST_ATTACK);
		MOD_OPT_CACHE(BALANCE_CORE_QUEST_CHANGES);
		MOD_OPT_CACHE(BALANCE_CORE_PUPPET_CHANGES);
		MOD_OPT_CACHE(BALANCE_CORE_CITY_DEFENSE_SWITCH);
		MOD_OPT_CACHE(BALANCE_CORE_ARCHAEOLOGY_FROM_GP);
		MOD_OPT_CACHE(BALANCE_CORE_GOLD_INTERNAL_TRADE_ROUTES);
		MOD_OPT_CACHE(BALANCE_CORE_BOMBARD_RANGE_BUILDINGS);
		MOD_OPT_CACHE(BALANCE_CORE_TOURISM_HUNDREDS);
		MOD_OPT_CACHE(BALANCE_FLIPPED_TOURISM_MODIFIER_OPEN_BORDERS);
		MOD_OPT_CACHE(BALANCE_CORE_RANGED_ATTACK_PENALTY);
		MOD_OPT_CACHE(BALANCE_CORE_INQUISITOR_TWEAKS);
		MOD_OPT_CACHE(CORE_DISABLE_LUA_HOOKS);
		MOD_OPT_CACHE(CORE_AREA_EFFECT_PROMOTIONS);
		MOD_OPT_CACHE(BALANCE_CORE_GOODY_RECON_ONLY);
		MOD_OPT_CACHE(YIELD_MODIFIER_FROM_UNITS);

		MOD_OPT_CACHE(ALTERNATE_CELTS);
		MOD_OPT_CACHE(CIV6_WORKER);
		MOD_OPT_CACHE(CIV6_ROADS);
		MOD_OPT_CACHE(CIV6_EUREKAS);
		MOD_OPT_CACHE(CITY_STATE_SCALE);
		MOD_OPT_CACHE(ANY_PANTHEON);
		MOD_OPT_CACHE(CARGO_SHIPS);
		MOD_OPT_CACHE(BARBARIAN_GG_GA_POINTS);
		MOD_OPT_CACHE(TRAITS_CROSSES_ICE);
		MOD_OPT_CACHE(TRAITS_CITY_WORKING);
		MOD_OPT_CACHE(TRAITS_CITY_AUTOMATON_WORKERS);
		MOD_OPT_CACHE(TRAITS_OTHER_PREREQS);
		MOD_OPT_CACHE(TRAITS_ANY_BELIEF);
		MOD_OPT_CACHE(TRAITS_TRADE_ROUTE_BONUSES);
		MOD_OPT_CACHE(TRAITS_TRADE_ROUTE_PRODUCTION_SIPHON);
		MOD_OPT_CACHE(TRAITS_YIELD_FROM_ROUTE_MOVEMENT_IN_FOREIGN_TERRITORY);
		MOD_OPT_CACHE(SANE_UNIT_MOVEMENT_COST);

		MOD_OPT_CACHE(POLICIES_CITY_WORKING);
		MOD_OPT_CACHE(POLICIES_CITY_AUTOMATON_WORKERS);
		MOD_OPT_CACHE(POLICIES_UNIT_CLASS_REPLACEMENTS);
		MOD_OPT_CACHE(ERA_RESTRICTION);
		MOD_OPT_CACHE(ERA_RESTRICTED_GENERALS);
		MOD_OPT_CACHE(NEW_GOODIES);
		MOD_OPT_CACHE(USE_TRADE_FEATURES);
		MOD_OPT_CACHE(TECHS_CITY_WORKING);
		MOD_OPT_CACHE(TECHS_CITY_AUTOMATON_WORKERS);

		MOD_OPT_CACHE(PROMOTIONS_GG_FROM_BARBARIANS);
		MOD_OPT_CACHE(PROMOTIONS_VARIABLE_RECON);
		MOD_OPT_CACHE(PROMOTIONS_CROSS_MOUNTAINS);
		MOD_OPT_CACHE(PROMOTIONS_CROSS_OCEANS);
		MOD_OPT_CACHE(PROMOTIONS_CROSS_ICE);
		MOD_OPT_CACHE(PROMOTIONS_HALF_MOVE);
		MOD_OPT_CACHE(PROMOTIONS_DEEP_WATER_EMBARKATION);
		MOD_OPT_CACHE(PROMOTIONS_FLAGSHIP);
		MOD_OPT_CACHE(PROMOTIONS_UNIT_NAMING);
		MOD_OPT_CACHE(PROMOTIONS_IMPROVEMENT_BONUS);

		MOD_OPT_CACHE(UI_CITY_PRODUCTION);
		MOD_OPT_CACHE(UI_CITY_EXPANSION);
		MOD_OPT_CACHE(WH_MILITARY_LOG);

		MOD_OPT_CACHE(BUILDINGS_NW_EXCLUDE_RAZING);
		MOD_OPT_CACHE(BUILDINGS_PRO_RATA_PURCHASE);
		MOD_OPT_CACHE(BUILDINGS_CITY_WORKING);
		MOD_OPT_CACHE(BUILDINGS_CITY_AUTOMATON_WORKERS);
		MOD_OPT_CACHE(BUILDINGS_THOROUGH_PREREQUISITES);

		MOD_OPT_CACHE(TRADE_ROUTE_SCALING);
		MOD_OPT_CACHE(TRADE_WONDER_RESOURCE_ROUTES);

		MOD_OPT_CACHE(UNITS_HOVERING_LAND_ONLY_HEAL);
		MOD_OPT_CACHE(UNITS_RESOURCE_QUANTITY_TOTALS);

		MOD_OPT_CACHE(RELIGION_NO_PREFERRENCES);
		MOD_OPT_CACHE(RELIGION_RANDOMISE);
		MOD_OPT_CACHE(RELIGION_CONVERSION_MODIFIERS);
		MOD_OPT_CACHE(RELIGION_KEEP_PROPHET_OVERFLOW);
		MOD_OPT_CACHE(RELIGION_ALLIED_INQUISITORS);
		MOD_OPT_CACHE(RELIGION_RECURRING_PURCHASE_NOTIFY);
		MOD_OPT_CACHE(RELIGION_POLICY_BRANCH_FAITH_GP);
		MOD_OPT_CACHE(RELIGION_LOCAL_RELIGIONS);
		MOD_OPT_CACHE(RELIGION_PASSIVE_SPREAD_WITH_CONNECTION_ONLY);
		MOD_OPT_CACHE(RELIGION_EXTENSIONS);
		MOD_OPT_CACHE(RELIGION_PERMANENT_PANTHEON);
		MOD_OPT_CACHE(CORE_REDUCE_NOTIFICATIONS);
		MOD_OPT_CACHE(CORE_NO_RANGED_ATTACK_FROM_CITIES);
		MOD_OPT_CACHE(CORE_RELAXED_BORDER_CHECK);

		MOD_OPT_CACHE(PROCESS_STOCKPILE);

		MOD_OPT_CACHE(RESOURCES_PRODUCTION_COST_MODIFIERS);

		MOD_OPT_CACHE(CIVILIZATIONS_UNIQUE_PROCESSES);
		
		MOD_OPT_CACHE(EVENTS_TERRAFORMING);
		MOD_OPT_CACHE(EVENTS_TILE_IMPROVEMENTS);
		MOD_OPT_CACHE(EVENTS_TILE_REVEALED);
		MOD_OPT_CACHE(EVENTS_CIRCUMNAVIGATION);
		MOD_OPT_CACHE(EVENTS_NEW_ERA);
		MOD_OPT_CACHE(EVENTS_NW_DISCOVERY);
		MOD_OPT_CACHE(EVENTS_DIPLO_EVENTS);
		MOD_OPT_CACHE(EVENTS_DIPLO_MODIFIERS);
		MOD_OPT_CACHE(EVENTS_MINORS);
		MOD_OPT_CACHE(EVENTS_MINORS_GIFTS);
		MOD_OPT_CACHE(EVENTS_MINORS_INTERACTION);
		MOD_OPT_CACHE(EVENTS_QUESTS);
		MOD_OPT_CACHE(EVENTS_BARBARIANS);
		MOD_OPT_CACHE(EVENTS_GOODY_CHOICE);
		MOD_OPT_CACHE(EVENTS_GOODY_TECH);
		MOD_OPT_CACHE(EVENTS_AI_OVERRIDE_TECH);
		MOD_OPT_CACHE(EVENTS_GREAT_PEOPLE);
		MOD_OPT_CACHE(EVENTS_FOUND_RELIGION);
		MOD_OPT_CACHE(EVENTS_ACQUIRE_BELIEFS);
		MOD_OPT_CACHE(EVENTS_RELIGION);
		MOD_OPT_CACHE(EVENTS_ESPIONAGE);
		MOD_OPT_CACHE(EVENTS_PLOT);
		MOD_OPT_CACHE(EVENTS_PLAYER_TURN);
		MOD_OPT_CACHE(EVENTS_GOLDEN_AGE);
		MOD_OPT_CACHE(EVENTS_CITY);
		MOD_OPT_CACHE(EVENTS_CITY_CAPITAL);
		MOD_OPT_CACHE(EVENTS_CITY_BORDERS);
		MOD_OPT_CACHE(EVENTS_CITY_FOUNDING);
		MOD_OPT_CACHE(EVENTS_LIBERATION);
		MOD_OPT_CACHE(EVENTS_CITY_RAZING);
		MOD_OPT_CACHE(EVENTS_CITY_AIRLIFT);
		MOD_OPT_CACHE(EVENTS_CITY_BOMBARD);
		MOD_OPT_CACHE(EVENTS_CITY_CONNECTIONS);
		MOD_OPT_CACHE(EVENTS_AREA_RESOURCES);
		MOD_OPT_CACHE(EVENTS_PARADROPS);
		MOD_OPT_CACHE(EVENTS_UNIT_RANGEATTACK);
		MOD_OPT_CACHE(EVENTS_UNIT_CREATED);
		MOD_OPT_CACHE(EVENTS_UNIT_FOUNDED);
		MOD_OPT_CACHE(EVENTS_UNIT_PREKILL);
		MOD_OPT_CACHE(EVENTS_UNIT_CAPTURE);
		MOD_OPT_CACHE(EVENTS_CAN_MOVE_INTO);
		MOD_OPT_CACHE(EVENTS_UNIT_ACTIONS);
		MOD_OPT_CACHE(EVENTS_UNIT_UPGRADES);
		MOD_OPT_CACHE(EVENTS_UNIT_CONVERTS);
		MOD_OPT_CACHE(EVENTS_UNIT_DATA);
		MOD_OPT_CACHE(EVENTS_WAR_AND_PEACE);
		MOD_OPT_CACHE(EVENTS_TRADE_ROUTES);
		MOD_OPT_CACHE(EVENTS_TRADE_ROUTE_PLUNDERED);
		MOD_OPT_CACHE(EVENTS_RESOLUTIONS);
		MOD_OPT_CACHE(EVENTS_IDEOLOGIES);
		MOD_OPT_CACHE(EVENTS_NUCLEAR_DETONATION);
		MOD_OPT_CACHE(EVENTS_AIRLIFT);
		MOD_OPT_CACHE(EVENTS_REBASE);
		MOD_OPT_CACHE(EVENTS_COMMAND);
		MOD_OPT_CACHE(EVENTS_CUSTOM_MISSIONS);
		MOD_OPT_CACHE(EVENTS_BATTLES);
		MOD_OPT_CACHE(EVENTS_BATTLES_DAMAGE);
		MOD_OPT_CACHE(EVENTS_GAME_SAVE);

		MOD_OPT_CACHE(EVENTS_RED_TURN);
		MOD_OPT_CACHE(EVENTS_RED_COMBAT);
		MOD_OPT_CACHE(EVENTS_RED_COMBAT_MISSION);
		MOD_OPT_CACHE(EVENTS_RED_COMBAT_ABORT);
		MOD_OPT_CACHE(EVENTS_RED_COMBAT_RESULT);
		MOD_OPT_CACHE(EVENTS_RED_COMBAT_ENDED);
		MOD_OPT_CACHE(ACTIVE_DIPLOMACY);
		MOD_OPT_CACHE(EXE_HACKING);
		MOD_OPT_CACHE(API_ACHIEVEMENTS);

		MOD_OPT_CACHE(ISKA_HERITAGE);
		MOD_OPT_CACHE(ISKA_PANTHEONS);
		MOD_OPT_CACHE(ISKA_GAMEOPTIONS);
		MOD_OPT_CACHE(ISKA_GOLDENAGEPOINTS_TO_PRESTIGE);

		MOD_OPT_CACHE(BATTLE_ROYALE);

		MOD_OPT_CACHE(CORE_TWO_PASS_DANGER);
		MOD_OPT_CACHE(CORE_DELAYED_VISIBILITY);
		MOD_OPT_CACHE(CORE_HOVERING_UNITS);
		MOD_OPT_CACHE(CORE_RESILIENT_PANTHEONS);

		MOD_OPT_CACHE(UNIT_KILL_STATS);

		MOD_OPT_CACHE(AI_UNIT_PRODUCTION);
		MOD_OPT_CACHE(ADJACENT_BLOCKADE);
		MOD_OPT_CACHE(ATTRITION);
		MOD_OPT_CACHE(CIVILIANS_RETREAT_WITH_MILITARY);
		MOD_OPT_CACHE(LINKED_MOVEMENT);
		MOD_OPT_CACHE(GP_ERA_SCALING);
		MOD_OPT_CACHE(SQUADS);
		MOD_OPT_CACHE(NOT_FOR_SALE);
		MOD_OPT_CACHE(NOTIFICATION_SETTINGS);
		MOD_OPT_CACHE(LOG_MAP_STATE);
		MOD_OPT_CACHE(ROUTE_PLANNER);

		m_bInit = true;
	}

	if (m_options.find(sOption) == m_options.end()) {
		return defValue;
	}

	return m_options[sOption];
}

int CustomMods::getCivOption(const char* szCiv, const char* szName, int defValue) {
	return getOption(string(szCiv) + "_" + szName, getOption(szName, defValue));
}

void CheckSentinel(uint value)
{
    if (value == 0xDEADBEEF)
        return; //everything ok

    CUSTOMLOG("Deserialization Error\n");

#if defined(VPDEBUG)
    char debugMsg[256];
    sprintf(debugMsg, "Sentinel value mismatch: Expected 0xDEADBEEF, Got 0x%08X\n", value);
    OutputDebugString(debugMsg);
#endif
}
