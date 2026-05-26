#include "AchievementManager.h"
#include "Core/Context.h"
#include "Game/Player/PlayerStatus.h"
#include "external/json/json.hpp"

#include <fstream>
#include <filesystem>
#include <algorithm>

using json = nlohmann::json;

// ── 정의 ──────────────────────────────────────────────────────────────────────
void AchievementManager::Init()
{
    achievements_ =
    {
        // ── 엔딩 ──────────────────────────────────────────────────────────────
        { "ending_alliance",    "ach.ending_alliance.name",    "ach.ending_alliance.desc"    },
        { "ending_compliance",  "ach.ending_compliance.name",  "ach.ending_compliance.desc"  },
        { "ending_resistance",  "ach.ending_resistance.name",  "ach.ending_resistance.desc"  },
        { "ending_retreat",     "ach.ending_retreat.name",     "ach.ending_retreat.desc"     },
        { "ending_all",         "ach.ending_all.name",         "ach.ending_all.desc"         },

        // ── 성향 수치 ──────────────────────────────────────────────────────────
        { "coldness_100",   "ach.coldness_100.name",   "ach.coldness_100.desc"   },
        { "empathy_100",    "ach.empathy_100.name",    "ach.empathy_100.desc"    },
        { "justice_100",    "ach.justice_100.name",    "ach.justice_100.desc"    },
        { "compliance_100", "ach.compliance_100.name", "ach.compliance_100.desc" },
        { "suspicion_100",  "ach.suspicion_100.name",  "ach.suspicion_100.desc"  },

        // ── 도시 수치 ──────────────────────────────────────────────────────────
        { "corruption_80",  "ach.corruption_80.name",  "ach.corruption_80.desc"  },
        { "trust_80",       "ach.trust_80.name",       "ach.trust_80.desc"       },

        // ── 기타 ───────────────────────────────────────────────────────────────
        { "first_duty",       "ach.first_duty.name",       "ach.first_duty.desc"       },
        { "vitality_min",     "ach.vitality_min.name",     "ach.vitality_min.desc"     },
        { "monitoring_high",  "ach.monitoring_high.name",  "ach.monitoring_high.desc"  },
    };

    Load();
}

// ── 해제 ──────────────────────────────────────────────────────────────────────
bool AchievementManager::Unlock(const std::string& id, Context& ctx)
{
    auto it = std::find_if(achievements_.begin(), achievements_.end(),
        [&id](const Achievement& a) { return a.id == id; });

    if ( it == achievements_.end() || it->unlocked )
        return false;

    it->unlocked = true;
    Save();

    // 4개 엔딩 모두 달성 시 "모든 결말" 자동 해제
    static const std::vector<std::string> endings = {
        "ending_alliance", "ending_compliance",
        "ending_resistance", "ending_retreat"
    };
    bool allDone = std::all_of(endings.begin(), endings.end(),
        [this](const std::string& eid) { return IsUnlocked(eid); });
    if ( allDone )
        Unlock("ending_all", ctx);

    return true;
}

bool AchievementManager::IsUnlocked(const std::string& id) const
{
    auto it = std::find_if(achievements_.begin(), achievements_.end(),
        [&id](const Achievement& a) { return a.id == id; });
    return it != achievements_.end() && it->unlocked;
}

// ── 스탯 조건 체크 ────────────────────────────────────────────────────────────
void AchievementManager::CheckStatAchievements(Context& ctx)
{
    const auto& p = ctx.player;

    if ( p.coldness   >= 100 ) Unlock("coldness_100",   ctx);
    if ( p.empathy    >= 100 ) Unlock("empathy_100",    ctx);
    if ( p.justice    >= 100 ) Unlock("justice_100",    ctx);
    if ( p.compliance >= 100 ) Unlock("compliance_100", ctx);
    if ( p.suspicion  >= 100 ) Unlock("suspicion_100",  ctx);

    if ( p.corruption   >= 80 ) Unlock("corruption_80", ctx);
    if ( p.citizenTrust >= 80 ) Unlock("trust_80",      ctx);

    if ( p.fatigue     >= PlayerStats::kMaxFatigue    ) Unlock("vitality_min",    ctx);
    if ( p.monitoring  >= 80                          ) Unlock("monitoring_high", ctx);
}

int AchievementManager::UnlockedCount() const
{
    return static_cast<int>(std::count_if(achievements_.begin(), achievements_.end(),
        [](const Achievement& a) { return a.unlocked; }));
}

// ── 파일 I/O ──────────────────────────────────────────────────────────────────
void AchievementManager::Load()
{
    std::ifstream file(kSavePath);
    if ( !file.is_open() ) return;

    json j;
    try { file >> j; } catch ( ... ) { return; }

    for ( const auto& id : j.value("unlocked", json::array()) )
    {
        std::string sid = id.get<std::string>();
        auto it = std::find_if(achievements_.begin(), achievements_.end(),
            [&sid](const Achievement& a) { return a.id == sid; });
        if ( it != achievements_.end() )
            it->unlocked = true;
    }
}

void AchievementManager::Save() const
{
    std::filesystem::create_directories("Data/saves");

    json j;
    j["unlocked"] = json::array();
    for ( const auto& a : achievements_ )
        if ( a.unlocked )
            j["unlocked"].push_back(a.id);

    std::ofstream file(kSavePath);
    if ( file.is_open() )
        file << j.dump(4);
}
