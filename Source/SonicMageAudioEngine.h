#pragma once
#include <JuceHeader.h>
#include <vector>
#include "utils.h"

/**
* Structure de données qui contient les informations de chaque partiel.
*/
class PartialTable
{
public:
	/**
	* Constructeur par défaut, si aucune image n'est fournie.
	*/
	PartialTable()
	{
		n_oscillators = 0;
		table_length = 0;
		pitch_table = std::vector<std::vector<float>>(table_length, std::vector<float>(n_oscillators));
		volume_table = std::vector<std::vector<float>>(table_length, std::vector<float>(n_oscillators));
		timbre_table = std::vector<std::vector<float>>(table_length, std::vector<float>(n_oscillators));
	}
	
	/**
	* Constructeur principal, la table est générée à partir d'une image. Les images peuvent avoir des hauteurs variables
	* et les ressources sont limitées, on doit spécifier un nombre de partiel fixe ainsi l'image sera réduite ou agrandie
	* pour ne pas se retrouver avec un nombre trop grand d'oscillateurs.
	* @param img Image à utiliser pour générer la table.
	* @param nosc Nombre de partiels pour chaque colonne.
	*/
	PartialTable(juce::Image img, int nosc)
	{
		table_length = img.getWidth();
		n_oscillators = nosc;
		img = img.rescaled(table_length, n_oscillators);
		pitch_table = std::vector<std::vector<float>>(table_length, std::vector<float>(n_oscillators));
		volume_table = std::vector<std::vector<float>>(table_length, std::vector<float>(n_oscillators));
		timbre_table = std::vector<std::vector<float>>(table_length, std::vector<float>(n_oscillators));
		float hue, bright, sat;
		for (int i = 0; i < table_length; i++) {
			for (int j = 0; j < n_oscillators; j++) {
				juce::Colour p = img.getPixelAt(i, j);
				hue = p.getHue();
				if (hue > 0.5)
					hue = (1 - hue);
				bright = p.getBrightness();
				sat = p.getSaturation();
				pitch_table[i][j] = hue * 2;
				volume_table[i][j] = bright;
				timbre_table[i][j] = sat;
			}
			std::sort(pitch_table[i].begin(), pitch_table[i].end(), [&](int a, int b) {return pitch_table[i][a] < pitch_table[i][b]; });
			// sort le volume et le timbre dans le même ordre
			std::sort(volume_table[i].begin(), volume_table[i].end(), [&](int a, int b) {return pitch_table[i][a] < pitch_table[i][b]; });
			std::sort(timbre_table[i].begin(), timbre_table[i].end(), [&](int a, int b) {return pitch_table[i][a] < pitch_table[i][b]; });
		}
	}

	~PartialTable() {}

	float getPitch(double table_pos, int oscillator)
	{
		auto index = (unsigned int)(table_pos+0.5);
		return pitch_table[index][oscillator];
	}

	float getVolume(double table_pos, int oscillator)
	{
		auto index = (unsigned int)(table_pos+0.5);
		return volume_table[index][oscillator];
	}

	float getTimbre(double table_pos, int oscillator)
	{
		auto index = (unsigned int)(table_pos+0.5);
		return timbre_table[index][oscillator];
	}

	int getTableLength()
	{
		return table_length;
	}
private:
	int table_length;
	int n_oscillators;
	std::vector<std::vector<float>> pitch_table;
	std::vector<std::vector<float>> volume_table;
	std::vector<std::vector<float>> timbre_table;
};


/**
* Implémentation d'un oscillateur par table d'onde.
*/
class WavetableOscillator
{
public:
	/**
	* Constructeur principal.
	* @param wavetableToUse Table qui contient la forme d'onde de l'oscillateur.
	*/
	WavetableOscillator(const juce::AudioSampleBuffer& wavetableToUse) : wavetable(wavetableToUse)
	{
		jassert(wavetable.getNumChannels() == 1);
	}

	~WavetableOscillator() {}

	void setFrequency(float f) { frequency = f; }

	void setGainLinear(float g) { gainLinear.setTarget(g); }

	void prepareToPlay(float sampleRate)
	{
		this->tableDuration = wavetable.getNumSamples() / sampleRate;
		this->gainLinear.prepare(10, sampleRate, 0.0f);
		this->pluckMax.prepare(10, sampleRate, 0.0f);
		this->pluckEnv.prepare(sampleRate);
		this->pluckEnv.setSustain(0.0f);
		this->pluckEnv.setRelease(0.0f);
	}

