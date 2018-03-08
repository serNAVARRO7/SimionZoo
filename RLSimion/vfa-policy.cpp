#include "named-var-set.h"
#include "noise.h"
#include "vfa.h"
#include "policy-learner.h"
#include "config.h"
#include "worlds/world.h"
#include "features.h"
#include "policy.h"
#include "app-rlsimion.h"
#include "SimGod.h"
#include "featuremap.h"
#include <iostream>
#include <typeinfo>
#include "named-var-set.h"
#include <algorithm>

#define PROBABILITY_INTEGRATION_WIDTH 0.05
//0.05

std::shared_ptr<CPolicy> CPolicy::getInstance(CConfigNode* pConfigNode)
{
	return CHOICE<CPolicy>(pConfigNode, "Policy", "The policy type",
	{
		{"Deterministic-Policy-Gaussian-Noise",CHOICE_ELEMENT_NEW<CDeterministicPolicyGaussianNoise>},
		{"Stochastic-Policy-Gaussian-Noise",CHOICE_ELEMENT_NEW<CStochasticGaussianPolicy>},
		{"Uniform-Policy",CHOICE_ELEMENT_NEW<CStochasticUniformPolicy>}
	});
}

CPolicy::CPolicy(CConfigNode* pConfigNode)
{
	m_outputActionIndex = ACTION_VARIABLE(pConfigNode, "Output-Action", "The output action variable");

	//only CDiscreteActionFeatureMap is supported for the discrete mode
	if (CSimGod::getGlobalActionFeatureMap().get() != NULL)
		m_discreteActionSpace = (typeid(*CSimGod::getGlobalActionFeatureMap().get()) == typeid(CDiscreteActionFeatureMap));
	else
		m_discreteActionSpace = false;

	if (m_discreteActionSpace)
	{
		//calculate the density of the space
		m_space_density = 1.0;
		for (size_t i = 0; i < CWorld::getDynamicModel()->getActionDescriptor().size(); i++)
		{
			m_space_density *= ((CSingleDimensionDiscreteActionVariableGrid*)(((CDiscreteActionFeatureMap*)CSimGod::getGlobalActionFeatureMap().get())->returnGrid()[i]))->getStepSize();
		}
	}
}

CPolicy::~CPolicy()
{
}

CDeterministicPolicy::CDeterministicPolicy(CConfigNode* pConfigNode) : CPolicy(pConfigNode) {}

CStochasticPolicy::CStochasticPolicy(CConfigNode* pConfigNode) : CPolicy(pConfigNode) {}


//CStochasticUniformPolicy/////////////////////////////////////////
/////////////////////////////////////////////////////////
CStochasticUniformPolicy::CStochasticUniformPolicy(CConfigNode* pConfigNode)
	: CStochasticPolicy(pConfigNode)
{
	CDescriptor& pActionDescriptor = CWorld::getDynamicModel()->getActionDescriptor();
	m_minActionValue = pActionDescriptor[m_outputActionIndex.get()].getMin();
	m_maxActionValue = pActionDescriptor[m_outputActionIndex.get()].getMax();
	m_action_width = pActionDescriptor[m_outputActionIndex.get()].getRangeWidth();
}

CStochasticUniformPolicy::~CStochasticUniformPolicy()
{
}

void CStochasticUniformPolicy::getParameterGradient(const CState* s, const CAction* a, CFeatureList* pOutGradient)
{
	throw "CStochasticUniformPolicy::getParameterGradient() is not implemented";
}

double CStochasticUniformPolicy::selectAction(const CState *s, CAction *a)
{
	int actionIndex = m_outputActionIndex.get();

	//values for the case of a continuous action space
	double randomValue = getRandomValue() * m_action_width + m_minActionValue;
	double probability = 1.0 / m_action_width * PROBABILITY_INTEGRATION_WIDTH;

	if (m_discreteActionSpace)
	{
		CSingleDimensionDiscreteActionVariableGrid* grid = ((CSingleDimensionDiscreteActionVariableGrid*)(((CDiscreteActionFeatureMap*)CSimGod::getGlobalActionFeatureMap().get())->returnGrid()[actionIndex]));
		randomValue = grid->getCenters()[grid->getClosestCenter(randomValue)];
		probability = 1.0 / grid->getNumCenters();
	}

	a->set(actionIndex, randomValue);

	return probability;
}

