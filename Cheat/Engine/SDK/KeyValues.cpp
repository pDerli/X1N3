#include "SDK.h"
#include "../../Main.h"
template <typename T>
static constexpr auto relativeToAbsolute(int* address) noexcept
{
    return reinterpret_cast<T>(reinterpret_cast<char*>(address + 1) + *address);
}

namespace SDK
{
    KeyValues* KeyValues::FromString(const char* name, const char* value)
    {
        const auto pFromString = relativeToAbsolute<decltype(keyValuesFromString)>(reinterpret_cast<int*>(Utils::PatternScan(clientFactory, XorStr("E8 ? ? ? ? 83 C4 04 89 45 D8")) + 1)); //need fix
        KeyValues* keyValues;
        __asm 
        {
            push 0
            mov edx, value
            mov ecx, name
            call pFromString
            add esp, 4
            mov keyValues, eax
        }

        return keyValues;
    }

    KeyValues* KeyValues::FindKey(const char* keyName, bool create)
    {
       //static auto pFindKey = reinterpret_cast<KeyValues*(__thiscall*)(KeyValues*, const char*, bool)>(Utils::PatternScan(clientFactory, XorStr("E8 ? ? ? ? F7 45")) + 1);//need fix
        auto pFindKey = relativeToAbsolute<decltype(keyValuesFindKey)>(reinterpret_cast<int*>(Utils::PatternScan(clientFactory, XorStr("E8 ? ? ? ? F7 45")) + 1));
        return pFindKey(this, keyName, create);
    }

    void KeyValues::SetString(const char* keyName, const char* value)
    {
        //static auto pSetSring = reinterpret_cast<void(__thiscall*)(KeyValues*, const char*)>(Utils::PatternScan(clientFactory, XorStr("E8 ? ? ? ? 89 77 38")) + 1);//need fix
        auto pSetSring = relativeToAbsolute<decltype(keyValuesSetString)>(reinterpret_cast<int*>(Utils::PatternScan(clientFactory, XorStr("E8 ? ? ? ? 89 77 38")) + 1));
        if (const auto key = FindKey(keyName, true))
            pSetSring(key, value);
    }
}                                                       