#pragma once
#include <JuceHeader.h>
#include "SonicMageAudioEngine.h"

//==============================================================================
/**
*/
class SonicMageAudioProcessor : private juce::AudioProcessorValueTreeState::Listener, public juce::AudioProcessor
{
public:
    //==============================================================================
    SonicMageAudioProcessor();
    ~SonicMageAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


    //==============================================================================
    void changeImage(std::shared_ptr<juce::Image> img);
    std::shared_ptr<juce::Image> getImage();

	juce::AudioProcessorValueTreeState parameters;
private:
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    std::unique_ptr<SonicMageAudioEngine> audioEngine;
    std::unique_ptr<SonicMageAudioEngine> newEngine;
    std::atomic<bool> osc_changed;

    int currentNote = -1;
    std::shared_ptr<juce::Image> img;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SonicMageAudioProcessor)
};
