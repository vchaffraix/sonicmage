#pragma once

#include "SonicMageGui.h"
#include "PluginEditor.h"

//=================================================================================================================================
//---------------------------------------------------- CUSTOM COMPONENTS ----------------------------------------------------------
//=================================================================================================================================

//------------------------------------------- OPEN BUTTON -------------------------------------------------
SonicMageOpenButton::SonicMageOpenButton(const juce::String& name) : juce::Button(name), openIcon()
{
	const unsigned char pathData[] = { 110,109,114,58,76,64,130,74,29,67,108,247,172,244,65,198,56,150,66,108,101,111,69,67,198,56,150,66,108,43,141,39,67,140,69,34,67,108,8,115,153,63,140,69,34,67,99,109,184,155,39,67,199,212,72,66,98,187,153,41,67,199,212,72,66,176,10,42,67,213,204,80,66,
	175,10,42,67,236,192,92,66,108,175,10,42,67,38,163,122,66,108,145,76,4,66,38,163,122,66,98,201,196,220,65,38,163,122,66,111,244,172,65,45,161,124,66,72,8,153,65,180,65,141,66,108,81,144,79,62,205,229,1,67,108,2,101,51,63,83,16,13,66,98,2,101,51,63,60,
	28,1,66,86,242,65,63,10,112,202,65,13,168,3,65,10,112,202,65,108,79,65,74,66,11,112,202,65,98,158,25,114,66,11,112,202,65,151,27,112,66,4,116,198,65,234,253,135,66,200,212,72,66,108,254,66,154,66,53,145,72,66,108,157,66,154,66,8,190,29,65,98,58,74,154,
	66,62,31,140,64,254,27,154,66,0,246,58,62,233,214,173,66,128,210,141,61,108,32,253,0,67,128,210,141,61,98,59,191,7,67,192,76,135,62,107,103,10,67,16,80,138,64,22,94,10,67,254,172,46,65,108,22,94,10,67,137,99,233,65,108,209,47,18,67,175,213,170,65,98,
	41,21,20,67,96,97,155,65,233,164,21,67,137,118,155,65,222,32,24,67,244,231,175,65,98,68,26,27,67,156,21,195,65,11,234,25,67,125,222,212,65,41,205,24,67,48,206,222,65,108,83,230,5,67,112,2,59,66,108,249,254,229,66,48,206,222,65,98,157,63,227,66,88,175,
	207,65,103,163,226,66,99,15,192,65,222,132,231,66,194,141,174,65,98,96,238,236,66,59,229,155,65,181,248,240,66,238,121,162,65,142,191,244,66,194,141,174,65,108,31,253,0,67,204,39,229,65,108,31,253,0,67,139,70,21,65,108,53,126,175,66,139,70,21,65,98,146,
	13,174,66,222,244,20,65,55,212,172,66,180,176,23,65,250,199,172,66,102,248,42,65,108,250,199,172,66,162,77,72,66,99,101,0,0 };
	openIcon.loadPathFromData(pathData, sizeof(pathData));
}

void SonicMageOpenButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	auto bounds = getLocalBounds().reduced(5);
	juce::Path border;
	border.addRoundedRectangle(bounds, 0.0);
	if(shouldDrawButtonAsDown)
		g.setGradientFill(juce::ColourGradient(juce::Colour(42, 42, 42), static_cast<float>(bounds.getCentreX()), static_cast<float>(bounds.getY()),
			juce::Colour(35, 35, 35), static_cast<float>(bounds.getCentreX()), static_cast<float>(bounds.getBottom()), false));
	else if(shouldDrawButtonAsHighlighted)
		g.setGradientFill(juce::ColourGradient(juce::Colour(75, 75, 75), static_cast<float>(bounds.getCentreX()), static_cast<float>(bounds.getY()),
			juce::Colour(65, 65, 65), static_cast<float>(bounds.getCentreX()), static_cast<float>(bounds.getBottom()), false));
	else
		g.setGradientFill(juce::ColourGradient(juce::Colour(65, 65, 65), static_cast<float>(bounds.getCentreX()), static_cast<float>(bounds.getY()),
			juce::Colour(55, 55, 55), static_cast<float>(bounds.getCentreX()), static_cast<float>(bounds.getBottom()), false));
	g.fillPath(border);
	
	if (shouldDrawButtonAsHighlighted) {
		g.setColour(juce::Colour(100, 100, 100));
		juce::Path outerBorder;
		outerBorder.addRoundedRectangle(bounds.reduced(-1,-1), 0.0);
		g.strokePath(outerBorder, juce::PathStrokeType(0.2f, juce::PathStrokeType::mitered));
	}
	g.setColour(juce::Colour(10, 10, 10));
	g.strokePath(border, juce::PathStrokeType(0.3f, juce::PathStrokeType::mitered));

	auto boundsIcon = bounds.withTrimmedTop(10).withTrimmedBottom(10);
	openIcon.scaleToFit(static_cast<float>(boundsIcon.getX()), static_cast<float>(boundsIcon.getY()), static_cast<float>(boundsIcon.getWidth()), static_cast<float>(boundsIcon.getHeight()), true);
	if(shouldDrawButtonAsHighlighted)
		g.setColour(juce::Colour(255, 255, 255));
	else
		g.setColour(juce::Colour(218, 218, 218));
	g.fillPath(openIcon);
}

