#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <iostream>

PlaylistComponent::PlaylistComponent(DeckGUI* deckGUI1, DeckGUI* deckGUI2) : leftDeck(deckGUI1), rightDeck(deckGUI2){
    addAndMakeVisible(searchBox);
    searchBox.setTextToShowWhenEmpty("Search for a track or artist...", juce::Colours::white.withAlpha(0.5f));
    searchBox.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black.brighter(0.1));
    searchBox.setColour(juce::TextEditor::outlineColourId, juce::Colours::white.withAlpha(0.5f));
    
    addAndMakeVisible(searchButton);
    searchButton.addListener(this);
    searchButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black.brighter(0.1));
    
    addAndMakeVisible(clearButton);
    clearButton.addListener(this);
    clearButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black.brighter(0.1));
    
    addAndMakeVisible(loadTrack);
    loadTrack.addListener(this);
    loadTrack.setColour(juce::TextButton::buttonColourId, juce::Colours::black.brighter(0.1));
    
    int columnWidth = 200;
    
    addAndMakeVisible(tableComponent);
    tableComponent.setHeaderHeight(25);
    tableComponent.getHeader().addColumn("#", 1, columnWidth/5);
    tableComponent.getHeader().addColumn("Name", 2, columnWidth/1.5);
    tableComponent.getHeader().addColumn("Artist", 3, columnWidth/1.5);
    tableComponent.getHeader().addColumn("Duration", 4, columnWidth/2);
    tableComponent.getHeader().addColumn("Ext", 5, columnWidth/4);
    tableComponent.getHeader().addColumn("Left Deck", 6, columnWidth/2);
    tableComponent.getHeader().addColumn("Right Deck", 7, columnWidth/2);
    tableComponent.getHeader().addColumn("", 8, columnWidth/2);
    tableComponent.setModel(this);
    tableComponent.setColour(juce::TableListBox::backgroundColourId, juce::Colours::black.brighter(0.1));
    tableComponent.getHeader().setColour(juce::TableHeaderComponent::textColourId, juce::Colours::white);
    tableComponent.getHeader().setColour(juce::TableHeaderComponent::backgroundColourId, juce::Colours::black.brighter(0.1));
    tableComponent.getHeader().setColour(juce::TableHeaderComponent::outlineColourId, juce::Colours::white.withAlpha(0.5f));
    
    readFromPlaylistFile();
}

PlaylistComponent::~PlaylistComponent(){
    
}

void PlaylistComponent::paint(juce::Graphics& g){
    g.fillAll(juce::Colours::black.brighter(0.1));
    g.setColour(juce::Colours::white);
    g.setFont (14.0f);
}

void PlaylistComponent::resized(){
    int padding = 5;
    int buttonWidth = 100;
    
    searchBox.setBounds(0 + padding, 10 + padding, getWidth() - (buttonWidth * 3) - (padding * 2), 30 - padding);
    searchButton.setBounds(getWidth() - buttonWidth * 3 + padding, 10 + padding, buttonWidth - (padding * 2), 30 - padding);
    clearButton.setBounds(getWidth() - buttonWidth * 2 + padding, 10 + padding, buttonWidth - (padding * 2), 30 - padding);
    loadTrack.setBounds(getWidth() - buttonWidth + padding, 10 + padding, 100 - (padding * 2), 30 - padding);
    
    tableComponent.setBounds(0 + padding, 40 + (padding * 2), getWidth() - (padding * 2), getHeight() - 40 - (padding * 3));
}

int PlaylistComponent::getNumRows(){
    return tracks.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics & g, int rowNumber, int width, int height, bool rowIsSelected){
    if(rowIsSelected){
        g.setColour(juce::Colours::teal);
        g.drawRoundedRectangle(2, 2, getWidth() - 2, 18, 5, 2);
    }
    else{
        g.setColour(juce::Colours::black.brighter(0.1));
    }
}

