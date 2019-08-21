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
   
char GetBit (char  data, char bit)
{
    return (data >> bit) & 1;   
}

void DrawByte (int x, int y, char  data)
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
	 data++;
	
   DrawByte (x, y, *data); 
	 data++;
   DrawByte (x + 1, y, *data); 
	 data++;
	 DrawByte (x + 2, y, *data); 
	 data++;
	 DrawByte (x + 3, y, *data); 
	 data++;
	 DrawByte (x + 4, y, *data); 
	 data++;
	 DrawByte (x + 5, y, *data); 
	 data++;
	 DrawByte (x + 6, y, *data);
   data++;	
	 DrawByte (x + 7, y, *data); 
	 data++;
	 DrawByte (x + 8, y, *data); 
	 data++;
	 DrawByte (x + 9, y, *data); 
	 data++;
   DrawByte (x + 10, y, *data); 
	 data++;
	 DrawByte (x + 11, y, *data); 
	 data++;
	 DrawByte (x + 12, y, *data); 
	 data++;
	 DrawByte (x + 13, y, *data); 
	 data++;
	 DrawByte (x + 14, y, *data); 
	 data++;
	 DrawByte (x + 15, y, *data);
   data++;	
	 DrawByte (x + 16, y, *data); 
	 data++;
	 DrawByte (x + 17, y, *data);
	 data++;
   DrawByte (x + 18, y, *data); 
	 data++;
   DrawByte (x + 19, y, *data); 
	 data++;
	 DrawByte (x + 20, y, *data); 
	 data++;
	 DrawByte (x + 21, y, *data); 
	 data++;
	 DrawByte (x + 22, y, *data); 
	 data++;
	 DrawByte (x + 23, y, *data); 
	 data++;
	 DrawByte (x + 24, y, *data);
   data++;	
	 DrawByte (x + 25, y, *data); 
	 data++;
	 DrawByte (x + 26, y, *data); 
	 data++;
	 DrawByte (x + 27, y, *data); 
	 data++;
	 DrawByte (x + 28, y, *data); 
	 data++;
	 DrawByte (x + 29, y, *data); 
	 data++;
	 DrawByte (x + 30, y, *data); 
	 data++;
	 DrawByte (x + 31, y, *data);
   data++;	
	 DrawByte (x + 32, y, *data); 
	 data++;
	 DrawByte (x + 33, y, *data);  
   data++;	
	 DrawByte (x + 34, y, *data); 
	 data++;
	 DrawByte (x + 35, y, *data);
}  

}