#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent{
public:
    MainComponent();
    ~MainComponent() override;

    /** Called before playback starts, to let the processor prepare itself.
     *Inputs: An integer of the samples expected, and a double for the sample rate.
     */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    
    /** Called repeatedly to fetch subsequent blocks of audio data.
     *Input: A reference to a Juce AudioSourceChannel. In this case it's the buffer to fill.
     */
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    
    /** Allows the source to release anything it no longer needs after playback has stopped */
    void releaseResources() override;

    /** Draws the components to the screen.
     * A juce graphics object.
     */
    void paint(juce::Graphics& g) override;
    
    /** When the window is resized this method is called */
    void resized() override;

private:
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache{100};
    
    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1, formatManager, thumbnailCache};
    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{&player2, formatManager, thumbnailCache};
    
    juce::MixerAudioSource mixerSource;
    
    PlaylistComponent playlistComponent{&deckGUI1, &deckGUI2};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
