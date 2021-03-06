/*
 *  Copyright 2008 Fabrice Colin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <errno.h>
#include <iostream>
#include <glibmm/convert.h>

#include "StringManip.h"
#include "TextConverter.h"

using std::cout;
using std::endl;
using std::string;
using namespace Glib;

#define USE_GLIBMM_ICONV 1
//#define USE_GLIBMM_FUNCS 1

TextConverter::TextConverter(unsigned int maxErrors) :
	m_utf8Locale(false),
	m_maxErrors(maxErrors),
	m_conversionErrors(0)
{
	// Get the locale charset
	m_utf8Locale = get_charset(m_localeCharset);
}

TextConverter::~TextConverter()
{
}

ustring TextConverter::toUTF8(const string &text, const string &charset)
{
	string textCharset(StringManip::toLowerCase(charset));
	char outputBuffer[8192];
	char *pInput = const_cast<char *>(text.c_str());

	m_conversionErrors = 0;

	if ((text.empty() == true) ||
		(textCharset == "utf-8"))
	{
		// No conversion necessary
		return text;
	}

	if (textCharset.empty() == true)
	{
		if (m_utf8Locale == true)
		{
			// The current locale uses UTF-8
			return text;
		}

		textCharset = m_localeCharset;
	}

#ifdef USE_GLIBMM_ICONV
	IConv converter("UTF-8", textCharset);
	string outputText;
	gsize inputSize = text.length();
	bool invalidSequence = false;

	while (inputSize > 0)
	{
		char *pOutput = outputBuffer;
		gsize outputSize = 8192;

		size_t conversions = converter.iconv(&pInput, &inputSize, &pOutput, &outputSize);
		int errorCode = errno;
		if (conversions == -1)
		{
			if (errorCode == EILSEQ)
			{
				// Conversion was only partially successful
				++m_conversionErrors;
#ifdef DEBUG
				cout << "TextConverter::toUTF8: invalid sequence at " << pInput - text.c_str() << endl;
#endif
				if (m_conversionErrors >= m_maxErrors)
				{
					// Give up
					return text; 
				}

				outputText.append(outputBuffer, 8192 - outputSize);
				if (invalidSequence == false)
				{
					outputText += "?";
					invalidSequence = true;
				}

				// Skip that
				++pInput;
				--inputSize;
				continue;
			}
			else if (errorCode != E2BIG)
			{
#ifdef DEBUG
				cout << "TextConverter::toUTF8: unknown error " << errorCode << endl;
#endif
				return text;
			}
		}
		else
		{
			invalidSequence = false;
		}

		// Append what was successfully converted
		outputText.append(outputBuffer, 8192 - outputSize);
	}
#ifdef DEBUG
	cout << "TextConverter::toUTF8: " << m_conversionErrors << " conversion errors" << endl;
#endif

	return outputText;
#else
#ifdef USE_GLIBMM_FUNCS
	try
	{
		if (textCharset.empty() == false)
		{
			return convert_with_fallback(text, "UTF-8", textCharset, " ");
		}
		else
		{
			return locale_to_utf8(text);
		}
	}
	catch (ConvertError &ce)
	{
#ifdef DEBUG
		cout << "TextConverter::toUTF8: cannot convert from " << textCharset << ": " << ce.what() << endl;
#endif
		if (textCharset.empty() == false)
		{
#ifdef DEBUG
			cout << "TextConverter::toUTF8: trying again" << endl;
#endif
			return TextConverter::toUTF8(text, "");
		}
	}

	return "";
#else
	return text;
#endif
#endif
}

unsigned int TextConverter::getErrorsCount(void) const
{
	return m_conversionErrors;
}

string TextConverter::fromUTF8(const ustring &text)
{
	try
	{
		return locale_from_utf8(text);
	}
	catch (ConvertError &ce)
	{
#ifdef DEBUG
		cout << "TextConverter::fromUTF8: " << ce.what() << endl;
#endif
	}
 
       return "";
}

