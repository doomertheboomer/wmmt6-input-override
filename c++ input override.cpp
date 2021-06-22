// c++ input override.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <windows.h>
#include <SFML/Window/Joystick.hpp>
#include "proc.h"

int main()
{
	
	//get wm6 proccess id
	DWORD procId = getProcId(L"wmn6r.exe");

	//get base addr
	uintptr_t imageBase = getModuleBaseAddress(procId, L"wmn6r.exe");

	//get process handle
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	//resolve base addr
	uintptr_t pCarStruct = (imageBase + 0x1E5B5B0);

	//set rolling start
	uintptr_t rollingAddr = (imageBase + 0x1E5B8FC);
	float rollingTimer = 0;

	//initialize gas and brake to minimize errors
	uintptr_t gasAddr = 0, brakeAddr = 0; 

	int x = 1;
	do
	{
		/* debug stuff
		sf::Joystick::update();
		float left = (sf::Joystick::getAxisPosition(0, sf::Joystick::U) + 100) / 200;
		float right = (sf::Joystick::getAxisPosition(0, sf::Joystick::V) + 100) / 200;
		std::cout << "Left Trigger: " << (std::to_string(left)) << "| Right Trigger: " << (std::to_string(right)) << (std::endl);
		*/

		ReadProcessMemory(hProcess, (DWORD*)rollingAddr, &rollingTimer, sizeof(rollingTimer), nullptr);
		if (rollingTimer != 0)
		{
			//resolve gas pointer
			std::vector<unsigned int> gasOffsets = { 0xB10 };
			uintptr_t gasAddr = findDMAAddy(hProcess, pCarStruct, gasOffsets);

			//resolve brake pointer
			std::vector<unsigned int> brakeOffsets = { 0xB14 };
			uintptr_t brakeAddr = findDMAAddy(hProcess, pCarStruct, brakeOffsets);

			std::cout << "Gas found at 0x" << std::hex << gasAddr << " Brake found at 0x" << std::hex << brakeAddr<< std::endl;
		}
		else
		{
			sf::Joystick::update();
			float left = (sf::Joystick::getAxisPosition(0, sf::Joystick::U) + 100) / 200;
			float right = (sf::Joystick::getAxisPosition(0, sf::Joystick::V) + 100) / 200;

			//write to gas
			WriteProcessMemory(hProcess, (DWORD*)gasAddr, &right, sizeof(right), nullptr);
			WriteProcessMemory(hProcess, (DWORD*)brakeAddr, &left, sizeof(left), nullptr);
			std::cout << "Left Trigger: " << (std::to_string(left)) << "| Right Trigger: " << (std::to_string(right)) << (std::endl);
		}
	} while (x = 1); //monitors trigger of dualshock 4


	
	/* leftovers

	std::cout << "coinAddr = " << "0x" << std::hex << coinAddr << std::endl;

	//read coin value
	int coinValue = 0;
	ReadProcessMemory(hProcess, (DWORD*)coinAddr, &coinValue, sizeof(coinValue), nullptr);
	std::cout << "Current coin = " << std::dec << coinValue << std::endl;

	//read coin value
	ReadProcessMemory(hProcess, (DWORD*)coinAddr, &coinValue, sizeof(coinValue), nullptr);
	std::cout << "New coin = " << std::dec << coinValue << std::endl;
	*/

	getchar();
	
	return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
