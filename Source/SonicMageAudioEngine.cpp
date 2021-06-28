#include "SonicMageAudioEngine.h"

SonicMageAudioEngine::SonicMageAudioEngine(juce::AudioProcessorValueTreeState& _parameters) : parameters(_parameters)
{
	createWavetable();
	fundamentalOscillator = std::make_unique<WavetableOscillator>(sineTable);
	for (int i = 0; i < getUpdateFlagCount(); i++)
	{
		updateFlags[i].clear();
	}
}

SonicMageAudioEngine::~SonicMageAudioEngine()
{
}

void SonicMageAudioEngine::createPartialTable(juce::Image _img, int _npartials)
{
	if (_npartials == 0)
		_npartials = _img.getHeight();
	if (_npartials > MAX_OSC)
		_npartials = MAX_OSC;
	partialTable = PartialTable(_img, _npartials);
	n_oscillators = _npartials;
	table_width = partialTable.getTableLength();
}

void SonicMageAudioEngine::createWavetable()
{
	sineTable.setSize(1, (int)tableSize);
	auto* samples = sineTable.getWritePointer(0);

	auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1);
	auto currentAngle = 0.0;

	for (unsigned int i = 0; i < tableSize; ++i) {
		auto sample = std::sin(currentAngle);
		samples[i] = (float)sample;
		currentAngle += angleDelta;
	}
}

void SonicMageAudioEngine::prepareToPlay(float _sampleRate)
{
	this->sampleRate = _sampleRate;
	outputGain.prepare(50, _sampleRate);
	outputGain.setTarget(*parameters.getRawParameterValue("OUTPUT_GAIN"));
	outputEnv.prepare(_sampleRate);
	outputEnv.setAttack(*parameters.getRawParameterValue("AMP_ATTACK"));
	outputEnv.setDecay(*parameters.getRawParameterValue("AMP_DECAY"));
	outputEnv.setSustain(*parameters.getRawParameterValue("AMP_SUSTAIN"));
	outputEnv.setRelease(*parameters.getRawParameterValue("AMP_RELEASE"));

	for (auto i = 0; i < n_oscillators; i++)
	{
		auto* oscillator = new WavetableOscillator(sineTable);
		oscillator->prepareToPlay(_sampleRate);
		oscillators.add(oscillator);
	}

	fundamentalOscillator->prepareToPlay(_sampleRate);
	fundamentalOscillator->setGainLinear(0.95f);
}

