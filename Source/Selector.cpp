#include "Selector.h"

Selector::Selector(const juce::String& labelText,
                   juce::AudioProcessorValueTreeState& apvts,
                   const juce::ParameterID& parameterID,
                   const juce::StringArray& items,
                   int width,
                   float fontSize)
    : m_hasLabel(labelText.isNotEmpty())
{
    if (fontSize > 0.0f)
    {
        m_laf.fontSize = fontSize;
        m_comboBox.setLookAndFeel(&m_laf);
    }

    m_comboBox.addItemList(items, 1);
    m_comboBox.setEditableText(false);
    addAndMakeVisible(m_comboBox);

    m_attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, parameterID.getParamID(), m_comboBox);

    if (m_hasLabel)
    {
        m_label.setText(labelText, juce::NotificationType::dontSendNotification);
        m_label.setFont(juce::Font(juce::FontOptions().withHeight(13.0f)));
        m_label.setJustificationType(juce::Justification::horizontallyCentred);
        m_label.setBorderSize(juce::BorderSize<int>(0));
        addAndMakeVisible(m_label);
    }

    setSize(width, (m_hasLabel ? s_labelH : 0) + s_comboH);
}

Selector::~Selector()
{
    m_comboBox.setLookAndFeel(nullptr);
}

void Selector::resized()
{
    const int comboY = m_hasLabel ? s_labelH : 0;
    if (m_hasLabel)
        m_label.setBounds(0, 0, getWidth(), s_labelH);
    m_comboBox.setBounds(0, comboY, getWidth(), s_comboH);
}
