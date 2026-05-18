#pragma once
#include "Achievement.h"
#include <vector>
#include <string>

class Context;

class AchievementManager
{
public:
    void Init();

    // id 업적을 해제. 새로 해제됐으면 true 반환.
    bool Unlock(const std::string& id, Context& ctx);

    bool IsUnlocked(const std::string& id) const;

    // 스탯 변화 후 조건 기반 업적 자동 체크
    void CheckStatAchievements(Context& ctx);

    const std::vector<Achievement>& GetAll() const { return achievements_; }
    int UnlockedCount() const;

private:
    void Load();
    void Save() const;

    std::vector<Achievement> achievements_;

    static constexpr const char* kSavePath = "Data/saves/achievements.json";
};