double CStochasticUniformPolicy::getProbability(const CState* s, const CAction* a, bool bStochastic)
{
	int actionIndex = m_outputActionIndex.get();

	//values for the case of a continuous action space
	double probability = 1.0 / m_action_width * PROBABILITY_INTEGRATION_WIDTH;

	if (m_discreteActionSpace)
	{
		CSingleDimensionDiscreteActionVariableGrid* grid = ((CSingleDimensionDiscreteActionVariableGrid*)(((CDiscreteActionFeatureMap*)CSimGod::getGlobalActionFeatureMap().get())->returnGrid()[actionIndex]));
		probability = 1.0 / grid->getNumCenters();
	}

	return probability;
}

void CStochasticUniformPolicy::getFeatures(const CState* state, CFeatureList* outFeatureList)
{
	throw "CStochasticUniformPolicy::getFeatures() is not implemented";
}

void CStochasticUniformPolicy::addFeatures(const CFeatureList* pFeatureList, double factor)
{
	throw "CStochasticUniformPolicy::addFeatures() is not implemented";
}

double CStochasticUniformPolicy::getDeterministicOutput(const CFeatureList* pFeatureList)
{
	throw "CStochasticUniformPolicy::getDeterministicOutput() is not implemented";
}


//CDetPolicyGaussianNoise////////////////////////////////
/////////////////////////////////////////////////////////
CDeterministicPolicyGaussianNoise::CDeterministicPolicyGaussianNoise(CConfigNode* pConfigNode)
	: CDeterministicPolicy(pConfigNode)
{
	m_pDeterministicVFA = CHILD_OBJECT<CLinearStateVFA>(pConfigNode, "Deterministic-Policy-VFA"
		, "The parameterized VFA that approximates the function");
	m_pExpNoise = CHILD_OBJECT_FACTORY<CNoise>(pConfigNode, "Exploration-Noise"
		, "Parameters of the noise used as exploration");

	CDescriptor& pActionDescriptor = CWorld::getDynamicModel()->getActionDescriptor();
	m_pDeterministicVFA->saturateOutput(pActionDescriptor[m_outputActionIndex.get()].getMin()
		, pActionDescriptor[m_outputActionIndex.get()].getMax());

	m_lastNoise = 0.0;
	CSimionApp::get()->pLogger->addVarToStats<double>("Policy", "Noise", m_lastNoise);
}

CDeterministicPolicyGaussianNoise::~CDeterministicPolicyGaussianNoise()
{
}

void CDeterministicPolicyGaussianNoise::getParameterGradient(const CState* s, const CAction* a, CFeatureList* pOutGradient)
{
	//0. Grad_u pi(a|s)/pi(a|s) = (a - pi(s)) * phi(s) / sigma*2
	m_pDeterministicVFA->getFeatures(s, pOutGradient);

	double sigma = std::max(0.0000001, m_pExpNoise->getVariance());

	double noise = a->get(m_outputActionIndex.get())
		- m_pDeterministicVFA->get((const CFeatureList*)pOutGradient);

	double unscaled_noise = m_pExpNoise->unscale(noise);
	double factor = unscaled_noise / (sigma*sigma);

	pOutGradient->mult(factor);
}

double CDeterministicPolicyGaussianNoise::selectAction(const CState *s, CAction *a)
{
	if (!CSimionApp::get()->pExperiment->isEvaluationEpisode())
		m_lastNoise = m_pExpNoise->getSample();
	else m_lastNoise = 0.0;

	double output = m_pDeterministicVFA->get(s);

	a->set(m_outputActionIndex.get(), output + m_lastNoise);

	if (!CSimionApp::get()->pExperiment->isEvaluationEpisode())
		return m_pExpNoise->getSampleProbability(m_lastNoise);
	return 1.0;
}

