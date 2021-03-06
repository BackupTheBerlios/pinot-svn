/*
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

#include <string.h>
#include <iostream>

#include "WordTokenizer.h"

/**
  * This returns the MIME type supported by the library's tokenizer.
  * The character string is allocated with new[].
  */
char *getTokenizerType(unsigned int typeNum)
{
	if (typeNum == 0)
	{
		char *pType = new char[16];
		strncpy(pType, "application/msword", 18);
		pType[18] = '\0';
		return pType;
	}

	return NULL;
}

/// This returns a pointer to a Tokenizer, allocated with new.
Tokenizer *getTokenizer(const Document *pDocument)
{
	return new WordTokenizer(pDocument);
}

WordTokenizer::WordTokenizer(const Document *pDocument) :
	Tokenizer(NULL)
{
	string cmdLine("antiword");

	// Run antiword
	Document *pOutputDocument = runHelperProgram(pDocument, cmdLine);
	if (pOutputDocument != NULL)
	{
		// Give the result to the parent class
		setDocument(pOutputDocument);
	}
}

WordTokenizer::~WordTokenizer()
{
	if (m_pDocument != NULL)
	{
		// This should have been set by setDocument()
		delete m_pDocument;
	}
}