void PlaylistComponent::paintCell(juce::Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected){
    g.setColour (juce::Colours::white);
    if(rowNumber < getNumRows()){
        if(columnId == 1){
            g.drawText(juce::String(rowNumber + 1), 2, 0, width - 4, height, juce::Justification::centred, true);
        }
        
        if(columnId == 2){
            g.drawText(getTrackName(rowNumber), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        }
        
        if(columnId == 3){
            g.drawText(getArtistName(rowNumber), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        }
        
        if(columnId == 4){
            g.drawText(getTrackDuration(rowNumber), 2, 1, width - 4, height, juce::Justification::centred, true);
        }
        if(columnId == 5){
            g.drawText(getTrackExtension(rowNumber), 2, 1, width - 4, height, juce::Justification::centredLeft, true);
        }
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component *existingComponentToUpdate){
    //Button to add to left deck
    if(columnId == 6){
        if(existingComponentToUpdate == nullptr){
            juce::TextButton* button = new juce::TextButton{"Add"};
            button->setColour(juce::TextButton::buttonColourId, juce::Colours::black.brighter(0.1));
            
            juce::String id{std::to_string(rowNumber)};
            button->setComponentID(id + ",L");

            button->addListener(this);
            existingComponentToUpdate = button;
        }
    }
    
    //Button to add to right deck
    if(columnId == 7){
        if(existingComponentToUpdate == nullptr){
            juce::TextButton* button = new juce::TextButton{"Add"};
            button->setColour(juce::TextButton::buttonColourId, juce::Colours::black.brighter(0.1));
            
            juce::String id{std::to_string(rowNumber)};
            button->setComponentID(id + ",R");
            
            button->addListener(this);
            existingComponentToUpdate = button;
        }
    }
    
    //Button to remove track from playlist component
    if(columnId == 8){
        if(existingComponentToUpdate == nullptr){
            juce::TextButton* button = new juce::TextButton{"Remove"};
            button->setColour(juce::TextButton::buttonColourId, juce::Colours::black.brighter(0.1));
            
            juce::String id{std::to_string(rowNumber)};
            button->setComponentID(id + ",Remove");
            
            button->addListener(this);
            existingComponentToUpdate = button;
        }
    }
    
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button){
    addRemoveButtonClicked(button);

    loadTrackButtonClicked(button);
    
    searchButtonClicked(button);
    
    clearButtonClicked(button);
}

void PlaylistComponent::addRemoveButtonClicked(juce::Button* button){
    if(tracks.size() != 0 && button != &loadTrack){
        //Add track to the left deck
        if(tokeniseButtonID(button)[1] == "L"){
            leftDeck->loadFromPlaylist(tracks[std::stoi(tokeniseButtonID(button)[0])], getTrackName(std::stoi(tokeniseButtonID(button)[0])), getArtistName(std::stoi(tokeniseButtonID(button)[0])), getTrackDuration(std::stoi(tokeniseButtonID(button)[0])));
        }
        
        //Add track to the right deck
        if(tokeniseButtonID(button)[1] == "R"){
            rightDeck->loadFromPlaylist(tracks[std::stoi(tokeniseButtonID(button)[0])], getTrackName(std::stoi(tokeniseButtonID(button)[0])), getArtistName(std::stoi(tokeniseButtonID(button)[0])), getTrackDuration(std::stoi(tokeniseButtonID(button)[0])));
        }
        
        if(tokeniseButtonID(button)[1] == "Remove"){
            tracks.erase(tracks.begin() + std::stoi(tokeniseButtonID(button)[0]));
            removeLineFromPlaylistFile(std::stoi(tokeniseButtonID(button)[0]));
            tableComponent.updateContent();
        }
    }
}

void PlaylistComponent::loadTrackButtonClicked(juce::Button* button){
    if(button == &loadTrack){
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;

        fileChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser){
            juce::File chosenFile = chooser.getResult();
            if(chosenFile.existsAsFile()){
                juce::String filePath = chosenFile.getFullPathName();
                tracks.push_back(filePath);
                tableComponent.updateContent();
                writeToPlaylistFile();
            };
        });
    }
}

void PlaylistComponent::searchButtonClicked(juce::Button* button){
    if(button == &searchButton){
        std::vector<int> tracksFound;
        std::vector<std::string> fileNames;
        std::string searchText = searchBox.getText().toStdString();
        
        for(int i = 0; i < tracks.size(); ++i){
            fileNames.push_back(getAllFileNames()[i].toStdString());
        }
        
        //Convert all file names to lowercase
        for(int i = 0; i < fileNames.size(); ++i){
            std::for_each(fileNames[i].begin(), fileNames[i].end(), [](char& ch){
                ch = tolower(ch);
            });
        }
        
        //Convert text from search bar to lowercase
        std::for_each(searchText.begin(), searchText.end(), [](char& ch){
            ch = tolower(ch);
        });
        
        if(searchBox.isEmpty() == false){
            for(int i = 0; i < tracks.size(); ++i){
                if(fileNames[i].find(searchText) != std::string::npos){
                    tracksFound.push_back(i);
                }
            }
            
            if(tracksFound.size() != 0){
                tableComponent.selectRow(tracksFound[selectedIndex]);
                selectedIndex ++;
                if(selectedIndex == tracksFound.size()){
                    selectedIndex = 0;
                }
            }
        }
        else{
            tableComponent.deselectAllRows();
            selectedIndex = 0;
        }
    }
}

void PlaylistComponent::clearButtonClicked(juce::Button* button){
    if(button == &clearButton){
        tableComponent.deselectAllRows();
        searchBox.clear();
    }
}

std::vector<std::string> PlaylistComponent::tokeniseButtonID(juce::Button* button){
    juce::String token = button->getComponentID();

    std::vector<std::string> result;
    
    std::stringstream s_stream(token.toStdString());
    while(s_stream.good()){
        std::string substring;
        std::getline(s_stream, substring, ',');
        result.push_back(substring);
    }
    
    return result;
}

juce::String PlaylistComponent::getTrackName(int rowNumber){
    juce::File trackFile = juce::File(tracks[rowNumber]);
    juce::String trackName = trackFile.getFileNameWithoutExtension();
    
    std::vector<std::string> result;
    
    std::stringstream s_stream(trackName.toStdString());
    while(s_stream.good()){
        std::string substring;
        std::getline(s_stream, substring, '-');
        result.push_back(substring);
    }
    
    if(result.size() == 2){
        return result[1];
    }
    else{
        return result[0];
    }
}

juce::String PlaylistComponent::getArtistName(int rowNumber){
    juce::File trackFile = juce::File(tracks[rowNumber]);
    juce::String trackName = trackFile.getFileNameWithoutExtension();
    
    std::vector<std::string> result;
    
    std::stringstream s_stream(trackName.toStdString());
    while(s_stream.good()){
        std::string substring;
        std::getline(s_stream, substring, '-');
        result.push_back(substring);
    }
    
    if(result.size() == 1){
        return juce::String("N/A");
    }
    else{
        return result[0];
    }
}

juce::String PlaylistComponent::getTrackDuration(int rowNumber){
    juce::File trackFile = juce::File(tracks[rowNumber]);
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(trackFile));
    
    juce::String duration;
    
    if (reader != nullptr){
        int seconds = reader->lengthInSamples / reader->sampleRate;
        int minutes = seconds / 60;
        
        juce::String durationSeconds = juce::String(seconds % 60);
        juce::String durationMinutes = juce::String(minutes);
        
        if((seconds % 60) < 10){
            durationSeconds = "0" + durationSeconds;
        }
        
        if((minutes % 60) < 10){
            durationMinutes = "0" + durationMinutes;
        }
    
        duration = durationMinutes + ":" + durationSeconds;
    }
    return duration;
}

