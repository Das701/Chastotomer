#include "Display/Primitives.h"


using namespace Display::Primitives;


extern "C" {
    
    
void DrawHLine (int x, int y, int wid)
{
    for(int s = x; s < x + wid; s++)
    {   
        Point().Draw(s, y, Color::WHITE);
    }
}
void DrawVLine (int x, int y, int heig)
{
    for(int g = y; g < y + heig; g++)
    {
        Point().Draw(x, g, Color::WHITE);
    }
}

 
void DrawRect(unsigned char x, unsigned char y, int wid, int heig)
{
    DrawHLine(x, y, wid);
    DrawHLine(x, y + heig, wid);
    DrawVLine(x, y, heig);
    DrawVLine(x + wid, y, heig);   
}
   
char GetBit (char data, char bit)
{
    return (data >> bit) & 1;   
}

void DrawByte (int x, int y, char data)
{
    for(int i = 0; i < 8; i++)  
    {
        char bit = GetBit(data, i);
        if (bit != 0)
        {
            Point() .Draw(x, y + i, Color::WHITE);            
        }
    }     
}

char Image(unsigned char x, unsigned char y, unsigned char * data)
{
   DrawRect(x, y, 35, 7);
   DrawByte (x, y, data); 
}  

}