#include "Context.h"
#include "State.h"
#include "StateMachine.h"
#include "Localization.h"

#include "external/json/json.hpp"
#include <fstream>
#include <filesystem>
#include <algorithm>

using json = nlohmann::json;

// ── 경로 헬퍼 ────────────────────────────────────────────────────────────────
std::string Context::SlotPath(int slot)
{
	return "Data/saves/slot_" + std::to_string(slot) + ".json";
}

// ── AddLog ───────────────────────────────────────────────────────────────────
void Context::AddLog(const std::string& text)
{
	log.push_back({ text, player.day, player.time });
}

// ── AddJournal ───────────────────────────────────────────────────────────────
void Context::AddJournal(const std::string& id,
                         const std::string& title,
                         const std::string& content)
{
	auto it = std::find_if(journal.begin(), journal.end(),
		[&id](const JournalEntry& e) { return e.id == id; });

	if (it != journal.end())
	{
		it->title   = title;
		it->content = content;
		it->day     = player.day;
	}
	else
	{
		journal.push_back({ id, title, /*outcome=*/"", content, /*description=*/"", player.day });
	}
}

// ── AddCaseRecord ─────────────────────────────────────────────────────────────
void Context::AddCaseRecord(const std::string& id,
                             const std::string& title,
                             const std::string& outcome,
                             const std::string& content,
                             const std::string& description)
{
	auto it = std::find_if(journal.begin(), journal.end(),
		[&id](const JournalEntry& e) { return e.id == id; });

	JournalEntry entry{ id, title, outcome, content, description, player.day };

	if (it != journal.end())
		*it = entry;
	else
		journal.push_back(entry);
}

// ── RevealCharacter ───────────────────────────────────────────────────────────
void Context::RevealCharacter(const std::string& id)
{
	auto it = std::find_if(characters.begin(), characters.end(),
		[&id](const CharacterEntry& e) { return e.id == id; });
	if (it != characters.end()) return;  // 이미 등록됨

	const std::string base = "char." + id;
	CharacterEntry entry;
	entry.id           = id;
	entry.name         = L(base + ".name");
	entry.role         = L(base + ".role");
	entry.affiliation  = L(base + ".affiliation");
	entry.relationship = L(base + ".relationship");
	entry.description  = L(base + ".desc");
	entry.revealedDay  = player.day;
	characters.push_back(std::move(entry));
}

// ── ResetGameState ────────────────────────────────────────────────────────────
void Context::ResetGameState()
{
	player        = PlayerStats{};
	flags         = {};
	currentNodeId = "";
	log           = {};
	journal       = {};
	characters    = {};
}

// ── SaveSlot ─────────────────────────────────────────────────────────────────
bool Context::SaveSlot(int slot) const
{
	std::filesystem::create_directories("Data/saves");

	json j;

	// 플레이어 능력치
	j["player"]["fatigue"]    = player.fatigue;
	j["player"]["monitoring"] = player.monitoring;
	j["player"]["wealth"]     = player.wealth;
	j["player"]["day"]        = player.day;
	j["player"]["time"]       = player.time;

	j["player"]["cityOrder"]    = player.cityOrder;
	j["player"]["citizenTrust"] = player.citizenTrust;
	j["player"]["corruption"]   = player.corruption;

	j["player"]["empathy"]    = player.empathy;
	j["player"]["coldness"]   = player.coldness;
	j["player"]["justice"]    = player.justice;
	j["player"]["compliance"] = player.compliance;
	j["player"]["suspicion"]  = player.suspicion;

	json inv = json::object();
	for (const auto& [id, qty] : player.inventory)
		inv[id] = qty;
	j["player"]["inventory"] = inv;

	// 플래그
	j["flags"] = json::array();
	for (const auto& f : flags)
		j["flags"].push_back(f);

	// 현재 노드
	j["currentNodeId"] = currentNodeId;

	// 로그
	j["log"] = json::array();
	for (const auto& e : log)
	{
		json entry;
		entry["text"] = e.text;
		entry["day"]  = e.day;
		entry["time"] = e.time;
		j["log"].push_back(entry);
	}

	// 저널
	j["journal"] = json::array();
	for (const auto& e : journal)
	{
		json entry;
		entry["id"]          = e.id;
		entry["title"]       = e.title;
		entry["outcome"]     = e.outcome;
		entry["content"]     = e.content;
		entry["description"] = e.description;
		entry["day"]         = e.day;
		j["journal"].push_back(entry);
	}

	// 인물
	j["characters"] = json::array();
	for (const auto& e : characters)
	{
		json entry;
		entry["id"]           = e.id;
		entry["name"]         = e.name;
		entry["role"]         = e.role;
		entry["affiliation"]  = e.affiliation;
		entry["relationship"] = e.relationship;
		entry["description"]  = e.description;
		entry["revealedDay"]  = e.revealedDay;
		j["characters"].push_back(entry);
	}

	std::ofstream file(SlotPath(slot));
	if (!file.is_open()) return false;
	file << j.dump(4);
	return true;
}

