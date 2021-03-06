#include "cheats.hpp"
#include "values.h"

namespace CTRPluginFramework
{
	
	u32 offset = 0;
	u32 random = 0;
	u32 dataX = 0;
	u32 dataY = 0;
	u32 dataZ = 0;
	u32 data = 0;
	u32 g_racePointer;
	u32 g_raceCondition;
	u32 g_FNsPointer;
	u32 g_oldRacePointer5CC;
	u32 g_oldRacePointer5D0;
	u32 g_itemPointer;
	static u32 x = 0;
	static u32 y = 0;
	static u32 z = 0;
	
	/////////////////////////////////////////////////////////    Start of custom functions    /////////////////////////////////////////////////////////

	bool is_in_range(u32 value, u32 lowerbound, u32 upperbound)
	{
		return (value > lowerbound && value < upperbound);
	}

	bool IsInRace()
	{
		u32 offset;
		if (Process::Read32(0x14000084, offset) && is_in_range(offset, 0x14000000, 0x18000000))
		{
			if (Process::Read32(offset + 0x316C, offset) && is_in_range(offset, 0x14000000, 0x18000000))
			{
				if (Process::Read32(offset + 0x118, offset) && (offset & 0xFF) == 1)
					return true;
			}
		}
		return false;
	}

	u32 GetRacePointer()
	{
		bool in_race = IsInRace();
		u32 pointer;
		if (in_race)
		{
			if (Process::Read32(0x140002F4, pointer) && is_in_range(pointer, 0x14000000, 0x18000000))
			{
				if (Process::Read32(pointer + 0x14, pointer) && is_in_range(pointer, 0x14000000, 0x18000000))
				{
					if (Process::Read32(pointer + 0x518, pointer) && is_in_range(pointer, 0x14000000, 0x18000000))
					{
						if (Process::Read32(pointer + 0x1C, pointer) && is_in_range(pointer, 0x14000000, 0x18000000))
							return pointer;
					}
				}
			}
		}
		return 0;
	}

	u32	GetFNsPointer()
	{
		u32 g_FNsPointer;
		bool in_race = IsInRace();
		if (in_race && Process::Read32(0xFFFF6F0, g_FNsPointer) && is_in_range(g_FNsPointer, 0x14000000, 0x18000000))
			return (g_FNsPointer + 8);
		return 0;
	}

	u32	GetOldPointer5CC()
	{
		u32 g_oldRacePointer5CC;
		bool in_race = IsInRace();
		if (in_race && Process::Read32(0xFFFFBF4, g_oldRacePointer5CC) && is_in_range(g_oldRacePointer5CC, 0x14000000, 0x18000000))
		{
			if (Process::Read32(g_oldRacePointer5CC + 0x5CC, g_oldRacePointer5CC) && g_oldRacePointer5CC > 0x14000000 && g_oldRacePointer5CC < 0x18000000)
				return g_oldRacePointer5CC;
		}
		return 0;
	}

	u32	GetOldPointer5D0()
	{
		u32 g_oldRacePointer5D0;
		bool in_race = IsInRace();
		if (in_race && Process::Read32(0xFFFFBF4, g_oldRacePointer5D0) && is_in_range(g_oldRacePointer5D0, 0x14000000, 0x18000000))
		{
			if (Process::Read32(g_oldRacePointer5D0 + 0x5D0, g_oldRacePointer5D0) && is_in_range(g_oldRacePointer5D0, 0x14000000, 0x18000000))
				return g_oldRacePointer5D0;
		}
		return 0;
	}

	u32	GetItemPointer()
	{
		u32 g_itemPointer;
		bool in_race = IsInRace();
		if (in_race && Process::Read32(0x14000074, g_itemPointer) && is_in_range(g_itemPointer, 0x14000000, 0x18000000))
		{
			if (Process::Read32(g_itemPointer - 0x1B5C, g_itemPointer) && is_in_range(g_itemPointer, 0x14000000, 0x18000000))
			{
				if (Process::Read32(g_itemPointer + 0x27AC, g_itemPointer) && is_in_range(g_itemPointer, 0x14000000, 0x18000000))
					return g_itemPointer;
			}
		}
		return 0;
	}

	u8 GetTotalPlayers()
	{		u32 pointer;
		u8 total;
		if (Process::Read32(0x14000074, pointer) && is_in_range(pointer, 0x14000000, 0x18000000))
		{
			if (Process::Read32(pointer - 0xC10, pointer) && is_in_range(pointer, 0x14000000, 0x18000000))
			{
				if (Process::Read32(pointer + 0x23C, pointer) && is_in_range(pointer, 0x14000000, 0x18000000))
				{
					if (Process::Read8(pointer - 0xF28, total) && is_in_range(pointer, 0x14000000, 0x18000000))
					{
						return total;
					}
				}
			}
		}
	}

	u16	GetTime()
	{
		u32 pointer = GetOldPointer5D0();
		u16 time = 0;
		if (is_in_range(pointer, 0x14000000, 0x18000000))
		{
			Process::Read16(pointer + 0x484, time);
			time /= 60;
			if (300 - time < 0)
				return 0;
			else
				return 300 - time;
		}
	}

	u16	GetMinutes()
	{
		u16 time = GetTime(), minutes = 0;
		while (time - 60 >= 0)
		{
			time -= 60;
			minutes++;
		}
		return minutes;
	}

	u16	GetSeconds()
	{
		u16 time = GetTime();
		while (time - 60 >= 0)
			time -= 60;
		return time;
	}

	void	SubToTime(u16 seconds)
	{
		u32 pointer = GetOldPointer5D0(), g_racePointer = GetRacePointer();
		u16 time = 0;
		if (is_in_range(pointer, 0x14000000, 0x18000000))
		{
			Process::Read16(pointer + 0x484, time);
			time -= (seconds * 60);
			Process::Write16(pointer + 0x484, time);
			Process::Write16(g_racePointer + 0xC4, time);
		}
	}

	void	writeItem(u32 item)
	{
		u32 g_itemPointer = GetItemPointer();
		bool in_race = IsInRace();
		if (in_race && is_in_range(g_itemPointer, 0x14000000, 0x18000000))
		{
			Process::Write32(g_itemPointer + 0x3C, 0xFFFFFFFF);
			Process::Write32(g_itemPointer + 0xA8, 3);
			Process::Write32(g_itemPointer + 0xC8, item);
			Process::Write32(g_itemPointer + 0xD8, 0x3F800000);
		}
	}

