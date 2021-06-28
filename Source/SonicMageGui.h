#pragma once
#include <JuceHeader.h>

//=================================================================================================================================
//------------------------------------------------------ LOOK AND FEEL ------------------------------------------------------------
//=================================================================================================================================

/**
* Le Look and Feel global de l'interface graphique.
*/
class SonicMageLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SonicMageLookAndFeel();
    
    /* KNOB */
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;

    /* SLIDER */
    int getSliderThumbRadius(juce::Slider& slider) override;
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override;
    juce::Label* createSliderTextBox(juce::Slider& slider) override;
	void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override;
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override;

    /**
    * Renvoie la police d'écriture principale utilisée par les labels. Elle est stockée en static.
    */
    static juce::Font getMainFont();

    /**
    * Couleurs utilisées dans l'interface graphique.
    */
    enum sonicMageGuiColors {
        mainColor,
        disabledColor,
        backgroundColor,
        deepBackgroundColor,
        neutralBackgroundColor,
        brandBackgroundColor,
        borderColor,
        innerDial,
        outerDialUp,
        outerDialDown
    };

private:
    static const juce::Font mainFont;
    /**
    * textBox custom pour l'édition des de la valeur du slider.
    */
    class customTextBox : public juce::Label
    {
		void paint(juce::Graphics& g) override;
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SonicMageLookAndFeel)
};


//=================================================================================================================================
//---------------------------------------------------- CUSTOM COMPONENTS ----------------------------------------------------------
//=================================================================================================================================

/**
*   Ensemble d'un slider rotatif (knob) et d'un label au dessus
*/
class KnobAndLabel : public juce::Component
{
public:
    /**
    * @param labelText  le texte du label
    * @param font       la police d'écriture du label
    */
    KnobAndLabel(const juce::String& labelText, juce::Font font = juce::Font())
    {
        addAndMakeVisible(slider);
        addAndMakeVisible(label);
        slider.setSliderStyle(juce::Slider::Rotary);
		slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        label.setText(labelText, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setFont(font);
    }
    
    ~KnobAndLabel() {}

    void resized() override
    {
        label.setBoundsRelative(0.0, 0.0, 1.0, labelHeight);
        slider.setBoundsRelative(0.0, labelHeight, 1.0, sliderHeight);
    }


    /**
    * permet de changer la police du label
    * @param font  la police à utiliser pour le label
    */
    void setFont(juce::Font font)
    {
        label.setFont(font);
        resized();
    }

    juce::Slider& getSlider() { return slider; }
    juce::Label& getLabel() { return label; }

private:
    juce::Slider slider;
    juce::Label label;
	static constexpr float sliderHeight = 0.68f;
	static constexpr float labelHeight = 1.f - sliderHeight;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KnobAndLabel)
};

class DeactivableKnobAndLabel : public juce::Component
{
public:
    DeactivableKnobAndLabel(const juce::String& labelText, juce::Font font = juce::Font()) : button(labelText)
    {
        addAndMakeVisible(button);
		addAndMakeVisible(slider);
        slider.setSliderStyle(juce::Slider::Rotary);
		slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        button.onStateChange = [this] { slider.setEnabled(button.getToggleState()); };
        setFont(font);
		slider.setEnabled(false);
        button.setToggleState(false, juce::NotificationType::dontSendNotification);
    }

    void resized() override
    {
		auto bounds = getLocalBounds().withHeight(buttonHeight * getHeight()).reduced(5);
		auto widthText = button.getFont().getStringWidth(button.getName());
		auto widthSlider = static_cast<int>(getWidth() * sliderHeight);
		auto width = juce::jmax(widthText, widthSlider);
		auto centre = bounds.getCentre();
		bounds = bounds.withWidth(width).withCentre(centre);
        button.setBounds(bounds);
        slider.setBoundsRelative(0.0, buttonHeight, 1.0, sliderHeight);
    }

	void setFont(juce::Font font) { button.setFont(font); }

    juce::Slider& getSlider() { return slider; }
    juce::ToggleButton& getButton() { return button; }

private:
    class SwitchButton : public juce::ToggleButton
    {
    public:
        SwitchButton(const juce::String& name) : ToggleButton(name) {}

        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

        void setFont(juce::Font _font) { textFont = _font; }
        juce::Font getFont() { return textFont; }
    private:
		juce::Font textFont;
	};
	juce::Slider slider;
    SwitchButton button;
	static constexpr float sliderHeight = 0.68f;
	static constexpr float buttonHeight = 1.f - sliderHeight;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeactivableKnobAndLabel)
};

