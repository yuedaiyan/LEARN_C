#include <stdio.h>
#include <string.h>

typedef struct {
  char name[20];
  int id;
  float score;
} Student;

Student s1 = {.name = "Zhang San", .id = 1001, .score = 92.5};

int main() { printf("name: %s, id: %d, score: %.1f",s1.name,s1.id,s1.score); }