	void	writeSpeed(u32 speed)
	{
		for (int i = 0; i < 0x2D; i++)
		{
			if (Process::Read32(0x140002F4, offset) && is_in_range(offset, 0x14000000, 0x18000000) && Process::Read32(offset - 0xA4, offset) && is_in_range(offset, 0x14000000, 0x18000000))
			{
				Process::Write32((offset - 0x2C3B0) + (i * 4), speed);
				Process::Write32((offset - 0x28E90) + (i * 4), speed);
				Process::Write32((offset - 0x1C730) + (i * 4), speed);
			}
		}
	}

	void	writeVR(u32 vr)
	{
		if (Process::Read32(0x663D04, offset) && is_in_range(offset, 0x10000000, 0x18000000))
			Process::Write32(offset - 0xE30, vr);
	}

	void	writeLocation(u32 location)
	{
		if (Process::Read32(0x6673C8, offset) && is_in_range(offset, 0x10000000, 0x18000000))
			Process::Write32(offset + 0xF7CC, location);
	}

	void	writeFlag(u16 flag)
	{
		if (Process::Read32(0x6673C8, offset) && is_in_range(offset, 0x10000000, 0x18000000))
		{
			Process::Write8(offset + 0xE7CA, flag);
			Process::Write8(offset + 0xF7D0, flag);
		}
	}

	/////////////////////////////////////////////////////////    Start of race codes    /////////////////////////////////////////////////////////

	void invincible(MenuEntry *entry)
	{
		u32 g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (in_race)
		{
			Process::Write16(g_racePointer + 0x102C, 0xFFFF);
		}
	}

	void alwaysStarPower(MenuEntry *entry)
	{
		u32 g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (in_race)
		{
			Process::Write16(g_racePointer + 0xFF4, 0xFFFF);
		}
	}

	void trickAnywhere(MenuEntry *entry)
	{
		u32 g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (in_race)
		{
			Process::Write16(g_racePointer + 0xFF0, 0xFFFF);
		}
	}

	void alwaysBlackKart(MenuEntry *entry)
	{
		u32 g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (in_race)
		{
			Process::Write16(g_racePointer + 0xFF8, 0xFFFF);
		}
	}

	void alwaysShocked(MenuEntry *entry)
	{
		u32 g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (in_race)
		{
			Process::Write16(g_racePointer + 0x1000, 0xFFFF);
		}
	}

	void alwaysCrushed(MenuEntry *entry)
	{
		u32 g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (in_race)
		{
			Process::Write16(g_racePointer + 0x1000, 0xFFFF);
		}
	}

	void	instantMT(MenuEntry *entry)
	{
		Process::Write32(0x665634, 0);
		Process::Write32(0x665638, 0);
	}

	void	hugeBunnyhop(MenuEntry *entry)
	{
		Process::Write32(0x6655D0, 0x40400000);
	}

	void	fastReverse(MenuEntry *entry)
	{
		Process::Write32(0x6655A8, 0x41800000);
	}

	void	noCountdown(MenuEntry *entry)
	{
		bool in_race = IsInRace();
		if (in_race && Process::Read32(0x65C528, data) && is_in_range(data, 0x14000000, 0x18000000))
		{
			Process::Write8(data + 0x109, 1);
			Process::Write8(data + 0x589, 1);
		}
	}

	void	moonjump(MenuEntry *entry)
	{
		unsigned int g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (in_race && Controller::IsKeysDown(Start + R))
		{
			Process::Write32(g_racePointer + 0x3C, 0x41200000);
		}
	}

	void	saveSlotTeleporter(MenuEntry *entry)
	{
		u32 g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		static u32 x = 0, y = 0, z = 0;
		if (in_race)
		{
			if (Controller::IsKeysDown(Start + X))
			{
				Process::Read32(g_racePointer + 0x24, x);
				Process::Read32(g_racePointer + 0x28, y);
				Process::Read32(g_racePointer + 0x2C, z);
			}
			if (Controller::IsKeysDown(Start + Y))
			{
				Process::Write32(g_racePointer + 0x24, x);
				Process::Write32(g_racePointer + 0x28, y);
				Process::Write32(g_racePointer + 0x2C, z);
			}
		}
	}

	void	maxTimer(MenuEntry *entry)
	{
		bool in_race = IsInRace();
		if (in_race && Process::Read32(0x65C528, offset) && is_in_range(offset, 0x14000000, 0x18000000))
		{
			Process::Write32(offset + 0x80, 0x4650);
		}
	}

	void	zeroTimer(MenuEntry *entry)
	{
		bool in_race = IsInRace();
		if (in_race && Process::Read32(0x65C528, offset) && is_in_range(offset, 0x14000000, 0x18000000))
		{
			Process::Write32(offset + 0x80, 0);
		}
	}

	void	waterEverywhere(MenuEntry *entry)
	{
		bool in_race = IsInRace();
		if (in_race && Process::Read32(0x663954, offset) && is_in_range(offset, 0x14000000, 0x18000000) && Process::Read32(offset + 0x58, offset) && is_in_range(offset, 0x14000000, 0x18000000))
		{
			Process::Write32(offset + 0x420, 0x48000000);
		}
	}

	void	driveAnywhere(MenuEntry *entry)
	{
		u32 g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		static bool enabled = false;
		if (!in_race)
		{
			enabled = false;
			Process::Write32(0x6656D8, 0x14);
			return;
		}
		else
		{
			if (Controller::IsKeyDown(Start))
			{
				enabled = false;
				Process::Write32(0x6656D8, 0x14);
				return;
			}
			if (!enabled && Controller::IsKeyDown(Select))
			{
				enabled = true;
				Process::Write32(0x6656D8, 0xFFFF);
			}
			if (enabled)
			{
				if (Controller::IsKeysDown(Select + DPadUp))
				{
					Process::Write32(g_racePointer + 0x3C, 0x41000000);
					return;
				}
				if (Controller::IsKeysDown(Select + DPadDown))
				{
					Process::Write32(g_racePointer + 0x3C, 0xC1000000);
					return;
				}
				Process::Write32(g_racePointer + 0x3C, 0);
			}
		}
	}

