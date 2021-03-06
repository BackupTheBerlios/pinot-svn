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

#ifndef _OPENDOCUMENT_TOKENIZER_H
#define _OPENDOCUMENT_TOKENIZER_H

#include <string>
#include <set>

#include "Document.h"
#include "XmlTokenizer.h"

/// This returns the MIME types supported by the library's tokenizer.
bool getTokenizerTypes(std::set<std::string> &types);
/// This returns a pointer to a Tokenizer, allocated with new.
Tokenizer *getTokenizer(const Document *pDocument);

class OpenDocumentTokenizer : public XmlTokenizer
{
	public:
		OpenDocumentTokenizer(const Document *pDocument);
		virtual ~OpenDocumentTokenizer();

	private:
		OpenDocumentTokenizer(const OpenDocumentTokenizer &other);
		OpenDocumentTokenizer& operator=(const OpenDocumentTokenizer& other);

};

#endif // _OPENDOCUMENT_TOKENIZER_H
