#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SonicMageAudioProcessor::SonicMageAudioProcessor() : parameters(*this, nullptr, juce::Identifier("APVTS_SonicMage"),
    {
        std::make_unique<juce::AudioParameterFloat>("TABLE_POS",        // parameterID
                                                     "Scan position",   // parameter name
                                                     0.0f,              // minimum value
                                                     1.0f,              // maximum value
                                                     0.0f),             // default value
        std::make_unique<juce::AudioParameterFloat>("OUTPUT_GAIN",
                                                     "Output Gain",
                                                     -100.0f,
                                                     24.0f,
                                                     0.0f),
        std::make_unique<juce::AudioParameterFloat>("PARTIALS",
                                                     "Partials",
                                                     0.01f,
                                                     0.99f,
                                                     0.5f),
        std::make_unique<juce::AudioParameterFloat>("PLUCK",
                                                     "Pluck",
                                                     0.0f,
                                                     1.0f,
                                                     0.0f),
        std::make_unique<juce::AudioParameterBool>("TOGGLE_PLUCK",
                                                   "Toggle pluck",
                                                   true),
        std::make_unique<juce::AudioParameterFloat>("AMP_ATTACK",
                                                     "Attack",
                                                     0.0f,
                                                     1.0f,
                                                     0.001f),
        std::make_unique<juce::AudioParameterFloat>("AMP_DECAY",
                                                     "Decay",
                                                     0.0f,
                                                     1.0f,
                                                     0.1f),
        std::make_unique<juce::AudioParameterFloat>("AMP_SUSTAIN",
                                                     "Sustain",
                      juce::NormalisableRange<float>(0.0f,
                                                     1.0f,
                                                     0.0f,
                                                     0.7f),
                                                     0.0f),
        std::make_unique<juce::AudioParameterFloat>("AMP_RELEASE",
                                                     "Release",
                                                     0.0f,
                                                     1.0f,
                                                     0.1f),
        std::make_unique<juce::AudioParameterFloat>("HARMONICITY",
                                                     "Harmonicity",
                                                     0.0f,
                                                     1.0f,
                                                     1.0f)
    }), audioEngine(std::make_unique<SonicMageAudioEngine>(parameters)), osc_changed(false)
#ifndef JucePlugin_PreferredChannelConfigurations
     , AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	parameters.addParameterListener("TABLE_POS", this);
    parameters.addParameterListener("OUTPUT_GAIN", this);
    parameters.addParameterListener("PARTIALS", this);
    parameters.addParameterListener("PLUCK", this);
    parameters.addParameterListener("AMP_ATTACK", this);
    parameters.addParameterListener("AMP_DECAY", this);
    parameters.addParameterListener("AMP_SUSTAIN", this);
    parameters.addParameterListener("AMP_RELEASE", this);
    parameters.addParameterListener("HARMONICITY", this);
}

SonicMageAudioProcessor::~SonicMageAudioProcessor()
{
}

//==============================================================================
const juce::String SonicMageAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SonicMageAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SonicMageAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SonicMageAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SonicMageAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SonicMageAudioProcessor::getNumPrograms()
{
    return 1;
}

int SonicMageAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SonicMageAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SonicMageAudioProcessor::getProgramName (int index)
{
    return {};
}

void SonicMageAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SonicMageAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	this->audioEngine->prepareToPlay(static_cast<float>(sampleRate));
}

void SonicMageAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SonicMageAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.HARMONICS
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SonicMageAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (osc_changed)
    {
        this->audioEngine.swap(this->newEngine);
        this->osc_changed = false;
    }

    /* Traitement du midi par sample */
	juce::MidiMessage m;
	int midiMessageTime;
	auto midiIt = juce::MidiBuffer::Iterator(midiMessages);

	auto* leftBuffer = buffer.getWritePointer(0);
	auto* rightBuffer = buffer.getWritePointer(1);
	for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
	{
		midiIt.setNextSamplePosition(sample);
		while (midiIt.getNextEvent(m, midiMessageTime) && midiMessageTime == sample)
        {
			if (m.isNoteOn())
			{
				audioEngine->setBaseFreq(static_cast<float>(m.getMidiNoteInHertz(m.getNoteNumber())));
				currentNote = m.getNoteNumber();
				audioEngine->noteOn();
			}
			else if (m.isNoteOff() && m.getNoteNumber() == currentNote)
			{
				audioEngine->noteOff();
			}
		}
		auto outputSample = audioEngine->getNextSample();
		leftBuffer[sample] = outputSample;
		rightBuffer[sample] = outputSample;
	}
}

//==============================================================================
bool SonicMageAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SonicMageAudioProcessor::createEditor()
{
    return new SonicMageAudioProcessorEditor (*this);
}

//==============================================================================
void SonicMageAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
	auto state = parameters.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void SonicMageAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(parameters.state.getType()))
			parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

void SonicMageAudioProcessor::changeImage(std::shared_ptr<juce::Image> _img)
{
    std::unique_ptr<SonicMageAudioEngine> _osc = std::make_unique<SonicMageAudioEngine>(parameters);
    _osc->createPartialTable(*_img);
    _osc->prepareToPlay(this->getSampleRate());
    this->newEngine.swap(_osc);
    this->osc_changed = true;
    this->img.swap(_img);
}

std::shared_ptr<juce::Image> SonicMageAudioProcessor::getImage()
{
    return this->img;
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SonicMageAudioProcessor();
}

void SonicMageAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if      (parameterID == "OUTPUT_GAIN")
    {
        audioEngine->setOutputGain(juce::Decibels::decibelsToGain((float)newValue));
    }
    else if (parameterID == "TABLE_POS"   ||
             parameterID == "PARTIALS"    ||
             parameterID == "HARMONICITY" ||
             parameterID == "PLUCK")
    {
        audioEngine->clearUpdateFlag(SonicMageAudioEngine::UpdateFlagIds::PARAMS_OSC);
    }
    else if (parameterID == "AMP_ATTACK"  ||
             parameterID == "AMP_DECAY"   ||
             parameterID == "AMP_SUSTAIN" ||
             parameterID == "AMP_RELEASE")
    {
        audioEngine->clearUpdateFlag(SonicMageAudioEngine::UpdateFlagIds::PARAMS_ENV);
    }
}