	void	stalking(MenuEntry *entry)
	{
		u32 g_racePointer = GetRacePointer(), tempActive = 0;
		static unsigned int pointer = 0, active = 0, player = 1;
		float dataY = 0, dataZ = 0;
		static bool held = false;
		bool in_race = IsInRace();
		if (!in_race)
		{
			player = 1;
			active = 0;
			pointer = 0;
			return;
		}
		else
		{
			if (!Controller::IsKeyDown(Y))
			{
				held = false;
			}
			else
			{
				tempActive = 1;
				if (Controller::IsKeyDown(DPadUp))
				{
					active = 1;
				}
				if (Controller::IsKeyDown(DPadDown))
				{
					active = 0;
					player = 1;
				}
				if (!held)
				{
					if (Controller::IsKeyDown(DPadRight))
					{
						held = true;
						player++;
					}
					if (Controller::IsKeyDown(DPadLeft))
					{
						held = true;
						player--;
					}
				}
			}
			if (active == 1 || tempActive == 1)
			{
				if (player > 8 || player < 1)
				{
					player = 1;
					return;
				}
				Process::Read32(0x65DA44, data);
				pointer = 0x209C + data + (player * 0x44);
				if (Process::Read32(pointer, offset) && is_in_range(offset, 0x18000000, 0x14000000) || offset == g_racePointer || Process::Read32(offset + 0x24, offset) && is_in_range(offset, 0xD0000000, 0x30000000))
				{
					player++;
					return;
				}
				if (player > 0 && player < 9 && Process::Read32(pointer, offset) && is_in_range(offset, 0x14000000, 0x18000000) && is_in_range(g_racePointer, 0x14000000, 0x18000000))
				{
					Process::ReadFloat(offset + 0x28, dataY);
					dataY += 40;
					Process::ReadFloat(offset + 0x2C, dataZ);
					if (dataY != 0 && dataY != 40 && dataZ != 0)
					{
						memcpy((void *)(g_racePointer), (void*)(offset), 0x28);
						Process::WriteFloat(g_racePointer + 0x28, dataY);
						Process::WriteFloat(g_racePointer + 0x2C, dataZ);
					}
					else
					{
						player++;
					}
				}
			}
		}
	}

	void	cpuBrawl(MenuEntry *entry)
	{
		u32 pointer = 0, pointer2 = 0, temp = 0;
		bool in_race = IsInRace();
		if (!in_race)
			return;
		for (int i = 2; i < 9; i++)
		{
			Process::Read32(0x65DA44, temp);
			Process::Read32(0x209C + temp + (i * 0x44), pointer);
			Process::Read32(0x209C + temp + ((i + 1) * 0x44), pointer2);
			if (is_in_range(pointer, 0x14000000, 0x18000000) && is_in_range(pointer2, 0x14000000, 0x18000000))
			{
				memcpy((void *)(pointer2 + 0x20), (void*)(pointer + 0x20), 24);
			}
		}
	}

	void sizeChanger(MenuEntry *entry) // causes problems sometimes
	{
		u32 g_racePointer = GetRacePointer();
		static float PlayerSize = 1.f, speed = 0.03f;
		static bool adding = true, held = false;
		bool in_race = IsInRace();
		if (in_race && is_in_range(g_racePointer, 0x14000000, 0x18000000))
		{
			if (PlayerSize < 3.f && adding)
			{
				PlayerSize += speed;
			}
			if (PlayerSize > 0.1f && !adding)
			{
				PlayerSize -= speed;
			}
			if (PlayerSize > 3.f && adding)
			{
				adding = false;
				PlayerSize -= speed;
			}
			if (PlayerSize < 0.1f && !adding)
			{
				adding = true;
				PlayerSize += speed;
			}
			Process::WriteFloat(g_racePointer + 0x100C, PlayerSize);
		}
		if (!held && in_race && -0.015f < speed && speed < 0.15f && speed != 0.15f && Controller::IsKeyDown(DPadRight))
		{
			held = true;
			speed += 0.01f;
		}
		if (!held && in_race && -0.015f < speed && speed < 0.15f && speed != -0.15f && Controller::IsKeyDown(DPadLeft))
		{
			held = true;
			speed -= 0.01f;
		}
		if (!held && in_race && Controller::IsKeyDown(DPadDown))
		{
			held = true;
			speed = 0;
		}
		if (!Controller::IsKeyDown(DPadRight) && !Controller::IsKeyDown(DPadLeft))
		{
			held = false;
		}
	}

	void	TouchCode(MenuEntry *entry)
	{
		u32 g_racePointer = GetRacePointer(), map_pointer = 0;
		bool in_race = IsInRace();
		if (Process::Read32(0x6789C8, data) && Process::Read32(data + 0x1A48, data) && data == 1 && in_race && is_in_range(g_racePointer, 0x14000000, 0x18000000))
		{
			unsigned int touch_pointer = *(unsigned int *)0x6789C8;
			float X_Axis = *(float *)(touch_pointer + 0x1BCC);
			float Y_Axis = *(float *)(touch_pointer + 0x1BD0);
			if (Process::Read32(0x140002F4, map_pointer) && Process::Read32(map_pointer + 0x14, map_pointer) && Process::Read32(map_pointer + 0x370, map_pointer) && Process::Read32(map_pointer + 0x270, map_pointer) && Process::Read32(map_pointer + 0x60, map_pointer) && Process::Read32(map_pointer + 0x688, map_pointer))
			{
				float MapStatic1 = *(float *)map_pointer;
				float Map_Scale1 = *(float *)(map_pointer + 4);
				float MapStatic2 = *(float *)(map_pointer + 8);
				float Map_Scale2 = *(float *)(map_pointer + 12);
				Map_Scale1 -= Map_Scale2;
				Map_Scale1 /= 240.f;
				X_Axis *= Map_Scale1;
				Y_Axis *= Map_Scale1;
				X_Axis -= ((320.f * Map_Scale1) - MapStatic2);
				Y_Axis += Map_Scale2;
				*(float *)(g_racePointer + 0x3C) = 0;
				*(float *)(g_racePointer + 0x24) = X_Axis;
				*(float *)(g_racePointer + 0x2C) = Y_Axis;
			}
		}
	}

	void	fastBlueShell(MenuEntry *entry)
	{
		Process::Write32(0x666094, 0x43000000);
	}

	void	stoppedBlueShell(MenuEntry *entry)
	{
		Process::Write32(0x666094, 0);
	}

	void	fastGreenShell(MenuEntry *entry)
	{
		Process::Write32(0x66619C, 0x43000000);
	}

	void	stoppedGreenShell(MenuEntry *entry)
	{
		Process::Write32(0x66619C, 0);
	}

	void	dropMushrooms(MenuEntry *entry)
	{
		u32 g_oldRacePointer5D0 = GetOldPointer5D0();
		bool in_race = IsInRace();
		if (in_race && is_in_range(g_oldRacePointer5D0, 0x14000000, 0x18000000))
		{
			Process::Read32(g_oldRacePointer5D0 + 0x1F8, data);
			Process::Write32(g_oldRacePointer5D0 + 0x1D0, data);
		}
	}

	void	bulletControl(MenuEntry *entry)
	{
		u32 g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (in_race && is_in_range(g_racePointer, 0x14000000, 0x18000000))
		{
			Process::Write8(g_racePointer + 0xC32, 64);
		}
	}

