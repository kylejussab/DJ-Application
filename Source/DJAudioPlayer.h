#pragma once

#include <JuceHeader.h>
#include "AudioPlayerState.h"

//Responisble for the audio processing
class DJAudioPlayer : public juce::AudioSource{
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
    
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
    
    /** Adds the track to the transport source to be played.
     *Input: A juce file URL, for the track to be loaded in.
     */
    void loadURL(juce::URL audioURL);
    
    /** Sets the volume of the track.
     *Input: A double for the volume.
     */
    void setGain(double gain);
    
    /** Sets the speed of the track.
     *Input: A double for the speed increase/decrease.
     */
    void setSpeed(double ratio);
    
    /** Sets the actual position in the track.
     *Input: A double for the actual position of the track in seconds.
     */
    void setPosition(double positionInSeconds);
    
    /** Sets the relative position of the playhead (used for the rectangle that goes over the waveform).
     *Input: A double for the relative position of the track in seconds (Between 0 and 1).
     */
    void setPositionRelative(double position);
    
    /** Gets the actual position of the playhead in the track.
     *Output: A double of the actual position of the track in seconds.
     */
    double getPosition();
    
    /** Get the relative position of the playhead.
     *Output: A double of the relative position of the track in seconds.
     */
    double getPositionRelative();
    
    /** Starts audio playback */
    void start();
    
    /** Stops audio playback */
    void stop();
    
    /** Sets the maximum frequency at which the low filter can start to amplify the lower frequencies.
     *Inputs: A double for the frequency to stop at, a double for the curve (between 0 and 1), and a float for how much to amplify the frequency (anything above 1 amplifies).
     */
    void setLowShelf(double cutOffFrequency, double Q, float gainFactor);
    
    /** Sets the minimum frequency at which the high filter can start to amplify the high frequencies.
     *Inputs: A double for the frequency to stop at, a double for the curve (between 0 and 1), and a float for how much to amplify the frequency (anything above 1 amplifies).
     */
    void setHighShelf(double cutOffFrequency, double Q, float gainFactor);
    
    /** Returns the state object to check if the track is playing.
     *Output: The state object.
     */
    AudioPlayerState& getState() {return state;}

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    
    juce::AudioTransportSource transportSource;
    
    juce::IIRFilterAudioSource lowFilterSource {&transportSource, false};
    juce::IIRFilterAudioSource highFilterSource {&lowFilterSource, false};
    
    juce::ResamplingAudioSource resampleSource{&highFilterSource, false, 2};
    
    AudioPlayerState state;
};
