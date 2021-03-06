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

#ifndef _WORKERTHREADS_HH
#define _WORKERTHREADS_HH

#include <time.h>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <pthread.h>
#include <sigc++/object.h>
#include <sigc++/slot.h>
#include <sigc++/connection.h>
#include <glibmm/dispatcher.h>
#include <glibmm/thread.h>
#include <glibmm/ustring.h>

#include "Document.h"
#include "MonitorInterface.h"
#include "IndexedDocument.h"
#include "DownloaderInterface.h"
#include "QueryProperties.h"
#include "Result.h"
#include "MonitorHandler.h"

class WorkerThread
{
	public:
		WorkerThread();
		virtual ~WorkerThread();

		static Glib::Dispatcher &getDispatcher(void);

		static void immediateFlush(bool doFlush);

		void setId(unsigned int id);

		unsigned int getId(void);

		void inBackground(void);

		bool isBackground(void);

		bool operator<(const WorkerThread &other) const;

		Glib::Thread *start(void);

		virtual std::string getType(void) const = 0;

		virtual bool stop(void) = 0;

		bool isDone(void) const;

		std::string getStatus(void) const;

	protected:
		/// Use a Dispatcher for thread safety
		static Glib::Dispatcher m_dispatcher;
		static pthread_mutex_t m_dispatcherMutex;
		static bool m_immediateFlush;
		bool m_joinable;
		unsigned int m_id;
		bool m_background;
		bool m_done;
		std::string m_status;

		void threadHandler(void);

		virtual void doWork(void) = 0;

		void emitSignal(void);

	private:
		WorkerThread(const WorkerThread &other);
		WorkerThread &operator=(const WorkerThread &other);

};

class ThreadsManager : virtual public SigC::Object
{
	public:
		ThreadsManager(const std::string &defaultIndexLocation,
			unsigned int maxIndexThreads);
		virtual ~ThreadsManager();

		bool start_thread(WorkerThread *pWorkerThread, bool inBackground = false);

		unsigned int get_threads_count(void);

		bool has_threads(void);

		void stop_threads(void);

		virtual void connect(void);

		virtual void disconnect(void);

		void on_thread_signal();

		bool read_lock_lists(void);

		bool write_lock_lists(void);

		void unlock_lists(void);

		Glib::ustring queue_index(const DocumentInfo &docInfo);

		bool pop_queue(const std::string &urlWasIndexed = "");

		void get_statistics(unsigned int &queueSize);

	protected:
		SigC::Connection m_threadsEndConnection;
		pthread_rwlock_t m_threadsLock;
		pthread_rwlock_t m_listsLock;
		std::map<WorkerThread *, Glib::Thread *> m_threads;
		std::string m_defaultIndexLocation;
		unsigned int m_maxIndexThreads;
		unsigned int m_nextThreadId;
		unsigned int m_backgroundThreadsCount;
		long m_numCPUs;
		SigC::Signal1<void, WorkerThread *> m_onThreadEndSignal;
		std::set<std::string> m_beingIndexed;

		bool read_lock_threads(void);
		bool write_lock_threads(void);
		void unlock_threads(void);
		WorkerThread *get_thread(void);
		Glib::ustring index_document(const DocumentInfo &docInfo);

	private:
		ThreadsManager(const ThreadsManager &other);
		ThreadsManager &operator=(const ThreadsManager &other);

};

class IndexBrowserThread : public WorkerThread
{
	public:
		IndexBrowserThread(const std::string &indexName, const std::string &labelName,
			unsigned int maxDocsCount, unsigned int startDoc);
		~IndexBrowserThread();

		std::string getType(void) const;

		std::string getIndexName(void) const;

		std::string getLabelName(void) const;

		unsigned int getDocumentsCount(void) const;

		const std::vector<IndexedDocument> &getDocuments(void) const;

		virtual bool stop(void);