	void	disableStarMusic(MenuEntry *entry)
	{
		u32 g_FNsPointer = GetFNsPointer();
		u8 temp = 0;
		bool in_race = IsInRace();
		if (in_race && is_in_range(g_FNsPointer, 0x14000000, 0x18000000) && Process::Read8(g_FNsPointer + 0x1F7, temp) && temp == 1)
		{
			Process::Write8(g_FNsPointer + 0x1F7, 0);
		}
	}

	void	bulletSpeed(MenuEntry *entry)
	{
		if (Controller::IsKeyDown(X))
			Process::Write32(0x6655A4, 0x41C00000);
		if (Controller::IsKeyDown(B))
			Process::Write32(0x6655A4, 0xFF7FFFFF);
		if (Controller::IsKeyDown(A))
			Process::Write32(0x6655A4, 0x40F33333);
	}

	void	blueShellRide(MenuEntry *entry)
	{
		u32 dataX = 0, dataY = 0, dataZ = 0, g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (Controller::IsKeyDown(DPadLeft))
		{
			if (in_race && Process::Read32(0xFFFFBF4, offset) && is_in_range(offset, 0x14000000, 0x18000000) && Process::Read32(offset - 0x63C, offset) && is_in_range(offset, 0x14000000, 0x18000000))
			{
				Process::Read32(offset - 0x3CB0, dataX);
				Process::Read32(offset - 0x3CAC, dataY);
				Process::Read32(offset - 0x3CA8, dataZ);
				if (is_in_range(dataX, 0x1000, 0xD0000000) && is_in_range(dataY, 0x1000, 0xD0000000) && is_in_range(dataZ, 0x1000, 0xD0000000))
				{
					Process::Write32(g_racePointer + 0x24, dataX);
					Process::Write32(g_racePointer + 0x28, dataY);
					Process::Write32(g_racePointer + 0x2C, dataZ);
				}
			}
		}
	}

	void	itemWheel(MenuEntry *entry)
	{
		u32 g_itemPointer = GetItemPointer();
		bool in_race = IsInRace();
		if (in_race && is_in_range(g_itemPointer, 0x14000000, 0x18000000))
		{
			if (Controller::IsKeyDown(X))
				writeItem(9);
			if (Controller::IsKeyDown(Y))
				writeItem(13);
			if (Controller::IsKeyDown(Start))
				writeItem(6);
			if (Controller::IsKeyDown(Select))
				writeItem(10);
			if (Controller::IsKeyDown(DPadLeft))
				writeItem(14);
			if (Controller::IsKeyDown(DPadRight))
				writeItem(2);
			if (Controller::IsKeyDown(DPadDown))
				writeItem(4);
			if (Controller::IsKeyDown(DPadUp))
				writeItem(1);
		}
	}

	void    SetItem(MenuEntry *entry)
	{
		static bool	shown_dialogue = false, execute = false;
		static u8	item = 0;
		std::string	original = "Enter your item ID (available on MK7 NTR Plugin GBAtemp Page):";
		if (!shown_dialogue)
		{
			Keyboard	keyboard(original);
			shown_dialogue = true;
			if (keyboard.Open(item) != -1)
			{
				execute = true;
			}
		}
		if (execute)
		{
			writeItem(item);
		}
	}

	/////////////////////////////////////////////////////////    Start of speed codes    /////////////////////////////////////////////////////////

	void	instantAcceleration(MenuEntry *entry)
	{
		unsigned int g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (in_race && Controller::IsKeyDown(A) && is_in_range(g_racePointer, 0x14000000, 0x18000000) && Process::Read32(0x140002F4, data) && Process::Read32(data - 0xA4, data) && Process::Read32(data - 0x2C3B0, data))
		{
			Process::Write32(g_racePointer + 0xF2C, data);
		}
	}

	void	instantBackAcceleration(MenuEntry *entry)
	{
		unsigned int g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (in_race && Controller::IsKeyDown(B) && is_in_range(g_racePointer, 0x14000000, 0x18000000) && Process::Read32(0x140002F4, data) && Process::Read32(data - 0xA4, data) && Process::Read32(data - 0x2C3B0, data))
		{
			Process::WriteFloat(g_racePointer + 0xF2C, -1.f * data);
		}
	}

	void	instantStop(MenuEntry *entry)
	{
		unsigned int g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (in_race && Controller::IsKeysDown(A + B) && is_in_range(g_racePointer, 0x14000000, 0x18000000))
		{
			Process::WriteFloat(g_racePointer + 0xF2C, 0);
		}
	}

	void	TwoHundredCC(MenuEntry *entry)
	{
		writeSpeed(0x413C0000);
	}

	void	FiveHundredCC(MenuEntry *entry)
	{
		writeSpeed(0x41A00000);
	}

	/////////////////////////////////////////////////////////    Start of game modes    /////////////////////////////////////////////////////////

