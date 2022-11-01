#include <windows.h>

int const Count = 2;
int i = 0;
struct InfoGame
{     
    const CHAR *srcGame;
    std::string NameGame;

    InfoGame(std::string Name, const CHAR *ipGame)
    {
        NameGame = Name;
        srcGame = ipGame;
    }
};

struct
{
    InfoGame Mafia = InfoGame("Mafia", "steam://rungameid/40990");
    InfoGame Steam = InfoGame("Steam", "C:\\Program Files (x86)\\Steam\\Steam.exe");
    using InfoGamePtr = InfoGame*;
    const InfoGamePtr values [2] = {&Mafia, &Steam};
}Name;

auto StartGame(InfoGame Games){return ShellExecuteA(nullptr, "open",
    Games.srcGame, nullptr, nullptr, SW_HIDE);}


