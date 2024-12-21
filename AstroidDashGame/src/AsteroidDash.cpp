#include "AsteroidDash.h"
#include "fstream"
#include "sstream"

// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    // TODO: Your code here

    fstream file(input_file,ios::in);
    if(!file.is_open()) {
        cout<<"error"<<endl;
        return;
    }

    string line;

    while (getline(file,line)) {
        stringstream ss(line);
        int number;
        vector<int> v;
        while (ss>>number) {
            v.push_back(number);
        }
       space_grid.push_back(v);
    }
    file.close();
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    // TODO: Your code here
    fstream file(player_file_name,ios::in);
    if(!file.is_open()) {
        cout<<"error"<<endl;
        return;
    }
    int row,col;
    file>>row>>col;
    file.ignore();
    vector<vector<bool>> v;
    string line;

    while (getline(file,line)) {
        vector<bool> v_row;
        for (char ch : line) {
            if (ch == '1') {
                v_row.push_back(1);
            } else if (ch == '0') {
                v_row.push_back(0);
            }
        }
        v.push_back(v_row);
    }
    player=new Player(v,row,col,player_name);

    file.close();
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    // TODO: Your code here

    fstream file(input_file, ios::in);
    if (!file.is_open()) {
        cout << "Error: Unable to open file " << input_file << endl;
        return;
    }

    vector<vector<bool>> v;
    string line;
    ObjectType type;
    int start_row;
    int time_of_appearance;
    bool isfirst = true;
    bool emptyChecker = false;

    CelestialObject *current = nullptr;
    int n = 0;

    while (getline(file, line)) {
        if (line.empty()) {
            if (!v.empty()) {
                if (isfirst) {
                    celestial_objects_list_head = new CelestialObject(v, type, start_row, time_of_appearance);
                    celestial_objects_list_head->next_celestial_object = nullptr;
                    current = celestial_objects_list_head;
                    isfirst = false;
                } else {
                    CelestialObject *newCele = new CelestialObject(v, type, start_row, time_of_appearance);

                    current->next_celestial_object = newCele;
                    current = newCele;
                }
                v.clear();
                emptyChecker = true;
            }
            continue;
        }

        char a = line[0];
        vector<bool> v_row;

        if (a == 's' || a == 't' || a == 'e') {
            emptyChecker = false;
            if (a == 's') {
                start_row = stoi(line.substr(2));
            } else if (a == 't') {
                time_of_appearance = stoi(line.substr(2));
            } else if (a == 'e') {
                string s = line.substr(2);
                if (s == "life") {
                    type = LIFE_UP;
                } else {
                    type = AMMO;
                }
            }
        } else {
            emptyChecker = false;
            for (char x : line) {
                if (x == '1') {
                    v_row.push_back(true);
                } else if (x == '0') {
                    v_row.push_back(false);
                } else if (x == ']') {
                    type = ASTEROID;
                }
            }
            v.push_back(v_row);
        }
        n++;
    }


    if (!v.empty()) {
        CelestialObject *newCele = new CelestialObject(v, type, start_row, time_of_appearance);
        newCele->next_celestial_object = nullptr;
        if (isfirst) {
            celestial_objects_list_head = newCele;
        } else {
            current->next_celestial_object = newCele;
        }
    }
    CelestialObject *temp = celestial_objects_list_head;
    while (temp != nullptr) {

        if (temp->right_rotation && temp->next_celestial_object && temp->next_celestial_object->right_rotation) {
            temp->right_rotation->next_celestial_object = temp->next_celestial_object;
        }

        if (temp->left_rotation && temp->next_celestial_object) {
            temp->left_rotation->next_celestial_object = temp->next_celestial_object;
        }

        if (temp->right_rotation && temp->right_rotation->right_rotation && temp->next_celestial_object && temp->next_celestial_object->right_rotation) {
            temp->right_rotation->right_rotation->next_celestial_object = temp->next_celestial_object;
        }

        temp = temp->next_celestial_object;
    }

    CelestialObject *curry = celestial_objects_list_head;
    while (curry) {
       int  resultt=0;
        for (int i=0;i<curry->shape.size();i++) {
            for (int j=0;j<curry->shape[0].size();j++) {
                if(curry->shape[i][j]==1) {
                    resultt++;
                }
            }
        }
        vector<int> v={curry->starting_row,curry->time_of_appearance,resultt};
        old_celestial_objects.push_back(v);
        curry->oldShape=resultt;
        curry=curry->next_celestial_object;
    }


    file.close();
}




// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    // TODO: Your code here
    cout<<"Tick: "<<game_time<<endl;
    cout<<"Lives: "<<player->lives<<endl;
    cout<<"Ammo: "<<player->current_ammo<<endl;
    cout<<"Score: "<<current_score<<endl;
    if(leaderboard.head_leaderboard_entry) {
        cout<<"High Score: "<< leaderboard.head_leaderboard_entry->score<<endl;
    }
    else {
        cout<<"High Score: 465"<<endl;
    }
    if(game_over) {
        cout<<"Player: "<<player->player_name<<endl;
    }

    for (int i=0;i<space_grid.size();i++) {
        for (int j=0;j<space_grid[0].size();j++) {
            if(space_grid[i][j]==0) {
                cout<<unoccupiedCellChar;
               // cout<<0<<" ";
            }
            else {
                cout<<occupiedCellChar;
                //cout<<1<<" ";
            }
        }
        cout<<endl;
    }

        cout<<endl;

}

bool AsteroidDash::controlAmmo(int row,int col) {

    for (Ammo ammo:ammos) {
        if(ammo.row==row&&ammo.col==col) {
            return true;
        }
    }

    return false;

}
void AsteroidDash::deleteAmmo(int row, int col) {
    for (auto it = ammos.begin(); it != ammos.end();) {
        if (it->row == row && it->col == col) {
            it = ammos.erase(it);
        } else {
            ++it;
        }
    }
}

void AsteroidDash::updatePlayer() {
    //player

    vector<vector<bool>> vec=player->spacecraft_shape;
    int row=player->position_row;
    int col=player->position_col;
    for (int i=0;i<vec.size();i++) {
        for (int j=0;j<vec[0].size();j++) {
            if ((row + i) < space_grid.size() && (col + j) < space_grid[0].size()) {
                space_grid[row+i][col+j]=vec[i][j];
            }

        }
    }
}
void AsteroidDash::updateAmmo() {
    //shoot
    for (Ammo &ammo:ammos) {
        if (ammo.col >= space_grid[0].size()) {
            ammo.col++;
            continue;
        }

        if (ammo.col<space_grid[0].size()) {
            ammo.col++;
        }

    }
}

int find(int row,CelestialObject *current) {
    for (int i = 0; i < current->shape[row].size(); i++) {
        if(current->shape[row][i]) {
            return  i;
        }
    }
    return -1;
}
bool AsteroidDash::controlCollision(vector<vector<int>> s, CelestialObject *current, int start_col, int &r, int &c) {

    for (int i = 0; i < current->shape.size(); i++) {
        for (int j = 0; j < current->shape[0].size(); j++) {

            if ((current->starting_row + i) < s.size() && (current->starting_row >= 0) &&
                (start_col + j) >= 0 && (start_col + j) < s[0].size()) {

                int row_diff = current->starting_row + i;
                int col_diff = start_col + j;
                //cout<<"row_diff =current->starting_row + i="<<current->starting_row + i<<endl;
                //cout<<"col_diff = start_col + j="<<start_col+j<<endl;

                    if (controlAmmo(row_diff, col_diff-1) && current->shape[i][j]) {
                        //cout<<"****2ye bölündü ve control ammoyu geçti****"<<endl;
                        if (current->object_type == ASTEROID) {
                            deleteAmmo(row_diff, col_diff-1);
                            r = i;
                            c = j;
                            return true;
                        }
                    }

                    if (controlAmmo(row_diff, col_diff) && current->shape[i][j]) {
                        //cout<<"****2ye bölünemedi ve control ammoyu geçti****"<<endl;
                        if (current->object_type == ASTEROID) {
                            deleteAmmo(row_diff, col_diff);
                            r = i;
                            c = j+1;
                            return true;
                        }
                    }

                }
        }
    }

    return false;
}


