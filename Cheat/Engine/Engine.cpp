#include "Engine.h"
#include "../Hacks/Setup.h"

bool CGlobal::IsGuiVisble = false;
bool CGlobal::IsGameReady = false;
bool CGlobal::AcceptMatchBeep = false;
int CGlobal::iScreenWidth = 1920;
int CGlobal::iScreenHeight = 1080;
string CGlobal::SystemDisk = "C:\\";
WEAPON_TYPE CGlobal::GWeaponType = WEAPON_TYPE::WEAPON_TYPE_UNKNOWN;
WEAPON_ID CGlobal::GWeaponID = WEAPON_ID::WEAPON_AK47;
float CGlobal::GFovView = 90;
float CGlobal::GFovViewExt = 90;
bool CGlobal::FullUpdateCheck = false;
Vector CGlobal::GViewAngle;
vector<string> CGlobal::ConfigList;
CBaseEntity* CGlobal::LocalPlayer;
CUserCmd* CGlobal::UserCmd;
bool CGlobal::bSendPacket;
QAngle CGlobal::RealAngle;
QAngle CGlobal::FakeAngle;

ConVar* CGlobal::viewmodel_offset_convar_x = new ConVar();
ConVar* CGlobal::viewmodel_offset_convar_y = new ConVar();
ConVar* CGlobal::viewmodel_offset_convar_z = new ConVar();
int CGlobal::old_viewmodel_offset_x = 1000;
int CGlobal::old_viewmodel_offset_y = 1000;
int CGlobal::old_viewmodel_offset_z = 1000;

void ReadConfigs(LPCTSTR lpszFileName)
{
	CGlobal::ConfigList.push_back(lpszFileName);
}

int CGlobal::WeaponItemIndex[34] =
{
	// ��������� - 0 - 9
	WEAPON_DEAGLE,WEAPON_ELITE,WEAPON_FIVESEVEN,
	WEAPON_GLOCK,WEAPON_HKP2000,WEAPON_P250,
	WEAPON_USP_SILENCER,WEAPON_CZ75A,WEAPON_REVOLVER,
	WEAPON_TEC9,
	// �������� - 10 - 30
	WEAPON_AK47,WEAPON_AUG,WEAPON_FAMAS,WEAPON_GALILAR,
	WEAPON_M249,WEAPON_M4A4,WEAPON_M4A1_SILENCER,WEAPON_MAC10,
	WEAPON_P90,WEAPON_UMP45,WEAPON_MP5,WEAPON_XM1014,WEAPON_BIZON,
	WEAPON_MAG7,WEAPON_NEGEV,WEAPON_SAWEDOFF,
	WEAPON_MP7,WEAPON_MP9,WEAPON_NOVA,WEAPON_SG553,
	WEAPON_SCAR20,WEAPON_G3SG1,
	// ���������  - 31 - 32
	WEAPON_AWP,WEAPON_SSG08
};

string CGlobal::WeaponNames[34] =
{
	M_WEAPON_DEAGLE_STR,M_WEAPON_ELITE_STR,M_WEAPON_FIVESEVEN_STR,
	M_WEAPON_GLOCK_STR,M_WEAPON_HKP2000_STR,M_WEAPON_P250_STR,
	M_WEAPON_USP_S_STR,M_WEAPON_CZ75A_STR,M_WEAPON_REVOLVER_STR,
	M_WEAPON_TEC9_STR,
	M_WEAPON_AK47_STR,M_WEAPON_AUG_STR,M_WEAPON_FAMAS_STR,M_WEAPON_GALILAR_STR,
	M_WEAPON_M249_STR,M_WEAPON_M4A4_STR,M_WEAPON_M4A1_S_STR,M_WEAPON_MAC10_STR,
	M_WEAPON_P90_STR,M_WEAPON_UMP45_STR,M_WEAPON_MP5_STR,M_WEAPON_XM1014_STR,M_WEAPON_BIZON_STR,
	M_WEAPON_MAG7_STR,M_WEAPON_NEGEV_STR,M_WEAPON_SAWEDOFF_STR,
	M_WEAPON_MP7_STR,M_WEAPON_MP9_STR,M_WEAPON_NOVA_STR,M_WEAPON_SG553_STR,
	M_WEAPON_SCAR20_STR,M_WEAPON_G3SG1_STR,
	M_WEAPON_AWP_STR,M_WEAPON_SSG08_STR,
};
int CGlobal::GetWeaponId()
{
	if (GP_EntPlayers && GP_EntPlayers->EntityLocal)
	{
		for (size_t i = 0; i < sizeof(WeaponItemIndex) / sizeof(*WeaponItemIndex); i++)
		{
			if (WeaponItemIndex[i] == GP_EntPlayers->EntityLocal->WeaponIdx)
				return i;
		}
	}
	return 0;
}

