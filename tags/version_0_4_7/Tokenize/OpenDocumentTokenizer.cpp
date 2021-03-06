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

#include "OpenDocumentTokenizer.h"

using std::string;
using std::set;

/// This returns the MIME types supported by the library's tokenizer.
bool getTokenizerTypes(set<string> &types)
{
	types.clear();
	types.insert("application/vnd.sun.xml.writer");
	types.insert("application/vnd.sun.xml.writer.template");
	types.insert("application/vnd.sun.xml.calc");
	types.insert("application/vnd.sun.xml.calc.template");
	types.insert("application/vnd.sun.xml.draw");
	types.insert("application/vnd.sun.xml.draw.template");
	types.insert("application/vnd.sun.xml.impress");
	types.insert("application/vnd.sun.xml.impress.template");
	types.insert("application/vnd.sun.xml.writer.global");
	types.insert("application/vnd.sun.xml.math");
	types.insert("application/vnd.oasis.opendocument.chart");
	types.insert("application/vnd.oasis.opendocument.database");
	types.insert("application/vnd.oasis.opendocument.formula");
	types.insert("application/vnd.oasis.opendocument.graphics");
	types.insert("application/vnd.oasis.opendocument.graphics-template");
	types.insert("application/vnd.oasis.opendocument.image");
	types.insert("application/vnd.oasis.opendocument.presentation");
	types.insert("application/vnd.oasis.opendocument.presentation-template");
	types.insert("application/vnd.oasis.opendocument.spreadsheet");
	types.insert("application/vnd.oasis.opendocument.spreadsheet-template");
	types.insert("application/vnd.oasis.opendocument.text");
	types.insert("application/vnd.oasis.opendocument.text-master");
	types.insert("application/vnd.oasis.opendocument.text-template");
	types.insert("application/vnd.oasis.opendocument.text-web");

	return true;
}

/// This returns a pointer to a Tokenizer, allocated with new.
Tokenizer *getTokenizer(const Document *pDocument)
{
	return new OpenDocumentTokenizer(pDocument);
}

OpenDocumentTokenizer::OpenDocumentTokenizer(const Document *pDocument) :
	XmlTokenizer(NULL)
{
	// FIXME: unzip meta.xml and extract document information
	// Unip content.xml
	Document *pOutputDocument = runHelperProgram(pDocument, "unzip -p", "content.xml");
	if (pOutputDocument != NULL)
	{
		// Give the result to the parent class
		initialize(pOutputDocument);
	}
}

OpenDocumentTokenizer::OpenDocumentTokenizer() :
	XmlTokenizer()
{
}

OpenDocumentTokenizer::~OpenDocumentTokenizer()
{
}
