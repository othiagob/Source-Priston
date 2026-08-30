#pragma once

// Compatibilidade local com CA2W do ATL (atlconv.h).
// O projeto nao usa ATL completo; o header da Microsoft nao entra no include path.

#include <windows.h>

class CA2W
{
public:
	explicit CA2W(const char* psz, UINT nCodePage = CP_ACP)
		: m_psz(nullptr)
	{
		if (!psz)
			return;

		const int nLen = ::MultiByteToWideChar(nCodePage, 0, psz, -1, nullptr, 0);
		if (nLen <= 0)
			return;

		m_psz = new wchar_t[static_cast<size_t>(nLen)];
		::MultiByteToWideChar(nCodePage, 0, psz, -1, m_psz, nLen);
	}

	~CA2W()
	{
		delete[] m_psz;
	}

	CA2W(const CA2W&) = delete;
	CA2W& operator=(const CA2W&) = delete;

	operator wchar_t*() const
	{
		return m_psz;
	}

private:
	wchar_t* m_psz;
};