double CDeterministicPolicyGaussianNoise::getProbability(const CState* s, const CAction* a, bool bStochastic)
{
	double noise;
	if (CSimionApp::get()->pSimGod->useSampleImportanceWeights())
	{
		noise = a->get(m_outputActionIndex.get()) - m_pDeterministicVFA->get(s);
		return m_pExpNoise->getSampleProbability(noise, !bStochastic);
	}
	return 1.0;
}

void CDeterministicPolicyGaussianNoise::getFeatures(const CState* state, CFeatureList* outFeatureList)
{
	m_pDeterministicVFA->getFeatures(state, outFeatureList);
}

void CDeterministicPolicyGaussianNoise::addFeatures(const CFeatureList* pFeatureList, double factor)
{
	m_pDeterministicVFA->add(pFeatureList, factor);
}

double CDeterministicPolicyGaussianNoise::getDeterministicOutput(const CFeatureList* pFeatureList)
{
	return m_pDeterministicVFA->get(pFeatureList);
}


//CStoPolicyGaussianNoise//////////////////////////
////////////////////////////////////////////////
CStochasticGaussianPolicy::CStochasticGaussianPolicy(CConfigNode* pConfigNode)
	: CStochasticPolicy(pConfigNode)
{
	m_pMeanVFA = CHILD_OBJECT<CLinearStateVFA>(pConfigNode, "Mean-VFA", "The parameterized VFA that approximates the function");
	m_pSigmaVFA = CHILD_OBJECT<CLinearStateVFA>(pConfigNode, "Sigma-VFA", "The parameterized VFA that approximates variance(s)");
	//m_pSigmaVFA->saturateOutput(0.0, 1.0);
	m_pSigmaVFA->setIndexOffset(m_pMeanVFA->getNumWeights());
	m_pMeanFeatures = new CFeatureList("Sto-Policy/mean-features");
	m_pSigmaFeatures = new CFeatureList("Sto-Policy/sigma-features");

	m_lastNoise = 0.0;
	CSimionApp::get()->pLogger->addVarToStats<double>("Stoch.Policy", "Noise", m_lastNoise);
	//will be used as temp. buffers in the add method
	//m_pMeanAddList = new CFeatureList("meanAddList");
	//m_pSigmaAddList = new CFeatureList("sigmaAddList");
}

CStochasticGaussianPolicy::~CStochasticGaussianPolicy()
{
	delete m_pMeanFeatures;
	delete m_pSigmaFeatures;
}

double clip(double n, double lower, double upper)
{
	return std::max(lower, std::min(n, upper));
}

double CStochasticGaussianPolicy::selectAction(const CState *s, CAction *a)
{
	double mean = m_pMeanVFA->get(s);
	double sigma = exp(m_pSigmaVFA->get(s));
	double output = mean;
	double probability;

	//TODO: Add If statement again?
	//if (!CSimionApp::get()->pExperiment->isEvaluationEpisode())
	{
		//Training: add noise
		sigma = exp(m_pSigmaVFA->get(s));
		output = CGaussianNoise::getNormalDistributionSample(mean, sigma);
	}

	//clip the output between the min and max value of the action space
	unsigned int actionIndex = m_outputActionIndex.get();
	output = clip(output, a->getProperties()[actionIndex].getMin(), a->getProperties()[actionIndex].getMax());

	//disctingtion between continuous and discrete action variables
	if (m_discreteActionSpace)
	{
		CSingleDimensionDiscreteActionVariableGrid* grid = ((CSingleDimensionDiscreteActionVariableGrid*)(((CDiscreteActionFeatureMap*)CSimGod::getGlobalActionFeatureMap().get())->returnGrid()[actionIndex]));
		output = grid->getCenters()[grid->getClosestCenter(output)];

		probability = CGaussianNoise::getPDF(mean, sigma, output) * m_space_density;
	}
	else
	{
		probability = CGaussianNoise::getSampleProbability(mean, sigma, output);
	}

	m_lastNoise = output - mean;

	a->set(actionIndex, output);

#ifdef _DEBUG
	cout << "select action - mean: " << mean << "\tsigma: " << sigma << "\tlog(sigma): " << m_pSigmaVFA->get(s) << "\toutput: " << output << "\n";
#endif
	//this is only an approximation as the PDF now looks differntly because of the clipping
	//this means, that the values at the borders will be higher as predicted by this function call
	//maybe this is causing problems, but I do not see a way to solve this
	return probability;
}

