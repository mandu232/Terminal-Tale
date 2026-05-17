#include "Context.h"
#include "State.h"
#include "StateMachine.h"

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

// ── ResetGameState ────────────────────────────────────────────────────────────
void Context::ResetGameState()
{
	player        = PlayerStats{};
	flags         = {};
	currentNodeId = "";
	log           = {};
	journal       = {};
}

// ── SaveSlot ─────────────────────────────────────────────────────────────────
bool Context::SaveSlot(int slot) const
{
	std::filesystem::create_directories("Data/saves");

	json j;

	// 플레이어 능력치
	j["player"]["vitality"]   = player.vitality;
	j["player"]["reputation"] = player.reputation;
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
	player.vitality   = p.value("vitality"  , 10);
	player.reputation = p.value("reputation", 0);
	player.wealth     = p.value("wealth"    , 0);
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

	return true;
}

// ── HasSaveSlot ───────────────────────────────────────────────────────────────
bool Context::HasSaveSlot(int slot) const
{
	return std::filesystem::exists(SlotPath(slot));
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
	if (stateMachine)
		stateMachine->ChangeState(std::move(state));
}

void Context::PushState(std::unique_ptr<State> state)
{
	if (stateMachine)
		stateMachine->PushState(std::move(state));
}

void Context::PopState()
{
	if (stateMachine)
		stateMachine->PopState();
}

void Context::ToggleFullscreen(bool mode)
{
	display.SetFullscreen(mode);
}