	void	eliminationMode(MenuEntry *entry)
	{
		Process::Write32(0x468D1C, 0xE1A00000);
		Process::Write32(0x469154, 0xE1A00000);
		Process::Write32(0x4165A8, 0xE1A00000);
		Process::Write32(0x233B54, 0xEA0EDF6D);
		offset = 0x5EB910;
		static const u8 buffer1[] = { 0xFF, 0x1F, 0x2D, 0xE9, 0xC4, 0x11, 0x9F, 0xE5, 0xC4, 0x31, 0x9F, 0xE5, 0xC4, 0x61, 0x9F, 0xE5, 0xC4, 0x71, 0x9F, 0xE5, 0xC4, 0x81, 0x9F, 0xE5, 0, 0x10, 0xD1, 0xE5, 0, 0, 0x51, 0xE3, 0x2, 0, 0, 0x1A, 0, 0x20, 0xA0, 0xE3, 0, 0x20, 0x83, 0xE5, 0x4, 0x20, 0x83, 0xE5, 0x1, 0, 0x51, 0xE3, 0x62, 0, 0, 0x1A, 0xA4, 0x11, 0x9F, 0xE5, 0, 0x90, 0x96, 0xE5, 0xC8, 0x90, 0x99, 0xE5, 0, 0, 0xA0, 0xE3, 0x34, 0, 0x89, 0xE5, 0x38, 0, 0x89, 0xE5, 0, 0x70, 0x97, 0xE5, 0x8, 0x70, 0x87, 0xE0, 0, 0x70, 0x97, 0xE5, 0x7, 0xB0, 0xA0, 0xE1, 0, 0x60, 0x96, 0xE5, 0xF4, 0x50, 0xD6, 0xE5, 0x2, 0, 0x55, 0xE3, 0x3, 0, 0, 0xA, 0x1E, 0x40, 0xA0, 0xE3, 0x46, 0x40, 0xC7, 0xE5, 0, 0, 0xD1, 0xE5, 0x4, 0, 0xC3, 0xE5, 0x2, 0, 0x55, 0xE3, 0x4E, 0, 0, 0x1A, 0, 0x20, 0x93, 0xE5, 0x46, 0x40, 0xD7, 0xE5, 0x1, 0x20, 0x82, 0xE2, 0x40, 0, 0x52, 0xE3, 0x8, 0, 0, 0xBA, 0, 0x20, 0xA0, 0xE3, 0, 0x20, 0x83, 0xE5, 0x1, 0x40, 0x44, 0xE2, 0, 0, 0x54, 0xE3, 0x2, 0, 0, 0xAA, 0x1E, 0x40, 0xA0, 0xE3, 0x46, 0x40, 0xC7, 0xE5, 0x1, 0, 0, 0xEA, 0x46, 0x40, 0xC7, 0xE5, 0, 0x20, 0x83, 0xE5, 0x46, 0x40, 0xD7, 0xE5, 0, 0, 0x54, 0xE3, 0x31, 0, 0, 0x1A, 0, 0x20, 0x93, 0xE5, 0x3E, 0, 0x52, 0xE3, 0x2E, 0, 0, 0x1A, 0x4, 0, 0xD3, 0xE5, 0xC8, 0x80, 0x96, 0xE5, 0, 0x6B, 0x96, 0xE5, 0x80, 0x50, 0xD8, 0xE5, 0x5, 0, 0x50, 0xE1, 0x1, 0, 0, 0x1A, 0x4, 0x90, 0xA0, 0xE3, 0x50, 0x90, 0xC6, 0xE5, 0xC4, 0x50, 0xD8, 0xE5, 0x5, 0, 0x50, 0xE1, 0x1, 0, 0, 0x1A, 0x4, 0x90, 0xA0, 0xE3, 0xC0, 0x90, 0xC6, 0xE5, 0x8, 0x51, 0xD8, 0xE5, 0x5, 0, 0x50, 0xE1, 0x1, 0, 0, 0x1A, 0x4, 0x90, 0xA0, 0xE3, 0x30, 0x91, 0xC6, 0xE5, 0x4C, 0x51, 0xD8, 0xE5, 0x5, 0, 0x50, 0xE1, 0x1, 0, 0, 0x1A, 0x4, 0x90, 0xA0, 0xE3, 0xA0, 0x91, 0xC6, 0xE5, 0x90, 0x51, 0xD8, 0xE5, 0x5, 0, 0x50, 0xE1, 0x1, 0, 0, 0x1A, 0x4, 0x90, 0xA0, 0xE3, 0x10, 0x92, 0xC6, 0xE5, 0xD4, 0x51, 0xD8, 0xE5, 0x5, 0, 0x50, 0xE1, 0x1, 0, 0, 0x1A, 0x4, 0x90, 0xA0, 0xE3, 0x80, 0x92, 0xC6, 0xE5, 0x18, 0x52, 0xD8, 0xE5, 0x5, 0, 0x50, 0xE1, 0x1, 0, 0, 0x1A, 0x4, 0x90, 0xA0, 0xE3, 0xF0, 0x92, 0xC6, 0xE5, 0x5C, 0x52, 0xD8, 0xE5, 0x5, 0, 0x50, 0xE1, 0x1, 0, 0, 0x1A, 0x4, 0x90, 0xA0, 0xE3, 0x60, 0x93, 0xC6, 0xE5, 0x4, 0, 0x93, 0xE5, 0x1, 0, 0x40, 0xE2, 0x4, 0, 0x83, 0xE5, 0xA, 0, 0, 0xEA, 0x4, 0, 0xD3, 0xE5, 0x8, 0x50, 0x93, 0xE5, 0x20, 0x50, 0x95, 0xE5, 0x1, 0, 0x55, 0xE3, 0x5, 0, 0, 0x1A, 0x1, 0, 0x50, 0xE3, 0x3, 0, 0, 0x1A, 0xC, 0x80, 0x93, 0xE5, 0, 0x90, 0xA0, 0xE3, 0x4, 0x90, 0x88, 0xE5, 0xB8, 0x95, 0xCB, 0xE1, 0xFF, 0x1F, 0xBD, 0xE8, 0x2C, 0x30, 0x93, 0xE5, 0x1D, 0x20, 0xF1, 0xEA, 0xF4, 0xFD, 0x78, 0x14, 0x7C, 0x1, 0x68, 0, 0x28, 0xC5, 0x65, 0, 0xAC, 0x5, 0, 0x14, 0xC4, 0x12, 0, 0, 0xE0, 0x4B, 0x47, 0x15 };
		memcpy((void *)(offset), buffer1, 0x1E8);
		Process::Write32(0x4690FC, 0xEB060A7D);
		offset = 0x5EBAF8;
		static const u8 buffer2[] = { 0x6C, 0x60, 0xA0, 0xE3, 0x59, 0x60, 0xC4, 0xE5, 0xE, 0xF0, 0xA0, 0xE1 };
		memcpy((void *)(offset), buffer2, 0xC);
		Process::Write32(0x3D4E04, 0xEB085B3E);
		offset = 0x5EBB04;
		static const u8 buffer3[] = { 0x8, 0x80, 0x9F, 0xE5, 0, 0, 0x88, 0xE5, 0x20, 0x90, 0x90, 0xE5, 0xE, 0xF0, 0xA0, 0xE1, 0x84, 0x1, 0x68, 0 };
		memcpy((void *)(offset), buffer3, 0x14);
		Process::Write32(0x45CBE4, 0xEA063BCB);
		offset = 0x5EBB18;
		static const u8 buffer4[] = { 0, 0x1, 0x2D, 0xE9, 0xC, 0x80, 0x9F, 0xE5, 0, 0, 0x88, 0xE5, 0, 0x1, 0xBD, 0xE8, 0x4, 0x10, 0x90, 0xE5, 0x2D, 0xC4, 0xF9, 0xEA, 0x88, 0x1, 0x68, 0 };
		memcpy((void *)(offset), buffer4, 0x1C);
	}

