// VirtualPet.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "chrono"
#include "windows.h"
#include "conio.h"
#include "PetFunctions.h"

int main()
{
#pragma region Pet setup

	HANDLE Cons = GetStdHandle(STD_OUTPUT_HANDLE);// honestly have no idea what this actually does but from my understanding if I have read the documentation correctly it's a pointer for the output but that's only a guess at this point

	TAMA CurrentPet{ "" }; // creates an instance of the pet. The brackets at the end get rid of an annoying warning of the "variables not initialzed" which for the most part doesn't matter as they get initialized in the next line
	Init(&CurrentPet);//initialises the pet variables
	NamingPet(&CurrentPet);// allows the pet to be named
#pragma endregion
#pragma region Prototype functions
	// declares a prototype of all the functions in this file here

	int Hung_Sleep_Hydrate();
	void HappinessCalc(TAMA*);
	void Print(TAMA*, int, HANDLE);
	void sleep(TAMA*, int, const clock_t);
	void Update(TAMA*, char, int, clock_t, clock_t, double, HANDLE);
#pragma endregion

#pragma region variables
	// this set up the variables and allows them to be used in the functions properly
	double time = 1;// this is used to check in the while statement if one second has passed
	int N = sizeof(CurrentPet.Levels) / sizeof(CurrentPet.Levels[0]); // this gets the size of the list for anything that requires it and will only ever get called 
	clock_t Timer = clock();
	clock_t BackgroundTimer = clock();
	char key = '1';
#pragma endregion

#pragma region Update
	// this is to make it easier to find the Update function 
	Update(&CurrentPet, key, N, Timer, BackgroundTimer, time, Cons);
#pragma endregion

#pragma region Dead
	// this is to find the bit that prints the string for when the pet is dead quicker

	system("CLS");
	Print(&CurrentPet, N, Cons);
#pragma endregion
}

int Hung_Sleep_Hydrate() // this will return a value to whatever one needs it. If the Bool is true it will be a positive number otherwise it will be negative. this should technically be in the petFunctions but since it doesn't use the TAMA struct it's fine here
{
	int value = 0;

	//positive number
	value = rand() % 15 + 10;
	return value;

}
void HappinessCalc(TAMA* Pet)
{
	Pet->Levels[2] = (Pet->Levels[1] + Pet->Levels[0] + Pet->Levels[3]) / 3; // this gives the average happiness
	Pet->Levels[2] += Pet->HAP;
}

void Print(TAMA* CurrentPet, int N, HANDLE Cons) // this just contains the majority of the functions that will print something in it and the stuff that will print when the pet 
{
	if (CurrentPet->isDead == false)
	{
		system("CLS");
		HappinessCalc(CurrentPet);
		StatCap(CurrentPet, N);
		DisplayStats(CurrentPet, Cons);
	}
	else
	{
		SetConsoleTextAttribute(Cons, 4);//red

		cout << "\n\n The Pet is dead.\n\n It survived " << (clock() - CurrentPet->Start) / (double)CLOCKS_PER_SEC << "Seconds";

	}
}

void Sleep(TAMA* CurrentPet, int N, const clock_t& Timer, HANDLE Cons) // this will constantly print whilst the pet is asleep and 
{
	Print(CurrentPet, N, Cons);
	cout << "The pet will sleep for " << CurrentPet->duration << " second/s";
	if (CurrentPet->isAsleep)
		cout << "\n\n Time Passed: " << ((double)clock() - Timer) / (double)CLOCKS_PER_SEC; // this will only print if the bool isAsleep is false
	Decrease(CurrentPet, N);
	CurrentPet->Levels[3]++;
}

void Update(TAMA* CurrentPet, char key, int N, clock_t Timer, clock_t BackgroundTimer, double time, HANDLE Cons)
{


	Print(CurrentPet, N, Cons);
	while (!CurrentPet->isDead)
	{
		if (CurrentPet->Levels[0] <= 0 || CurrentPet->Levels[1] <= 0 || CurrentPet->Levels[2] <= 0) // if the hunger,hydration or happiness hit 0 or less then the pet dies
		{
			CurrentPet->isDead = true;
			break;
		}
		while (time > (double)((double)clock() - BackgroundTimer) / (double)CLOCKS_PER_SEC)// checks if a second has passed and if not then it will do what's inside this statement
		{


			if (_kbhit() && CurrentPet->isAsleep == false && CurrentPet->PassedOut == false)// this checks if the keyboard has been hit and if it hasn't it will set it to a a default value of 1 which will never be used and allows the background timer to work properly. This also checks that the pet is asleep so you can't feed or water it when it's asleep
			{
				key = _getch();
			}
			else
			{
				key = '1';
			}
			if (CurrentPet->Levels[3] == 0)
			{
				CurrentPet->PassedOut = true;
			}
			if (CurrentPet->PassedOut)// this will force the pet to collapse and fall asleep 
			{
				while (CurrentPet->Levels[3] < 50)
				{
					Sleep(CurrentPet, N, Timer, Cons);

				}
				CurrentPet->PassedOut = false;
			}
			if (key == 'f' || key == 'F')// the hunger stat is satisfied by a random amount
			{
				CurrentPet->Levels[0] += Hung_Sleep_Hydrate();
				Print(CurrentPet, N, Cons);


			}
			if (key == 'h' || key == 'H') // the hydration stat is quenched by a random amount
			{
				CurrentPet->Levels[1] += Hung_Sleep_Hydrate();
				Print(CurrentPet, N, Cons);

			}
			if (key == 'p' || key == 'P') // this was 
			{
				CurrentPet->HAP++; // this increases the 
				Print(CurrentPet, N, Cons);

			}
			if (key == 's' || key == 'S') // puts it to sleep
			{
				CurrentPet->duration = rand() % 3 + 0b1;// randomly decides how long the pet will sleep
				Timer = clock();// sets Timer to the current time
				while (CurrentPet->duration > (double)((double)clock() - Timer) / (double)CLOCKS_PER_SEC)// this essentially is checking that the duration has passed and if it hasn't it will loop the inside of this till it has
				{
					CurrentPet->isAsleep = true; // used for painting the pet asleep in the console
					Sleep(CurrentPet, N, Timer, Cons);
				}
				CurrentPet->isAsleep = false;//used for painting the pet awake in the console
				Print(CurrentPet, N, Cons);
			}
			if (key == 'z')
			{
				CurrentPet->isAsleep = true;
			}
			if (key == 'x' || key == 'X')
			{
				CurrentPet->isDead = true;
			}
		}
		//once a second has passed it will do this stuff bellow
		for (int i = 0; i < N; i++)
		{
			CurrentPet->Levels[i] -= rand() % 2 + 1; // this will decrease all the values by a random value when the background timer hits
		}
		CurrentPet->HAP -= rand() % 2; // this will randomly decrease the petting happiness

		Print(CurrentPet, N, Cons);

		BackgroundTimer = clock(); // this resets the background timer and restarts the count



	}
}
