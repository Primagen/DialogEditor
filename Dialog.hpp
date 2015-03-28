
#include <iostream>
#include <string>
#include <vector>


// TODO:
//     Consider the Player's replies have a system of nodes, ex:
//       Reply Node 1 gives options {2, 3, 4}

// Should automatically re-adjust index values in order of presentation, no blank indices
// First NPC phrase : index N
// 1st reply option : index N + 1
// 2nd reply option : index N + 2

// 1st reply's response: index (N + # reponse options) + 0
// 2nd reply's reponse: index (0 + #

// it's something of a graph data structure


struct Dialog_NPC_Data {
    std::string name;
    bool        sex;
    int         age;
};

struct Reply {
    // Vector of requirements?
    std::size_t requirement_skill;       // ex: 0 for Small Guns
    std::size_t requirement_skill_level; // ex: 105 for 105% needed
    std::size_t index;
};

const std::string extension = ".dlg";

const std::string sentinel_phrases[] = {
  "[Dialog End]",
  "[Attack]",
  "[Barter]"
};

namespace dialog{
  constexpr std::size_t end_sentinel    = 0;
  constexpr std::size_t barter_sentinel = 1;
  constexpr std::size_t combat_sentinel = 2;
}

struct Dialog {
    Dialog(const std::string& name);
    
    void run() const;

    Dialog_NPC_Data npc_data; // TODO: Implement
    std::vector<std::string> strings;
    std::vector<std::vector<std::size_t>> index_list;
    
    bool good() const {return m_state;}
    bool m_state;
    
    /*  index_list example:
    {
        [0]  {2, 3, 4} 
        [1]  {2, 3}
        
        [2]  {5}
        [3]  ...
        [4]  0
        [5]  0
    } */
};