#include "Tables.h"

int __fastcall hkDoPostScreenEffects(void* ecx, int edx, int a1)
{
	static auto oDoPostScreenEffects = HookTables::pDoPostScreenEffects->GetTrampoline();

	if (GP_Esp && CGlobal::IsGameReady && !CGlobal::FullUpdateCheck)
		GP_Esp->DrawGlow();

	return oDoPostScreenEffects(ecx, a1);
}

EGCResults __fastcall hkRetrieveMessage(void* ecx, void* edx, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	EGCResults status = HookTables::pRetrieveMessage->GetTrampoline()(ecx, punMsgType, pubDest, cubDest, pcubMsgSize);

	if (status != k_EGCResultOK)
		return status;

	uint32_t messageType = *punMsgType & 0x7FFFFFFF;

	GP_Inventory->PostRetrieveMessage(punMsgType, pubDest, cubDest, pcubMsgSize);

	return status;
}

EGCResults __fastcall hkSendMessage(void* ecx, void* edx, uint32_t unMsgType, const void* pubData, uint32_t cubData)
{
	EGCResults status;
	uint32_t messageType = unMsgType & 0x7FFFFFFF;
	void* pubDataMutable = const_cast<void*>(pubData);

	status = HookTables::pSendMessage->GetTrampoline()(ecx, unMsgType, pubDataMutable, cubData);

	GP_Inventory->PreSendMessage(unMsgType, pubDataMutable, cubData);

	if (status != k_EGCResultOK)
		return status;

	return status;
}