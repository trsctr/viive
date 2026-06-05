#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ProtectYourEars.h"

//==============================================================================
ViiveAudioProcessor::ViiveAudioProcessor()
     : AudioProcessor (BusesProperties()
                     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
    ),
    m_params(apvts)
{
}

ViiveAudioProcessor::~ViiveAudioProcessor()
{
}

//==============================================================================
const juce::String ViiveAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ViiveAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ViiveAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ViiveAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ViiveAudioProcessor::getTailLengthSeconds() const
{
    return 5.0;
}

int ViiveAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ViiveAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ViiveAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ViiveAudioProcessor::getProgramName (int index)
{
    return {};
}

void ViiveAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ViiveAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	m_params.prepareToPlay(sampleRate);
    m_params.reset();
	m_delayEngine.prepareToPlay(sampleRate, samplesPerBlock);
	m_delayEngine.reset();
    m_tempo.reset();
    m_outputLevelL.store(0.0f);
    m_outputLevelR.store(0.0f);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ViiveAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ViiveAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainInputChannels() == 2 && layouts.getMainOutputChannels() == 2;
}
#endif

void ViiveAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    m_tempo.update(getPlayHead());

    m_params.update(m_tempo);
    if (m_tempo.getIsPlaying())
        m_delayEngine.updateSyncedModulators(m_tempo.getPpqPosition());
    m_delayEngine.setDelayMode(m_params.delayMode());
    m_delayEngine.setInsertEffect(m_params.insertEffectType());

    auto mainInput = getBusBuffer(buffer, true, 0);
    const float* inputDataL = mainInput.getReadPointer(0);
    const float* inputDataR = mainInput.getReadPointer(1);
    auto mainOutput = getBusBuffer(buffer, false, 0);
    float* outputDataL = mainOutput.getWritePointer(0);
    float* outputDataR = mainOutput.getWritePointer(1);
    
    float maxL = 0.0f, maxR = 0.0f;
    
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float outL = 0.0f, outR = 0.0f;
        m_params.smoothen();
        m_delayEngine.update(m_params);
		m_delayEngine.processSample(inputDataL[sample], inputDataR[sample],
			outL, outR);
        outputDataL[sample] = outL;
        outputDataR[sample] = outR;

        maxL = std::max(maxL, std::abs(outL));
        maxR = std::max(maxR, std::abs(outR));
    }

    m_outputLevelL.store(maxL);
    m_outputLevelR.store(maxR);

#if JUCE_DEBUG
	protectYourEars(buffer);
#endif
}

//==============================================================================
bool ViiveAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ViiveAudioProcessor::createEditor()
{
    return new ViiveAudioProcessorEditor (*this);
}

//==============================================================================
void ViiveAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ViiveAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ViiveAudioProcessor();
}
