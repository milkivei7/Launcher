#include <cstdio>
#include <cstring>
#include <iostream>



int const Count = 2;

struct InfoGame
{     
    const CHAR *srcGame;
    char NameGame;

    InfoGame(char Name, const CHAR *ipGame)
    {
        NameGame = Name;
        srcGame = ipGame;
    }
};



auto StartGame(InfoGame Games)
{
    return ShellExecuteA(nullptr, "open", Games.srcGame, nullptr, nullptr, SW_HIDE);
}

void CheckGame(std::string *NameApplication);

/*class NameGames
{
    CHAR *srcGames;
    public:
        static auto StartGame(const CHAR *srcGames);
};

auto NameGames::StartGame(const CHAR *srcGames) {

    return ShellExecuteA(NULL, "open", srcGames, 0, 0, SW_HIDE);
}
*/