// ── LoadSlot ─────────────────────────────────────────────────────────────────
bool Context::LoadSlot(int slot)
{
	std::ifstream file(SlotPath(slot));
	if (!file.is_open()) return false;

	json j;
	try { file >> j; }
	catch (...) { return false; }

	// 플레이어 능력치
	const auto& p    = j["player"];
	player.fatigue    = p.value("fatigue"    , 0);
	player.monitoring = p.value("monitoring" , 0);
	player.wealth     = p.value("wealth"     , 0);
	player.day        = p.value("day"       , 0);
	player.time       = p.value("time"      , 0);

	player.cityOrder    = p.value("cityOrder"   , 50);
	player.citizenTrust = p.value("citizenTrust", 50);
	player.corruption   = p.value("corruption"  , 0);

	player.empathy    = p.value("empathy"   , 0);
	player.coldness   = p.value("coldness"  , 0);
	player.justice    = p.value("justice"   , 0);
	player.compliance = p.value("compliance", 0);
	player.suspicion  = p.value("suspicion" , 0);

	player.inventory.clear();
	if (p.contains("inventory"))
		for (const auto& [id, qty] : p["inventory"].items())
			player.inventory[id] = qty.get<int>();

	// 플래그
	flags.clear();
	for (const auto& f : j.value("flags", json::array()))
		flags.insert(f.get<std::string>());

	// 현재 노드
	currentNodeId = j.value("currentNodeId", "");

	// 로그
	log.clear();
	for (const auto& e : j.value("log", json::array()))
		log.push_back({ e.value("text",""), e.value("day",0), e.value("time",0) });

	// 저널
	journal.clear();
	for (const auto& e : j.value("journal", json::array()))
		journal.push_back({
			e.value("id"         , ""),
			e.value("title"      , ""),
			e.value("outcome"    , ""),
			e.value("content"    , ""),
			e.value("description", ""),
			e.value("day"        , 0)
		});

	// 인물
	characters.clear();
	for (const auto& e : j.value("characters", json::array()))
		characters.push_back({
			e.value("id"          , ""),
			e.value("name"        , ""),
			e.value("role"        , ""),
			e.value("affiliation" , ""),
			e.value("relationship", ""),
			e.value("description" , ""),
			e.value("revealedDay" , 0)
		});

	return true;
}

// ── HasSaveSlot ───────────────────────────────────────────────────────────────
bool Context::HasSaveSlot(int slot) const
{
	return std::filesystem::exists(SlotPath(slot));
}

// ── QuickSave / QuickLoad ─────────────────────────────────────────────────────
static std::string QuickSlotPath(int activeSlot, int quickSlot)
{
	return "Data/saves/quick/slot_" + std::to_string(activeSlot)
	     + "/quick_" + std::to_string(quickSlot) + ".json";
}

bool Context::SaveQuick(int slot) const
{
	if ( activeSlot <= 0 ) return false;

	const std::string dir = "Data/saves/quick/slot_" + std::to_string(activeSlot);
	std::filesystem::create_directories(dir);

	json j;

	j["player"]["fatigue"]      = player.fatigue;
	j["player"]["monitoring"]   = player.monitoring;
	j["player"]["wealth"]       = player.wealth;
	j["player"]["day"]          = player.day;
	j["player"]["time"]         = player.time;
	j["player"]["cityOrder"]    = player.cityOrder;
	j["player"]["citizenTrust"] = player.citizenTrust;
	j["player"]["corruption"]   = player.corruption;
	j["player"]["empathy"]      = player.empathy;
	j["player"]["coldness"]     = player.coldness;
	j["player"]["justice"]      = player.justice;
	j["player"]["compliance"]   = player.compliance;
	j["player"]["suspicion"]    = player.suspicion;

	json inv = json::object();
	for (const auto& [id, qty] : player.inventory)
		inv[id] = qty;
	j["player"]["inventory"] = inv;

	j["flags"] = json::array();
	for (const auto& f : flags)
		j["flags"].push_back(f);

	j["currentNodeId"] = currentNodeId;

	j["log"] = json::array();
	for (const auto& e : log)
	{
		json entry;
		entry["text"] = e.text;
		entry["day"]  = e.day;
		entry["time"] = e.time;
		j["log"].push_back(entry);
	}

	j["journal"] = json::array();
	for (const auto& e : journal)
	{
		json entry;
		entry["id"]          = e.id;
		entry["title"]       = e.title;
		entry["outcome"]     = e.outcome;
		entry["content"]     = e.content;
		entry["description"] = e.description;
		entry["day"]         = e.day;
		j["journal"].push_back(entry);
	}

	j["characters"] = json::array();
	for (const auto& e : characters)
	{
		json entry;
		entry["id"]           = e.id;
		entry["name"]         = e.name;
		entry["role"]         = e.role;
		entry["affiliation"]  = e.affiliation;
		entry["relationship"] = e.relationship;
		entry["description"]  = e.description;
		entry["revealedDay"]  = e.revealedDay;
		j["characters"].push_back(entry);
	}

	std::ofstream file(QuickSlotPath(activeSlot, slot));
	if (!file.is_open()) return false;
	file << j.dump(4);
	return true;
}

