/*

	osu-relax - external 'relax-like' input simulator
	Read more at https://aixxe.net/2016/10/osu-game-hacking

	Copyright (C) 2017, aixxe. <me@aixxe.net>
	
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with osu-relax. If not, see <http://www.gnu.org/licenses/>.
	
	This version is modified by Shiruba in order to add a more
	human "feel" to the cheat.
	<http://steamcommunity.com/id/filipesilvens>

*/

#include <Windows.h>
#include <thread>
#include <iostream>
#include <random>
#include <tchar.h>
#include "relax.h"
#include "configs.h"
#include "../resource.h"

using namespace std;

//keys
char left_click_key;
char right_click_key;

//single tap interval
int single_tap_interval;
int single_tap_current;

//total of 100's to issue
int total100s;

DWORD time_address = NULL;
DWORD game_process_id = NULL;
HANDLE game_process = NULL;

//rng stuff
mt19937 randGen(GetTickCount() * GetProcessId(nullptr) - time(nullptr) * 69 - 1371);

// shutdown
DWORD WINAPI shutdownkey(LPVOID lpParam);
void shutdown();

int wmain(int argc, wchar_t* argv[]) 
{
	FreeConsole();

	// set window title for undetectability.
	SetConsoleTitle(newWindowTitle);

	// parse the beatmap we're going to play from argv.
	beatmap active_beatmap;
	
	if (argc < 2 || !active_beatmap.Parse(argv[1])) 
	{
		cerr << "usage: " << argv[0] << " [beatmap]" << endl;
		return EXIT_FAILURE;
	}

	// Set it so the user is not currently playing a beatmap.
	bool isPlaying = false;

	// load from configs.h
	left_click_key = config_left_key;
	right_click_key = config_right_key;
	uniform_int_distribution<int> delayStart(config_delaystartmin, config_delaystartmax);
	uniform_int_distribution<int> delayRelease(config_delayreleasemin, config_delayreleasemax);
	single_tap_interval = config_singletapinterval;
	total100s = config_total100s;

	//rng stuff
	srand(GetTickCount() * GetProcessId(nullptr) - time(nullptr) * 69 - 6969123);

	// shutdown thread
	CreateThread(NULL, NULL, shutdownkey, NULL, NULL, NULL);

	// find the process id of osu!
	game_process_id = get_process_id();

	if(!game_process_id) 
	{
		cerr << " Couldn't find osu!.exe" << endl;
		return false;
	}

	// open the process with memory reading access.
	game_process = OpenProcess(PROCESS_VM_READ, false, game_process_id);
	if (!game_process) 
	{
		cerr << "error: failed to open handle to process. (" << GetLastError() << ")" << endl;
		return false;
	}

	// find the actual time address.
	time_address = find_time_address();

	if (!time_address) 
	{
		cerr << "error: failed to find pointer to time address." << endl;
		return EXIT_FAILURE;
	}

	// define which object are we currently on and get the size of the beatmap
	size_t current_object = 0;
	size_t mapSize = active_beatmap.hitobjects.size();
	size_t mapSize2 = mapSize - 1;

	// iterate hitobjects to find out where we need to start from.
	int32_t time = get_elapsed_time();

	//Check if current time is less than 15. If yes, the user started a beatmap (or started playing another song) and we're ready to advance.
	while (isPlaying == false)
	{
		int32_t currentTime = get_elapsed_time();
		if (currentTime < 15)
		{
			PlaySound(MAKEINTRESOURCE(READY), NULL, SND_RESOURCE);
			isPlaying = true;
			current_object = 0;
		}
	}

	// wait for the beatmap to start.
	while (current_object == 0 && get_elapsed_time() < active_beatmap.hitobjects.begin()->start_time) 
	{
		this_thread::sleep_for(chrono::milliseconds(1));
	}

	// enter playback loop.
	while (current_object < mapSize) 
	{
		// define whether we've hit the current object.
		static bool key_down = false;

		// set the currently active key so we can alternate.
		static char active_key = left_click_key;

		// update the elapsed time.
		time = get_elapsed_time();

		// set single tap interval to higher than the required (to issue single tap)
		single_tap_current = single_tap_interval + 1;

		// if elapsed time is less than 5, set current object to 0
		if (time < 5)
		{
			//PlaySound(MAKEINTRESOURCE(READY), NULL, SND_RESOURCE);
			current_object = 0;
		}

		// set current hitobject
		hitobject& object = active_beatmap.hitobjects.at(current_object);

		// get a reference to the hitobject after the current hitobject and single tap interval.
		if (current_object != mapSize2)
		{
			hitobject& object2 = active_beatmap.hitobjects.at(current_object + 1);
			int32_t obj1time = object.start_time;
			int32_t obj2time = object2.start_time;
			single_tap_current = obj2time - obj1time;
		}

		if (time >= object.start_time + delayStart(randGen) && !key_down)
		{			
			// chance to issue a 100
			int hundredchance = rand() % 100 + 1;

			if (hundredchance <= 1 && total100s != 0)
			{
				this_thread::sleep_for(chrono::milliseconds(102));
				--total100s;
				//cout << "100 issued! 100s left: " << total100s << endl;
			}

			set_key_pressed(active_key, true);
			key_down = true;

			continue;
		}

		if (time > object.end_time && key_down) 
		{
			this_thread::sleep_for(chrono::milliseconds(delayRelease(randGen)));
			
			// release key.
			set_key_pressed(active_key, false);
			key_down = false;

			// alternate keys based on single tap interval.
			if (single_tap_current < single_tap_interval)
			{
				active_key = (active_key == left_click_key) ? right_click_key : left_click_key;
			}

			if (single_tap_current > single_tap_interval)
			{
				active_key = left_click_key;
			}

			// counts up the current objects
			current_object++;

			// closes when beatmap is over
			if (current_object == mapSize)
			{
				shutdown();
				break;
			}

			// advance to next object.
			if (current_object != mapSize)
			{
				object = active_beatmap.hitobjects.at(current_object);
			}
		}
		Sleep(1);
		//this_thread::sleep_for(chrono::milliseconds(1));
	}

	return EXIT_SUCCESS;
}

DWORD WINAPI shutdownkey(LPVOID lpParam)
{
	while(1)
	{
		if (GetAsyncKeyState(VK_HOME) && 0x8000)
		{
			PlaySound(MAKEINTRESOURCE(GOODBYE), NULL, SND_RESOURCE);
			CloseHandle(game_process);
			exit(0);
		}
		this_thread::sleep_for(chrono::milliseconds(25)); //optimization
	}
	this_thread::sleep_for(chrono::milliseconds(500));
	return 0;
}

void shutdown()
{
	//PlaySound(MAKEINTRESOURCE(GOODBYE), NULL, SND_RESOURCE);
	CloseHandle(game_process);
	exit(0);
}
