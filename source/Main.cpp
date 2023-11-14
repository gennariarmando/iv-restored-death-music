#include "plugin.h"
#include "audFrontendAudioEntity.h"
#include "CGeneral.h"

using namespace plugin;

class RestoredDeathMusicIV {
public:
    static void TempHackyPlayDeathMusic(int32_t i) {
        const char* prev = plugin::patch::Get<const char*>(0x8E4803 + 1);
        char buf[32];
        sprintf(buf, "DEATH_MUSIC_%02d", i);
        plugin::patch::Set<const char*>(0x8E4803 + 1, buf);

        FrontendAudioEntity.StartLoadingTune();

        plugin::patch::Set<const char*>(0x8E4803 + 1, prev);
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
