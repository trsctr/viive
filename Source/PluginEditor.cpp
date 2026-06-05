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
	m_delayGroup.addChildComponent(m_delayNoteLKnob);
	m_delayGroup.addChildComponent(m_delayNoteRKnob);
    m_delayGroup.addAndMakeVisible(m_feedbackKnob);
	m_delayGroup.addAndMakeVisible(m_offsetKnob);
	m_delayGroup.addAndMakeVisible(m_tempoSyncLButton);
	m_delayGroup.addAndMakeVisible(m_tempoSyncRButton);
	addAndMakeVisible(m_modeSelector);
	addAndMakeVisible(m_delayGroup);

    m_filterModGroup.setText("Modulation");
	m_filterModGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
	m_filterModGroup.setColour(juce::GroupComponent::outlineColourId, juce::Colours::transparentBlack);
	m_filterModGroup.addAndMakeVisible(m_lowCutModRateKnob);
	m_filterModGroup.addChildComponent(m_lowCutModNoteKnob);
	m_filterModGroup.addAndMakeVisible(m_lowCutModDepthKnob);
	m_filterModGroup.addAndMakeVisible(m_lowCutModPhaseKnob);
	m_filterModGroup.addAndMakeVisible(m_highCutModRateKnob);
	m_filterModGroup.addChildComponent(m_highCutModNoteKnob);
	m_filterModGroup.addAndMakeVisible(m_highCutModDepthKnob);
	m_filterModGroup.addAndMakeVisible(m_highCutModPhaseKnob);
	m_filterModGroup.addAndMakeVisible(m_lowCutModTempoSyncButton);
	m_filterModGroup.addAndMakeVisible(m_highCutModTempoSyncButton);
	m_filterModGroup.addAndMakeVisible(m_lowCutModShapeSelector);
	m_filterModGroup.addAndMakeVisible(m_highCutModShapeSelector);
	m_filterModGroup.addAndMakeVisible(m_lowCutLfoIndicator);
	m_filterModGroup.addAndMakeVisible(m_highCutLfoIndicator);
	m_filterModGroup.addAndMakeVisible(m_highCutModInvertButton);

	m_filterGroup.setText("Filter");
    m_filterGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
	m_filterGroup.addAndMakeVisible(m_filterModGroup);
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

    m_chorusGroup.setText("");
	m_chorusGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
	m_chorusGroup.addAndMakeVisible(m_chorusIntensityKnob);
	m_chorusGroup.addAndMakeVisible(m_chorusModRateKnob);
	m_chorusGroup.addAndMakeVisible(m_chorusModDepthKnob);
	addAndMakeVisible(m_chorusGroup);

	m_lofiGroup.setText("");
	m_lofiGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
	m_lofiGroup.addAndMakeVisible(m_lofiResampleFreqKnob);
	m_lofiGroup.addAndMakeVisible(m_lofiDampenFreqKnob);
	m_lofiGroup.addAndMakeVisible(m_lofiMixLevelKnob);
	m_lofiGroup.addAndMakeVisible(m_lofiNoiseEnabledButton);
	addChildComponent(m_lofiGroup);
//	addAndMakeVisible(m_lofiGroup);
	addAndMakeVisible(m_insertEffectTypeSelector);

	addAndMakeVisible(m_feedbackKillButton);
	addAndMakeVisible(m_meter);

#if JUCE_DEBUG
	m_lfoScope = std::make_unique<LFOScope>(p.getLFOScopeBuffer());
	addAndMakeVisible(*m_lfoScope);
#endif

	bool syncL = m_audioProcessor.apvts.getParameter(tempoSyncLParamID.getParamID())->getValue() > 0.5f;
	bool syncR = m_audioProcessor.apvts.getParameter(tempoSyncRParamID.getParamID())->getValue() > 0.5f;
	bool lowCutSync = m_audioProcessor.apvts.getParameter(lowCutModTempoSyncParamID.getParamID())->getValue() > 0.5f;
	bool highCutSync = m_audioProcessor.apvts.getParameter(highCutModTempoSyncParamID.getParamID())->getValue() > 0.5f;

	updateSyncedKnobs(syncL, syncR, lowCutSync, highCutSync);

	auto* insertEffectParam = dynamic_cast<juce::AudioParameterChoice*>(m_audioProcessor.apvts.getParameter(insertEffectTypeParamID.getParamID()));
	updateInsertEffectControls(insertEffectParam->getIndex());
	
	m_audioProcessor.apvts.addParameterListener(tempoSyncLParamID.getParamID(), this);
	m_audioProcessor.apvts.addParameterListener(tempoSyncRParamID.getParamID(), this);
	m_audioProcessor.apvts.addParameterListener(lowCutModTempoSyncParamID.getParamID(), this);
	m_audioProcessor.apvts.addParameterListener(highCutModTempoSyncParamID.getParamID(), this);
	m_audioProcessor.apvts.addParameterListener(insertEffectTypeParamID.getParamID(), this);
#if JUCE_DEBUG
	setSize(770, 670);