juce::String PlaylistComponent::getTrackExtension(int rowNumber){
    juce::File trackFile = juce::File(tracks[rowNumber]);
    juce::String trackExtension = trackFile.getFileExtension();
    
    return trackExtension;
}

std::vector<juce::String> PlaylistComponent::getAllFileNames(){
    std::vector<juce::String> allFileNames;
    
    for(int i = 0; i < tracks.size(); ++i){
        juce::File trackFile = juce::File(tracks[i]);
        juce::String trackName = trackFile.getFileNameWithoutExtension();
        allFileNames.push_back(trackName);
    }
    return allFileNames;
}

void PlaylistComponent::writeToPlaylistFile(){
    auto playlistFilePath = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("Playlist.txt");
    playlistFile.open(playlistFilePath.getFullPathName().toStdString());

    if(playlistFile.is_open()){
        for(const auto& track : tracks){
            playlistFile << track << std::endl;
            std::cout << "Wrote to file" << std::endl;
        }
    }
    playlistFile.close();
}

void PlaylistComponent::readFromPlaylistFile(){
    auto playlistFilePath = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("Playlist.txt");
    playlistFileRead.open(playlistFilePath.getFullPathName().toStdString());
    std::string title;

    if(playlistFileRead.is_open()){
        while(std::getline(playlistFileRead, title)){
            tracks.push_back(title);
        }
        playlistFileRead.close();
    }
}

void PlaylistComponent::removeLineFromPlaylistFile(int lineToDelete){
    auto playlistFilePath = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("Playlist.txt");
    std::string fileName = playlistFilePath.getFullPathName().toStdString();
    
    std::vector<std::string> lines;
    std::string line;

    std::fstream readFile;
    readFile.open(fileName);
    
    while(getline(readFile, line)){
        lines.push_back(line);
    }
    readFile.close();
    
    std::ofstream writeFile;
    writeFile.open(fileName);
    
    for(int i = 0; i < lines.size(); ++i){
        if(i != lineToDelete){
            writeFile << lines[i] << std::endl;
        }
    }
    writeFile.close();
}