//---------------------------------------- SWITCH KNOB BUTTON ---------------------------------------------
void DeactivableKnobAndLabel::SwitchButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	auto bounds = getLocalBounds();
	if (shouldDrawButtonAsHighlighted)
	{
		g.setColour(getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::backgroundColor).brighter(0.03f));
		g.fillRoundedRectangle(bounds.toFloat(), 6.0f);
	}
	bounds = bounds.reduced(1);

	if (getToggleState())
	{
		g.setColour(getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::outerDialDown).darker(0.8f));
		g.fillRoundedRectangle(bounds.toFloat(), 6.0f);
		g.setColour(getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::mainColor).darker(0.5f));
		g.fillRoundedRectangle(bounds.reduced(1).toFloat(), 5.0f);
		if (shouldDrawButtonAsDown)
		{
			g.setColour(getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::mainColor).darker(0.5f));
		}
		else
		{
			g.setColour(getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::mainColor));
		}
		g.fillRoundedRectangle(bounds.reduced(2).toFloat(), 4.0f);
	}
	else
	{
		g.setColour(getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::outerDialDown).darker(0.8f));
		g.fillRoundedRectangle(bounds.toFloat(), 6.0f);
		if (shouldDrawButtonAsDown)
		{
			g.setColour(getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::innerDial).darker(1.2f));
			g.fillRoundedRectangle(bounds.reduced(1).toFloat(), 4.0f);
		}
		else
		{
			g.setColour(getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::innerDial).darker(0.8f));
			g.fillRoundedRectangle(bounds.reduced(1).toFloat(), 4.0f);
			g.setColour(getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::innerDial));
			g.fillRoundedRectangle(bounds.reduced(2).toFloat(), 4.0f);
		}
	}

	if (shouldDrawButtonAsHighlighted || getToggleState())
		g.setColour(juce::Colours::white);
	else
		g.setColour(getLookAndFeel().findColour(juce::Label::ColourIds::textColourId));
	g.setFont(textFont);
	g.drawText(getName(), bounds.toFloat(), juce::Justification::centred);
}

//---------------------------------------------- LOGOS ----------------------------------------------------
SonicMageTitleLogo::SonicMageTitleLogo() : sonic(), mage()
{
	const unsigned char sonicPathData[] = { 110,109,38,108,103,66,107,11,158,65,113,243,168,98,66,107,11,158,65,145,101,95,66,169,132,151,65,113,48,34,92,66,231,253,144,65,48,34,92,66,128,119,135,65,108,48,34,92,66,84,159,52,64,113,48,34,92,66,181,170,211,63,145,101,95,66,46,125,86,63,113,243,
	168,98,66,0,0,0,181,38,108,103,66,0,0,0,181,108,3,165,132,66,0,0,0,181,108,3,165,132,66,84,159,52,64,108,38,108,103,66,84,159,52,64,108,38,108,103,66,128,119,135,65,108,3,165,132,66,128,119,135,65,108,3,165,132,66,107,11,158,65,99,109,92,250,46,66,107,
	11,158,65,108,92,250,46,66,128,119,135,65,108,81,68,58,66,128,119,135,65,108,81,68,58,66,84,159,52,64,108,92,250,46,66,84,159,52,64,108,92,250,46,66,0,0,0,181,108,60,216,80,66,0,0,0,181,108,60,216,80,66,84,159,52,64,108,71,142,69,66,84,159,52,64,108,
	71,142,69,66,128,119,135,65,108,60,216,80,66,128,119,135,65,108,60,216,80,66,107,11,158,65,99,109,44,199,225,65,107,11,158,65,108,44,199,225,65,0,0,0,181,108,23,91,248,65,0,0,0,181,108,113,102,24,66,43,199,97,65,108,113,102,24,66,0,0,0,181,108,102,176,
	35,66,0,0,0,181,108,102,176,35,66,107,11,158,65,108,113,102,24,66,107,11,158,65,108,23,91,248,65,85,159,180,64,108,23,91,248,65,107,11,158,65,99,109,86,159,180,65,128,119,135,65,108,86,159,180,65,84,159,52,64,108,128,119,135,65,84,159,52,64,108,128,119,
	135,65,128,119,135,65,99,109,128,119,135,65,107,11,158,65,113,50,226,123,65,107,11,158,65,174,212,110,65,169,132,151,65,113,43,199,97,65,231,253,144,65,43,199,97,65,128,119,135,65,108,43,199,97,65,84,159,52,64,113,43,199,97,65,181,170,211,63,174,212,
	110,65,46,125,86,63,113,52,226,123,65,0,0,0,181,129,119,135,65,0,0,0,181,108,86,159,180,65,0,0,0,181,113,149,248,189,65,0,0,0,181,87,127,196,65,46,125,86,63,113,65,51,203,65,180,170,211,63,65,51,203,65,84,159,52,64,108,65,51,203,65,128,119,135,65,113,
	65,51,203,65,231,253,144,65,87,127,196,65,169,132,151,65,113,149,248,189,65,107,11,158,65,86,159,180,65,107,11,158,65,99,109,80,159,52,64,107,11,158,65,113,48,216,208,63,107,11,158,65,36,216,80,63,169,132,151,65,113,0,0,0,0,231,253,144,65,0,0,0,0,128,
	119,135,65,108,0,0,0,0,43,199,97,65,108,86,159,52,64,43,199,97,65,108,86,159,52,64,128,119,135,65,108,129,119,7,65,128,119,135,65,108,129,119,7,65,86,159,52,65,108,86,159,52,64,86,159,52,65,113,60,216,208,63,86,159,52,65,60,216,80,63,210,145,39,65,113,
	0,0,0,0,78,132,26,65,0,0,0,0,128,119,7,65,108,0,0,0,0,84,159,52,64,113,0,0,0,0,180,170,211,63,59,216,80,63,46,125,86,63,113,60,216,208,63,0,0,0,181,86,159,52,64,0,0,0,181,108,128,119,7,65,0,0,0,181,113,254,41,26,65,0,0,0,181,130,55,39,65,46,125,86,63,
	113,86,159,52,65,180,170,211,63,86,159,52,65,84,159,52,64,108,86,159,52,65,85,159,180,64,108,129,119,7,65,85,159,180,64,108,129,119,7,65,84,159,52,64,108,86,159,52,64,84,159,52,64,108,86,159,52,64,128,119,7,65,108,128,119,7,65,128,119,7,65,113,254,41,
	26,65,128,119,7,65,130,55,39,65,83,223,20,65,113,86,159,52,65,215,236,33,65,86,159,52,65,86,159,52,65,108,86,159,52,65,128,119,135,65,113,86,159,52,65,231,253,144,65,131,55,39,65,169,132,151,65,113,0,42,26,65,107,11,158,65,129,119,7,65,107,11,158,65,
	99,101,0,0 };
	const unsigned char magePathData[] = { 110,109,86,159,52,66,107,11,158,65,108,86,159,52,66,0,0,0,181,108,44,199,97,66,0,0,0,181,108,44,199,97,66,84,159,52,64,108,76,233,63,66,84,159,52,64,108,76,233,63,66,128,119,7,65,108,55,125,86,66,128,119,7,65,108,55,125,86,66,86,159,52,65,108,76,233,
	63,66,86,159,52,65,108,76,233,63,66,128,119,135,65,108,44,199,97,66,128,119,135,65,108,44,199,97,66,107,11,158,65,99,109,128,119,7,66,107,11,158,65,113,76,180,2,66,107,11,158,65,215,225,254,65,169,132,151,65,113,21,91,248,65,231,253,144,65,21,91,248,
	65,128,119,135,65,108,21,91,248,65,84,159,52,64,113,21,91,248,65,181,170,211,63,215,225,254,65,46,125,86,63,113,77,180,2,66,0,0,0,181,128,119,7,66,0,0,0,181,108,107,11,30,66,0,0,0,181,113,10,184,34,66,0,0,0,181,108,251,37,66,46,125,86,63,113,97,85,41,
	66,180,170,211,63,97,85,41,66,84,159,52,64,108,97,85,41,66,85,159,180,64,108,107,11,30,66,85,159,180,64,108,107,11,30,66,84,159,52,64,108,128,119,7,66,84,159,52,64,108,128,119,7,66,128,119,135,65,108,107,11,30,66,128,119,135,65,108,107,11,30,66,86,159,
	52,65,108,118,193,18,66,86,159,52,65,108,118,193,18,66,128,119,7,65,108,102,176,35,66,128,119,7,65,113,108,251,37,66,128,119,7,65,102,168,39,66,106,43,14,65,113,96,85,41,66,84,223,20,65,96,85,41,66,107,11,30,65,108,96,85,41,66,128,119,135,65,113,96,85,
	41,66,231,253,144,65,107,251,37,66,169,132,151,65,113,10,184,34,66,107,11,158,65,106,11,30,66,107,11,158,65,99,109,64,51,203,65,128,119,7,65,108,64,51,203,65,84,159,52,64,108,106,11,158,65,84,159,52,64,108,106,11,158,65,128,119,7,65,99,109,128,119,135,
	65,107,11,158,65,108,128,119,135,65,84,159,52,64,113,128,119,135,65,181,170,211,63,66,254,141,65,46,125,86,63,113,3,133,148,65,0,0,0,181,107,11,158,65,0,0,0,181,108,64,51,203,65,0,0,0,181,113,127,140,212,65,0,0,0,181,65,19,219,65,46,125,86,63,113,43,
	199,225,65,180,170,211,63,43,199,225,65,84,159,52,64,108,43,199,225,65,107,11,158,65,108,64,51,203,65,107,11,158,65,108,64,51,203,65,86,159,52,65,108,106,11,158,65,86,159,52,65,108,106,11,158,65,107,11,158,65,99,109,0,0,0,0,107,11,158,65,108,0,0,0,0,
	0,0,0,181,108,86,159,52,64,0,0,0,181,108,43,199,225,64,128,119,7,65,108,86,159,52,65,0,0,0,181,108,44,199,97,65,0,0,0,181,108,44,199,97,65,107,11,158,65,108,86,159,52,65,107,11,158,65,108,86,159,52,65,85,159,180,64,108,129,119,7,65,86,159,52,65,108,86,
	159,180,64,86,159,52,65,108,86,159,52,64,85,159,180,64,108,86,159,52,64,107,11,158,65,99,101,0,0 };
	sonic.loadPathFromData(sonicPathData, sizeof(sonicPathData));
	mage.loadPathFromData(magePathData, sizeof(magePathData));
}

