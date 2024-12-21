#include "CelestialObject.h"


// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
        : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {
    // TODO: Your code here

    initialize_rotations();

}
CelestialObject::CelestialObject(ObjectType type, int start_row, int time_of_appearance) :object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance){

}

void CelestialObject::initialize_rotations() {
    auto right = convertRight(shape);
    auto left = convertRight(convertRight(convertRight(shape)));

    int n=0;
    if(right!=left &&shape!=right) {
        n=3;
    }
    else if(right==left &&shape==right) {
        n=0;
    }
    else if(right==left &&shape!=right) {
        n=1;
    }

    CelestialObject *curr=this;
    for (int i=0;i<n;i++) {
        CelestialObject *rotation=new CelestialObject(curr->object_type,curr->starting_row,curr->time_of_appearance);
        vector<vector<bool>> shape=curr->shape;
        rotation->shape=convertRight(shape);
        curr->right_rotation=rotation;
        rotation->left_rotation=curr;
        curr=rotation;
    }
    curr->right_rotation = this;
    this->left_rotation = curr;
}
vector<vector<bool>> CelestialObject::convertRight(vector<vector<bool>> shape) {
    vector<vector<bool>> right;
    for (int i=0;i<shape[0].size();i++) {
        vector<bool> right_row;
        for (int j=shape.size()-1;j>=0;j--) {
            right_row.push_back(shape[j][i]);
        }
        right.push_back(right_row);
    }
    return right;
}

// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape),  // Copy the 2D vector shape
          object_type(other->object_type),  // Copy the object type
          starting_row(other->starting_row),  // Copy the starting row
          time_of_appearance(other->time_of_appearance)  // Copy the time of appearance
{
    // TODO: Your code here
    oldShape=other->oldShape;
    initialize_rotations();

}

// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target) {
    // TODO: Your code here
    CelestialObject *current = target->right_rotation;
    while (current != target && current != nullptr) {
        CelestialObject *next = current->right_rotation;
        delete current;
        current = next;
    }
    target->right_rotation = nullptr;
    target->left_rotation = nullptr;
}