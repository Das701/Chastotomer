char Image(unsigned char x, unsigned char y, unsigned char * data)
{
	unsigned int a;
	unsigned char c, point;
	point = *data;

	Cursor_Set(x, y);
	Write_To_Display(0x1F);
	Write_To_Display(0x28);
	Write_To_Display(0x66);
	Write_To_Display(0x11);
	Write_To_Display(*data);
	Write_To_Display(0x00);
	data ++;
	Write_To_Display(*data);
	Write_To_Display(0x00);
	Write_To_Display(0x01);


	a=point*(*data);
	data ++;

	for(;a !=0; data++)
		{

			c = *data;
			a--;

			c = ((c & 0x55) << 1) | ((c & 0xAA) >> 1);
			c = ((c & 0xCC) >> 2) | ((c & 0x33) << 2);
			c = (c >> 4) | (c << 4);

			Write_To_Display(c);
		}

	return x + point;
}