WEAPON_TYPE CGlobal::GetWeaponType(CBaseWeapon* pWeaponEntity)
{
	if (!pWeaponEntity)
		return WEAPON_TYPE_UNKNOWN;

	if (!*pWeaponEntity->GeteAttributableItem()->GetItemDefinitionIndex())
		return WEAPON_TYPE_UNKNOWN;

	int iItemDefinitionIndex = *pWeaponEntity->GeteAttributableItem()->GetItemDefinitionIndex();
	switch (iItemDefinitionIndex)
	{
	case WEAPON_DEAGLE:
		return WEAPON_TYPE_PISTOL;
	case WEAPON_ELITE:
		return WEAPON_TYPE_PISTOL;
	case WEAPON_FIVESEVEN:
		return WEAPON_TYPE_PISTOL;
	case WEAPON_GLOCK:
		return WEAPON_TYPE_PISTOL;
	case WEAPON_AK47:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_AUG:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_AWP:
		return WEAPON_TYPE_SNIPER;
	case WEAPON_FAMAS:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_G3SG1:
		return WEAPON_TYPE_SNIPER;
	case WEAPON_GALILAR:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_M249:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_M4A4:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_MAC10:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_P90:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_UMP45:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_MP5:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_XM1014:
		return WEAPON_TYPE_SHOTGUN;
	case WEAPON_BIZON:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_MAG7:
		return WEAPON_TYPE_SHOTGUN;
	case WEAPON_NEGEV:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_SAWEDOFF:
		return WEAPON_TYPE_SHOTGUN;
	case WEAPON_TEC9:
		return WEAPON_TYPE_PISTOL;
	case WEAPON_TASER:
		return WEAPON_TYPE_PISTOL;
	case WEAPON_HKP2000:
		return WEAPON_TYPE_PISTOL;
	case WEAPON_MP7:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_MP9:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_NOVA:
		return WEAPON_TYPE_SHOTGUN;
	case WEAPON_P250:
		return WEAPON_TYPE_PISTOL;
	case WEAPON_SCAR20:
		return WEAPON_TYPE_SNIPER;
	case WEAPON_SG553:
		return WEAPON_TYPE_SNIPER;
	case WEAPON_SSG08:
		return WEAPON_TYPE_SNIPER;
	case WEAPON_KNIFE:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_FLASHBANG:
		return WEAPON_TYPE_GRENADE;
	case WEAPON_HEGRENADE:
		return WEAPON_TYPE_GRENADE;
	case WEAPON_SMOKEGRENADE:
		return WEAPON_TYPE_GRENADE;
	case WEAPON_MOLOTOV:
		return WEAPON_TYPE_GRENADE;
	case WEAPON_DECOY:
		return WEAPON_TYPE_GRENADE;
	case WEAPON_INCGRENADE:
		return WEAPON_TYPE_GRENADE;
	case WEAPON_C4:
		return WEAPON_TYPE_C4;
	case WEAPON_KNIFE_T:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_M4A1_SILENCER:
		return WEAPON_TYPE_RIFEL;
	case WEAPON_USP_SILENCER:
		return WEAPON_TYPE_PISTOL;
	case WEAPON_CZ75A:
		return WEAPON_TYPE_PISTOL;
	case WEAPON_REVOLVER:
		return WEAPON_TYPE_PISTOL;
	case WEAPON_KNIFE_CSS:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_BAYONET:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_FLIP:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_GUT:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_KARAMBIT:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_M9_BAYONET:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_TACTICAL:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_FALCHION:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_BUTTERFLY:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_PUSH:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_CORD:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_CANIS:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_URSUS:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_OUTDOOR:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_STILETTO:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_WIDOWMAKER:
		return WEAPON_TYPE_KNIFE;
	case WEAPON_KNIFE_SKELETON:
		return WEAPON_TYPE_KNIFE;
	default:
		return WEAPON_TYPE_UNKNOWN;
	}
}

int CGlobal::GetBestHeadAngle(float yaw)
{
	float Back, Right, Left;

	Vector src3D, dst3D, forward, right, up, src, dst;
	trace_t tr;
	Ray_t ray, ray2, ray3, ray4, ray5;
	CTraceFilter filter;

	QAngle engineViewAngles;

	engineViewAngles.x = 0;
	engineViewAngles.y = yaw;

	AngleVectors(engineViewAngles, forward, right, up);

	filter.pSkip = CGlobal::LocalPlayer;
	src3D = CGlobal::LocalPlayer->GetEyePosition();
	dst3D = src3D + (forward * 384);

	ray.Init(src3D, dst3D);

	I::EngineTrace()->TraceRay(ray, MASK_SHOT, &filter, &tr);

	Back = (tr.endpos - tr.startpos).Length();

	ray2.Init(src3D + right * 35, dst3D + right * 35);

	I::EngineTrace()->TraceRay(ray2, MASK_SHOT, &filter, &tr);

	Right = (tr.endpos - tr.startpos).Length();

	ray3.Init(src3D - right * 35, dst3D - right * 35);

	I::EngineTrace()->TraceRay(ray3, MASK_SHOT, &filter, &tr);

	Left = (tr.endpos - tr.startpos).Length();

	static int result = 0;

	if (Left > Right)
	{
		result = -1;
	}
	else if (Right > Left)
	{
		result = 1;
	}

	return result;
}