#include <stdio.h>
#include <string.h>
void get_char(char *i_ad, int *sb_n_ad, int *sb_t_ad, int *sb_s_ad) {

  switch (*i_ad) {
  case '\n':
    (*sb_n_ad)++;
    break;
  case '\t':
    (*sb_t_ad)++;
    break;
  case ' ':
    (*sb_s_ad)++;
    break;
  default:
    break;
  }
}
int main() {
  char line[] = "akdd\no\t  dfhu\na\tf \tn y\n\t\nba\tcxjk z\n\n\tg\tfk "
                "agy\tner q b\nfdahf g\t\na\nsdf  akd\nfa \tdfa";
  int sb_n = 0, sb_t = 0, sb_s = 0;
  printf("%s", line);

  for (int i = 0; i < (strlen(line)); i++) {
    get_char(&line[i], &sb_n, &sb_t, &sb_s);
  }
  printf("\nnweLine:%d tab:%d space:%d\n", sb_n, sb_t, sb_s);
}