void SonicMageTitleLogo::paint(juce::Graphics& g)
{
	g.setColour(getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::mainColor));
	g.fillPath(sonic);
	g.setColour(juce::Colours::white);
	g.fillPath(mage);
}

void SonicMageTitleLogo::resized()
{
	auto bounds = getBounds();
	juce::AffineTransform transform = sonic.getTransformToScaleToFit(0.0f, 0.0f,
		bounds.getWidth()*0.5f,static_cast<float>(bounds.getHeight()), true);
	sonic.applyTransform(transform);
	transform = transform.translated(bounds.getWidth()*0.57f, 0.0f);
	mage.applyTransform(transform);
}

SonicMageBrandLogo::SonicMageBrandLogo() : juval(), audio(), juvalDrawable()
{
	const unsigned char juvalPathData[] = { 110,109,2,89,18,67,162,130,103,66,108,2,89,18,67,0,0,0,182,108,172,157,26,67,0,0,0,182,108,172,157,26,67,248,111,70,66,108,172,107,51,67,248,111,70,66,108,172,107,51,67,162,130,103,66,99,109,172,74,4,67,145,90,251,65,108,172,74,4,67,55,133,145,65,108,
	3,32,238,66,55,133,145,65,108,3,32,238,66,145,90,251,65,99,109,87,229,224,66,162,130,103,66,108,87,229,224,66,55,133,145,65,113,87,229,224,66,24,56,119,65,77,184,228,66,108,160,88,65,113,67,139,232,66,21,53,57,65,3,32,238,66,21,53,57,65,108,172,74,4,
	67,21,53,57,65,113,210,7,7,67,21,53,57,65,76,241,8,67,108,160,88,65,113,2,232,10,67,24,56,119,65,2,232,10,67,55,133,145,65,108,2,232,10,67,162,130,103,66,108,172,74,4,67,162,130,103,66,108,172,74,4,67,159,34,24,66,108,2,32,238,66,159,34,24,66,108,2,32,
	238,66,162,130,103,66,99,109,1,166,171,66,162,130,103,66,108,0,85,148,66,160,62,10,66,108,0,85,148,66,52,63,94,64,108,86,224,163,66,52,63,94,64,108,86,224,163,66,160,62,10,66,108,172,107,179,66,246,107,72,66,108,2,247,194,66,160,62,10,66,108,2,247,194,
	66,52,63,94,64,108,88,130,210,66,52,63,94,64,108,88,130,210,66,160,62,10,66,108,86,49,187,66,162,130,103,66,99,109,253,131,60,66,162,130,103,66,113,125,90,49,66,162,130,103,66,146,180,41,66,183,220,95,66,113,167,14,34,66,204,54,88,66,167,14,34,66,76,
	13,77,66,108,167,14,34,66,22,53,57,65,108,253,131,60,66,22,53,57,65,108,253,131,60,66,76,13,77,66,108,170,110,113,66,76,13,77,66,108,170,110,113,66,22,53,57,65,108,0,242,133,66,22,53,57,65,108,0,242,133,66,76,13,77,66,113,0,242,133,66,204,54,88,66,149,
	4,130,66,183,220,95,66,113,63,99,124,66,162,130,103,66,170,110,113,66,162,130,103,66,99,109,168,74,4,65,162,130,103,66,113,84,246,152,64,162,130,103,66,88,246,24,64,61,243,93,66,113,0,0,0,0,216,99,84,66,0,0,0,0,248,111,70,66,108,0,0,0,0,250,211,20,66,
	108,166,74,4,65,250,211,20,66,108,166,74,4,65,248,111,70,66,108,249,111,198,65,248,111,70,66,108,249,111,198,65,0,0,0,182,108,166,74,4,66,0,0,0,182,108,166,74,4,66,248,111,70,66,113,166,74,4,66,216,99,84,66,56,242,244,65,60,243,93,66,113,110,211,225,
	65,161,130,103,66,250,111,198,65,161,130,103,66,99,101,0,0 };
	const unsigned char audioPathData[] = { 110,109,205,140,209,66,153,153,228,65,108,205,140,209,66,100,102,152,64,108,0,128,190,66,100,102,152,64,108,0,128,190,66,153,153,228,65,99,109,1,128,190,66,153,89,5,66,113,78,123,186,66,153,89,5,66,116,186,183,66,204,175,255,65,113,154,249,180,66,102,
	172,244,65,154,249,180,66,153,153,228,65,108,154,249,180,66,100,102,152,64,113,154,249,180,66,252,151,50,64,116,186,183,66,144,249,180,63,113,77,123,186,66,0,0,128,181,0,128,190,66,0,0,128,181,108,205,140,209,66,0,0,128,181,113,116,126,213,66,0,0,128,
	181,78,63,216,66,145,249,180,63,113,51,19,219,66,252,151,50,64,51,19,219,66,100,102,152,64,108,51,19,219,66,153,153,228,65,113,51,19,219,66,102,172,244,65,78,63,216,66,204,175,255,65,113,116,126,213,66,153,89,5,66,205,140,209,66,153,89,5,66,99,109,0,
	224,142,66,153,89,5,66,108,0,224,142,66,152,153,228,65,108,102,102,152,66,152,153,228,65,108,102,102,152,66,100,102,152,64,108,0,224,142,66,100,102,152,64,108,0,224,142,66,0,0,128,181,108,51,115,171,66,0,0,128,181,108,51,115,171,66,100,102,152,64,108,
	205,236,161,66,100,102,152,64,108,205,236,161,66,153,153,228,65,108,51,115,171,66,153,153,228,65,108,51,115,171,66,153,89,5,66,99,109,102,166,119,66,152,153,228,65,108,102,166,119,66,100,102,152,64,108,205,140,81,66,100,102,152,64,108,205,140,81,66,153,
	153,228,65,99,109,0,128,62,66,153,89,5,66,108,0,128,62,66,0,0,128,181,108,102,166,119,66,0,0,128,181,113,179,137,127,66,0,0,128,181,179,133,130,66,145,249,180,63,113,153,89,133,66,252,151,50,64,153,89,133,66,100,102,152,64,108,153,89,133,66,153,153,228,
	65,113,153,89,133,66,102,172,244,65,179,133,130,66,204,175,255,65,113,179,137,127,66,153,89,5,66,102,166,119,66,153,89,5,66,99,109,154,153,228,65,153,89,5,66,113,205,134,212,65,153,89,5,66,103,131,201,65,204,175,255,65,113,1,128,190,65,102,172,244,65,
	1,128,190,65,153,153,228,65,108,1,128,190,65,0,0,128,181,108,154,153,228,65,0,0,128,181,108,154,153,228,65,153,153,228,65,108,102,102,24,66,153,153,228,65,108,102,102,24,66,0,0,128,181,108,51,115,43,66,0,0,128,181,108,51,115,43,66,153,153,228,65,113,
	51,115,43,66,102,172,244,65,102,203,37,66,204,175,255,65,113,179,73,32,66,153,89,5,66,102,102,24,66,153,89,5,66,99,109,153,153,100,65,152,153,100,65,108,153,153,100,65,100,102,152,64,108,102,102,152,64,100,102,152,64,108,102,102,152,64,152,153,100,65,
	99,109,0,0,0,0,153,89,5,66,108,0,0,0,0,100,102,152,64,113,0,0,0,0,252,151,50,64,102,54,176,63,145,249,180,63,113,102,54,48,64,0,0,128,181,102,102,152,64,0,0,128,181,108,153,153,100,65,0,0,128,181,113,102,19,130,65,0,0,128,181,205,22,141,65,145,249,180,
	63,113,102,102,152,65,252,151,50,64,102,102,152,65,100,102,152,64,108,102,102,152,65,153,89,5,66,108,153,153,100,65,153,89,5,66,108,153,153,100,65,101,102,152,65,108,102,102,152,64,101,102,152,65,108,102,102,152,64,153,89,5,66,99,101,0,0 };
	juval.loadPathFromData(juvalPathData, sizeof(juvalPathData));
	audio.loadPathFromData(audioPathData, sizeof(audioPathData));
	audio.applyTransform(juce::AffineTransform(1.0f, 0, 0, 0, 0.7f, 0));
	juvalDrawable.setPath(juval);
	audioDrawable.setPath(audio);
	addAndMakeVisible(&juvalDrawable);
	addAndMakeVisible(&audioDrawable);
}

