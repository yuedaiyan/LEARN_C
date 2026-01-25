/*
编程挑战：打造“复古 MP3 播放器”的核心歌单系统

1. 场景背景 (Scenario)
假设你正在为一款复古风格的 MP3 播放器编写嵌入式 C
语言程序。该播放器最核心的功能是管理一个播放列表 (Playlist)。
用户不仅可以顺序播放音乐，还可以随时点击“上一首 (Previous)”和“下一首
(Next)”，甚至觉得某首歌不好听时，直接从歌单中“删除 (Delete)”当前播放的歌曲。

2. 需求描述 (Requirements)
你需要实现一个基于双向链表 (Doubly Linked List)
的歌单管理系统。请完成以下具体任务：

任务 A：定义结构体 (参照 PPT p.34)
定义一个名为 Song (或者 node) 的结构体，它应该包含：

char *title: 指向歌曲标题的指针（注意：不要用定长数组，要用指针，模拟 PPT p.14
的动态内存处理）。 struct Song *prev: 指向上一首歌的指针。 struct Song *next:
指向下一首歌的指针。 任务 B：动态添加歌曲 (参照 PPT p.14 - p.21, p.35)
编写一个函数 add_song(char *title_text)。

内存分配：不仅要为 Song 结构体 malloc 内存，还要为 title 字符串 malloc
内存并使用 strcpy 复制内容（这一点在 PPT p.14-16 中被反复强调）。
链表链接：将新歌添加到链表的末尾，并正确维护 prev 和 next 指针。
任务 C：播放控制 (参照 PPT p.37 - p.40)
编写遍历逻辑，模拟播放器界面：

打印整个歌单（正向遍历）。
（选做）尝试反向遍历歌单（从 tail 走向 head），验证你的 prev 指针是否连接正确。
任务 D：删除当前歌曲 (参照 PPT p.28 - p.31)
编写一个函数 delete_song(char *target_title)。

找到名为 target_title 的节点。
将其从链表中移除。注意： PPT p.29-31
详细画图展示了删除节点时需要重新连接前后节点的逻辑（尤其是红色的叉叉和箭头）。
重要：别忘了 free 掉你申请的内存（包括结构体本身和字符串内存）。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXINPUT 1024

typedef struct Song {
    char *title;
    struct Song *prev;
    struct Song *next;
} Song;

typedef struct List {
    struct Song *head;
    struct Song *tail;
} List;

void add_song(List *my_list, char *input_text) {
    char *save = (char *)malloc(strlen(input_text) + 1);
    if (save == NULL)
        return;
    strcpy(save, input_text);
    Song *new_song = (Song *)malloc(sizeof(Song));
    if (new_song == NULL)
        return;
    new_song->title = save;
    new_song->next = NULL;
    new_song->prev = NULL;
    if (my_list->head == NULL) {
        my_list->head = new_song;
        my_list->tail = new_song;
        return;
    }
    new_song->prev = my_list->tail;
    my_list->tail->next = new_song;
    my_list->tail = new_song;
    return;
}

void delete_song(List *my_list, int delete_index) {
    Song *curr = my_list->head;

    for (int i = 0; i < delete_index && curr != NULL; i++) {
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("Index no fond\n");
        return;
    }
    // three situation
    // if (curr->prev == NULL) {
    //     // curr is the head
    //     my_list->head = curr->next;
    //     curr->next->prev = NULL;
    // } else if (curr->next == NULL) {
    //     // curr is the tail
    //     my_list->tail = curr->prev;
    //     curr->prev->next = NULL;
    // } else {
    //     // curr is normal situation
    //     curr->prev->next = curr->next;
    //     curr->next->prev = curr->prev;
    // }

    // curr's prev exit
    if (curr->prev != NULL) {
        curr->prev->next = curr->next;
    } else {
        my_list->head = curr->next;
    }
    // curr's next exit
    if (curr->next != NULL) {
        curr->next->prev = curr->prev;
    } else {
        my_list->tail = curr->prev;
    }

    // free memory
    free(curr->title);
    free(curr);
    printf("Delete success!\n");
    return;
}

// void delete_song(List *my_list, int delete_index) {
//     Song *curr = my_list->head;
//     for (int i = 0; i < delete_index && curr != NULL; i++) {
//         curr = curr->next;
//     }

//     if (curr == NULL) {
//         printf("Index no fond\n");
//         return;
//     }
//     if (curr == my_list->head) {
//         // if delete song is the head
//         my_list->head = curr->next;
//         if (curr->next != NULL) {
//             // if there only one song in chain
//             curr->next->prev = NULL;
//         }

//     } else if (curr == my_list->tail) {
//         // if delete song is the tail
//         my_list->tail = curr->prev;
//         curr->prev->next = NULL;
//     } else {
//         // normal situation
//         Song *curr_prev = curr->prev;
//         curr_prev->next = curr->next;
//         Song *curr_next = curr->next;
//         curr_next->prev = curr->prev;
//     }
// free(curr->title);
// free(curr);
// printf("Delete success!\n");
// return;
// }

void play_song(List *my_list, int seq) {
    if (seq != 0 && seq != 1)
        return;
    Song *curr = NULL;
    if (seq == 1) {
        // Ascending order
        curr = my_list->head;
        while (curr != NULL) {
            printf("%s", curr->title);
            curr = curr->next;
        }
        return;
    }
    if (seq == 0) {
        // Descending order
        curr = my_list->tail;
        while (curr != NULL) {
            printf("%s", curr->title);
            curr = curr->prev;
        }
        return;
    }
}

void delete_list(List *my_list) {
    Song *curr = my_list->head;
    while (curr != NULL) {
        Song *temp = curr;
        curr = curr->next;
        free(temp->title);
        free(temp);
    }
}

int main() {
    List my_list;
    my_list.head = NULL;
    my_list.tail = NULL;

    char input_text[MAXINPUT];
    printf("\nPlease input your song's title: ");
    while (fgets(input_text, MAXINPUT, stdin) != NULL) {
        add_song(&my_list, input_text);
        printf("\nPlease input your song's title: ");
    }
    printf("\nAdd song over.\n");
    clearerr(stdin);
    // Print all songs
    int seq;
    printf("\nPlease input play sequence (1=Ascending order, 0=descending "
           "order):");
    scanf("%d", &seq);
    play_song(&my_list, seq);

    // Delete song
    int delete_index;
    printf("\nPlease input delete indext:");
    scanf("%d", &delete_index);
    delete_song(&my_list, delete_index);

    delete_list(&my_list);
}