#define OLC_PGE_APPLICATION
#include "Game.h"


int main() {
    Game app;
    if (app.Construct(512, 512, 2, 2, false, false)) {
        app.Start();
    }
    
    return 0;
}