	void noteOn()
	{
		pluckEnv.noteOn();
	}
	
	/**
	* Récupère le prochain échantillon de l'oscillateur.
	* @param pluck Spécifie si l'oscillateur devrait être en mode pluck ou non.
	*/
	inline float getNextSample(bool pluck) noexcept
	{
		this->tableDelta = frequency * tableDuration;
		// on récupère notre position dans la wavetable
		auto tableSize = (unsigned int)wavetable.getNumSamples();
		auto index0 = (unsigned int)currentIndex;
		auto index1 = index0 == (tableSize - 1) ? (unsigned int)0 : index0 + 1;
		// on calcule le facteur d'interpolation ([0,1]) entre index0 et index1
		auto frac = currentIndex - (float)index0;
		// on récupère les deux samples dans la wavetable
		auto* table = wavetable.getReadPointer(0);
		auto value0 = table[index0];
		auto value1 = table[index1];
		// on interpole entre les deux samples
		auto currentSample = value0 + frac * (value1 - value0);
		// on avance la position dans la wavetable
		currentIndex += tableDelta;
		while (currentIndex > (float)tableSize)
			currentIndex -= (float)tableSize;
		auto g = gainLinear.process();
		if (pluck)
			return currentSample * (g + (pluckMax.process() - g) * pluckEnv.process());
		else
			return currentSample * g;
	}

	void setAttackPluck(float attack) { pluckEnv.setAttack(attack); }
	void setDecayPluck (float decay)  { pluckEnv.setDecay(decay); }
	void setPluckMax(float pluck)     { pluckMax.setTarget(pluck); }

private:
    const juce::AudioSampleBuffer& wavetable;
    float tableDuration = 0.0f, currentIndex = 0.0f, tableDelta = 0.0f;
	ParamSmoothExp gainLinear, pluckMax;
	float frequency;
	ADSREnveloppe pluckEnv;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableOscillator)
};


/**
* Moteur audio, génère le signal en fonction des paramètres et de la note jouée.
* C'est un moteur de synthèse additive qui fonctionne avec une banque d'oscillateur.
*/
class SonicMageAudioEngine
{
public:
	/**
	* Constructeur principal. La classe récupère les paramètres du plugin directement par le ValueTree.
	* @param params Paramètres du plugin.
	*/
	SonicMageAudioEngine(juce::AudioProcessorValueTreeState&);

	~SonicMageAudioEngine();

	/**
	* Permet d'instancier la table des partiels.
	*/
	void createPartialTable(juce::Image, int=0);

	/**
	* Flags qui signalent un changement de paramètre.
	*/
	enum class UpdateFlagIds {
		PARAMS_OSC = 0,
		PARAMS_ENV,
		FLAG_COUNT
	};
	/**
	* Permet de réinitialiser un flag.
	*/
	void clearUpdateFlag(UpdateFlagIds flagId) { updateFlags[static_cast<int>(flagId)].clear(); }
	/**
	* Opération atomique de test and set d'un flag.
	*/
	bool testUpdateFlag (UpdateFlagIds flagId) { return updateFlags[static_cast<int>(flagId)].test_and_set(); }
	/**
	* Retourne le nombre de flags.
	*/
	unsigned int getUpdateFlagCount() { return static_cast<unsigned int>(UpdateFlagIds::FLAG_COUNT); };

	void prepareToPlay(float _sampleRate);
	inline float getNextSample() noexcept;
	void setBaseFreq(float newbf) { baseFreq = newbf; clearUpdateFlag(SonicMageAudioEngine::UpdateFlagIds::PARAMS_OSC); }
	void setOutputGain(float g) { outputGain.setTarget(g); }
	void noteOn()  { outputEnv.noteOn(); for (int i = 0; i < n_oscillators; i++) oscillators[i]->noteOn(); }
	void noteOff() { outputEnv.noteOff(); }

private:
	/**
	* Crée la table d'onde sinusoïdale à utiliser pour chaque oscillateur.
	*/
	void createWavetable();

	inline void updateOsc(int _id) noexcept;

	juce::AudioProcessorValueTreeState& parameters;

	int n_oscillators = 0;
	int table_width = 0;
    float sampleRate = 0.0f;

