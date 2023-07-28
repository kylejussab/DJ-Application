#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//Each component responsible for playback is within this class, strictly UI, calls on other functions from DJAudioPlayer for actual functionality
class DeckGUI  : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener, public juce::FileDragAndDropTarget, public juce::Timer, public juce::ChangeListener{
public:
    DeckGUI(DJAudioPlayer* player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    /** Draws the components to the screen.
     * A juce graphics object.
     */
    void paint(juce::Graphics&) override;
    
    /** When the window is resized this method is called */
    void resized() override;
    
    /** Implements the button listener.
     *Input: A juce button pointer.
     */
    void buttonClicked(juce::Button *button) override;
    
    /** Implements the slider listener.
     *Input: A juce slider pointer.
     */
    void sliderValueChanged(juce::Slider *slider) override;
    
    /** Callback to check whether this target is interested in the set of files being offered.
     *Input: A reference to a Juce stringArray. In this case it would be all the files you're trying to drag.
     *Output: A boolean representing whether or not these items are over the component.
     */
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    
    /** When the user drops the files, the first track gets loaded to the player.
     *Inputs: A reference to a Juce stringArray, in this case it would be all the files you're trying to drag, an integer x, and an integer y.
     */
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    
    /** Sets the relative position for the waveform */
    void timerCallback() override;
    
    /** Takes a path, track name, track artist, and track duration to load a song from the playlist component into a deck.
     *Inputs: All inputs are Juce strings, and are for the track, the inputs are as follows: the path, the title, the artist, and the duration.
     */
    void loadFromPlaylist(juce::String path, juce::String title, juce::String artist, juce::String duration);
    
    /** Checks the state of the audio, if a track is playing it calls the repaint method.
     *Input: A pointer to a juce Change Broadcaster.
     */
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
private:
    DJAudioPlayer* player;
    
    juce::Label trackTitleLabel;
    juce::Label trackDurationLabel;
    WaveformDisplay waveformDisplay;
    juce::Slider positionSlider;
    
    juce::TextButton playButton{"Play"};
    juce::TextButton pauseButton{"Pause"};
    juce::TextButton stopButton{"Stop"};
    juce::TextButton setCueButton{"Set Cue"};
    juce::TextButton playFromCueButton{"Play from Cue"};
    double cueTime;
    
    juce::Slider volumeSlider;
    juce::Label volumeLabel;
    
    juce::Slider speedSlider;
    juce::Label speedLabel;
    
    juce::Slider lowSlider;
    juce::Label lowLabel;
    
    juce::Slider highSlider;
    juce::Label highLabel;
    
    AudioPlayerState& state;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
