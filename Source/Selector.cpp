#include "Selector.h"

Selector::Selector(const juce::String& labelText,
                   juce::AudioProcessorValueTreeState& apvts,
                   const juce::ParameterID& parameterID,
                   const juce::StringArray& items,
                   int width,
                   float fontSize,
                   int leftLabelWidth,
                   int height)
	: m_hasLabel(labelText.isNotEmpty()), m_leftLabelWidth(leftLabelWidth)
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
		addAndMakeVisible(m_label);
	}

	const int autoH = (m_hasLabel && m_leftLabelWidth == 0 ? s_labelH : 0) + s_comboH;
	setSize(width, height > 0 ? height : autoH);
}

Selector::~Selector()
{
	m_comboBox.setLookAndFeel(nullptr);
}

void Selector::resized()
{
	if (m_hasLabel && m_leftLabelWidth > 0)
	{
		m_label.setBounds(0, 0, m_leftLabelWidth, getHeight());
		m_comboBox.setBounds(m_leftLabelWidth, 0, getWidth() - m_leftLabelWidth, getHeight());
	}
	else
	{
		const int comboY = m_hasLabel ? s_labelH : 0;
		if (m_hasLabel)
			m_label.setBounds(0, 0, getWidth(), s_labelH);
		m_comboBox.setBounds(0, comboY, getWidth(), s_comboH);
	}
}
