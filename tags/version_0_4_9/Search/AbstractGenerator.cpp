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
#include <ctype.h>
#include <sys/time.h>
#include <map>
#include <iostream>
#include <utility>

#include "Timer.h"
#include "AbstractGenerator.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::map;

unsigned int AbstractGenerator::m_maxSeedTerms = 4;
unsigned int AbstractGenerator::m_minTermPositions = 10;

AbstractGenerator::PositionWindow::PositionWindow() :
	m_backWeight(0),
	m_forwardWeight(0)
{
}

AbstractGenerator::PositionWindow::~PositionWindow()
{
}

AbstractGenerator::AbstractGenerator(const Xapian::Database *pIndex,
	unsigned int wordsCount) :
	m_pIndex(pIndex),
	m_wordsCount(wordsCount)
{
}

AbstractGenerator::~AbstractGenerator()
{
}

/// Attempts to generate an abstract of wordsCount words.
string AbstractGenerator::generateAbstract(const vector<string> &seedTerms,
	Xapian::docid docId)
{
	map<Xapian::termpos, PositionWindow> abstractWindows;
	map<Xapian::termpos, string> wordsBuffer;
	string summary;
	Xapian::termpos bestPosition = 0, startPosition =0;
	unsigned int seedTermsCount = 0, bestWeight = 0;
	bool topTerm = true;

	if ((m_pIndex == NULL) ||
		(seedTerms.empty() == true))
	{
		return "";
	}

#ifdef DEBUG
	Timer timer;
	timer.start();
#endif
	for (vector<string>::const_iterator termIter = seedTerms.begin();
		termIter != seedTerms.end(); ++termIter)
	{
		string termName(*termIter);

		if (seedTermsCount >= m_maxSeedTerms)
		{
			// Enough terms
			break;
		}

#ifdef DEBUG
		cout << "AbstractGenerator::generateAbstract: term " << termName << endl;
#endif
		try
		{
			// Go through that term's position list in the document
			for (Xapian::PositionIterator positionIter = m_pIndex->positionlist_begin(docId, termName);
				positionIter != m_pIndex->positionlist_end(docId, termName); ++positionIter)
			{
				Xapian::termpos termPos = *positionIter;

				// Take all the top term's positions into account, and some of 
				// the other terms' too if the minimum number is not reached
				if ((m_minTermPositions > abstractWindows.size()) ||
					(topTerm == true))
				{
					abstractWindows[termPos] = PositionWindow();
				}

				// Look for other terms close to that position
				for (map<Xapian::termpos, PositionWindow>::iterator winIter = abstractWindows.begin();
					winIter != abstractWindows.end(); ++winIter)
				{
					// Is this within the number of words we are interested in ?
					if (winIter->first > termPos)
					{
						if (winIter->first - termPos <= m_wordsCount)
						{
							++winIter->second.m_backWeight;
						}
					}
					else
					{
						if (termPos - winIter->first <= m_wordsCount)
						{
							++winIter->second.m_forwardWeight;
						}
					}
				}
			}

			topTerm = false;
			++seedTermsCount;
		}
		catch (const Xapian::Error &error)
		{
#ifdef DEBUG
			cout << "AbstractGenerator::generateAbstract: " << error.get_msg() << endl;
#endif
		}

#ifdef DEBUG
		cout << "AbstractGenerator::generateAbstract: " << abstractWindows.size()
			<< " positions, " << seedTermsCount << " terms" << endl;
#endif
	}

	// Go through positions and find out which one has the largest
	// number of terms nearby
	for (map<Xapian::termpos, PositionWindow>::iterator winIter = abstractWindows.begin();
		winIter != abstractWindows.end(); ++winIter)
	{
		if (bestWeight < winIter->second.m_backWeight)
		{
			bestPosition = winIter->first;
			startPosition = bestPosition - m_wordsCount;
			bestWeight = winIter->second.m_backWeight;
		}
		if (bestWeight < winIter->second.m_forwardWeight)
		{
			bestPosition = startPosition = winIter->first;
			bestWeight = winIter->second.m_forwardWeight;
		}
	}
#ifdef DEBUG
	cout << "AbstractGenerator::generateAbstract: best position is "
		<< bestPosition << ":" << startPosition << " with weight " << bestWeight << endl;
#endif

	// Go through the position list of each term
	for (Xapian::TermIterator termIter = m_pIndex->termlist_begin(docId);
		termIter != m_pIndex->termlist_end(docId); ++termIter)
	{
		string termName(*termIter);

		// Skip prefixed terms
		if (isupper((int)termName[0]) != 0)
		{
			continue;
		}

		for (Xapian::PositionIterator positionIter = m_pIndex->positionlist_begin(docId, termName);
			positionIter != m_pIndex->positionlist_end(docId, termName); ++positionIter)
		{
			Xapian::termpos termPos = *positionIter;

			// ...and get those that fall in the abstract window
			if ((startPosition <= termPos + 1) &&
				(termPos < startPosition + m_wordsCount))
			{
				wordsBuffer[termPos] = termName;
			}
		}
	}

	for (map<Xapian::termpos, string>::iterator wordIter = wordsBuffer.begin();
		wordIter != wordsBuffer.end(); ++wordIter)
	{
		summary += " ";
		summary += wordIter->second;
	}
#ifdef DEBUG
	cout << "AbstractGenerator::generateAbstract: summarized document "
		<< docId << " in " << timer.stop()/1000 << " ms" << endl;
#endif

	return summary;
}
