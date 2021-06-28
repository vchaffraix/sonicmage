#pragma once
#include <JuceHeader.h>
#include <math.h>

#define DEFAULT_SMOOTHING_TIME_MS 1
#define DEFAULT_SAMPLERATE 44100
#define THRESHOLD_NULLIFY 1e-3

class ParamSmoothExp
{
public:
	ParamSmoothExp() { prepare(DEFAULT_SMOOTHING_TIME_MS, DEFAULT_SAMPLERATE); }
	ParamSmoothExp(float smoothingTimeMs, float sampleRate) { prepare(smoothingTimeMs, sampleRate); }
	~ParamSmoothExp() {}

	void prepare(float smoothingTimeMs, float sampleRate, float z_init = 0.0f)
	{
		const float c_twoPi = 6.283185307179586476925286766559f;
		a = exp(-c_twoPi / (smoothingTimeMs * 0.001f * sampleRate));
		b = 1.0f - a;
		z = z_init;
		target = z_init;
		idle = true;
	}

	inline float process()
    {
		if (!idle) {
			z = z * a + target  * b;
			auto _nullify = z - target;
			_nullify = _nullify >= 0 ? _nullify : -_nullify;
			if (_nullify < THRESHOLD_NULLIFY)
			{
				z = target;
				idle = true;
			}
		}
        return z;
    }

	void setTarget(float t)
	{
		target = t;
		idle = false;
	}

	float getTarget()
	{
		return target;
	}

	float getValue()
	{
		return z;
	}

private:
	float a;
	float b;
	float z;
	bool idle;
	std::atomic<float> target;
};

class ParamSmoothLin
{
public:
	ParamSmoothLin() { prepare(DEFAULT_SMOOTHING_TIME_MS, DEFAULT_SAMPLERATE); }
	ParamSmoothLin(float smoothingTimeMs, float sampleRate) { prepare(smoothingTimeMs, sampleRate); }
	~ParamSmoothLin() {}

	void prepare(float smoothingTimeMs, float sampleRate, float z_init = 0.0f)
	{
		steps = sampleRate * (smoothingTimeMs / 1000.0f);
		currentStep = steps;
		z = z_init;
		target = z_init;
	}

	inline float process()
    {
		if (currentStep>0) {
			z += stepDelta;
			currentStep--;
		}
        return z;
    }

	void setTarget(float t)
	{
		target = t;
		stepDelta = (t - z) / steps;
		currentStep = steps;
	}

	float getTarget()
	{
		return target;
	}

	float getValue()
	{
		return z;
	}

private:
	float target;
	float stepDelta;
	unsigned int steps;
	unsigned int currentStep;
	float z;
};

class ADSREnveloppe
{
public:
	enum class EnvelopeStage
	{
		IDLE,
		ATTACK,
		DECAY,
		SUSTAIN,
		RELEASE
	};

	ADSREnveloppe() : stage(EnvelopeStage::IDLE), env(0.0f), sampleRate(DEFAULT_SAMPLERATE)
	{
		setAttack(0.01f);
		setDecay(0.01f);
		setSustain(1.0f);
		setRelease(0.01f);
	}

	void prepare(float sr)
	{
		sampleRate = sr;
		/* En cas de changement de sampleRate on doit recalculer les coefficients */
		updateCoeff();
	}

	inline float process()
	{
		if (stage == EnvelopeStage::ATTACK)
		{
			/* On target une valeur de 1 */
			env = RCF(attackTarget, attackCoeff);
			if (env >= 1.0 - THRESHOLD_NULLIFY) {
				stage = EnvelopeStage::DECAY;
			}
		}
		else if (stage == EnvelopeStage::DECAY)
		{
			/* On target la valeur du sustain */
			env = RCF(decayTarget, decayCoeff);
			if (env < sustainLevel - THRESHOLD_NULLIFY)
				stage = EnvelopeStage::SUSTAIN;
		}
		else if (stage == EnvelopeStage::SUSTAIN)
		{
			env = RCF(sustainLevel, sustainCoeff);
		}
		else if (stage == EnvelopeStage::RELEASE)
		{
			/* On target 0 */
			env = RCF(releaseTarget, releaseCoeff);
			if (env < THRESHOLD_NULLIFY)
			{
				env = 0.0f;
				stage = EnvelopeStage::IDLE;
			}
		}

		return env;
	}

	void noteOn()
	{
		stage = EnvelopeStage::ATTACK;
	}

	void noteOff()
	{
		releaseTarget = env - 1 / (1 - exp(-decaySlope)) * env;
		stage = EnvelopeStage::RELEASE;
	}

	/* Le changement d'un paramètre demande le recalcul des coefficients */
	void setAttack(float attack) { attackTime = attack / attackSlope;   computeCoeff(attackTime, attackCoeff); }
	void setDecay(float decay) { decayTime = decay / decaySlope; computeCoeff(decayTime, decayCoeff); }
	void setRelease(float release) { releaseTime = release / decaySlope; computeCoeff(releaseTime, releaseCoeff); }
	void setSustain(float sustain)
	{
		sustainLevel = sustain;
		decayTarget = 1.0f - 1.0f / (1.0f - exp(-decaySlope)) * (1.0f - sustainLevel);
	}

private:
	void updateCoeff()
	{
		computeCoeff(attackTime, attackCoeff);
		computeCoeff(decayTime, decayCoeff);
		computeCoeff(0.01f, sustainCoeff);
		computeCoeff(releaseTime, releaseCoeff);
	}

	/* Les coefficients sont calculés à chaque modification d'un paramètre de l'enveloppe */
	void computeCoeff(float t, float* coeffs)
	{
		/*
			alpha = 1-e^(-1/tau)
			REFERENCES :
			https://tomroelandts.com/articles/low-pass-single-pole-iir-filter
			http://www.dspguide.com/graphics/E_19_4.gif
			https://dsp.stackexchange.com/questions/54086/single-pole-iir-low-pass-filter-which-is-the-correct-formula-for-the-decay-coe
		*/
		coeffs[0] = exp(-1.0f / (t * sampleRate));
		coeffs[1] = 1.0f - coeffs[0];
	}

	/* IIR Lowpass - RC filter */
	inline float RCF(float x, float* coeff)
	{
		return env * coeff[0] + x * coeff[1];
	}

	/* État de l'enveloppe */
	EnvelopeStage stage;
	/* Paramètres de l'enveloppe */
	std::atomic<float> attackTime;
	std::atomic<float> decayTime;
	std::atomic<float> releaseTime;
	std::atomic<float> sustainLevel;
	/* Coefficients du filtre */
	float attackCoeff [2];
	float decayCoeff  [2];
	float releaseCoeff[2];
	/* pour le smoothing en cas de modif en live */
	float sustainCoeff[2];
	/* Valeur de sortie de l'enveloppe */
	float env;

	float sampleRate;
	
	static constexpr float attackSlope = 1.0f;
	static constexpr float decaySlope = 3.0f;

	const float attackTarget = 1 / (1 - exp(-attackSlope));
	float decayTarget;
	float releaseTarget;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSREnveloppe)
};
