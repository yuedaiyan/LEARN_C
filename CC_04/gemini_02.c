#include <stdio.h>
#include <string.h>

int main() {
  char a;
  char line[] = "akdd\no\t  dfhu\na\tf \tn y\n\t\nba\tcxjk "
                "z\n\n\tg\tfkagy\tner q b\nfdahf g\t\na\nsdf  akd\nfa \tdfa";
  while ((a = getchar(line)) == a = '\n' || a = '\t' || a = ' ')
    ;
  printf("%c\n", a);
}