/**
*   Le Slider vertical de gain du master.
*   Il est composé d'un label au dessus et d'un slider custom avec une textbox en dessous.
*/
class GainSlider : public juce::Component
{
public:
    /**
    * @param labelText  texte du label au dessus du slider
    * @param font       police d'écriture du label
    */
    GainSlider(const juce::String& labelText, juce::Font font = juce::Font())
    {
		addAndMakeVisible(slider);
        addAndMakeVisible(label);
        slider.setSliderStyle(juce::Slider::LinearVertical);
        slider.setRange(-100.0f, 24.0f);
        label.setText(labelText, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setFont(font);
		labelHeight = 0.1f;
		sliderHeight = 1.0f - labelHeight;
    }

    ~GainSlider() {}

	void resized() override
    {
        labelHeight = (label.getFont().getHeight() + 10) / getHeight();
        label.setBoundsRelative(0.0, 0.0, 1.0, labelHeight);
        slider.setBoundsRelative(0.0, labelHeight, 1.0, sliderHeight);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, static_cast<int>(getWidth() * 0.55f) , 24);
    }

	/**
    * permet de changer la police du label
    * @param font  la police à utiliser pour le label
    */
    void setFont(juce::Font font)
    {
        label.setFont(font);
        resized();
    }

    juce::Slider& getSlider() { return slider; }
    juce::Label& getLabel() { return label; }
private:
    /**
        Slider custom en decibels.
    */
	class CustomSlider : public juce::Slider
	{
	public:
		CustomSlider() {}
		double getValueFromText(const juce::String& text) override
		{
			auto minusInfinitydB = -100.0;

			auto decibelText = text.upToFirstOccurrenceOf("dB", false, false).trim();

			return decibelText.equalsIgnoreCase("-INF") ? minusInfinitydB
				: decibelText.getDoubleValue();
		}
		juce::String getTextFromValue(double value) override
		{
			return juce::Decibels::toString(value);
		}
	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomSlider)
	};
    CustomSlider slider;
    juce::Label label;
    float labelHeight;
    float sliderHeight;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainSlider)
};

/**
*   Le bouton d'ouverture d'image.
*/
class SonicMageOpenButton : public juce::Button
{
public:
    SonicMageOpenButton(const juce::String&);
    ~SonicMageOpenButton() {};
    virtual void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
private:
    juce::Path openIcon;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SonicMageOpenButton)
};

/**
*   Le logo du plugin.
*/
class SonicMageTitleLogo : public juce::Component
{
public:
    SonicMageTitleLogo();
    ~SonicMageTitleLogo() {};
    void paint (juce::Graphics& g) override;
    void resized() override;
private:
    juce::Path sonic;
    juce::Path mage;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SonicMageTitleLogo)
};

/**
*   Le logo de marque.
*/
class SonicMageBrandLogo : public juce::Component
{
public:
    SonicMageBrandLogo();
    ~SonicMageBrandLogo() {};
    void paint (juce::Graphics& g) override;
    void resized() override;
private:
    juce::Path juval;
    juce::DrawablePath juvalDrawable;
    juce::Path audio;
    juce::DrawablePath audioDrawable;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SonicMageBrandLogo)
};

/**
*   Le curseur qui indique la postion scannée sur l'image.
*/
class ScanCursor : public juce::Component
{
public:
    ScanCursor() {};
    ~ScanCursor() {};
    void setPosition(float p) { position = p; repaint(); };
    void paint (juce::Graphics& g) override;
private:
    float position = 0.0f;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScanCursor)
};


//=================================================================================================================================
//------------------------------------------------------ COMPONENT LAYOUT ---------------------------------------------------------
//=================================================================================================================================

/**
* Component global qui contient tout le layout du plugin.
*/
class SonicMageGuiComponent  : public juce::Component
{
public:
    SonicMageGuiComponent ();
    ~SonicMageGuiComponent() override;
    void paint (juce::Graphics& g) override;
    void resized() override;
    void setImage(std::shared_ptr<juce::Image> img);
private:
    SonicMageLookAndFeel smLookAndFeel;

    /* Les différentes zones de l'interface */
    juce::Rectangle<int> header;
    juce::Rectangle<int> sideLeft;
    juce::Rectangle<int> sideRight;
    juce::Rectangle<int> midImage;
    juce::Rectangle<int> master;
    juce::Rectangle<int> brand;

    /* Les logos */
    SonicMageTitleLogo titleLogo;
    SonicMageBrandLogo brandLogo;
    
    /* Le bouton d'ouverture */
    SonicMageOpenButton openButton;

    /* Les rotatifs */
    KnobAndLabel scanPositionKnob;
    KnobAndLabel partialsKnob;
    DeactivableKnobAndLabel pluckKnob;
    KnobAndLabel harmonicityKnob;
    KnobAndLabel attackKnob;
    KnobAndLabel decayKnob;
    KnobAndLabel sustainKnob;
    KnobAndLabel releaseKnob;

    /* Le slider de gain */
    GainSlider gainSlider;

    /* L'image à afficher */
    std::unique_ptr<juce::Image> img;
    //juce::Image img;
    //std::shared_ptr<juce::Image> img;
    ScanCursor cursor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SonicMageGuiComponent)
};

