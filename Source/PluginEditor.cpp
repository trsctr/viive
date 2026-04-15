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
    m_delayGroup.addAndMakeVisible(m_delayTimeLKnob);
	m_delayGroup.addAndMakeVisible(m_delayTimeRKnob);
    m_delayGroup.addAndMakeVisible(m_feedbackKnob);
	m_delayGroup.addAndMakeVisible(m_spreadKnob);
	m_delayGroup.addAndMakeVisible(m_delayLinkButton);
	m_delayGroup.addAndMakeVisible(m_tempoSyncLButton);
	m_delayGroup.addAndMakeVisible(m_tempoSyncRButton);
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
	m_outputGroup.addAndMakeVisible(m_stereoKnob);
	m_outputGroup.addAndMakeVisible(m_mixKnob);
	m_outputGroup.addAndMakeVisible(m_gainKnob);
    addAndMakeVisible(m_outputGroup);

    m_chorusGroup.setText("Chorus");
	m_chorusGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
	m_chorusGroup.addAndMakeVisible(m_chorusIntensityKnob);
	m_chorusGroup.addAndMakeVisible(m_chorusModRateKnob);
	m_chorusGroup.addAndMakeVisible(m_chorusModDepthKnob);
	addAndMakeVisible(m_chorusGroup);

	addAndMakeVisible(m_meter);
	linkParameters(delayTimeLParamID.getParamID(), delayTimeRParamID.getParamID());
	setSize (770, 330);
}

ViiveAudioProcessorEditor::~ViiveAudioProcessorEditor()
{
	juce::HashMap<juce::String, juce::String>::Iterator it(m_linkedParams);
	while (it.next())
		m_audioProcessor.apvts.removeParameterListener(it.getKey(), this);
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

    m_chorusGroup.setBounds(bounds.getWidth() - 360, y, 300, height / 2);
    m_delayGroup.setBounds(10, y, m_chorusGroup.getX() - 20, height / 2);
	m_outputGroup.setBounds(bounds.getWidth() - 360, height / 2 + 20, 300, height / 2 - 10);
	m_filterGroup.setBounds(10, height / 2 + 20, m_outputGroup.getX() - 20, height / 2 - 10);

	m_delayTimeLKnob.setTopLeftPosition(20, 20);
	m_delayTimeRKnob.setTopLeftPosition(m_delayTimeLKnob.getRight() + 20, 20);
	m_delayLinkButton.setTopLeftPosition(m_delayTimeLKnob.getRight() - 10, m_delayTimeLKnob.getBottom() - 45);

	m_feedbackKnob.setTopLeftPosition(m_delayTimeRKnob.getRight() + 20, 20);
	m_spreadKnob.setTopLeftPosition(m_feedbackKnob.getRight() + 20, 20);
	m_lowCutFreqKnob.setTopLeftPosition(20, 20);
	m_lowCutQKnob.setTopLeftPosition(m_lowCutFreqKnob.getRight() + 20, 20);
	m_highCutFreqKnob.setTopLeftPosition(m_lowCutQKnob.getRight() + 20, 20);
	m_highCutQKnob.setTopLeftPosition(m_highCutFreqKnob.getRight() + 20, 20);
	m_stereoKnob.setTopLeftPosition(20, 20);
	m_mixKnob.setTopLeftPosition(m_stereoKnob.getRight() + 20, 20);
	m_gainKnob.setTopLeftPosition(m_mixKnob.getRight() + 20, 20);
	m_chorusIntensityKnob.setTopLeftPosition(20, 20);
	m_chorusModRateKnob.setTopLeftPosition(m_chorusIntensityKnob.getRight() + 20, 20);
	m_chorusModDepthKnob.setTopLeftPosition(m_chorusModRateKnob.getRight() + 20, 20);

	m_meter.setBounds(m_outputGroup.getRight() + 15, 20, 35, height - 15);
}

void ViiveAudioProcessorEditor::linkParameters(const juce::String& idA, const juce::String& idB) {
	m_linkedParams.set(idA, idB);
	m_linkedParams.set(idB, idA);
	m_audioProcessor.apvts.addParameterListener(idA, this);
	m_audioProcessor.apvts.addParameterListener(idB, this);
}

void ViiveAudioProcessorEditor::parameterChanged(const juce::String& paramID, float newValue) {
	if (m_audioProcessor.apvts.getParameter(delayLinkParamID.getParamID())->getValue()
		&& m_linkedParams.contains(paramID)) {
		auto* param = m_audioProcessor.apvts.getParameter(m_linkedParams[paramID]);
		param->setValueNotifyingHost(param->convertTo0to1(newValue));
	}
}