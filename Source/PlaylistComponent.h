#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <fstream>
#include "DeckGUI.h"

//Keeps a list of tracks that can be searched for, and added to a specific deck
class PlaylistComponent  : public juce::Component, public juce::TableListBoxModel, public juce::Button::Listener{
public:
    PlaylistComponent(DeckGUI* deckGUI1, DeckGUI* deckGUI2);
    ~PlaylistComponent() override;

    /** Draws the components to the screen.
     * A juce graphics object.
     */
    void paint(juce::Graphics&) override;
    
    /** When the window is resized this method is called */
    void resized() override;
    
    /** Get's the number of rows in the table list box model.
     *Output: An integer that quantifies how many rows are in the table.
     */
    int getNumRows() override;
    
    /** Paints the colour of the row, when and when not selected
     *Inputs: A juce graphics object, an integer row number, an integer for the width, an integer for the height, and a boolean for if the row is selected.
     */
    void paintRowBackground(juce::Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    
    /** Displays all the data within the table, from track name to track duration EXCEPT the buttons.
     *Inputs: A juce graphics object, an integer row number, an integer for the column ID, an integer for the width, an integer for the height, and a boolean for if the row is selected.
     */
    void paintCell(juce::Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    
    /** Displays the buttons for each row
     *Inputs: An integer for the row number, an integer for the column ID, a boolean for if the row is selected, and a juce component to update.
     *Output: A juce component, in this case it's the button.
     */
    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component *existingComponentToUpdate) override;
    
    /** Implements the button listener.
     *Input: A juce button pointer.
     */
    void buttonClicked(juce::Button* button) override;

    /** Method for when the clicked button is in the row of the table list box.
     *Input: A juce button pointer.
     */
    void addRemoveButtonClicked(juce::Button* button);
    
    /** Load a track to playlist when this button is clicked.
     *Input: A juce button pointer.
     */
    void loadTrackButtonClicked(juce::Button* button);
    
    /** Searches the tracks in the playlist component for what was in the search bar when the button is clicked.
     *Input: A juce button pointer.
     */
    void searchButtonClicked(juce::Button* button);
    
    /** Clears the content the search bar and resets the rows when the button is clicked.
     *Input: A juce button pointer.
     */
    void clearButtonClicked(juce::Button* button);
    
    /** Takes the string ID from a button, tokenises the string to get which track to add, and which deck to add it to.
     *Input: A juce button pointer.
     *Output: A vector of strings, with two elements. 1st element is the row index. 2nd element is what the button is (ie. Right Deck, Left Deck, Remove)*
     */
    std::vector<std::string> tokeniseButtonID(juce::Button* button);
    
    /** Given a row number, the name of the track will be returned.
     *Input: An integer for the row number.
     *Output: A juce string of the track name.
     */
    juce::String getTrackName(int rowNumber);
    
    /** Given a row number, the name of the artist will be returned.
     *Input: An integer for the row number.
     *Output: A juce string of the artist name.
     */
    juce::String getArtistName(int rowNumber);
    
    /** Given a row number, the duration of the track will be returned.
     *Input: An integer for the row number.
     *Output: A juce string of the track duration.
     */
    juce::String getTrackDuration(int rowNumber);
    
    /** Given a row number, the extension of the track will be returned.
     *Input: An integer for the row number.
     *Output: A juce string of the track extension.
     */
    juce::String getTrackExtension(int rowNumber);
    
    /** Gets and returns a vector of all the files currently in the playlist.
     *Output: A vector of juce strings, with all the track names.
     */
    std::vector<juce::String> getAllFileNames();
    
    /** Takes all the tracks currently in the playlist component and writes them to a txt to be saved */
    void writeToPlaylistFile();
    
    /** Reads and adds all the tracks in the txt file to the playlist component */
    void readFromPlaylistFile();
    
    /** Removes a track from the txt file
     *Input: An integer for the line number in the txt file.
     */
    void removeLineFromPlaylistFile(int lineToDelete);

private:
    DeckGUI* leftDeck;
    DeckGUI* rightDeck;
    
    juce::TableListBox tableComponent;
    std::vector<juce::String> tracks;
    
    juce::FileChooser fileChooser{"Select a file..."};
    
    juce::TextEditor searchBox;
    int selectedIndex = 0;
    juce::TextButton searchButton {"Search"};
    juce::TextButton clearButton {"Clear"};
    
    juce::TextButton loadTrack {"Load track"};
    
    std::ofstream playlistFile;
    std::ifstream playlistFileRead;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
