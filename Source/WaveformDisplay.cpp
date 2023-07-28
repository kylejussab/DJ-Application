#include <JuceHeader.h>
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse) : audioThumbnail(1000, formatManagerToUse, cacheToUse), fileLoaded(false), waveformPosition(0){
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay(){
    
}

void WaveformDisplay::paint(juce::Graphics& g){
    g.fillAll(juce::Colours::black.brighter(0.1));
    g.setColour (juce::Colours::teal);
    
    if(fileLoaded){
        audioThumbnail.drawChannel(g, getLocalBounds(), 0, audioThumbnail.getTotalLength(), 0, 1.0);
        g.setColour(juce::Colours::white);
        g.drawRect(waveformPosition * getWidth(), 0, 2, getHeight());
    }
    else{
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(), juce::Justification::centred, true);
    }
}

void WaveformDisplay::resized(){
    //Nothing needed here :)
}

void WaveformDisplay::loadURL(juce::URL audioURL){
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster *source){
    repaint();
}

void WaveformDisplay::setPositionRelative(double position){
    if(position != waveformPosition){
        waveformPosition = position;
        repaint();
    }
}
