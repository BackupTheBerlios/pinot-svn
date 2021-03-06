/*
 *  Copyright 2005,2006 Fabrice Colin
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

#ifndef _WORD_TOKENIZER_H
#define _WORD_TOKENIZER_H

#include <string>
#include <set>

#include "Document.h"
#include "Tokenizer.h"

/// This returns the MIME types supported by the library's tokenizer.
bool getTokenizerTypes(std::set<std::string> &types);
/// This returns the data needs of the provided Tokenizer(s).
int getTokenizerDataNeeds(void);
/// This returns a pointer to a Tokenizer, allocated with new.
Tokenizer *getTokenizer(const Document *pDocument);

class WordTokenizer : public Tokenizer
{
	public:
		WordTokenizer(const Document *pDocument);
		virtual ~WordTokenizer();

	protected:
		Document *m_pStrippedDocument;

	private:
		WordTokenizer(const WordTokenizer &other);
		WordTokenizer& operator=(const WordTokenizer& other);

};

#endif // _WORD_TOKENIZER_H