double CStochasticGaussianPolicy::getProbability(const CState *s, const CState *a, bool bStochastic)
{
	double mean = m_pMeanVFA->get(s);

	if (bStochastic && CSimionApp::get()->pSimGod->useSampleImportanceWeights())
	{
		unsigned int actionIndex = m_outputActionIndex.get();
		double value = a->get(actionIndex);

		if (m_discreteActionSpace)
		{
			CSingleDimensionDiscreteActionVariableGrid* grid = ((CSingleDimensionDiscreteActionVariableGrid*)(((CDiscreteActionFeatureMap*)CSimGod::getGlobalActionFeatureMap().get())->returnGrid()[actionIndex]));
			//TODO: Multiply with width of the action space!
			//P(x) = pdf(x) * space_density
			return CGaussianNoise::getPDF(mean, exp(m_pSigmaVFA->get(s)), value) * m_space_density;
		}
		else
		{
			return CGaussianNoise::getSampleProbability(mean, exp(m_pSigmaVFA->get(s)), value);
		}
	}
	return 1.0;
}

//according to https://hal.inria.fr/hal-00764281/document
void CStochasticGaussianPolicy::getParameterGradient(const CState* s, const CAction* a, CFeatureList* pOutGradient)
{
	m_pMeanFeatures->clear();
	m_pSigmaFeatures->clear();

	m_pMeanVFA->getFeatures(s, m_pMeanFeatures);
	m_pSigmaVFA->getFeatures(s, m_pSigmaFeatures);

	//mu(s) = u_mu * x_mu(s)
	//sigma(s) = exp(u_sigma * x_sigma(s))
	double mean = m_pMeanVFA->get(m_pMeanFeatures);
	double sigma = exp(m_pSigmaVFA->get(m_pSigmaFeatures));

	//a. Grad_u_mu pi(a|s)/pi(a|s) = (a - mu(s)) / sigma(s)^2 * x_mu(s)
	double noise = a->get(m_outputActionIndex.get()) - mean;

	double factor = noise / (sigma*sigma);
	pOutGradient->addFeatureList(m_pMeanFeatures, factor);

	//b. Grad_u_sigma pi(a|s)/pi(a|s) = ((a - mu(s))^2 / (sigma(s)^2) - 1) * x_sigma(s)
	factor = (noise*noise) / (sigma*sigma) - 1.0;
	pOutGradient->addFeatureList(m_pSigmaFeatures, factor);
}

void CStochasticGaussianPolicy::getFeatures(const CState* state, CFeatureList* outFeatureList)
{
	m_pMeanVFA->getFeatures(state, outFeatureList);
	m_pSigmaVFA->getFeatures(state, m_pSigmaFeatures);
	outFeatureList->addFeatureList(m_pSigmaFeatures);
}
void CStochasticGaussianPolicy::addFeatures(const CFeatureList* pFeatureList, double factor)
{
	//m_pMeanAddList->clear();
	//m_pSigmaAddList->clear();

	//split the long pFeatureList into the two sublists, one for the mean and one for the std.dev.
	//unsigned int m_numWeights = m_pMeanVFA->getNumWeights();
	//pFeatureList->split(m_pMeanAddList, m_pSigmaAddList, m_numWeights);

	//m_pMeanVFA->add(m_pMeanAddList);
	//m_pSigmaVFA->add(m_pSigmaAddList);
	m_pMeanVFA->add(pFeatureList);
	m_pSigmaVFA->add(pFeatureList);
}
double CStochasticGaussianPolicy::getDeterministicOutput(const CFeatureList* pFeatureList)
{
	return m_pMeanVFA->get(pFeatureList);
}