	/* void	tagMode(void)
	{
		u32 g_racePointer = GetRacePointer(), pointer = 0, d0pointer = 0, ccpointer = 0;
		static u8 playerSlot = 0, taggedPlayer = 0, score = 0;
		static u16 time = 0;
		bool in_race = IsInRace();
		static bool tagged = false;
		if (in_race)
		{
			playerSlot = 0;
			pointer = 0;
			taggedPlayer = 0;
			time = 0;
			score = 30;
			tagged = false;
			return;
		}
		if (in_race && g_racePointer > 0x16000000 && g_racePointer < 0x18000000 && Process::Read32(0x65DA44, offset) && offset > 0x14000000 && offset < 0x18000000 && READU8(READU32(0x65C9A8) + 0x175A8) < 10 && READU32(0x140005AC) > 0x14000000 && READU32(0x140005AC) < 0x18000000)
		{
			if (READU32(READU32(0x140005AC) + 0x12C8) > 0x14000000 && READU32(READU32(0x140005AC) + 0x12C8) < 0x18000000 && READU32(READU32(0x140005AC) + 0x12C4) > 0x14000000 && READU32(READU32(0x140005AC) + 0x12C4) < 0x18000000)
			{
				ccpointer = READU32(READU32(0x140005AC) + 0x12C4);
				d0pointer = READU32(READU32(0x140005AC) + 0x12C8);
				playerSlot = READU8(READU32(0x65C9A8) + 0x175A8) + 1;
				if (READU16(d0pointer + 0x1878) == 7200)
				{
					score = 30;
					WRITEU8(ccpointer + 0x46, score);
					WRITEU8(ccpointer + 0x54, score);
					if (playerSlot == 1)
					{
						tagged = true;
						score = 51;
					}
				}
				for (int i = 1; i < 9; i++)
				{
					pointer = READU32(READU32(0x65DA44) + (i * 0x44) + 0x209C);
					if (pointer > 0x14000000 && pointer < 0x18000000)
					{
						if (READU16(pointer + 0xFF4) > 0x40 && READU16(pointer + 0xFF4) < 0x180)
						{
							taggedPlayer = i;
						}
					}
				}
				if (tagged)
				{
					WRITEU16(g_racePointer + 0xFF4, 0xB0);
					if (time == 0 && READU16(d0pointer + 0x1878) != 7200 || (time - 120) > READU16(d0pointer + 0x1878))
					{
						score--;
						time = READU16(d0pointer + 0x1878);
						WRITEU8(ccpointer + 0x46, score);
						WRITEU8(ccpointer + 0x54, score);
						if (time < 6900 && READU16(g_racePointer + 0xFF4) > 0x40 && taggedPlayer != playerSlot)
						{
							WRITEU16(g_racePointer + 0xFF4, 0);
							tagged = false;
							return;
						}
					}
					if (READU16(g_racePointer + 0x1F6C) > 5 && READU16(g_racePointer + 0x1F6C) < 0xA000)
					{
						for (int i = 1; i < 9; i++)
						{
							pointer = READU32(READU32(0x65DA44) + (i * 0x44) + 0x209C);
							if (i == playerSlot)
							{
								continue;
							}
							if (READFLOAT(pointer + 0x24) - READFLOAT(g_racePointer + 0x24) < 40.f && READFLOAT(pointer + 0x24) - READFLOAT(g_racePointer + 0x24) > -40.f)
							{
								if (READFLOAT(pointer + 0x28) - READFLOAT(g_racePointer + 0x28) < 40.f && READFLOAT(pointer + 0x28) - READFLOAT(g_racePointer + 0x28) > -40.f)
								{
									if (READFLOAT(pointer + 0x2C) - READFLOAT(g_racePointer + 0x2C) < 40.f && READFLOAT(pointer + 0x2C) - READFLOAT(g_racePointer + 0x2C) > -40.f)
									{
										WRITEU16(g_racePointer + 0xFF4, 0);
										tagged = false;
										taggedPlayer = i;
									}
								}
							}
						}
					}
				}
				if (!tagged)
				{
					pointer = READU32(READU32(0x65DA44) + (taggedPlayer * 0x44) + 0x209C);
					WRITEU16(g_racePointer + 0xFF4, 0);
					if (time == 0 || (time - 120) > READU16(d0pointer + 0x1878))
					{
						if (score < 99)
							score++;
						time = READU16(d0pointer + 0x1878);
						WRITEU8(ccpointer + 0x46, score);
						WRITEU8(ccpointer + 0x54, score);
						if (time < 6900 && READU16(READU32(READU32(0x65DA44) + (taggedPlayer * 0x44) + 0x209C) + 0xFF4) < 0x40 && playerSlot == taggedPlayer)
						{
							tagged = true;
							taggedPlayer = playerSlot;
							return;
						}
					}
					if (READU16(g_racePointer + 0xC32) == 1 && READFLOAT(pointer + 0x24) - READFLOAT(g_racePointer + 0x24) < 40.f && READFLOAT(pointer + 0x24) - READFLOAT(g_racePointer + 0x24) > -40.f)
					{
						if (READFLOAT(pointer + 0x28) - READFLOAT(g_racePointer + 0x28) < 40.f && READFLOAT(pointer + 0x28) - READFLOAT(g_racePointer + 0x28) > -40.f)
						{
							if (READFLOAT(pointer + 0x2C) - READFLOAT(g_racePointer + 0x2C) < 40.f && READFLOAT(pointer + 0x2C) - READFLOAT(g_racePointer + 0x2C) > -40.f)
							{
								WRITEU16(g_racePointer + 0xFF4, 0xB0);
								tagged = true;
								taggedPlayer = playerSlot;
							}
						}
					}
				}
			}
		}
	}

	void	shineTheif(void)
	{
		u32 g_racePointer = GetRacePointer(), pointer = 0, d0pointer = 0, ccpointer = 0;
		static u8 playerSlot = 0, shinePlayer = 0, score = 0;
		static u16 time = 0;
		bool in_race = IsInRace();
		static bool withShine = false;
		if (in_race)
		{
			playerSlot = 0;
			pointer = 0;
			shinePlayer = 0;
			time = 0;
			score = 0;
			withShine = false;
			return;
		}
		if (in_race && g_racePointer > 0x16000000 && g_racePointer < 0x18000000 && READU32(0x65DA44) > 0x14000000 && READU32(0x65DA44) < 0x18000000 && READU8(READU32(0x65C9A8) + 0x175A8) < 10 && READU32(0x140005AC) > 0x14000000 && READU32(0x140005AC) < 0x18000000)
		{
			if (READU32(READU32(0x140005AC) + 0x12C8) > 0x14000000 && READU32(READU32(0x140005AC) + 0x12C8) < 0x18000000 && READU32(READU32(0x140005AC) + 0x12C4) > 0x14000000 && READU32(READU32(0x140005AC) + 0x12C4) < 0x18000000)
			{
				ccpointer = READU32(READU32(0x140005AC) + 0x12C4);
				d0pointer = READU32(READU32(0x140005AC) + 0x12C8);
				playerSlot = READU8(READU32(0x65C9A8) + 0x175A8) + 1;
				if (READU16(d0pointer + 0x1878) == 7200)
				{
					score = 0;
					WRITEU8(ccpointer + 0x46, score);
					WRITEU8(ccpointer + 0x54, score);
					if (playerSlot == 1)
					{
						withShine = true;
					}
				}
				for (int i = 1; i < 9; i++)
				{
					pointer = READU32(READU32(0x65DA44) + (i * 0x44) + 0x209C);
					if (pointer > 0x14000000 && pointer < 0x18000000)
					{
						if (READU16(pointer + 0xFF4) > 0x40 && READU16(pointer + 0xFF4) < 0x180)
						{
							shinePlayer = i;
						}
					}
				}
				if (withShine)
				{
					WRITEU16(g_racePointer + 0xFF4, 0xB0);
					if (time == 0 && READU16(d0pointer + 0x1878) != 7200 || (time - 120) > READU16(d0pointer + 0x1878))
					{
						score++;
						time = READU16(d0pointer + 0x1878);
						WRITEU8(ccpointer + 0x46, score);
						WRITEU8(ccpointer + 0x54, score);
						if (time < 6900 && READU16(g_racePointer + 0xFF4) > 0x40 && shinePlayer != playerSlot)
						{
							WRITEU16(g_racePointer + 0xFF4, 0);
							withShine = false;
							return;
						}
					}
					if (READU16(g_racePointer + 0x1F6C) > 5 && READU16(g_racePointer + 0x1F6C) < 0xA000)
					{
						for (int i = 1; i < 9; i++)
						{
							pointer = READU32(READU32(0x65DA44) + (i * 0x44) + 0x209C);
							if (i == playerSlot)
							{
								continue;
							}
							if (READFLOAT(pointer + 0x24) - READFLOAT(g_racePointer + 0x24) < 40.f && READFLOAT(pointer + 0x24) - READFLOAT(g_racePointer + 0x24) > -40.f)
							{
								if (READFLOAT(pointer + 0x28) - READFLOAT(g_racePointer + 0x28) < 40.f && READFLOAT(pointer + 0x28) - READFLOAT(g_racePointer + 0x28) > -40.f)
								{
									if (READFLOAT(pointer + 0x2C) - READFLOAT(g_racePointer + 0x2C) < 40.f && READFLOAT(pointer + 0x2C) - READFLOAT(g_racePointer + 0x2C) > -40.f)
									{
										WRITEU16(g_racePointer + 0xFF4, 0);
										withShine = false;
										shinePlayer = i;
									}
								}
							}
						}
					}
				}
				if (!withShine)
				{
					pointer = READU32(READU32(0x65DA44) + (shinePlayer * 0x44) + 0x209C);
					WRITEU16(g_racePointer + 0xFF4, 0);
					if (time == 0 || (time - 120) > READU16(d0pointer + 0x1878))
					{
						time = READU16(d0pointer + 0x1878);
						if (time < 6900 && READU16(READU32(READU32(0x65DA44) + (shinePlayer * 0x44) + 0x209C) + 0xFF4) < 0x40 && playerSlot == shinePlayer)
						{
							withShine = true;
							shinePlayer = playerSlot;
							return;
						}
					}
					if (READU16(g_racePointer + 0xC32) == 1 && READFLOAT(pointer + 0x24) - READFLOAT(g_racePointer + 0x24) < 40.f && READFLOAT(pointer + 0x24) - READFLOAT(g_racePointer + 0x24) > -40.f)
					{
						if (READFLOAT(pointer + 0x28) - READFLOAT(g_racePointer + 0x28) < 40.f && READFLOAT(pointer + 0x28) - READFLOAT(g_racePointer + 0x28) > -40.f)
						{
							if (READFLOAT(pointer + 0x2C) - READFLOAT(g_racePointer + 0x2C) < 40.f && READFLOAT(pointer + 0x2C) - READFLOAT(g_racePointer + 0x2C) > -40.f)
							{
								WRITEU16(g_racePointer + 0xFF4, 0xB0);
								withShine = true;
								shinePlayer = playerSlot;
							}
						}
					}
				}
			}
		}
	} */

