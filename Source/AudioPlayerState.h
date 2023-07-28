#pragma once

#include <JuceHeader.h>

//Set by the DJAudioPlayer, observed by the DeckGUI, maintains playing state in the application
struct AudioPlayerState : public juce::ChangeBroadcaster
{
    bool isPlaying = false;
    
    /** Sets the isPlaying boolean to the input it recieves.
     *Input: A boolean.
     */
    void setPlaying(bool playing){
        isPlaying = playing;
        sendChangeMessage();
    }
};
