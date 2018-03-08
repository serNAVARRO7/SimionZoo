#include "experience-replay.h"
#include "app.h"
#include "config.h"
#include "logger.h"
#include "named-var-set.h"
#include "simgod.h"
#include <algorithm>

CExperienceTuple::CExperienceTuple()
{
	s = CSimionApp::get()->pWorld->getDynamicModel()->getStateInstance();
	a = CSimionApp::get()->pWorld->getDynamicModel()->getActionInstance();
	s_p = CSimionApp::get()->pWorld->getDynamicModel()->getStateInstance();
}

void CExperienceTuple::copy(const CState* s, const CAction* a, const CState* s_p, double r, double probability)
{
	this->s->copy(s);
	this->a->copy(a);
	this->s_p->copy(s_p);
	this->r = r;
	this->probability = probability;
}


CExperienceReplay::CExperienceReplay(CConfigNode* pConfigNode)
{
	m_blockSize = INT_PARAM(pConfigNode, "Buffer-Size", "Size of the buffer used to store experience tuples", 1000);
	m_updateBatchSize = INT_PARAM(pConfigNode, "Update-Batch-Size", "Number of tuples used each time-step in the update", 10);

	CLogger::logMessage(MessageType::Info, "Experience replay buffer initialized");

	m_pTupleBuffer = 0;
	m_currentPosition = 0;
	m_numTuples = 0;
}

CExperienceReplay::CExperienceReplay() : CDeferredLoad()
{
	//default behaviour when experience replay is not used
	m_blockSize.set(0);
	m_updateBatchSize.set(0);

	m_pTupleBuffer = 0;
	m_currentPosition = 0;
	m_numTuples = 0;
}

bool CExperienceReplay::bUsing()
{
	return m_blockSize.get() != 0;
}

void CExperienceReplay::deferredLoadStep()
{
	m_pTupleBuffer = new CExperienceTuple[m_blockSize.get()];
}

CExperienceReplay::~CExperienceReplay()
{
	if (m_pTupleBuffer)
		delete[] m_pTupleBuffer;
}

int CExperienceReplay::getUpdateBatchSize()
{
	return std::min(m_updateBatchSize.get(), m_numTuples);
}

void CExperienceReplay::addTuple(const CState* s, const  CAction* a, const CState* s_p, double r, double probability)
{
	//add the experience tuple to the buffer
	if (!bUsing()) return;

	if (m_numTuples < m_blockSize.get())
	{
		//the buffer is not yet full
		m_pTupleBuffer[m_currentPosition].copy(s, a, s_p, r, probability);
		++m_numTuples;
	}
	else
	{
		//the buffer is full
		m_pTupleBuffer[m_currentPosition].copy(s, a, s_p, r, probability);
	}
	m_currentPosition = ++m_currentPosition % m_blockSize.get();
}

CExperienceTuple* CExperienceReplay::getRandomTupleFromBuffer()
{
	int randomIndex = rand() % m_numTuples;

	return &m_pTupleBuffer[randomIndex];
}