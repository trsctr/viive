/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ViiveAudioProcessorEditor::ViiveAudioProcessorEditor (ViiveAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    m_delayGroup.setText("Delay");
    m_delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    addAndMakeVisible(m_delayGroup);

    m_feedbackGroup.setText("Feedback");
    m_feedbackGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    addAndMakeVisible(m_feedbackGroup);

    m_filterGroup.setText("Filter");
    m_filterGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    addAndMakeVisible(m_filterGroup);

	m_outputGroup.setText("Output");
	m_outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
	addAndMakeVisible(m_outputGroup);

    setSize (600, 330);
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

	m_delayGroup.setBounds(10, y, 130, height / 2 - 10);
    m_outputGroup.setBounds(bounds.getWidth() - 140, y, 130, height);
	m_feedbackGroup.setBounds(m_delayGroup.getRight() + 10, y, m_outputGroup.getX() - m_delayGroup.getRight() - 20, height / 2 - 10);
    m_filterGroup.setBounds(10, height / 2 + 10, m_outputGroup.getX() - 20, height / 2);
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
