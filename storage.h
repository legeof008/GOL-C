#ifndef _STORAGE_H_

#define _STORAGE_H_

struct Node
{
    int value;
    int row_position;
    int column_postion;
    struct Node *next;
}typedef crs;

void create_new_node(struct Node** start, int non_zero_element,int row_index, int column_index );
void PrintList(struct Node* start);
#endif /* _STORAGE_H_ */
