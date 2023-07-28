#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse) : player(_player), waveformDisplay(formatManagerToUse, cacheToUse), state(_player->getState()){
    addAndMakeVisible(trackTitleLabel);
    trackTitleLabel.setText("N/A", juce::dontSendNotification);
    addAndMakeVisible(trackDurationLabel);
    trackDurationLabel.setText("N/A", juce::dontSendNotification);
    trackDurationLabel.setJustificationType(juce::Justification::centredRight);
    
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(positionSlider);
    positionSlider.addListener(this);
    positionSlider.setRange(0.0, 1.0, 0.01);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 40, 20);
    positionSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::transparentWhite);
    positionSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::transparentWhite);
    positionSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::transparentWhite);
    
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(volumeLabel);
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centred);
    volumeSlider.addListener(this);
    volumeSlider.setRange(0, 100, 1);
    volumeSlider.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    volumeSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::teal);
    volumeSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::white.withAlpha(0.5f));
    volumeSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::teal);
    volumeSlider.setDoubleClickReturnValue(true, 50);
    volumeSlider.setValue(50);
    
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);
    speedSlider.addListener(this);
    speedSlider.setRange(0.1, 3.0, 0.01);
    speedSlider.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    speedSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::teal);
    speedSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::white.withAlpha(0.5f));
    speedSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::teal);
    speedSlider.setDoubleClickReturnValue(true, 1.0);
    speedSlider.setValue(1.0);
    
    addAndMakeVisible(lowSlider);
    addAndMakeVisible(lowLabel);
    lowLabel.setText("Low", juce::dontSendNotification);
    lowLabel.setJustificationType(juce::Justification::centred);
    lowSlider.addListener(this);
    lowSlider.setRange(0.1, 3, 0.1);
    lowSlider.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
    lowSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    lowSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::teal);
    lowSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::white.withAlpha(0.5f));
    lowSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::teal);
    
    addAndMakeVisible(highSlider);
    addAndMakeVisible(highLabel);
    highLabel.setText("High", juce::dontSendNotification);
    highLabel.setJustificationType(juce::Justification::centred);
    highSlider.addListener(this);
    highSlider.setRange(0.1, 3, 0.1);
    highSlider.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
    highSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    highSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::teal);
    highSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::white.withAlpha(0.5f));
    highSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::teal);
    
    addAndMakeVisible(playButton);
    playButton.addListener(this);
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black.brighter(0.1));
    
    addAndMakeVisible(pauseButton);
    pauseButton.addListener(this);
    pauseButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black.brighter(0.1));
    
    addAndMakeVisible(stopButton);
    stopButton.addListener(this);
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black.brighter(0.1));
    
    addAndMakeVisible(setCueButton);
    setCueButton.addListener(this);
    setCueButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black.brighter(0.1));
    cueTime = 0.0;
    
    addAndMakeVisible(playFromCueButton);
    playFromCueButton.addListener(this);
    playFromCueButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black.brighter(0.1));
    
    startTimer(250);
    state.addChangeListener(this);
}

DeckGUI::~DeckGUI(){
    stopTimer();
    state.removeChangeListener(this);
}

void DeckGUI::paint(juce::Graphics& g){
    g.fillAll(juce::Colours::black.brighter(0.1));
    g.setColour(juce::Colours::grey);
    
    //Draws the disc, the code for spinning the disc is calculated here, it spins according the the relative position from the player
    juce::Rectangle<float> discArea{(getHeight() / 3) * 2.0f + 20, 100.0f, 200.0f, 200.0f};
    juce::Rectangle<float> discCover{(getHeight() / 3) * 2.0f + 15, 95.0f, 210.0f, 210.0f};
    juce::Rectangle<float> discLabel{(getHeight() / 3) * 2.0f - 5, 95.0f, 20.0f, 210.0f};
    juce::Rectangle<float> discCoverOutline{(getHeight() / 3) * 2.0f - 5, 95.0f, 230.0f, 210.0f};
    auto discCenter = discArea.getCentre();
    
    g.drawEllipse(discArea, 5.0f);
    
    g.setColour(juce::Colours::teal);
    g.fillEllipse(discCenter.getX() - 15, discCenter.getY() - 15, 30, 30);

    float radius { discArea.getWidth() / 2.0f };
    
    if(isnan(player->getPositionRelative()) == false){
        g.drawLine(discCenter.getX(), discCenter.getY(), discCenter.getX() + radius * (float)std::sin (2.0 * juce::MathConstants<float>::pi * player->getPositionRelative() * 100),
                   discCenter.getY() - radius * (float)std::cos (2.0f * juce::MathConstants<float>::pi * player->getPositionRelative() * 100), 5.0f);
        
        g.setColour(juce::Colours::white.withAlpha(0.2f));
        g.fillRect(discCover);
        
        g.setColour(juce::Colours::black);
        g.fillRect(discLabel);
        
        g.setColour(juce::Colours::grey);
        g.drawRect(discCoverOutline, 2.0f);
        g.drawRect(discLabel, 2.0f);
        
        g.setColour(juce::Colours::white);
        g.setFont (12.0f);
    
        g.addTransform(juce::AffineTransform::rotation(-juce::MathConstants<float>::pi / 2.0f, discCenter.getX(), discCenter.getY()));
        g.drawText(trackTitleLabel.getText(), (getHeight() / 3) * 2.0f + 20, 75, 200, 20, juce::Justification::centredLeft, true);
        
    }
}