	protected:
		std::string m_indexName;
		std::string m_labelName;
		unsigned int m_indexDocsCount;
		unsigned int m_maxDocsCount;
		unsigned int m_startDoc;
		std::vector<IndexedDocument> m_documentsList;

		virtual void doWork(void);

	private:
		IndexBrowserThread(const IndexBrowserThread &other);
		IndexBrowserThread &operator=(const IndexBrowserThread &other);

};

class QueryingThread : public WorkerThread
{
	public:
		QueryingThread(const std::string &engineName, const std::string &engineDisplayableName,
			const std::string &engineOption, const QueryProperties &queryProps);
		virtual ~QueryingThread();

		virtual std::string getType(void) const;

		std::string getEngineName(void) const;

		QueryProperties getQuery(void) const;

		const std::vector<Result> &getResults(std::string &charset) const;

		virtual bool stop(void);

	protected:
		std::string m_engineName;
		std::string m_engineDisplayableName;
		std::string m_engineOption;
		QueryProperties m_queryProps;
		std::vector<Result> m_resultsList;
		std::string m_resultsCharset;
		std::set<std::string> m_expandTerms;

		virtual void doWork(void);

	private:
		QueryingThread(const QueryingThread &other);
		QueryingThread &operator=(const QueryingThread &other);

};

class ExpandQueryThread : public WorkerThread
{
	public:
		ExpandQueryThread(const std::string &engineName, const std::string &engineOption,
			const QueryProperties &queryProps, const std::set<unsigned int> &relevantDocs);
		virtual ~ExpandQueryThread();

		virtual std::string getType(void) const;

		QueryProperties getQuery(void) const;

		const std::set<std::string> &getExpandTerms(void) const;

		virtual bool stop(void);

	protected:
		std::string m_engineName;
		std::string m_engineOption;
		QueryProperties m_queryProps;
		std::set<unsigned int> m_relevantDocs;
		std::set<std::string> m_expandTerms;

		virtual void doWork(void);

	private:
		ExpandQueryThread(const ExpandQueryThread &other);
		ExpandQueryThread &operator=(const ExpandQueryThread &other);

};

class LabelUpdateThread : public WorkerThread
{
	public:
		LabelUpdateThread(const std::set<std::string> &labelsToDelete,
			const std::map<std::string, std::string> &labelsToRename);
		virtual ~LabelUpdateThread();

		virtual std::string getType(void) const;

		virtual bool stop(void);

	protected:
		std::set<std::string> m_labelsToDelete;
		std::map<std::string, std::string> m_labelsToRename;

		virtual void doWork(void);

	private:
		LabelUpdateThread(const LabelUpdateThread &other);
		LabelUpdateThread &operator=(const LabelUpdateThread &other);

};

class DownloadingThread : public WorkerThread
{
	public:
		DownloadingThread(const DocumentInfo &docInfo);
		virtual ~DownloadingThread();

		virtual std::string getType(void) const;

		std::string getURL(void) const;

		const Document *getDocument(void) const;

		virtual bool stop(void);

	protected:
		DocumentInfo m_docInfo;
		Document *m_pDoc;
		DownloaderInterface *m_pDownloader;

		virtual void doWork(void);

	private:
		DownloadingThread(const DownloadingThread &other);
		DownloadingThread &operator=(const DownloadingThread &other);

};

class IndexingThread : public DownloadingThread
{
	public:
		IndexingThread(const DocumentInfo &docInfo, unsigned int docId,
			const std::string &indexLocation, bool allowAllMIMETypes = true);
		virtual ~IndexingThread();

		virtual std::string getType(void) const;

		const DocumentInfo &getDocumentInfo(void) const;

		std::string getLabelName(void) const;

		unsigned int getDocumentID(void) const;

		bool isNewDocument(void) const;

		virtual bool stop(void);

