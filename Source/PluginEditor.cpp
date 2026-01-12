/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ViiveAudioProcessorEditor::ViiveAudioProcessorEditor(ViiveAudioProcessor& p)
	: AudioProcessorEditor(&p), m_audioProcessor(p), m_meter{ p.getOutputLevelL(), p.getOutputLevelR() }
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    m_delayGroup.setText("Delay");
    m_delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    m_delayGroup.addAndMakeVisible(m_delayTimeKnob);
    m_delayGroup.addAndMakeVisible(m_feedbackKnob);
	m_delayGroup.addAndMakeVisible(m_stereoKnob);
	m_delayGroup.addAndMakeVisible(m_effectAmtKnob);
    addAndMakeVisible(m_delayGroup);

    m_filterGroup.setText("Filter");
    m_filterGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
	m_filterGroup.addAndMakeVisible(m_lowCutFreqKnob);
	m_filterGroup.addAndMakeVisible(m_lowCutQKnob);
	m_filterGroup.addAndMakeVisible(m_highCutFreqKnob);
	m_filterGroup.addAndMakeVisible(m_highCutQKnob);
    addAndMakeVisible(m_filterGroup);

	m_outputGroup.setText("Output");
	m_outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
	m_outputGroup.addAndMakeVisible(m_mixKnob);
	m_outputGroup.addAndMakeVisible(m_gainKnob);
    addAndMakeVisible(m_outputGroup);

    m_fxGroup.setText("FX");
	m_fxGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
	m_fxGroup.addAndMakeVisible(m_fxParam1Knob);
	m_fxGroup.addAndMakeVisible(m_fxParam2Knob);
	addAndMakeVisible(m_fxGroup);

	addAndMakeVisible(m_meter);

    setSize (670, 330);
}

ViiveAudioProcessorEditor::~ViiveAudioProcessorEditor()
{
}

//==============================================================================
void ViiveAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkgrey);
}

void ViiveAudioProcessorEditor::resized()
{
	auto bounds = getLocalBounds();
    int y = 10;
	int height = bounds.getHeight() - 20;

    m_fxGroup.setBounds(bounds.getWidth() - 260, y, 200, height / 2 - 10);
    m_delayGroup.setBounds(10, y, m_fxGroup.getX() - 20, height / 2 - 10);
    m_filterGroup.setBounds(10, height / 2 + 10, m_fxGroup.getX() - 20, height / 2);
	m_outputGroup.setBounds(m_fxGroup.getX(), height / 2 + 10, 200, height / 2);

	m_delayTimeKnob.setTopLeftPosition(20, 20);
	m_feedbackKnob.setTopLeftPosition(m_delayTimeKnob.getRight() + 20, 20);
	m_stereoKnob.setTopLeftPosition(m_feedbackKnob.getRight() + 20, 20);
	m_effectAmtKnob.setTopLeftPosition(m_stereoKnob.getRight() + 20, 20);
	m_lowCutFreqKnob.setTopLeftPosition(20, 20);
	m_lowCutQKnob.setTopLeftPosition(m_lowCutFreqKnob.getRight() + 20, 20);
	m_highCutFreqKnob.setTopLeftPosition(m_lowCutQKnob.getRight() + 20, 20);
	m_highCutQKnob.setTopLeftPosition(m_highCutFreqKnob.getRight() + 20, 20);
	m_mixKnob.setTopLeftPosition(20, 20);
	m_gainKnob.setTopLeftPosition(m_mixKnob.getRight() + 20, 20);
	m_fxParam1Knob.setTopLeftPosition(20, 20);
	m_fxParam2Knob.setTopLeftPosition(m_fxParam1Knob.getRight() + 20, 20);

	m_meter.setBounds(m_outputGroup.getRight() + 15, 20, 30, height - 10);
	// This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
