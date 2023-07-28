#pragma once

#include <JuceHeader.h>

//This class is focused on getting a waveform from the AudioThumbnailCache
class WaveformDisplay  : public juce::Component, public juce::ChangeListener{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    /** Draws the components to the screen.
     * A juce graphics object.
     */
    void paint(juce::Graphics&) override;
    
    /** When the window is resized this method is called */
    void resized() override;
    
    /** Calls repaint when it recieves a broadcast.
     *Input: A pointer to a juce Change Broadcaster.
     */
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    
    /** Set the audio thumbnail source to the file being loaded in.
     *Input: A juce file URL, for the track to be loaded in.
     */
    void loadURL(juce::URL audioURL);
    
    /** Set the relative position of the playhead.
     *Input: A position in the track represented as a double.
     */
    void setPositionRelative(double position);

private:
    juce::AudioThumbnail audioThumbnail;
    
    bool fileLoaded;
    double waveformPosition;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