bool AsteroidDash::control(vector<vector<int>> s, CelestialObject *current, int start_col) {

    int player_row = player->position_row;
    int player_col = player->position_col;

    for (int i = 0; i < player->spacecraft_shape.size(); i++) {
        for (int j = 0; j < player->spacecraft_shape[0].size(); j++) {
            if (player->spacecraft_shape[i][j]) {
                int player_current_row = player_row + i;
                int player_current_col = player_col + j;

                for (int k = 0; k < current->shape.size(); k++) {
                    for (int l = 0; l < current->shape[0].size(); l++) {
                        if (current->shape[k][l] == 1) {
                            int current_row = current->starting_row + k;
                            int current_col = start_col + l;

                            if (player_current_row == current_row && player_current_col == current_col) {

                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool AsteroidDash::notPrint(vector<vector<int>> s, CelestialObject *current, int start_col) {
    int player_row = player->position_row;
    int player_col = player->position_col;

    for (int i = 0; i < player->spacecraft_shape.size(); i++) {
        for (int j = 0; j < player->spacecraft_shape[0].size(); j++) {
            if (player->spacecraft_shape[i][j]) {
                int player_current_row = player_row + i;
                int player_current_col = player_col + j;

                for (int k = 0; k < current->shape.size(); k++) {
                    for (int l = 0; l < current->shape[0].size(); l++) {
                        if (current->shape[k][l] == 0) {
                            int current_row = current->starting_row + k;
                            int current_col = start_col + l;

                            if (player_current_row == current_row && player_current_col == current_col) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool bonus(CelestialObject *current) {
    for (int i=0;i<current->shape.size();i++) {
        for (int j=0;j<current->shape[0].size();j++) {
            if(current->shape[i][j]!=0) {
                return false;
            }
        }
    }
    return true;
}

int findOldShape(CelestialObject *current,vector<vector<int>> v) {



    for (vector<int> vec: v) {
        if(vec[1]==current->time_of_appearance && vec[0]==current->starting_row) {
            return vec[2];
        }
    }

}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    // TODO: Your code here

    scoreincrease=true;
    player->isMove=false;
    for (auto &row : space_grid) {
        fill(row.begin(), row.end(), 0);
    }
    updatePlayer();
    //Celestial

    CelestialObject *current=celestial_objects_list_head;
    CelestialObject *prev=nullptr;
    int r=-1,c=-1;
    while (current) {

        if(current->time_of_appearance<=game_time) {
            player->isMove=false;
            int start_col= space_grid[0].size()-1-(game_time - current->time_of_appearance);
            CelestialObject *newcele=nullptr;
            if(!controlCollision(space_grid,current,start_col,r,c)&&!control(space_grid,current,start_col)) {
                for (int i=0;i<current->shape.size();i++) {
                for (int j=0;j<current->shape[0].size();j++) {

                    if((current->starting_row + i) < space_grid.size() &&(current->starting_row>=0)&&
                    (start_col + j) >= 0 && (start_col + j) < space_grid[0].size() ) {

                        if(current->shape[i][j]){
                            space_grid[current->starting_row+i][start_col+j]=1;
                        }
                        if( !current->shape[i][j]&&!notPrint(space_grid,current,start_col)) {
                            space_grid[current->starting_row+i][start_col+j]=0;
                        }



                    }

                }
            }



            }
            else if(!controlCollision(space_grid,current,start_col,r,c)&&control(space_grid,current,start_col)) {

                if(current->object_type==ASTEROID) {
                    player->lives--;
                    scoreincrease=false;
                    if(player->lives==0) {
                        game_over=true;
                    }
                }
                else if(current->object_type==AMMO) {
                    player->current_ammo=player->max_ammo;

                }
                else {
                    player->lives++;

                }

                CelestialObject *del=current;

                current=current->next_celestial_object;

                if (prev) {
                    prev->next_celestial_object=current;
                    if(prev->right_rotation) {
                        prev->right_rotation->next_celestial_object=current;
                    }
                    if(prev->left_rotation) {
                        prev->left_rotation->next_celestial_object=current;
                    }
                    if(prev->right_rotation &&prev->right_rotation->right_rotation) {
                        prev->right_rotation->right_rotation->next_celestial_object=current;
                    }

                }
                else {
                    celestial_objects_list_head=current;
                }
                CelestialObject::delete_rotations(del);
                delete del;
                continue;

            }
            else {

                //scoreincrease=false;

                if(r!=-1 && c!=-1) {
                    for(int i=0;i<current->shape[r].size();i++) {
                        if(current->shape[r][i]==1) {

                            current->shape[r][i]=0;
                            break;
                        }
                    }
                    if(bonus(current)) {
                        int number=findOldShape(current,old_celestial_objects);
                        current_score+=100*number;
                    }

                    current_score+=10;


                }

                newcele=new CelestialObject(current);
                if(current->shape.size()%2==0) {
                    if(r<current->shape.size()/2) {
                        if (newcele->right_rotation != nullptr) {
                            newcele = newcele->right_rotation;
                        }
                    }
                    else {
                        if(newcele->left_rotation) {
                            newcele=newcele->left_rotation;
                        }

                    }
                }
                else {
                    if(r<current->shape.size()/2) {
                        if (newcele->right_rotation != nullptr) {
                            newcele = newcele->right_rotation;
                        }
                    }
                    else if(r>current->shape.size()/2) {
                        if (newcele->left_rotation != nullptr) {
                            newcele = newcele->left_rotation;
                        }
                    }
                    else {
                    }
                }
                if(prev) {
                    prev->next_celestial_object=newcele;
                    if(prev->right_rotation) {
                        prev->right_rotation->next_celestial_object=newcele;
                    }
                    if(prev->left_rotation) {
                        prev->left_rotation->next_celestial_object=newcele;
                    }
                    if(prev->right_rotation &&prev->right_rotation->right_rotation) {
                        prev->right_rotation->right_rotation->next_celestial_object=newcele;
                    }
                }
                else {
                    celestial_objects_list_head=newcele;
                }
                newcele->next_celestial_object=current->next_celestial_object;
                CelestialObject *del=current;
                current=newcele;
                CelestialObject::delete_rotations(del);
                delete del;

                for (int i=0;i<current->shape.size();i++) {
                    for (int j=0;j<current->shape[0].size();j++) {
                        if((current->starting_row + i) < space_grid.size() &&(current->starting_row>=0)&&
                        (start_col + j) >= 0 && (start_col + j) < space_grid[0].size()) {
                                if( current->shape[i][j]){
                                    space_grid[current->starting_row+i][start_col+j]=1;
                                }
                                if(!current->shape[i][j]) {
                                    space_grid[current->starting_row+i][start_col+j]=0;
                                }

                        }
                    }
                }



            }
        }

        prev=current;
    current=current->next_celestial_object;

    }


        for (Ammo &ammo:ammos) {
            if (ammo.col >= space_grid[0].size()) {
                ammo.col++;
                continue;
            }
            space_grid[ammo.row][ammo.col]=1;
            if (ammo.col<space_grid[0].size()) {
                ammo.col++;
            }

        }


}
// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    // TODO: Your code here
    if (player->current_ammo<=0) {
        return;
    }
    int mid=player->position_row+player->spacecraft_shape.size()/2;
    int col=player->spacecraft_shape[player->spacecraft_shape.size()/2].size()+player->position_col;

    if (col < space_grid[0].size()) {
        Ammo ammo;
        ammo.col=col;
        ammo.row=mid;
        ammos.push_back(ammo);
    }


    player->current_ammo--;
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    // TODO: Your code here
    CelestialObject *current = celestial_objects_list_head;
    while (current) {
        CelestialObject *next = current->next_celestial_object;
        CelestialObject::delete_rotations(current);
        delete current;
        current = next;
    }


delete player;
}