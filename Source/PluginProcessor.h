#pragma once

#include <JuceHeader.h>
#include "Parameters.h"
#include "DelayEngine.h"
#include "Tempo.h"
#if JUCE_DEBUG
#include "LFOScopeBuffer.h"
#endif

//==============================================================================
/**
*/
class ViiveAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ViiveAudioProcessor();
    ~ViiveAudioProcessor() override;

    juce::AudioProcessorValueTreeState apvts{
    *this, nullptr, "Parameters", Parameters::createParameterLayout()
    };

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

    std::atomic<float>& getOutputLevelL() { return m_outputLevelL; }
    std::atomic<float>& getOutputLevelR() { return m_outputLevelR; }
    std::atomic<float>& getLowCutLfoValue()  { return m_delayEngine.getLowCutLfoValue(); }
    std::atomic<float>& getHighCutLfoValue() { return m_delayEngine.getHighCutLfoValue(); }

#if JUCE_DEBUG
    LFOScopeBuffer& getLFOScopeBuffer() { return m_delayEngine.getLFOScopeBuffer(); }
#endif

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    Parameters m_params;
    DelayEngine m_delayEngine;
    Tempo m_tempo;

    std::atomic<float> m_outputLevelL, m_outputLevelR;
//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViiveAudioProcessor)
};