void SonicMageBrandLogo::paint(juce::Graphics&)
{
	juvalDrawable.setFill(juce::Colours::white);
	audioDrawable.setFill(getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::mainColor));
}

void SonicMageBrandLogo::resized()
{
	juvalDrawable.setTransformToFit(juce::Rectangle<float>(0, 0, static_cast<float>(getWidth()), getHeight()*0.6f), juce::RectanglePlacement(12));
	auto juvalBounds = juvalDrawable.getBounds();
	audioDrawable.setTransformToFit(juce::Rectangle<float>(getWidth()*0.25f, getHeight()*0.6f+4.0f, getWidth()*0.5f, getHeight()*0.4f), juce::RectanglePlacement(12));
}

//=================================================================================================================================
//------------------------------------------------------ LOOK AND FEEL ------------------------------------------------------------
//=================================================================================================================================

/* la police est stockée en binary data */
const juce::Font SonicMageLookAndFeel::mainFont(juce::Typeface::createSystemTypefaceFor(
	BinaryData::firasans_medium_ttf, BinaryData::firasans_medium_ttfSize));

SonicMageLookAndFeel::SonicMageLookAndFeel()
{
	/* Définition des couleurs */
	//setColour(sonicMageGuiColors::mainColor, juce::Colour(255, 160, 59));
	//setColour(sonicMageGuiColors::mainColor, juce::Colour(103, 124, 225));
	setColour(sonicMageGuiColors::mainColor, juce::Colour(250, 86, 114));
	setColour(sonicMageGuiColors::disabledColor, juce::Colour(120, 120, 120));
	setColour(sonicMageGuiColors::backgroundColor, juce::Colour(32, 32, 33));
	setColour(sonicMageGuiColors::deepBackgroundColor, juce::Colour(22, 23, 25));
	setColour(sonicMageGuiColors::neutralBackgroundColor, juce::Colour(42, 42, 42));
	setColour(sonicMageGuiColors::brandBackgroundColor, juce::Colour(54, 54, 54));
	setColour(sonicMageGuiColors::borderColor, juce::Colour(15, 15, 15));
	setColour(sonicMageGuiColors::innerDial, juce::Colour(0.0f, 0.0f, 0.23f, 1.0f));
	setColour(sonicMageGuiColors::outerDialUp, juce::Colour(0.48f, 0.02f, 0.43f, 1.0f));
	setColour(sonicMageGuiColors::outerDialDown, juce::Colour(0.40f, 0.04f, 0.15f, 1.0f));

	setColour(juce::Label::ColourIds::textColourId, juce::Colour(205, 205, 205));
	setColour(juce::TextEditor::highlightColourId, juce::Colour(103, 124, 225));
}

void SonicMageLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
	/* LOOK AND FEEL COLORS */
	const juce::Colour mainColor = findColour(sonicMageGuiColors::mainColor);
	const juce::Colour disabledColor = findColour(sonicMageGuiColors::disabledColor);
	const juce::Colour innerDial = findColour(sonicMageGuiColors::innerDial);
	const juce::Colour outerDialDown = findColour(sonicMageGuiColors::outerDialDown);
	const juce::Colour outerDialUp = findColour(sonicMageGuiColors::outerDialUp);

	/* BOUNDS */
	auto boundsArc = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(1);
	auto boundsDial = boundsArc.reduced(juce::jmin(boundsArc.getWidth(), boundsArc.getHeight()) * 0.16f);
	float centreX, centreY, rx, ry, rw, angle, radius;

	/* SHADOW */
	radius = (float)juce::jmin(boundsArc.getWidth() / 2, boundsArc.getHeight() / 2);
	centreX = boundsArc.getCentreX();
	centreY = boundsArc.getCentreY();
	rx = centreX - radius;
	ry = centreY;
	rw = radius * 2.0f;
	g.setGradientFill(juce::ColourGradient(juce::Colours::black, centreX, centreY + radius * 0.25f,
		juce::Colour(0.0f, 0.0f, 0.0f, 0.0f), centreX, boundsArc.getBottom(), true));
	g.fillRect(0, 0, width, height);
	g.setGradientFill(juce::ColourGradient(juce::Colours::black, centreX, centreY + radius * 0.20f,
		juce::Colour(0.0f, 0.0f, 0.0f, 0.0f), centreX, boundsArc.getBottom(), true));
	g.fillRect(0, 0, width, height);
	g.setGradientFill(juce::ColourGradient(juce::Colours::black, centreX, centreY + radius * 0.15f,
		juce::Colour(0.0f, 0.0f, 0.0f, 0.0f), centreX, boundsArc.getBottom() * 0.95f, true));
	g.fillRect(0, 0, width, height);

	/* ARC */
	juce::Path backgroundArc;
	radius = juce::jmin(boundsArc.getWidth(), boundsArc.getHeight()) / 2.0f;
	auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
	auto lineW = radius * 0.1f;
	auto arcRadius = radius - lineW * 0.5f;
	backgroundArc.addCentredArc(boundsArc.getCentreX(),
		boundsArc.getCentreY(),
		arcRadius,
		arcRadius,
		0.0f,
		rotaryStartAngle,
		rotaryEndAngle,
		true);

	g.setColour(juce::Colour(72, 72, 72));
	g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

	juce::Path valueArc;
	valueArc.addCentredArc(boundsArc.getCentreX(),
		boundsArc.getCentreY(),
		arcRadius,
		arcRadius,
		0.0f,
		rotaryStartAngle,
		toAngle,
		true);
	if (slider.isEnabled()) {
		g.setColour(mainColor);
	}
	else {
		g.setColour(disabledColor);
	}
	g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

	// BACKGROUND
	auto minDialSize = (float)juce::jmin(boundsDial.getWidth(), boundsDial.getHeight());
	auto borderLarge = juce::jmin<float>(2.0f, minDialSize*0.025f);
	auto borderThin = juce::jmin<float>(1.5f, minDialSize*0.02f);

	radius = (float)juce::jmin(boundsDial.getWidth() / 2, boundsDial.getHeight() / 2);
	centreX = boundsDial.getCentreX();
	centreY = boundsDial.getCentreY();
	rx = centreX - radius;
	ry = centreY - radius;
	rw = radius * 2.0f;
	angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
	// bg gradient
	g.setGradientFill(juce::ColourGradient(outerDialUp, boundsDial.getCentreX(), boundsDial.getY(), outerDialDown, 
		boundsDial.getCentreX(), boundsDial.getBottom() * 0.9f, false));
	g.fillEllipse(rx, ry, rw, rw);
	// bg border
	g.setColour(juce::Colours::black);
	g.drawEllipse(rx, ry, rw, rw, borderLarge);

	// FOREGROUND
	radius *= 0.7f;
	rx = centreX - radius;
	ry = centreY - radius;
	rw = radius * 2.0f;
	// bg fill
	g.setColour(innerDial);
	g.fillEllipse(rx, ry, rw, rw);
	// border
	g.setColour(juce::Colours::black);
	g.drawEllipse(rx, ry, rw, rw, borderThin);

	// POINTER
	radius = (float)juce::jmin(boundsDial.getWidth() / 2, boundsDial.getHeight() / 2) * 0.975f;
	// border rounded
	juce::Path p_border;
	auto pointerLength = radius * 0.666f;
	auto pointerThickness = minDialSize * 0.12f;
	p_border.addRoundedRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength * 1.05f, borderThin);
	p_border.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
	g.setColour(juce::Colours::black);
	g.fillPath(p_border);
	// pointer fill
	juce::Path p;
	pointerThickness = minDialSize * 0.08f;
	p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
	p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
	if (slider.isEnabled()) {
		g.setColour(mainColor);
	}
	else {
		g.setColour(disabledColor);
	}
	g.fillPath(p);
}

