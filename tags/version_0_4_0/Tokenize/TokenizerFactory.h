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

#ifndef _TOKENIZER_FACTORY_H
#define _TOKENIZER_FACTORY_H

#include <string>
#include <map>

#include "Tokenizer.h"

#include <string>
#include <map>
#include <set>

class TokenizerFactory
{
	public:
		virtual ~TokenizerFactory();

		/**
		  * This returns the MIME type supported by the library's tokenizer.
		  * The character string is allocated with new[].
		  */
		typedef char *(getTokenizerTypeFunc)(unsigned int typeNum);
		/// This returns a pointer to a Tokenizer, allocated with new.
		typedef Tokenizer *(getTokenizerFunc)(const Document *);

		/// Loads the tokenizer libraries in the given directory.
		static unsigned int loadTokenizers(const string &dirName);

		/// Returns a Tokenizer that handles the given file's type; NULL if unavailable.
		static Tokenizer *getTokenizer(const string &fileName, const Document *pDocument);

		/// Returns a Tokenizer that handles the given MIME type; NULL if unavailable.
		static Tokenizer *getTokenizerByType(const string &type, const Document *pDocument);

		/// Returns all supported MIME types.
		static void getSupportedTypes(set<string> &types);

		/// Indicates whether a MIME type is supported or not.
		static bool isSupportedType(const string &type);

		/// Unloads all tokenizer libraries.
		static void unloadTokenizers(void);

	protected:
		static map<string, string> m_types;
		static map<string, void *> m_handles;

		TokenizerFactory();

		static Tokenizer *getLibraryTokenizer(const string &type, const Document *pDocument);

	private:
		TokenizerFactory(const TokenizerFactory &other);
		TokenizerFactory& operator=(const TokenizerFactory& other);

};

#endif // _TOKENIZER_FACTORY_H