bool Context::LoadQuick(int slot)
{
	if ( activeSlot <= 0 ) return false;

	std::ifstream file(QuickSlotPath(activeSlot, slot));
	if (!file.is_open()) return false;

	json j;
	try { file >> j; }
	catch (...) { return false; }

	const auto& p    = j["player"];
	player.fatigue    = p.value("fatigue"    , 0);
	player.monitoring = p.value("monitoring" , 0);
	player.wealth     = p.value("wealth"     , 0);
	player.day        = p.value("day"        , 0);
	player.time       = p.value("time"      , 0);
	player.cityOrder    = p.value("cityOrder"   , 50);
	player.citizenTrust = p.value("citizenTrust", 50);
	player.corruption   = p.value("corruption"  , 0);
	player.empathy    = p.value("empathy"   , 0);
	player.coldness   = p.value("coldness"  , 0);
	player.justice    = p.value("justice"   , 0);
	player.compliance = p.value("compliance", 0);
	player.suspicion  = p.value("suspicion" , 0);

	player.inventory.clear();
	if (p.contains("inventory"))
		for (const auto& [id, qty] : p["inventory"].items())
			player.inventory[id] = qty.get<int>();

	flags.clear();
	for (const auto& f : j.value("flags", json::array()))
		flags.insert(f.get<std::string>());

	currentNodeId = j.value("currentNodeId", "");

	log.clear();
	for (const auto& e : j.value("log", json::array()))
		log.push_back({ e.value("text",""), e.value("day",0), e.value("time",0) });

	journal.clear();
	for (const auto& e : j.value("journal", json::array()))
		journal.push_back({
			e.value("id"         , ""),
			e.value("title"      , ""),
			e.value("outcome"    , ""),
			e.value("content"    , ""),
			e.value("description", ""),
			e.value("day"        , 0)
		});

	characters.clear();
	for (const auto& e : j.value("characters", json::array()))
		characters.push_back({
			e.value("id"          , ""),
			e.value("name"        , ""),
			e.value("role"        , ""),
			e.value("affiliation" , ""),
			e.value("relationship", ""),
			e.value("description" , ""),
			e.value("revealedDay" , 0)
		});

	return true;
}

bool Context::HasQuickSave(int slot) const
{
	if ( activeSlot <= 0 ) return false;
	return std::filesystem::exists(QuickSlotPath(activeSlot, slot));
}

Context::SlotInfo Context::GetQuickSlotInfo(int slot) const
{
	SlotInfo info;
	if ( activeSlot <= 0 ) return info;
	std::ifstream file(QuickSlotPath(activeSlot, slot));
	if (!file.is_open()) return info;

	json j;
	try { file >> j; } catch (...) { return info; }

	info.exists = true;
	if (j.contains("player"))
	{
		info.day  = j["player"].value("day" , 0);
		info.time = j["player"].value("time", 0);
	}
	info.nodeId = j.value("currentNodeId", "");
	return info;
}

// ── GetSlotInfo ───────────────────────────────────────────────────────────────
Context::SlotInfo Context::GetSlotInfo(int slot) const
{
	SlotInfo info;
	std::ifstream file(SlotPath(slot));
	if (!file.is_open()) return info;

	json j;
	try { file >> j; } catch (...) { return info; }

	info.exists = true;
	if (j.contains("player"))
	{
		info.day  = j["player"].value("day" , 0);
		info.time = j["player"].value("time", 0);
	}
	info.nodeId = j.value("currentNodeId", "");
	return info;
}

// ── StateMachine Helpers ─────────────────────────────────────────────────────
void Context::ChangeState(std::unique_ptr<State> state)
{
	pendingStateOps.push_back({ PendingStateOp::Type::Change, std::move(state) });
}

void Context::PushState(std::unique_ptr<State> state)
{
	pendingStateOps.push_back({ PendingStateOp::Type::Push, std::move(state) });
}

void Context::PopState()
{
	pendingStateOps.push_back({ PendingStateOp::Type::Pop, nullptr });
}

void Context::ToggleFullscreen(bool mode)
{
	display.SetFullscreen(mode);
}