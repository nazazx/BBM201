#include "Leaderboard.h"

#include <iostream>

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    // TODO: Your code here
    fstream file(filename);
    if (!file.is_open()) {
        return;
    }
    string line;
    while (getline(file,line)) {
        unsigned long score;
        time_t time;
        string name;
        stringstream ss(line);
        ss>>score>>time;
        getline(ss >> ws, name);
        LeaderboardEntry *entry=new LeaderboardEntry(score,time,name);
        insert(entry);
    }
    file.close();
}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    // TODO: Your code here
    fstream file(filename,ios::out);
    if (!file.is_open()) {
      return;
    }
    LeaderboardEntry *current=head_leaderboard_entry;
    while (current) {
        file<<current->score<<" "<<current->last_played<<" "<<current->player_name<<endl;
        current=current->next;
    }
    file.close();
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    // TODO: Your code here
    cout << "Leaderboard" << endl;
    cout << "-----------" << endl;

    LeaderboardEntry *current = head_leaderboard_entry;
    int n=1;
    while (current && n<=MAX_LEADERBOARD_SIZE) {
        char buffer[25];
        strftime(buffer, sizeof(buffer), "%H:%M:%S/%d.%m.%Y", localtime(&current->last_played));
        cout<<n<<". "<<current->player_name<<" "<<current->score<<" "<<buffer<<endl;
        current = current->next;
        n++;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    if (head_leaderboard_entry == nullptr || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry *curr = head_leaderboard_entry;
        while (curr->next != nullptr && curr->next->score >= new_entry->score) {
            curr = curr->next;
        }
        new_entry->next = curr->next;
        curr->next = new_entry;
    }

    LeaderboardEntry *curr = head_leaderboard_entry;
    int count = 1;
    while (curr && curr->next) {
        if (count == MAX_LEADERBOARD_SIZE) {
            LeaderboardEntry *del = curr->next;
            curr->next = nullptr;
            while (del != nullptr) {
                LeaderboardEntry *next = del->next;
                delete del;
                del = next;
            }
            break;
        }
        count++;
        curr = curr->next;
    }
}



// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    // TODO: Your code here
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        LeaderboardEntry *next = current->next;
        delete current;
        current = next;
    }

}
