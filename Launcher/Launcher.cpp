#include <iostream>
#include <windows.h>
#include "Launcher.h"

using namespace std;


int main()
{

    setlocale(0, "");
    std::string NameApplication;
    //InfoGame Mafia("Mafia", "steam://rungameid/40990");

    cout<< "Write name of game please: ";
    cin>> NameApplication;
    for (i ; i<2;i++)
    {
        if (Name.values[i]->NameGame == NameApplication){StartGame(*Name.values[i]); cout<<"Game playing....";}
        
        
    }


    
    
    //StartGame(Mafia);

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