	protected:
		DocumentInfo m_docInfo;
		unsigned int m_docId;
		std::string m_indexLocation;
		bool m_allowAllMIMETypes;
		bool m_ignoreRobotsDirectives;
		bool m_update;

		virtual void doWork(void);

	private:
		IndexingThread(const IndexingThread &other);
		IndexingThread &operator=(const IndexingThread &other);

};

class UnindexingThread : public WorkerThread
{
	public:
		// Unindex documents from the internal index
		UnindexingThread(const std::set<unsigned int> &docIdList);
		// Unindex from the given index documents that have one of the labels
		UnindexingThread(const std::set<std::string> &labelNames, const std::string &indexLocation);
		virtual ~UnindexingThread();

		virtual std::string getType(void) const;

		unsigned int getDocumentsCount(void) const;

		virtual bool stop(void);

	protected:
		std::set<unsigned int> m_docIdList;
		std::set<std::string> m_labelNames;
		std::string m_indexLocation;
		unsigned int m_docsCount;

		virtual void doWork(void);

	private:
		UnindexingThread(const UnindexingThread &other);
		UnindexingThread &operator=(const UnindexingThread &other);

};

class UpdateDocumentThread : public WorkerThread
{
	public:
		// Update a document's properties
		UpdateDocumentThread(const std::string &indexName,
			unsigned int docId, const DocumentInfo &docInfo);
		virtual ~UpdateDocumentThread();

		virtual std::string getType(void) const;

		unsigned int getDocumentID(void) const;

		const DocumentInfo &getDocumentInfo(void) const;

		virtual bool stop(void);

	protected:
		std::string m_indexName;
		unsigned int m_docId;
		DocumentInfo m_docInfo;

		virtual void doWork(void);

	private:
		UpdateDocumentThread(const UpdateDocumentThread &other);
		UpdateDocumentThread &operator=(const UpdateDocumentThread &other);

};

class MonitorThread : public WorkerThread
{
	public:
		MonitorThread(MonitorInterface *pMonitor, MonitorHandler *pHandler);
		virtual ~MonitorThread();

		virtual std::string getType(void) const;

		virtual bool stop(void);

		SigC::Signal3<void, const DocumentInfo&, const std::string&, bool>& getDirectoryFoundSignal(void);

	protected:
		int m_ctrlReadPipe;
		int m_ctrlWritePipe;
		MonitorInterface *m_pMonitor;
		MonitorHandler *m_pHandler;
		SigC::Signal3<void, const DocumentInfo&, const std::string&, bool> m_signalDirectoryFound;

		void processEvents(void);
		virtual void doWork(void);

	private:
		MonitorThread(const MonitorThread &other);
		MonitorThread &operator=(const MonitorThread &other);

};

class DirectoryScannerThread : public WorkerThread
{
	public:
		DirectoryScannerThread(const std::string &dirName, bool isSource,
			unsigned int maxLevel, bool followSymLinks,
			MonitorInterface *pMonitor, MonitorHandler *pHandler);
		virtual ~DirectoryScannerThread();

		virtual std::string getType(void) const;

		virtual std::string getDirectory(void) const;

		virtual bool stop(void);

		SigC::Signal3<void, const DocumentInfo&, const std::string&, bool>& getFileFoundSignal(void);

	protected:
		std::string m_dirName;
		unsigned int m_maxLevel;
		bool m_followSymLinks;
		MonitorInterface *m_pMonitor;
		MonitorHandler *m_pHandler;
		unsigned int m_currentLevel;
		unsigned int m_sourceId;
		SigC::Signal3<void, const DocumentInfo&, const std::string&, bool> m_signalFileFound;

		void foundFile(const DocumentInfo &docInfo);
		bool scanEntry(const std::string &entryName);
		virtual void doWork(void);

	private:
		DirectoryScannerThread(const DirectoryScannerThread &other);
		DirectoryScannerThread &operator=(const DirectoryScannerThread &other);

};

#endif // _WORKERTHREADS_HH
