#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000

typedef struct Node {
    char *text;
    struct Node *next;
} Node;

typedef struct NodeList {
    Node *head;
    Node *tail;
} NodeList;

void AddNode(NodeList *Lst, char *line_char) {
    char *save = (char *)malloc(strlen(line_char) + 1);
    if (save == NULL)
        return;
    strcpy(save, line_char);
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL)
        return;
    new_node->next = NULL;
    new_node->text = save;
    if (Lst->head == NULL) {
        Lst->head = new_node;
    }
    if (Lst->tail != NULL) {
        Lst->tail->next = new_node;
    }
    Lst->tail = new_node;
}

int main() {
    char line[MAXLINE];
    printf("input:");
    NodeList my_list;
    my_list.head = NULL;
    my_list.tail = NULL;

    while (fgets(line, MAXLINE, stdin) != NULL) {
        printf("input:");
        line[strspn(line, "\n")] = 0;
        AddNode(&my_list, line);
    }
    printf("input while is over\n");
    printf("start read out\n");
    Node *curr = my_list.head;
    while (curr != NULL) {
        printf("%s", curr->text);
        curr = curr->next;
    }
}