#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXINPUT 1024

typedef struct action_node {
    int id;
    char* action_name;
    int action_parameter;
    struct action_node* next;
    void (*execute)(const struct action_node* self_ptr);
    void (*destroy)(struct action_node* self_ptr);
} action_node;
// self must be point !!

typedef struct action_list {
    struct action_node* head;
    struct action_node* tail;
} action_list;

// create list
struct action_list* creat_action_list() {
    action_list* new_list = (action_list*)malloc(sizeof(action_list));
    if (new_list == NULL)
        return NULL;
    new_list->head = NULL;
    new_list->tail = NULL;
    return new_list;
}

// execute
void execute(const struct action_node* self_ptr) {
    printf("%d, %s, %d\n", self_ptr->id, self_ptr->action_name, self_ptr->action_parameter);
    return;
}

// Destroy
void destroy(struct action_node* self_ptr) {
    printf("%s is destroy\n", self_ptr->action_name);
    free(self_ptr->action_name);
    // free(self_ptr->next);
    free(self_ptr);

    return;
}
// crteat object
struct action_node* creat_action(int new_id, char *new_name, int new_parameter) {
    action_node* new_action = (action_node*)malloc(sizeof(action_node));
    new_action->id = new_id;
    new_action->action_name = strdup(new_name);
    new_action->action_parameter = new_parameter;
    new_action->next = NULL;
    
    new_action->execute = execute;
    new_action->destroy = destroy;
    return new_action;
}

// Add
void list_append(struct action_list* self_list, char* action_add, int parameter_add) {
    action_node* new_node =(action_node*)malloc(sizeof(action_node));
    char* action_add_save = (char*)malloc(strlen(action_add) + 1);
    if (action_add_save == NULL)
        return;

    new_node->action_name = strdup(action_add_save);
    new_node->action_parameter = parameter_add;
    new_node->next = NULL;

    if (self_list->head == NULL) {
        // if new node is the firest one
        self_list->head = new_node;
        self_list->tail = new_node;
        new_node->id = 0;

    } else {
        // if new node is the firest one
        new_node->id++;
        self_list->tail->next = new_node;
    }
    return;
}



// main node
int main() {
    action_node* node_1 = creat_action(1, "run", 10);
    action_node* node_2 = creat_action(2, "jump", 20);
    action_node* node_3 = creat_action(3, "walk", 30);
    //
    node_1->execute(node_1);
    node_2->execute(node_2);
    node_3->execute(node_3);
    //
    node_1->destroy(node_1);
    node_2->destroy(node_2);
    node_3->destroy(node_3);
    return 0;
}
