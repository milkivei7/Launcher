#include <iostream>
#include <Windows.h>
#include "Launcher.h"

using namespace std;


int main()
{
    setlocale(LC_ALL, "RU");
    char NameApplication;
    InfoGame Mafia('Mafia', "steam://rungameid/40990");

    //cout<< "Введите название игры: ";
    //cin>> NameApplication;
    //StartGame(Mafia);

    cout<< Mafia.srcGame<<" "<< Mafia.NameGame;
    
    return 0;
}

/*void CheckGame(std::string* NameApplication)
{
    
        for (int i = 0; i < Count; i++)
        {
            if (stricmp((NameApplication, CountGames[i].NameGame)) == 0)
            {

            }
        }
    
}*/