	float baseFreq = 440.0f;
	std::atomic_flag updateFlags[static_cast<int>(UpdateFlagIds::FLAG_COUNT)];
	ParamSmoothExp   outputGain;
	ADSREnveloppe outputEnv;

    const unsigned int tableSize = 2048;
	PartialTable partialTable;
	juce::AudioSampleBuffer sineTable;
	juce::OwnedArray<WavetableOscillator> oscillators;
	std::unique_ptr<WavetableOscillator> fundamentalOscillator;

	static constexpr unsigned int MAX_OSC = 24;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SonicMageAudioEngine)
};

//==============================| INLINE FUNCTIONS |================================

/**
* Retourne le prochain échantillon du signal.
*/
inline float SonicMageAudioEngine::getNextSample() noexcept
{
	// Si un des paramètres de l'enveloppe a été modifié
	if (!testUpdateFlag(UpdateFlagIds::PARAMS_ENV))
	{
		outputEnv.setAttack (*parameters.getRawParameterValue("AMP_ATTACK"));
		outputEnv.setDecay  (*parameters.getRawParameterValue("AMP_DECAY"));
		outputEnv.setSustain(*parameters.getRawParameterValue("AMP_SUSTAIN"));
		outputEnv.setRelease(*parameters.getRawParameterValue("AMP_RELEASE"));
	}

	float sample = 0.0;
	bool pluck = *parameters.getRawParameterValue("TOGGLE_PLUCK");

	// Code redondant mais nécessaire pour améliorer la vitesse d'exécution dans le thread audio.
	if (!testUpdateFlag(UpdateFlagIds::PARAMS_OSC))
	{
		/* La fonction updateOsc demande beaucoup de ressource on l'appelle donc seulement
		   quand on en a besoin pour optimiser les ressources. */
		for (int i = 0; i < n_oscillators; i++)
		{
			updateOsc(i);
			sample += oscillators[i]->getNextSample(pluck);
		}
		fundamentalOscillator->setFrequency(baseFreq);
	}
	else
	{
		for (int i = 0; i < n_oscillators; i++)
		{
			sample += oscillators[i]->getNextSample(pluck);
		}
	}

	sample += fundamentalOscillator->getNextSample(pluck);
	return sample * outputGain.process()* outputEnv.process();
}

/**
* Met à jour un oscillateur.
* @param _id Id de l'oscillateur à mettre à jour.
*/
inline void SonicMageAudioEngine::updateOsc(int osc_id) noexcept
{
	/* On récupère les paramètres */
	float pos = *parameters.getRawParameterValue("TABLE_POS") * (table_width - 1.0f);
	float inharmonicity = 1.0f - *parameters.getRawParameterValue("HARMONICITY");
	float partials = *parameters.getRawParameterValue("PARTIALS");
	float pluck = *parameters.getRawParameterValue("PLUCK");
	float attack = *parameters.getRawParameterValue("AMP_ATTACK");
	float decay = *parameters.getRawParameterValue("AMP_DECAY");

	float freq = partialTable.getPitch(pos, osc_id);
							   // 20.0f = 1.0f / 0.05f
	//freq = freq / (freq + (1.0f - 20.0f) * (freq - 1.0f));
	freq = baseFreq + (20000 - baseFreq) * freq;
	float part = freq / baseFreq;
	int harm = (int)(part + 0.5f);
	//if(osc_id%2==0)
	//	harm = osc_id + 1;
	freq = baseFreq * (harm + (harm - part) * inharmonicity);
	float gain = 1.0f / harm;
	/* nyquist check */
	if (freq > sampleRate * 0.5 || freq < 20 || harm < 2)
	{
		freq = 440.0f;
		gain = 0.0f;
	}
	else
	{
		auto pluckosc = gain;
		gain = gain / (gain + (1.0f - (1.0f / partials)) * (gain - 1.0f));
		gain *= partialTable.getVolume(pos, osc_id);
		oscillators[osc_id]->setAttackPluck(attack);
		oscillators[osc_id]->setDecayPluck(decay);
		pluckosc = pluckosc / (pluckosc + (1.0f - (1.0f / (pluck + 0.001f))) * (pluckosc - 1.0f));
		oscillators[osc_id]->setPluckMax(pluckosc);
	}
	oscillators[osc_id]->setFrequency(freq);
	oscillators[osc_id]->setGainLinear(gain);
}
