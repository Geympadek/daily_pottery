#include <iostream>
#include "jsonval.h"
#include <ctime>
#include "application.h"

using namespace engix;

struct TimeSpan
{
    long hours = 0;
    long minuts = 0;
};

enum Month
{
    JANUARY = 1,
    FEBRUARY,
    MARCH,
    APRIL
};

struct Date
{
    unsigned day;
    unsigned month;
};

enum Success 
{
    CRACKED_WET,//Треснуло при сушке
    CRACKED_FIRE,//Треснуло при обжиге
    PERFECT //Идеально
};

enum Drying 
{
    OPEN_AIR,//Открытый воздух тень
    OPEN_AIR_SUN,//Открытый воздух солнце
    CLOSED_AIR,//Закрытый воздух
    PACKAGED,//В пакете
    FABRIC//Под тканью
};

struct DryingSpan
{
    Drying type;
    TimeSpan duration;
};

struct FireingSpan
{
    unsigned temperature;
    TimeSpan duration;
};

struct Item
{
    float temper;//Соотношение песка к глине (доли)
    float thickness_mm;//Толщина в милиметрах
    
    bool isSimple; //Простое или сложное изделие (в зависимости от наличия ручек и т.д.)

    TimeSpan fireDuration;//Длительность обжига

    std::vector<DryingSpan> drying;//Периоды сушки

    std::string name; //Название 

    bool isWarm;//Была ли печь прогрета перед обжигом (от 200+)
    std::vector<FireingSpan> fireing;//Периоды обжига

    Success success;//Успех изделия
};

void initLibraries();
void freeLibraries();

int main(int argc, char** argv)
{
    initLibraries();

    Application* game = nullptr;
    try
    {
        SDL_ShowCursor(SDL_DISABLE); 
        game = new Application();
        game->loop();
    }
    catch(const std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }
    if (game != nullptr)
    {
        delete game;
    }
    
    freeLibraries();
    return 0;
}

void initLibraries()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL couldn\'t initializeSDL_Error: %s", SDL_GetError());
        exit(1);
    }
    int imageFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imageFlags) & imageFlags))
    {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_Image could not initialize! SDL_image Error: %s", IMG_GetError());
        SDL_Quit();
        exit(1);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_Mixer could not initialize! SDL_Mixer Error: %s", Mix_GetError());
        IMG_Quit();
        SDL_Quit();
        exit(1);
    }
}

void freeLibraries()
{
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}