#else
	setSize(770, 490);
#endif
}

ViiveAudioProcessorEditor::~ViiveAudioProcessorEditor()
{
	m_audioProcessor.apvts.removeParameterListener(tempoSyncLParamID.getParamID(), this);
	m_audioProcessor.apvts.removeParameterListener(tempoSyncRParamID.getParamID(), this);
	m_audioProcessor.apvts.removeParameterListener(lowCutModTempoSyncParamID.getParamID(), this);
	m_audioProcessor.apvts.removeParameterListener(highCutModTempoSyncParamID.getParamID(), this);
	m_audioProcessor.apvts.removeParameterListener(insertEffectTypeParamID.getParamID(), this);
}

//==============================================================================
void ViiveAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkgrey);
	g.setColour(juce::Colours::white);
	g.setFont(12.0f);
	g.drawText("viive 0.3.0-beta.2", getLocalBounds().reduced(5), juce::Justification::topRight);
//	g.drawText("viive " JucePlugin_VersionString, getLocalBounds().reduced(5), juce::Justification::topRight);
}

void ViiveAudioProcessorEditor::resized()
{
	auto bounds = getLocalBounds();
    int y = 10;
#ifdef JUCE_DEBUG
	int height = bounds.getHeight() - 30 - 180;
#else
	int height = bounds.getHeight() - 20;
#endif
    m_chorusGroup.setBounds(bounds.getWidth() - 360, y + 35, 300, 165);
    m_lofiGroup.setBounds(bounds.getWidth() - 360, y + 35, 300, 165);

    m_delayGroup.setBounds(10, y + 35, m_chorusGroup.getX() - 20, 165);
	m_outputGroup.setBounds(bounds.getWidth() - 360, m_delayGroup.getBottom() + 10, 300, 155);
	m_filterGroup.setBounds(10, m_delayGroup.getBottom() + 10, m_outputGroup.getX() - 20, 265);
	m_filterModGroup.setBounds(0, m_lowCutFreqKnob.getBottom() + 30, m_filterGroup.getWidth(), 120);
	
	m_delayTimeLKnob.setTopLeftPosition(20, 20);
	m_delayTimeRKnob.setTopLeftPosition(m_delayTimeLKnob.getRight() + 20, 20);
	m_delayNoteLKnob.setTopLeftPosition(m_delayTimeLKnob.getX(), m_delayTimeLKnob.getY());
	m_delayNoteRKnob.setTopLeftPosition(m_delayTimeRKnob.getX(), m_delayTimeRKnob.getY());
	m_tempoSyncLButton.setCentrePosition(m_delayTimeLKnob.getRight() - 35, m_delayTimeLKnob.getBottom() + 15);
	m_tempoSyncRButton.setCentrePosition(m_delayTimeRKnob.getRight() - 35, m_delayTimeRKnob.getBottom() + 15);
	m_feedbackKnob.setTopLeftPosition(m_delayTimeRKnob.getRight() + 20, 20);
	
	m_modeSelector.setTopLeftPosition(10, y);

	m_insertEffectTypeSelector.setCentrePosition(m_chorusGroup.getX()+m_chorusGroup.getWidth()/2, m_chorusGroup.getY() + 15);

	m_offsetKnob.setTopLeftPosition(m_feedbackKnob.getRight() + 20, 20);
	m_lowCutFreqKnob.setTopLeftPosition(20, 20);
	m_lowCutQKnob.setTopLeftPosition(m_lowCutFreqKnob.getRight() + 20, 20);
	m_lowCutModRateKnob.setTopLeftPosition(20, 20);
	m_lowCutModNoteKnob.setTopLeftPosition(m_lowCutModRateKnob.getX(), m_lowCutModRateKnob.getY());
	m_lowCutModDepthKnob.setTopLeftPosition(m_lowCutModRateKnob.getRight() + 10, 20);
	m_lowCutModPhaseKnob.setTopLeftPosition(m_lowCutModDepthKnob.getRight() + 10, 20);
	m_lowCutModTempoSyncButton.setTopLeftPosition(20, m_lowCutModRateKnob.getBottom() + 5);
	m_lowCutModShapeSelector.setTopLeftPosition(m_lowCutModDepthKnob.getX(), m_lowCutModTempoSyncButton.getY());

	constexpr int indicatorSize = 8;
	m_lowCutLfoIndicator.setSize(indicatorSize, indicatorSize);
	m_lowCutLfoIndicator.setCentrePosition(m_lowCutModPhaseKnob.getBounds().getCentreX(),
	                                        m_lowCutModPhaseKnob.getBottom() + 15);
	m_highCutFreqKnob.setTopLeftPosition(m_lowCutQKnob.getRight() + 20, 20);
	m_highCutQKnob.setTopLeftPosition(m_highCutFreqKnob.getRight() + 20, 20);
	m_highCutModPhaseKnob.setTopRightPosition(m_highCutQKnob.getRight(), 20);
	m_highCutModDepthKnob.setTopRightPosition(m_highCutModPhaseKnob.getX() - 10, 20);
	m_highCutModRateKnob.setTopRightPosition(m_highCutModDepthKnob.getX() - 10, 20);
	m_highCutModNoteKnob.setTopLeftPosition(m_highCutModRateKnob.getX(), m_highCutModRateKnob.getY());
	m_highCutModTempoSyncButton.setTopLeftPosition(m_highCutModRateKnob.getX(), m_highCutModRateKnob.getBottom() + 5);
	m_highCutModShapeSelector.setTopLeftPosition(m_highCutModDepthKnob.getX(), m_highCutModTempoSyncButton.getY());

	m_highCutLfoIndicator.setSize(indicatorSize, indicatorSize);
	m_highCutLfoIndicator.setCentrePosition(m_highCutModPhaseKnob.getBounds().getCentreX(),
	                                         m_highCutModPhaseKnob.getBottom() + 15);
	m_highCutModInvertButton.setCentrePosition(m_highCutLfoIndicator.getBounds().getCentreX() + 25,
	                                            m_highCutLfoIndicator.getBounds().getCentreY());
	m_stereoKnob.setTopLeftPosition(20, 20);
	m_mixKnob.setTopLeftPosition(m_stereoKnob.getRight() + 20, 20);
	m_gainKnob.setTopLeftPosition(m_mixKnob.getRight() + 20, 20);

	m_chorusIntensityKnob.setTopLeftPosition(20, 20);
	m_chorusModRateKnob.setTopLeftPosition(m_chorusIntensityKnob.getRight() + 20, 20);
	m_chorusModDepthKnob.setTopLeftPosition(m_chorusModRateKnob.getRight() + 20, 20);

	m_lofiMixLevelKnob.setTopLeftPosition(20, 20);
	m_lofiResampleFreqKnob.setTopLeftPosition(m_lofiMixLevelKnob.getRight() + 20, 20);
	m_lofiDampenFreqKnob.setTopLeftPosition(m_lofiResampleFreqKnob.getRight() + 20, 20);
	m_lofiNoiseEnabledButton.setCentrePosition(m_lofiDampenFreqKnob.getRight() - 35, m_lofiDampenFreqKnob.getBottom() + 15);

	m_feedbackKillButton.setCentrePosition(m_outputGroup.getX() + m_outputGroup.getWidth() / 2,
	                                        m_outputGroup.getBottom() + 20);
	m_meter.setBounds(m_outputGroup.getRight() + 15, y + 45, 35, height - 45);

#if JUCE_DEBUG
	m_lfoScope->setBounds(10, 490, bounds.getWidth() - 20, 170);
#endif
}

