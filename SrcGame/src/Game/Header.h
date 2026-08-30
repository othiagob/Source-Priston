#include <windows.h>
extern void setcolordae( HDC hdc, COLORREF color );
extern void createfontdae( int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic,
						   DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision,
						   DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName );
extern void deletedae( HGDIOBJ ho );