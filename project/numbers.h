#include <stdbool.h>

typedef struct pole_
{
	bool matrix[8][4];
}Pole;

typedef struct doublePole_
{
	Pole pole0;
	Pole pole1;
}DoublePole;

void setNumToPole(Pole*pole , int num)
{
	for (int k = 0; k < 8; k ++)
	for (int i = 0; i < 4; i ++)
		{
			pole->matrix[k][i] = false;
		}
	
	if (num == 0)
	{
		bool m[8][4] = {{true,true,true,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,true,true,true}};
		
		for (int k = 7; k >=0; k --)
			for (int i = 3; i>=0; i --)
			{
				pole->matrix[k][i] = m[7-k][i];
			}
	}
	if (num == 1)
	{
		bool m[8][4] = {{false,false,false,true},
									 {false,false,false,true},
									 {false,false,true,true},
									 {false,true,false,true},
									 {true,false,false,true},
									 {false,false,false,true},
									 {false,false,false,true},
									 {false,false,false,true}};
		
		for (int k = 7; k >=0; k --)
			for (int i = 3; i>=0; i --)
			{
				pole->matrix[k][i] = m[7-k][i];
			}
	}
	if (num == 2) 
	{
		bool m[8][4] = {{true,true,true,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {false,false,false,true},
									 {false,false,true,false},
									 {false,true,false,false},
									 {true,false,false,true},
									 {true,true,true,true}};
		
		for (int k = 7; k >=0; k --)
			for (int i = 3; i>=0; i --)
			{
				pole->matrix[k][i] = m[7-k][i];
			}
	}
	if (num == 3)
	{
		bool m[8][4] = {{true,true,true,true},
									 {true,false,false,true},
									 {false,false,false,true},
									 {true,true,true,true},
									 {false,false,false,true},
									 {false,false,false,true},
									 {true,false,false,true},
									 {true,true,true,true}};
		
		for (int k = 7; k >=0; k --)
			for (int i = 3; i>=0; i --)
			{
				pole->matrix[k][i] = m[7-k][i];
			}
	}
	if (num == 4)
	{
		bool m[8][4] = {{true,false,false,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,true,true,true},
									 {false,false,false,true},
									 {false,false,false,true},
									 {false,false,false,true}};
		
		for (int k = 7; k >=0; k --)
			for (int i = 3; i>=0; i --)
			{
				pole->matrix[k][i] = m[7-k][i];
			}
	}
	if (num == 5)
	{
		bool m[8][4] = {{true,true,true,true},
									 {true,false,false,true},
									 {true,false,false,false},
									 {true,true,true,true},
									 {false,false,false,true},
									 {false,false,false,true},
									 {true,false,false,true},
									 {true,true,true,true}};
		
		for (int k = 7; k >=0; k --)
			for (int i = 3; i>=0; i --)
			{
				pole->matrix[k][i] = m[7-k][i];
			}
	}
	if (num == 6)
	{
		bool m[8][4] = {{true,true,true,true},
									 {true,false,false,true},
									 {true,false,false,false},
									 {true,true,true,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,true,true,true}};
		
		for (int k = 7; k >=0; k --)
			for (int i = 3; i>=0; i --)
			{
				pole->matrix[k][i] = m[7-k][i];
			}
	}
	if (num == 7)
	{
		bool m[8][4] = {{true,true,true,true},
									 {true,false,false,true},
									 {false,false,false,true},
									 {false,false,true,false},
									 {false,true,false,false},
									 {true,false,false,false},
									 {true,false,false,false},
									 {true,false,false,false}};
		
		for (int k = 7; k >=0; k --)
			for (int i = 3; i>=0; i --)
			{
				pole->matrix[k][i] = m[7-k][i];
			}
	}
	if (num == 8)
	{
		bool m[8][4] = {{true,true,true,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {false,true,true,false},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,true,true,true}};
		
		for (int k = 7; k >=0; k --)
			for (int i = 3; i>=0; i --)
			{
				pole->matrix[k][i] = m[7-k][i];
			}
	}
	if (num == 9)
	{
		bool m[8][4] = {{true,true,true,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,false,false,true},
									 {true,true,true,true},
									 {false,false,false,true},
									 {true,false,false,true},
									 {true,true,true,true}};
		
		for (int k = 7; k >=0; k --)
			for (int i = 3; i>=0; i --)
			{
				pole->matrix[k][i] = m[7-k][i];
			}
	}
}


void setNumber(DoublePole * poles, int num)
{
	if (num >=100)
	{
		setNumToPole(&poles->pole0 ,9);
		setNumToPole(&poles->pole1, 9);
	}
	else
	{
		int left = num / 10;
		int right = num % 10;
		
		setNumToPole(&poles->pole0 ,left);
		setNumToPole(&poles->pole1, right);
	}
}


/*void drawPole(Pole pole , int pos);
void drawPole(Pole pole , int pos)
{
	for (int k = 0; k < 8; k ++)
			for (int i =  pos* 4; i < 8; i ++)
				{
						packet.data[k][i] = pole.matrix[k-1*pos][i];
				}
}

void draw(DoublePole poles);
void draw(DoublePole poles)
{
	drawPole(poles.pole0 , 0);
	drawPole(poles.pole1 , 1);
}*/