	void	CountdownMode(MenuEntry *entry)
	{
		u32 pointer = 0, value = 0, g_racePointer = GetRacePointer(), g_oldRacePointer5D0 = GetOldPointer5D0(), g_oldRacePointer5CC = GetOldPointer5CC();
		bool in_race = IsInRace();
		static u8 score = 0;
		static bool AddedToScore = false, AddedToTime = false, end_race = false;
		if (!in_race)
		{
			score = 0;
			AddedToScore = false;
			AddedToTime = false;
			end_race = false;
			return;
		}
		else
		{
			if (Process::Read32(0x65C528, pointer) && Process::Read32(pointer + 0xC8, pointer) && is_in_range(pointer, 0x16000000, 0x18000000))
			{
				Process::Write32(pointer + 0x34, 0);
				Process::Write32(pointer + 0x38, 0);
			}
			if (GetTime() == 300 && !AddedToTime)
			{
				SubToTime(-180);
				AddedToTime = true;
				Process::Write8(g_oldRacePointer5CC + 0x59, 0x25);
			}

			if (!AddedToScore && Process::Read32(0xFFFF6F0, pointer) && Process::Read32(pointer - 0xC, pointer) && Process::Read32(pointer - 0x24, pointer) && Process::Read32(pointer + 0x7C, pointer) && Process::Read32(pointer + 0x20, pointer) && Process::Read32(pointer + 0x70, pointer) && Process::Read32(pointer - 0x30, pointer) && Process::Read32(pointer + 0x38, value) && Process::Read32(pointer + 0x48, pointer) && is_in_range(value, 0x40000000, pointer))
			{
				SubToTime(3);
				AddedToScore = true;
				score++;
			}
			if (AddedToScore && Process::Read32(0xFFFF6F0, pointer) && Process::Read32(pointer - 0xC, pointer) && Process::Read32(pointer - 0x24, pointer) && Process::Read32(pointer + 0x7C, pointer) && Process::Read32(pointer + 0x20, pointer) && Process::Read32(pointer + 0x70, pointer) && Process::Read32(pointer - 0x30, pointer) && Process::Read32(pointer + 0x38, value) && Process::Read32(pointer + 0x48, pointer) && value == pointer)
			{
				AddedToScore = false;
			}

			if (GetTime() == 0 || end_race)
			{
				u8 total = 0;
				u32 totalPlayers = (int)GetTotalPlayers() + 1;
				Process::Write32(g_racePointer + 0xF2C, 0);
				end_race = true;
				Process::Read32(0x65DA44, value);
				for (int i = 1; i < totalPlayers; i++)
				{
					if (Process::Read32(value + 0x209C + (i * 0x44), pointer) && Process::Read32(pointer + 0xF2C, pointer) && pointer < 0x3DCCCCCD)
					{
						total++;
					}
					else if (pointer > 0x3DCCCCCD)
					{
						break;
					}
					if (total + 1 == totalPlayers)
					{
						Process::Write8(g_oldRacePointer5D0 + 0x4F8, 3);
						Process::Write8(g_oldRacePointer5D0 + 0x10EC, 3);
						Process::Write8(g_oldRacePointer5D0 + 0x18EC, 3);	
						break;
					}
				}
			}
			OSD::Run([](const Screen &screen)
			{
				if (!screen.IsTop)
					return false;
				screen.Draw(Utils::Format("Score: %01d", score), 10, 211);
				screen.Draw(Utils::Format("Time: %01d:%02d", GetMinutes(), GetSeconds()), 10, 223);
				return true;
			});
		}
	}