void ViiveAudioProcessorEditor::updateSyncedKnobs(bool syncLActive, bool syncRActive, bool lowCutSyncActive, bool highCutSyncActive)
{
	m_delayTimeLKnob.setVisible(!syncLActive);
	m_delayNoteLKnob.setVisible(syncLActive);
	m_delayTimeRKnob.setVisible(!syncRActive);
	m_delayNoteRKnob.setVisible(syncRActive);
	m_lowCutModRateKnob.setVisible(!lowCutSyncActive);
	m_lowCutModNoteKnob.setVisible(lowCutSyncActive);
	m_highCutModRateKnob.setVisible(!highCutSyncActive);
	m_highCutModNoteKnob.setVisible(highCutSyncActive);
}

void ViiveAudioProcessorEditor::updateInsertEffectControls(int insertEffectType)
{
	DBG("Insert effect type changed: " << insertEffectType);
	bool isChorus = (insertEffectType == 0);
	bool isLofi = (insertEffectType == 1);

	m_chorusGroup.setVisible(isChorus);
	m_lofiGroup.setVisible(isLofi);
}

void ViiveAudioProcessorEditor::parameterChanged(const juce::String& paramID, float newValue) {
	if (paramID == tempoSyncLParamID.getParamID() || paramID == tempoSyncRParamID.getParamID() || paramID == lowCutModTempoSyncParamID.getParamID() || paramID == highCutModTempoSyncParamID.getParamID() || paramID == insertEffectTypeParamID.getParamID()){
		juce::MessageManager::callAsync([this] {
			bool syncL = m_audioProcessor.apvts.getParameter(tempoSyncLParamID.getParamID())->getValue() > 0.5f;
			bool syncR = m_audioProcessor.apvts.getParameter(tempoSyncRParamID.getParamID())->getValue() > 0.5f;
			bool lowCutSync = m_audioProcessor.apvts.getParameter(lowCutModTempoSyncParamID.getParamID())->getValue() > 0.5f;
			bool highCutSync = m_audioProcessor.apvts.getParameter(highCutModTempoSyncParamID.getParamID())->getValue() > 0.5f;
			updateSyncedKnobs(syncL, syncR, lowCutSync, highCutSync);
			auto* param = dynamic_cast<juce::AudioParameterChoice*>(m_audioProcessor.apvts.getParameter(insertEffectTypeParamID.getParamID()));
			updateInsertEffectControls(param->getIndex());
			});
	}
}

