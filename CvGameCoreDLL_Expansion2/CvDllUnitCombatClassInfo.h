/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#pragma once
#include "CvDllInterfaces.h"

class CvBaseInfo;

class CvDllUnitCombatClassInfo : public ICvUnitCombatClassInfo1
{
public:
	CvDllUnitCombatClassInfo(_In_ CvBaseInfo* pUnitCombatClassInfo);
	virtual ~CvDllUnitCombatClassInfo();

	void* DLLCALL QueryInterface(GUID guidInterface);

	unsigned int IncrementReference();
	unsigned int DecrementReference();
	unsigned int GetReferenceCount() const;

	static void operator delete(void* p);
	static void* operator new(size_t bytes);

	CvBaseInfo* GetInstance();

	const char* DLLCALL GetType();
	const char* DLLCALL GetDescription();

private:
	void DLLCALL Destroy();

	unsigned int m_uiRefCount;
	CvBaseInfo* m_pUnitCombatClassInfo;
};