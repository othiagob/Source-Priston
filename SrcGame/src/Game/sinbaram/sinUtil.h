#pragma once

class sinSTRING{
private:

char			*m_aString;

public:
sinSTRING(void);
sinSTRING(sinSTRING &rString);
sinSTRING(char *pString);

~sinSTRING(void);

sinSTRING & operator = (sinSTRING &rString);
sinSTRING & operator = (char *pString);
bool operator == (sinSTRING &rString);
bool operator == (char *pString);
bool operator != (sinSTRING &rString);
bool operator != (char *pString);

char *GetString(void);
int  GetLength(void);
};

int halpRelease(LPDIRECT3DTEXTURE9 lpTemp);
int haSearchString(char *strBuffer,char KeyBuffer);
int haStringCenter(int PosiX,char *strbuff);