#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    /*
    levelSlider.setRange(0.0, 0.25);
    levelSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    levelLabel.setText("Noise Level", juce::dontSendNotification);

    addAndMakeVisible(levelSlider);
    addAndMakeVisible(levelLabel);
    */

    //changed
    decibelSlider.setRange(-100, -12);
    decibelSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    decibelSlider.onValueChange = [this] { level = juce::Decibels::decibelsToGain((float)decibelSlider.getValue()); };
    decibelSlider.setValue(juce::Decibels::gainToDecibels(level));
    decibelLabel.setText("Noise Level in dB", juce::dontSendNotification);

    addAndMakeVisible(decibelSlider);
    addAndMakeVisible(decibelLabel);

    setSize(800, 600);
    setAudioChannels(0, 2); // no inputs, two outputs
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate){}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    //auto level = (float)levelSlider.getValue();
    //changed
    auto currentLevel = level;
    auto levelScale = currentLevel * 2.0f;

    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            /*auto noise = random.nextFloat() * 2.0f - 1.0f;
            buffer[sample] = noise * level;*/
            //changed
            buffer[sample] = random.nextFloat() * levelScale - currentLevel;
        }
    }
}

void MainComponent::releaseResources()
{
    juce::Logger::getCurrentLogger()->writeToLog("Releasing audio resources");
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    /*levelLabel.setBounds(10, 10, 90, 20);
    levelSlider.setBounds(100, 10, getWidth() - 110, 20);*/
    //changed
    decibelLabel.setBounds(10, 10, 120, 20);
    decibelSlider.setBounds(130, 10, getWidth() - 140, 20);
}

//added DecibelSlider
double DecibelSlider::getValueFromText(const juce::String& text)
{
    auto minusInfinitydB = -100.0;

    auto decibelText = text.upToFirstOccurrenceOf("dB", false, false).trim();    // [1]

    return decibelText.equalsIgnoreCase("-INF") ? minusInfinitydB
        : decibelText.getDoubleValue();  // [2]
}

juce::String DecibelSlider::getTextFromValue(double value)
{
    return juce::Decibels::toString(value);
}