int SonicMageLookAndFeel::getSliderThumbRadius (juce::Slider&)
{
    return 32;
}

void SonicMageLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	float, float, const juce::Slider::SliderStyle, juce::Slider& slider)
{
	juce::Colour deepBackgroundColor = findColour(SonicMageLookAndFeel::sonicMageGuiColors::deepBackgroundColor);

	float thumbHeight = static_cast<float>(getSliderThumbRadius(slider) * 2 - 4);
	float thumbWidth = thumbHeight * 0.375f;

	auto trackWidth = juce::jmin(9.0f, (float)height * 0.25f);

	juce::Point<float> startPoint((float)(x+width*0.5f), static_cast<float>(y));

	juce::Point<float> endPoint((float)(width * 0.5f + x), startPoint.y + height);

	juce::Path valueTrack;
	juce::Point<float> minPoint, maxPoint, thumbPoint;

	auto kx = startPoint.getX();
	auto ky =  sliderPos;

	minPoint = startPoint;
	maxPoint = { kx, ky };


	valueTrack.startNewSubPath(minPoint);
	valueTrack.lineTo(maxPoint);
	g.setColour(deepBackgroundColor);
	g.strokePath(valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

	valueTrack.clear();
	valueTrack.startNewSubPath(maxPoint);
	valueTrack.lineTo(endPoint);
	g.setColour(slider.findColour(sonicMageGuiColors::mainColor));
	g.strokePath(valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

	g.setColour(slider.findColour(juce::Slider::thumbColourId));
	
	juce::Colour thumbBgColor(65,65,65); 
	g.setColour(thumbBgColor);
	juce::Rectangle<float> thumb(startPoint.getX() - thumbWidth / 2, maxPoint.getY() - thumbHeight / 2, thumbWidth, thumbHeight);
	g.fillRoundedRectangle(thumb, 2.5);
	juce::Path thumbBorder;
	thumbBorder.addRoundedRectangle(thumb.expanded(1, 1), 2.5);
	g.setColour(deepBackgroundColor);
	g.strokePath(thumbBorder, juce::PathStrokeType(1));
	thumbBorder.clear();
	thumbBorder.addRoundedRectangle(thumb, 2.5);
	g.setColour(thumbBgColor.darker(0.8f));
	g.strokePath(thumbBorder, juce::PathStrokeType(1));
	thumbBorder.clear();
	thumbBorder.addRoundedRectangle(thumb.reduced(1, 1), 2.5);
	g.setColour(thumbBgColor.brighter(0.2f));
	g.strokePath(thumbBorder, juce::PathStrokeType(0.5));

	juce::Path thumbLight;
	float thumbLightHeight = 4;
	juce::Rectangle<float> thumbLightBounds(thumb.getX() + 1, thumb.getCentreY() - thumbLightHeight * 0.5f,
		thumb.getWidth() - 2, thumbLightHeight);
	thumbLight.addRoundedRectangle(thumbLightBounds, 1.5);
	g.setColour(findColour(sonicMageGuiColors::mainColor).brighter(0.5f));
	g.fillPath(thumbLight);
	g.setColour(findColour(sonicMageGuiColors::mainColor));
	g.strokePath(thumbLight, juce::PathStrokeType(1.0f));
	g.setColour(findColour(sonicMageGuiColors::mainColor).darker());
	g.strokePath(thumbLight, juce::PathStrokeType(0.5f));
	
	thumbBorder.clear();
	thumbBorder.addLineSegment(juce::Line<float>(thumb.getX()+1, thumb.getCentreY()-thumbLightHeight/2 - 1,
		thumb.getX()+thumb.getWidth()-1, thumb.getCentreY()-thumbLightHeight/2 - 1), 0.5);
	thumbBorder.addLineSegment(juce::Line<float>(thumb.getX()+1, thumb.getCentreY()+thumbLightHeight/2 + 1,
		thumb.getX()+thumb.getWidth()-1, thumb.getCentreY()+thumbLightHeight/2 + 1), 0.5);
	g.setColour(thumbBgColor.darker(0.7f));
	g.strokePath(thumbBorder, juce::PathStrokeType(0.5f));
	thumbBorder.clear();

	thumbBorder.addLineSegment(juce::Line<float>(thumb.getX()+1, thumb.getCentreY()-thumbLightHeight/2 - 2,
		thumb.getX()+thumb.getWidth()-1, thumb.getCentreY()-thumbLightHeight/2 - 2), 0.5);
	thumbBorder.addLineSegment(juce::Line<float>(thumb.getX()+1, thumb.getCentreY()+thumbLightHeight/2 + 2,
		thumb.getX()+thumb.getWidth()-1, thumb.getCentreY()+thumbLightHeight/2 + 2), 0.5);
	g.setColour(thumbBgColor.brighter(0.05f));
	g.strokePath(thumbBorder, juce::PathStrokeType(0.5f));
}

juce::Label* SonicMageLookAndFeel::createSliderTextBox(juce::Slider& slider)
{
	juce::Label* l = new customTextBox();

    l->setJustificationType (juce::Justification::centred);
    l->setKeyboardType (juce::TextInputTarget::decimalKeyboard);

    l->setColour (juce::Label::textColourId, slider.findColour (juce::Slider::textBoxTextColourId));
    l->setColour (juce::Label::backgroundColourId, findColour(sonicMageGuiColors::deepBackgroundColor));
    l->setColour (juce::Label::outlineColourId, slider.findColour (juce::Slider::textBoxOutlineColourId));

    return l;
}

void SonicMageLookAndFeel::customTextBox::paint(juce::Graphics& g)
{
	const juce::Font _font = getMainFont();
	g.setColour(findColour(juce::Label::backgroundColourId));
	g.fillRoundedRectangle(getLocalBounds().toFloat(), 3.5);

	auto alpha = isEnabled() ? 1.0f : 0.5f;

	g.setColour(findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));
	g.setFont(_font);
	auto textArea = getLookAndFeel().getLabelBorderSize(*this).subtractedFrom(getLocalBounds());

	g.drawFittedText(getText(), textArea, getJustificationType(),
		juce::jmax(1, (int)((float)textArea.getHeight() / _font.getHeight())),
		getMinimumHorizontalScale());
}

void SonicMageLookAndFeel::fillTextEditorBackground (juce::Graphics& g, int, int, juce::TextEditor& textEditor)
{
	g.setColour(findColour(sonicMageGuiColors::deepBackgroundColor));
	g.fillRoundedRectangle(textEditor.getLocalBounds().toFloat(), 3.5);
}

void SonicMageLookAndFeel::drawTextEditorOutline (juce::Graphics&, int, int, juce::TextEditor&)
{
}

juce::Font SonicMageLookAndFeel::getMainFont()
{
	return mainFont;
}

//=================================================================================================================================
//------------------------------------------------------ COMPONENT LAYOUT ---------------------------------------------------------
//=================================================================================================================================

SonicMageGuiComponent::SonicMageGuiComponent() : smLookAndFeel(), titleLogo(), brandLogo(), openButton("openButton"),
				scanPositionKnob("SCAN POSITION"), partialsKnob("PARTIALS"), pluckKnob("PLUCK"), harmonicityKnob("HARMONICITY"),
				attackKnob("A"), decayKnob("D"), sustainKnob("S"), releaseKnob("R"), gainSlider("OUTPUT GAIN")
{
	juce::Font mainFont = smLookAndFeel.getMainFont();
	mainFont.setHeight(28.0f);
	scanPositionKnob.setFont(mainFont);
	mainFont.setHeight(18.0f);
	partialsKnob.setFont(mainFont);
	pluckKnob.setFont(mainFont);
	harmonicityKnob.setFont(mainFont);
	mainFont.setHeight(20.0f);
	gainSlider.setFont(mainFont);

    addAndMakeVisible(&scanPositionKnob);
    addAndMakeVisible(&partialsKnob);
    addAndMakeVisible(&pluckKnob);
    addAndMakeVisible(&harmonicityKnob);
	addAndMakeVisible(&attackKnob);
	addAndMakeVisible(&decayKnob);
	addAndMakeVisible(&sustainKnob);
	addAndMakeVisible(&releaseKnob);
	addAndMakeVisible(&gainSlider);
	addAndMakeVisible(&openButton);
	addAndMakeVisible(&titleLogo);
	addAndMakeVisible(&brandLogo);
	addChildComponent(&cursor);
	scanPositionKnob.getSlider().onValueChange = [this] { cursor.setPosition(static_cast<float>(scanPositionKnob.getSlider().getValue())); };
	cursor.setVisible(true);

	scanPositionKnob.setComponentID("SCAN_KNOB");
	partialsKnob.setComponentID("PARTIALS_KNOB");
	pluckKnob.setComponentID("PLUCK_KNOB");
	harmonicityKnob.setComponentID("HARMONICITY_KNOB");
	gainSlider.setComponentID("GAIN_SLIDER");
	attackKnob.setComponentID("ATTACK_KNOB");
	decayKnob.setComponentID("DECAY_KNOB");
	sustainKnob.setComponentID("SUSTAIN_KNOB");
	releaseKnob.setComponentID("RELEASE_KNOB");
	openButton.setComponentID("OPEN_BUTTON");

    setLookAndFeel(&smLookAndFeel);
    setSize (1024, 512);
}

SonicMageGuiComponent::~SonicMageGuiComponent()
{
    setLookAndFeel(nullptr);
}

void SonicMageGuiComponent::paint (juce::Graphics& g)
{
	juce::Colour backgroundColor = getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::backgroundColor);
	juce::Colour deepBackgroundColor = getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::deepBackgroundColor);
	juce::Colour neutralBackgroundColor = getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::neutralBackgroundColor);
	juce::Colour brandBackgroundColor = getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::brandBackgroundColor);
	juce::Colour borderColor = getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::borderColor);

	/* Backgrounds */
    g.fillAll (backgroundColor);
	g.setColour(neutralBackgroundColor);
	g.fillRect(header);
	g.fillRect(master);
	g.setColour(deepBackgroundColor);
	g.fillRect(midImage);
	g.setColour(brandBackgroundColor);
	g.fillRect(brand);

	/* Borders */
	juce::Path borders;
	borders.addRectangle(brand.withTrimmedTop(0).withTrimmedRight(0));
	g.setColour(deepBackgroundColor);
	g.strokePath(borders, juce::PathStrokeType(2.5f));
	borders.clear();
	borders.addLineSegment(juce::Line<int>(header.getBottomLeft().translated(0, -1), header.getBottomRight().translated(0, -1)).toFloat(), 0.5f);
	borders.addLineSegment(juce::Line<int>(sideLeft.getTopRight(), sideLeft.getBottomRight()).toFloat(), 0.5f);
	borders.addLineSegment(juce::Line<int>(midImage.getTopRight(), midImage.getBottomRight()).toFloat(), 0.5f);
	borders.addLineSegment(juce::Line<int>(sideRight.getTopRight(), sideRight.getBottomRight()).toFloat(), 0.5f);
	g.setColour(borderColor);
	g.strokePath(borders, juce::PathStrokeType(0.5f));

	g.setColour(neutralBackgroundColor.darker(0.3f));
	g.drawRect(master.reduced(1), 2);
	g.setColour(neutralBackgroundColor.brighter(0.02f));
	g.drawRect(master.reduced(3), 1);
	juce::Rectangle<int> hbox = sideLeft.withTrimmedTop(static_cast<int>(sideLeft.getHeight() * 0.5 + 5));
	borders.clear();
	borders.addLineSegment(juce::Line<int>(hbox.getTopRight(), hbox.getTopLeft()).toFloat(), 0.5f);
	g.setColour(borderColor);
	g.strokePath(borders, juce::PathStrokeType(0.5f));

	borders.clear();
	borders.addLineSegment(juce::Line<int>(sideRight.getTopRight().translated(-1, 1), sideRight.getTopLeft().translated(1,1)).toFloat(), 0.5f);
	borders.addLineSegment(juce::Line<int>(sideLeft.getTopRight().translated(-1, 1), sideLeft.getTopLeft().translated(1,1)).toFloat(), 0.5f);
	borders.addLineSegment(juce::Line<int>(hbox.getTopRight().translated(-1, 1), hbox.getTopLeft().translated(1,1)).toFloat(), 0.5f);
	g.setColour(backgroundColor.darker(0.3f));
	g.strokePath(borders, juce::PathStrokeType(0.5f));

	borders.clear();
	borders.addLineSegment(juce::Line<int>(sideRight.getTopRight().translated(-1, 1), sideRight.getTopLeft().translated(1,1)).toFloat(), 0.5f);
	borders.addLineSegment(juce::Line<int>(sideLeft.getTopRight().translated(-1, 1), sideLeft.getTopLeft().translated(1,1)).toFloat(), 0.5f);
	borders.addLineSegment(juce::Line<int>(hbox.getTopRight().translated(-1, 2), hbox.getTopLeft().translated(1,2)).toFloat(), 0.5f);
	g.setColour(backgroundColor.brighter(0.05f));
	g.strokePath(borders, juce::PathStrokeType(0.5f));

	if (this->img != nullptr){
		g.drawImage(*this->img, midImage.withTrimmedLeft(1).withTrimmedRight(1).toFloat());
    }
}

