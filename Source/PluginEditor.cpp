/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ViiveAudioProcessorEditor::ViiveAudioProcessorEditor (ViiveAudioProcessor& p)
    : AudioProcessorEditor (&p), m_audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    m_delayGroup.setText("Delay");
    m_delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    m_delayGroup.addAndMakeVisible(m_delayTimeKnob);
    m_delayGroup.addAndMakeVisible(m_feedbackKnob);
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

    setSize (550, 330);
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

    m_outputGroup.setBounds(bounds.getWidth() - 140, y, 130, height);
    m_delayGroup.setBounds(10, y, m_outputGroup.getX() - 20, height / 2 - 10);
    m_filterGroup.setBounds(10, height / 2 + 10, m_outputGroup.getX() - 20, height / 2);
    
	m_delayTimeKnob.setTopLeftPosition(20, 20);
	m_feedbackKnob.setTopLeftPosition(m_delayTimeKnob.getRight() + 20, 20);
	m_lowCutFreqKnob.setTopLeftPosition(20, 20);
	m_lowCutQKnob.setTopLeftPosition(m_lowCutFreqKnob.getRight() + 20, 20);
	m_highCutFreqKnob.setTopLeftPosition(m_lowCutQKnob.getRight() + 20, 20);
	m_highCutQKnob.setTopLeftPosition(m_highCutFreqKnob.getRight() + 20, 20);
	m_mixKnob.setTopLeftPosition(20, 20);
	m_gainKnob.setTopLeftPosition(20, m_mixKnob.getBottom() + 20);

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