	/////////////////////////////////////////////////////////    Start of menu codes    /////////////////////////////////////////////////////////

	void	disableFirstPersonView(MenuEntry *entry)
	{
		if (Process::Read32(0x14000084, offset) && Process::Read32(offset + 0x316C, offset) && is_in_range(offset, 0x14000000, 0x18000000))
		{
			Process::Write8(offset + 0x119, 0);
		}
	}

	void	timeTrialGhost(MenuEntry *entry)
	{
		/*
		-You must enter a race and then exit/finish/restart the race in order for this to take effect
		-Having this enabled for more than 1 race will crash the game, unless you click restart and not exit or next course or anything else.
		*/
		u32 pointer = 0, g_racePointer = GetRacePointer();
		bool in_race = IsInRace();
		if (Process::Read32(0xFFFF5D4, pointer) && Process::Read32(pointer - 4, pointer) && Process::Read32(pointer + 0x18, pointer))
		{
			if (Process::Read32(0x14000084, offset) && Process::Read32(offset + 0x316C, offset) && is_in_range(offset, 0x14000000, 0x18000000))
			{
				Process::Write8(offset + 0x119, 0);
				Process::Write32(pointer + 0x208, 0x10001);
				if (in_race && Process::Read32(0x65DA44, pointer) && Process::Read32(pointer + 0x20E0, pointer))
				{
					Process::Write32(pointer + 0x24, 0x49000000);
				}
			}
		}
	}

	void	vrExtender(MenuEntry *entry)
	{
		if (Process::Read32(0x663D04, offset) && offset > 0x10000000)
		{
			if (Controller::IsKeyDown(Start))
			{
				writeVR(999999);
			}
			if (Controller::IsKeyDown(Select))
			{
				writeVR(420000);
			}
			if (Controller::IsKeyDown(DPadDown))
			{
				writeVR(0);
			}
			if (Controller::IsKeyDown(DPadUp))
			{
				writeVR(696969);
			}
			if (Controller::IsKeyDown(DPadRight))
			{
				writeVR(666666);
			}
			if (Controller::IsKeyDown(DPadLeft))
			{
				writeVR(777777);
			}
		}
	}

	void    SetVR(MenuEntry *entry)
	{
		static bool	shown_dialogue = false, execute = false;
		static u32	vr = 0;
		std::string	original = "Enter your desired VR (in hex):";
		if (!shown_dialogue)
		{
			Keyboard	keyboard(original);
			shown_dialogue = true;
			if (keyboard.Open(vr) != -1)
			{
				execute = true;
			}
		}
		if (execute)
		{
			writeVR(vr);
		}
	}

	void	randomVR(MenuEntry *entry)
	{
		u16 data16 = 0;
		if (Process::Read16(0x14296A90, data16) && is_in_range(data16, 20000, 200000))
		{
			random = (data * 5);
			if (Process::Read32(0x663D04, offset) && offset > 0x10000000)
			{
				writeVR(random);
			}
		}
	}

	void	unlockEverything(MenuEntry *entry)
	{
		if (Process::Read32(0x6673C8, offset) && is_in_range(offset, 0x10000000, 0x18000000))
		{
			Process::Write8(0x6BA3 + offset, 0x3F);
			Process::Write32(0x6BA4 + offset, 0x1FF003F);
			Process::Write16(0x6BAC + offset, 0x3FFF);
			Process::Write8(0x6BB0 + offset, 0x7F);
			Process::Write8(0x6BB4 + offset, 0x3F);
			Process::Write8(0x6BB8 + offset, 0xFF);
		}
	}

	void	NoDC(MenuEntry *entry)
	{
		u32 g_oldRacePointer5CC = GetOldPointer5CC();
		bool in_race = IsInRace();
		if (in_race && is_in_range(g_oldRacePointer5CC, 0x14000000, 0x18000000))
		{
			Process::Write8(g_oldRacePointer5CC + 0x40, 2);
		}
	}

	void	fastGame(MenuEntry *entry)
	{
		if (Process::Read32(0x140002F4, offset) && is_in_range(offset, 0x14000000, 0x18000000) && Process::Read32(offset + 0x14, offset))
		{
			Process::Write32(offset + 0x2B4, 0);
		}
	}

	/////////////////////////////////////////////////////////    Start of region & flag codes    /////////////////////////////////////////////////////////

	void    SetFlag(MenuEntry *entry) // beta
	{
		static bool	shown_dialogue = false, error = false;
		static u16	flag;
		std::string	original = "Enter your flag ID (available on MK7 NTR Plugin GBAtemp Page):";
		std::string	errorMessage = "Invalid input! Please try again or press B to cancel.";
		if (error)
		{
			Keyboard	keyboard(errorMessage);
			if (keyboard.Open(flag) == -1)
			{
				error = false;
			}
			if (flag > 0 && flag < 178)
			{
				error = false;
			}
		}
		if (!shown_dialogue)
		{
			Keyboard	keyboard(original);
			shown_dialogue = true;
			if (keyboard.Open(flag) != -1)
			{
				error = false;
			}
		}
		if (flag > 0 && flag < 178)
		{
			writeFlag(flag);
			error = false;
		}
		else
		{
			error = true;
		}
	}

	void    SetCoordinates(MenuEntry *entry) // beta
	{
		static bool	shown_dialogue = false;
		static u32	coordinates = 0;
		std::string	original = "Enter your globe coordinates (available on MK7 NTR Plugin GBAtemp Page):";
		if (!shown_dialogue)
		{
			Keyboard	keyboard(original);
			shown_dialogue = true;
			if (keyboard.Open(coordinates) != -1)
			{
				writeLocation(coordinates);
			}
		}
	}

	/////////////////////////////////////////////////////////    End of codes    /////////////////////////////////////////////////////////

}
