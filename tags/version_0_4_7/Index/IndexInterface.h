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
 
#ifndef _INDEX_INTERFACE_H
#define _INDEX_INTERFACE_H

#include <string>
#include <set>

#include "Tokenizer.h"
#include "DocumentInfo.h"

/// Interface implemented by indexes.
class IndexInterface
{
	public:
		virtual ~IndexInterface() {};

		/// Returns false if the index couldn't be opened.
		bool isGood(void) const { return m_goodIndex; }

		typedef enum { STORE_UNSTEM = 0, STORE_STEM, STORE_BOTH } StemmingMode;

		/// Sets the stemming mode.
		virtual void setStemmingMode(StemmingMode mode) { m_stemMode = mode; }

		/// Gets the index location.
		virtual std::string getLocation(void) const = 0;

		/// Indexes the given data.
		virtual bool indexDocument(Tokenizer &tokens, const std::set<std::string> &labels,
			unsigned int &docId) = 0;

		/// Returns a document's properties.
		virtual bool getDocumentInfo(unsigned int docId, DocumentInfo &docInfo) const = 0;

		/// Determines whether a document has a label.
		virtual bool hasLabel(unsigned int docId, const std::string &name) const = 0;

		/// Returns a document's labels.
		virtual bool getDocumentLabels(unsigned int docId, std::set<std::string> &labels) const = 0;

		/// Updates the given document.
		virtual bool updateDocument(unsigned int docId, Tokenizer &tokens) = 0;

		/// Updates a document's properties.
		virtual bool updateDocumentInfo(unsigned int docId, const DocumentInfo &docInfo) = 0;

		/// Sets a document's labels.
		virtual bool setDocumentLabels(unsigned int docId, const std::set<std::string> &labels,
			bool resetLabels = true) = 0;

		/// Checks whether the given URL is in the index.
		virtual unsigned int hasDocument(const std::string &url) const = 0;

		/// Unindexes the given document.
		virtual bool unindexDocument(unsigned int docId) = 0;

		/// Unindexes documents with the given label.
		virtual bool unindexDocuments(const std::string &labelName) = 0;

		/// Suggests terms.
		virtual unsigned int getCloseTerms(const std::string &term, std::set<std::string> &suggestions) = 0;

		/// Renames a label.
		virtual bool renameLabel(const std::string &name, const std::string &newName) = 0;

		/// Deletes all references to a label.
		virtual bool deleteLabel(const std::string &name) = 0;

		/// Flushes recent changes to the disk.
		virtual bool flush(void) = 0;

		/// Returns the number of documents.
		virtual unsigned int getDocumentsCount(const std::string &labelName = "") const = 0;

		/// Lists document IDs.
		virtual unsigned int listDocuments(std::set<unsigned int> &docIDList,
			unsigned int maxDocsCount = 0, unsigned int startDoc = 0) const = 0;

		/// Lists documents that have a label.
		virtual bool listDocumentsWithLabel(const std::string &name, std::set<unsigned int> &docIds,
			unsigned int maxDocsCount = 0, unsigned int startDoc = 0) const = 0;

	protected:
		StemmingMode m_stemMode;
		bool m_goodIndex;

		IndexInterface() { m_stemMode = STORE_UNSTEM; m_goodIndex = false; };

	private:
		IndexInterface(const IndexInterface &other);
		IndexInterface &operator=(const IndexInterface &other);

};

#endif // _INDEX_INTERFACE_H
