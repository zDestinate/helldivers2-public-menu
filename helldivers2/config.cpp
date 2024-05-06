#include "config.h"

config Config;

void config::Init()
{
    //register hook
    Config.ClientBase = reinterpret_cast<__int64>(GetModuleHandle(0));
}