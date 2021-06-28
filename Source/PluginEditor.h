#pragma once
#include <JuceHeader.h>
#include "SonicMageGui.h"
#include "PluginProcessor.h"


//==============================================================================
class SonicMageAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SonicMageAudioProcessorEditor (SonicMageAudioProcessor&);
    ~SonicMageAudioProcessorEditor() override;

private:
    SonicMageAudioProcessor& audioProcessor;

    SonicMageGuiComponent gui;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderPosAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderHarmonicsAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lockHarmAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderPluckAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> togglePluckAttachment;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> envAttachments;

    //==============================================================================
    void openButtonClicked();
    void loadImage(juce::File img);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SonicMageAudioProcessorEditor)
};
