#include "Display/Primitives.h"


using namespace Display::Primitives;


extern "C" {

char Image(unsigned char x, unsigned char y, unsigned char * data)
{
    Point().Draw(x, y, Color::WHITE);
}

}
