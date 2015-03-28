
#include <iostream>
#include <fstream>
#include <sstream>

#include "Dialog.hpp"

Dialog::Dialog(const std::string& name)
: m_state(true)
{
    std::ifstream f(name + extension);
    if (!f) {std::cout << "File not found..." << std::endl; m_state = false; return;}
    /////////////////////////////////////////
    ///  PARSE AND LOAD FILE INTO MEMORY  ///
    /////////////////////////////////////////

    std::size_t size;
    f >> size;
    index_list.resize(size); // note: string list is resized later down

    /** First load into vector of direction numbers
     * Puts each line into a stringstream,
     * then parses each whitespace-delimited token
     * into the array of arrays of ints representing each index
     */
    std::size_t i = 0;
    std::string line;
    while (getline(f, line))
    {
        if (line.size() == 0) continue;

        std::stringstream ss_line(line);
        std::size_t phrase_index;
        std::size_t reply_index;

        ss_line >> phrase_index;
        while (ss_line >> reply_index)
        {
            index_list[phrase_index].push_back(reply_index);
        }
        i++;
        if (i == index_list.size())
            break;
    }

    // Test completion:
    for (auto& v : index_list)
    {
        for (auto& n : v)
        {
            std::cout << n << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;


    /// Then load the actual strings
    strings.resize(size);

    std::size_t phrase_index;
    while (f >> phrase_index)
    {
        // Ineffecient? Ask forum for better solution.

        std::string phrase;

        // TODO: \" escaping
        //  += '\'
        //  if (next char is ") -> string[size-1] = "
        //  else += char

        // Find first "
        while(true)
        {
            char c = f.get();
            if (c == '\"') break;
        }

        // Added until next " found: TODO: \" escape
        while(true)
        {
            char c = f.get();
            if (c == '\"') break;
            phrase += c;
        }

        strings[phrase_index] = phrase;
    }

    // Test: Display the strings in index order
    for (auto& s : strings)
    {
        std::cout << s << std::endl;
    }
}

void Dialog::run() const
{
    /////////////////////////
    ///  START DIALOG     ///
    ///  THROUGH CONSOLE  ///
    /////////////////////////
    std::cout << "\n\n";

    std::size_t curr_phrase_index = 0;

    // TODO: [Dead end] if dialog runs into a dead end... (Q: How could this happen?)
    //    - Make this error be shown when loading file as opposed to running it?

    do // while (currrent phrase indexx is not a sentinel)
    {
        // Display NPC's dialog:
        std::cout << "NPC: " << strings[curr_phrase_index] << std::endl;
        std::cout << std::endl;

        // Display Player's options:
        for (int i = 0; i < index_list[curr_phrase_index].size(); i++)
        {
                                    /* accesses each available option for player */
            std::cout << i << ". " << strings[ index_list[curr_phrase_index][i] ] << std::endl;

            //TODO: Change to base-1 instead of base-0 do subtraction: [option-1]
        }

        std::size_t option;
        std::cout << "\n>";
        std::cin >> option;

        // Determine which phrase NPC should say next based on option index,
        // by finding what the current phrase's option then points to.
        // Inner index is always [0] becaues response can't lead to more than one NPC reply:

        std::size_t next_str_index = index_list[  index_list[curr_phrase_index][option]  ][0];
        curr_phrase_index = next_str_index;

    } while (curr_phrase_index != dialog::end_sentinel    &&
             curr_phrase_index != dialog::combat_sentinel &&
             curr_phrase_index != dialog::barter_sentinel );

    // Display the reason the dialogue ended:
    std::cout << sentinel_phrases[curr_phrase_index] << std::endl;

}


void console_run(const Dialog& dialog)
{
    // alternative to dialog.run()
}

#ifndef CODEBLOCKS
/**
 * Run dialog file through console prompting.
 * Keep this file uncoupled from Dialog Editor SFML program
 */
int main()
{
    std::string filename;
    std::cout << "Enter dialog name, w/o extension:\n>";
    std::cin >> filename;

    Dialog dialog(filename);

    if (dialog.good())
        dialog.run();

    return 0;
}

#endif // NOT DEFINED CODEBLOCKS