void DeckGUI::resized(){
    int padding = 20;
    
    trackTitleLabel.setBounds(padding/2, 0, getWidth()/2 - padding, 20);
    trackDurationLabel.setBounds(getWidth()/2 + padding/2, 0, getWidth()/2 - padding, 20);
    
    positionSlider.setBounds(0, 20, getWidth(), 50);
    waveformDisplay.setBounds(padding/2, 20, getWidth() - padding, 50);
    
    volumeSlider.setBounds(0, 70, getHeight() / 3, getHeight() / 3);
    volumeLabel.setBounds(getHeight() / 11, 70 + getHeight() / 8, getHeight() / 6.5, 20);
    
    speedSlider.setBounds(getHeight() / 3, 70, getHeight() / 3, getHeight() / 3);
    speedLabel.setBounds(getHeight() / 3 + getHeight() / 11, 70 + getHeight() / 8, getHeight() / 6.5, 20);
    
    lowSlider.setBounds(0, 70 + getHeight() / 3, getHeight() / 3, getHeight() / 3);
    lowLabel.setBounds(getHeight() / 11, 70 + getHeight() / 8 + getHeight() / 3, getHeight() / 6.5, 20);
    
    highSlider.setBounds(getHeight() / 3, 70 + getHeight() / 3, getHeight() / 3, getHeight() / 3);
    highLabel.setBounds(getHeight() / 3 + getHeight() / 11, 70 + getHeight() / 8 + getHeight() / 3, getHeight() / 6.5, 20);
    
    playButton.setBounds(padding/2, 80 + (getHeight() / 3) * 2, getWidth()/4 - padding/4, 50 - padding/2);
    pauseButton.setBounds(getWidth()/4 + padding/4, 80 + (getHeight() / 3) * 2, getWidth()/4 , 50 - padding/2);
    stopButton.setBounds((getWidth()/4) * 2 + padding/4, 80 + (getHeight() / 3) * 2, getWidth()/4 - padding/4, 50 - padding/2);
    
    setCueButton.setBounds((getWidth()/4) * 3, 80 + (getHeight() / 3) * 2, getWidth()/4 - padding/2, (50 - padding/2) / 2);
    playFromCueButton.setBounds((getWidth()/4) * 3, 80 + (getHeight() / 3) * 2 + ((50 - padding/2) / 2), getWidth()/4 - padding/2, (50 - padding/2) / 2);
}

void DeckGUI::buttonClicked(juce::Button* button){
    if(button == &playButton){
        player->start();
        repaint();
    }

    if(button == &pauseButton){
        player->stop();
    }
    
    if(button == &stopButton){
        player->stop();
        player->setPosition(0.0);
    }
    
    if(button == &setCueButton){
        cueTime = player->getPosition();
    }
    
    if(button == &playFromCueButton){
        player->setPosition(cueTime);
        player->start();
    }
}

void DeckGUI::sliderValueChanged(juce::Slider *slider){
    if(slider == &volumeSlider){
        player->setGain(slider->getValue()/100);
    }

    if(slider == &speedSlider){
        player->setSpeed(slider->getValue());
    }

    if(slider == &positionSlider){
        player->setPositionRelative(slider->getValue());
    }
    
    //Error is thrown when these sliders are changed before a track is loaded
    if(slider == &lowSlider && trackTitleLabel.getText() != "N/A"){
        player->setLowShelf(400, 0.5, slider->getValue());
    }
    
    if(slider == &highSlider && trackTitleLabel.getText() != "N/A"){
        player->setHighShelf(2500, 0.5, slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files){
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y){
    if(files.size() == 1){
        player->loadURL(juce::URL{juce::File{files[0]}});
    }
}

void DeckGUI::timerCallback(){
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

void DeckGUI::loadFromPlaylist(juce::String path, juce::String title, juce::String artist, juce::String duration){
    juce::File chosenFile = path;
    player->loadURL(juce::URL{chosenFile});
    waveformDisplay.loadURL(juce::URL{chosenFile});
    
    //Resets values for new track
    positionSlider.setValue(0.0);
    lowSlider.setValue(1.0);
    highSlider.setValue(1.0);
    lowSlider.setDoubleClickReturnValue(true, 1.0);
    highSlider.setDoubleClickReturnValue(true, 1.0);
    cueTime = 0.0;
    
    trackTitleLabel.setText(title + " - " + artist, juce::dontSendNotification);
    trackDurationLabel.setText(duration, juce::dontSendNotification);
}

void DeckGUI::changeListenerCallback(juce::ChangeBroadcaster* source){
    if(source == &state){
        repaint();
    }
}
