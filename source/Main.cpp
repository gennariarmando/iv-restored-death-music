#include "plugin.h"
#include "audFrontendAudioEntity.h"
#include "CGeneral.h"

using namespace plugin;

class RestoredDeathMusicIV {
public:
    static void TempHackyPlayDeathMusic(int32_t i) {
        static auto pattern = plugin::pattern::Get("68 ? ? ? ? 8B CF C7 44 24 ? ? ? ? ? E8 ? ? ? ? 83 3E 00 74 2A 6A FF 6A 01 68 ? ? ? ? E8 ? ? ? ? 8B 0E 83 C4 04 50 E8 ? ? ? ? FF 35 ? ? ? ? 8B 0D ? ? ? ? E8 ? ? ? ? 5E 5F 83 C4 48 C3 CC CC CC CC 83 EC 48", 1);
        const char* prev = plugin::patch::Get<const char*>(pattern);
        char buf[32];
        sprintf(buf, "DEATH_MUSIC_%02d", i);
        plugin::patch::Set<const char*>(pattern, buf);

        FrontendAudioEntity.StartLoadingTune();

        plugin::patch::Set<const char*>(pattern, prev);
    }

    RestoredDeathMusicIV() {
        static bool justDied = false;
        plugin::Events::gameProcessEvent += []() {
            auto playa = FindPlayerPed(0);
            if (!playa)
                return;

            if (playa->m_nPedState == PEDSTATE_DIE) {
                if (!justDied) {
                    TempHackyPlayDeathMusic(CGeneral::GetRandomNumberInRange(1, 6));
                    justDied = true;
                }
            }
            else {
                justDied = false;
            }
        };
    }
} restoredDeathMusicIV;