void SonicMageGuiComponent::resized()
{
	header.setBounds(0, 0, 1024, 92);
	sideLeft.setBounds(0, 92, 258, 420);
	midImage.setBounds(258, 92, 420, 420);
	sideRight.setBounds(678, 92, 185, 420);
	master.setBounds(863, 92, 161, 420);
	brand = header.withLeft(sideRight.getRight());
    int k_size = 200;
    scanPositionKnob.setBounds(static_cast<int>((sideLeft.getWidth() - k_size) * 0.5), sideLeft.getY(), k_size, k_size);

	int b_w = 85;
	int b_h = 60;
	
	auto buttonBounds = header.withRight(sideLeft.getRight());
	auto centre = buttonBounds.getCentre();
	buttonBounds.setWidth(b_w);
	buttonBounds.setHeight(b_h);
	buttonBounds.setCentre(centre);
	openButton.setBounds(buttonBounds);

	auto titleBounds = header.withLeft(midImage.getX()+40).withRight(midImage.getRight()-40);
	titleLogo.setBounds(titleBounds);

	brandLogo.setBounds(brand.reduced(20));

	k_size = 110;
	
	juce::Rectangle<int> partialsbox = sideLeft.withTrimmedTop(static_cast<int>(sideLeft.getHeight() * 0.5)).withTrimmedLeft(15).withTrimmedRight(15);
	partialsKnob.setBounds(partialsbox.getX(), static_cast<int>(partialsbox.getCentreY() - k_size * 0.5), k_size, k_size);
	harmonicityKnob.setBounds(partialsbox.getRight() - k_size, static_cast<int>(partialsbox.getCentreY() - k_size * 0.5f), k_size, k_size);

	juce::Rectangle<int> envbox = sideRight.withTrimmedTop(10).withTrimmedBottom(10);
	k_size = 100;
	pluckKnob.setBounds(static_cast<int>(envbox.getCentreX() - k_size * 0.5f), envbox.getY(), k_size, k_size);
	envbox.removeFromTop(k_size+10);
	k_size = 69;
	attackKnob.setBounds(static_cast<int>(envbox.getCentreX() - k_size * 0.5f), envbox.getY(), k_size, k_size);
	envbox.removeFromTop(k_size+3);
	decayKnob.setBounds(static_cast<int>(envbox.getCentreX() - k_size * 0.5f), envbox.getY(), k_size, k_size);
	envbox.removeFromTop(k_size+3);
	sustainKnob.setBounds(static_cast<int>(envbox.getCentreX() - k_size * 0.5f), envbox.getY(), k_size, k_size);
	envbox.removeFromTop(k_size+3);
	releaseKnob.setBounds(static_cast<int>(envbox.getCentreX() - k_size * 0.5f), envbox.getY(), k_size, k_size);
	envbox.removeFromTop(k_size+3);
	gainSlider.setBounds(master.reduced(20));

	cursor.setBounds(midImage);
}

void SonicMageGuiComponent::setImage(std::shared_ptr<juce::Image> _img)
{
	this->img = std::make_unique<juce::Image>(_img->rescaled(midImage.getWidth(), midImage.getHeight(), juce::Graphics::ResamplingQuality::lowResamplingQuality));
	repaint();
}

void ScanCursor::paint(juce::Graphics& g)
{
	static const float CURSOR_W = 6.0;
	auto mainColor = getLookAndFeel().findColour(SonicMageLookAndFeel::sonicMageGuiColors::mainColor);
	auto bounds = getLocalBounds().toFloat();
	auto rect = bounds.withWidth(CURSOR_W).withX(bounds.getX() + (bounds.getWidth() - CURSOR_W + 4) * position - 2);

	g.setColour(mainColor.withAlpha(0.2f));
	g.fillRoundedRectangle(rect, 2.0f);
	g.setColour(mainColor.brighter(0.4f).withAlpha(0.6f));
	g.fillRoundedRectangle(rect.reduced(1), 2.0f);
	g.setColour(mainColor.brighter(0.4f));
	g.fillRoundedRectangle(rect.reduced(2), 2.0f);
}

