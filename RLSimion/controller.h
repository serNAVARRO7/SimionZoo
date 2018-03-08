#pragma once

#include "simion.h"
#include "parameters.h"
class CNamedVarSet;
typedef CNamedVarSet CState;
typedef CNamedVarSet CAction;

class CConfigNode;
class CNumericValue;

class CController : public CSimion
{

public:
	virtual ~CController(){}
	virtual int getNumOutputs()= 0;
	virtual int getOutputActionIndex(int output)= 0;

	static std::shared_ptr<CController> getInstance(CConfigNode* pConfigNode);

	//regular controllers need not update. Default implementation does nothing but it can be overriden
	//by adaptive controllers if need to
	virtual double update(const CState *s, const CAction *a, const CState *s_p, double r, double behaviorProb) = 0;

	virtual double selectAction(const CState *s, CAction *a)= 0;
};

class CLQRGain
{
public:
	CLQRGain(CConfigNode* pConfigNode);
	virtual ~CLQRGain(){}
	STATE_VARIABLE m_variableIndex;
	DOUBLE_PARAM m_gain;
};

class CLQRController : public CController
{
	MULTI_VALUE<CLQRGain> m_gains;
	ACTION_VARIABLE m_outputActionIndex;
public:
	CLQRController(CConfigNode* pConfigNode);
	virtual ~CLQRController();

	int getNumOutputs();
	int getOutputActionIndex(int output);

	double selectAction(const CState *s,CAction *a);
	virtual double update(const CState *s, const CAction *a, const CState *s_p, double r, double behaviorProb) { return 1.0; }
};

class CPIDController : public CController
{
	CHILD_OBJECT_FACTORY<CNumericValue> m_pKP;
	CHILD_OBJECT_FACTORY<CNumericValue> m_pKI;
	CHILD_OBJECT_FACTORY<CNumericValue> m_pKD;
	ACTION_VARIABLE m_outputActionIndex;
	double m_intError;
	STATE_VARIABLE m_errorVariableIndex;
public:
	CPIDController(CConfigNode* pConfigNode);
	virtual ~CPIDController();

	int getNumOutputs();
	int getOutputActionIndex(int output);

	double selectAction(const CState *s,CAction *a);
	virtual double update(const CState *s, const CAction *a, const CState *s_p, double r, double behaviorProb) { return 1.0; }
};

class CWindTurbineVidalController : public CController
{
protected:
	//aux function
	double sgn(double value);

	//state variable indices
	int m_omega_g, m_d_omega_g;
	int m_omega_r, m_d_omega_r;
	int m_E_p, m_T_g, m_beta;
	int m_E_int_omega_r;
	//action variable indices
	int m_a_beta, m_a_T_g;
	double m_ratedPower;
	double m_genElecEff;
	double m_lastT_g = 0.0;
	DOUBLE_PARAM m_pA, m_pK_alpha, m_pKP, m_pKI;
public:
	CWindTurbineVidalController(CConfigNode* pConfigNode);
	virtual ~CWindTurbineVidalController();

	int getNumOutputs();
	int getOutputActionIndex(int output);

	virtual double selectAction(const CState *s,CAction *a);
	virtual double update(const CState *s, const CAction *a, const CState *s_p, double r, double behaviorProb) { return 1.0; }
};

class CWindTurbineBoukhezzarController : public CController
{
protected:
	//state variable indices
	int m_omega_g, m_d_omega_g;
	int m_E_p, m_T_a_index, m_T_g, m_d_T_g;
	int m_beta, m_E_int_omega_g;

	//action variable indices
	int m_a_beta, m_a_T_g;
	
	DOUBLE_PARAM m_pC_0, m_pKP, m_pKI;
	double m_K_t, m_J_t;
	double m_lastT_g = 0.0;
	double m_genElecEff;
public:
	CWindTurbineBoukhezzarController(CConfigNode* pConfigNode);
	virtual ~CWindTurbineBoukhezzarController();

	int getNumOutputs();
	int getOutputActionIndex(int output);

	virtual double selectAction(const CState *s,CAction *a);
	virtual double update(const CState *s, const CAction *a, const CState *s_p, double r, double behaviorProb) { return 1.0; }
};

class CWindTurbineJonkmanController : public CController
{
protected:
	int m_omega_g, m_d_omega_g;
	int m_E_p, m_T_g, m_beta;
	int m_a_beta, m_a_T_g;

	//generator speed filter's parameters and variables
	DOUBLE_PARAM m_CornerFreq;
	double m_GenSpeedF;
	//generator torque controller's parameters and variables
	DOUBLE_PARAM m_VS_SlPc, m_VS_Rgn2K, m_VS_Rgn2Sp, m_VS_CtInSp;
	DOUBLE_PARAM m_VS_Rgn3MP;
	double m_ratedGenSpeed, m_ratedPower;
	double m_VS_SySp, m_VS_Slope15, m_VS_Slope25, m_VS_TrGnSp;
	//pitch controller's parameters and variables
	double m_IntSpdErr;
	DOUBLE_PARAM m_PC_KK, m_PC_KP, m_PC_KI;
	DOUBLE_PARAM m_PC_RefSpd;
public:
	CWindTurbineJonkmanController(CConfigNode* pConfigNode);
	virtual ~CWindTurbineJonkmanController();

	int getNumOutputs();
	int getOutputActionIndex(int output);

	virtual double selectAction(const CState *s,CAction *a);
	virtual double update(const CState *s, const CAction *a, const CState *s_p, double r, double behaviorProb) { return 1.0; }
};