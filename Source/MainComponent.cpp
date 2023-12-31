#include "MainComponent.h"

MainComponent::MainComponent(){
    setSize (1000, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio)){
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else{
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }
    
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    
    addAndMakeVisible(playlistComponent);
    
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent(){
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate){
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill){
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources(){
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

void MainComponent::paint(juce::Graphics& g){
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized(){
    deckGUI1.setBounds(0, 0, getWidth()/2, getHeight() * 0.625);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, getHeight() * 0.625);
    playlistComponent.setBounds(0, getHeight() * 0.625, getWidth(), getHeight() * 0.375);
}
