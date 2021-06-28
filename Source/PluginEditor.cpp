#include "PluginEditor.h"

//==============================================================================
SonicMageAudioProcessorEditor::SonicMageAudioProcessorEditor(SonicMageAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), gui() 
{
	addAndMakeVisible(gui);
    setSize(gui.getBounds().getWidth(), gui.getBounds().getHeight());

    juce::Slider& sliderPos = static_cast<KnobAndLabel*>(gui.findChildWithID("SCAN_KNOB"))->getSlider();
    juce::Slider& sliderPartials = static_cast<KnobAndLabel*>(gui.findChildWithID("PARTIALS_KNOB"))->getSlider();
    juce::Slider& sliderHarmonicity = static_cast<KnobAndLabel*>(gui.findChildWithID("HARMONICITY_KNOB"))->getSlider();
    juce::Slider& sliderGain = static_cast<GainSlider*>(gui.findChildWithID("GAIN_SLIDER"))->getSlider();
    juce::Slider& sliderAttack = static_cast<KnobAndLabel*>(gui.findChildWithID("ATTACK_KNOB"))->getSlider();
    juce::Slider& sliderDecay = static_cast<KnobAndLabel*>(gui.findChildWithID("DECAY_KNOB"))->getSlider();
    juce::Slider& sliderSustain = static_cast<KnobAndLabel*>(gui.findChildWithID("SUSTAIN_KNOB"))->getSlider();
    juce::Slider& sliderRelease = static_cast<KnobAndLabel*>(gui.findChildWithID("RELEASE_KNOB"))->getSlider();
    juce::Slider& sliderPluck = static_cast<DeactivableKnobAndLabel*>(gui.findChildWithID("PLUCK_KNOB"))->getSlider();
    juce::ToggleButton& togglePluck = static_cast<DeactivableKnobAndLabel*>(gui.findChildWithID("PLUCK_KNOB"))->getButton();
    juce::Button& openButton = *static_cast<juce::Button*>(gui.findChildWithID("OPEN_BUTTON"));
    
    sliderPosAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "TABLE_POS", sliderPos);
    sliderHarmonicsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "PARTIALS", sliderPartials);
    lockHarmAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "HARMONICITY", sliderHarmonicity);
    sliderGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "OUTPUT_GAIN", sliderGain);
    envAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "AMP_ATTACK", sliderAttack));
    envAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "AMP_DECAY", sliderDecay));
    envAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "AMP_SUSTAIN", sliderSustain));
    envAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "AMP_RELEASE", sliderRelease));
    sliderPluckAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "PLUCK", sliderPluck);
    togglePluckAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.parameters, "TOGGLE_PLUCK", togglePluck);

	openButton.onClick = [this] { openButtonClicked(); };

    if (audioProcessor.getImage() != nullptr)
        gui.setImage(audioProcessor.getImage());
}

SonicMageAudioProcessorEditor::~SonicMageAudioProcessorEditor()
{
}

//==============================================================================
void SonicMageAudioProcessorEditor::openButtonClicked()
{
    juce::FileChooser chooser ("Ouvrir une image PNG ou JPEG", juce::File::getSpecialLocation(juce::File::userDesktopDirectory), "*.png;*.jpg;*.jpeg)");
    if (chooser.browseForFileToOpen()) {
        juce::File file;
        file = chooser.getResult();
        loadImage(chooser.getResult());
    }
}

void SonicMageAudioProcessorEditor::loadImage(juce::File file) 
{ 
    juce::Image img = juce::ImageFileFormat::loadFrom(file);

    int w = img.getWidth();
    int h = img.getHeight();
    if (w < h)
        img = img.getClippedImage(juce::Rectangle<int>(0, (h-w)/2, w, w));
    else if(w > h)
        img = img.getClippedImage(juce::Rectangle<int>((w-h)/2, 0, h, h));

    std::shared_ptr<juce::Image> _croppedImg = std::make_shared<juce::Image>(img);
    audioProcessor.changeImage(_croppedImg);
    gui.setImage(_croppedImg);
}
