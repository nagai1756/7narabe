#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "conio.h"
void set_name();
void set_card();
void display();
int next_turn(int turn);
void method_display();
void GoToNext(char ch);
int CanPut(int x, int y);
int joker_CanPut(int x, int y);
void joker_process(int x, int y);
enum
{
  SPADE,
  HEART,
  DIAMOND,
  CLUB,
  MARK_MAX // 4
};
enum
{
  ACE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  JACK,
  QUEEN,
  KING,
  NUMBER_MAX // 13
};
enum
{
  PLAYER_1,
  PLAYER_2,
  PLAYER_3,
  PLAYER_4,
  PLAYER_MAX // 4
};
int canview = 0;
int turn = PLAYER_1;
int rule = 0;
int cursorX = 0, cursorY = 0;
int game_started = 0;
int joker_selected = 0;
int ranks[PLAYER_MAX] = {1, 1, 1, 1};
int remain_player_count = 0;
char mark[][10] = { //マーク
    " ♤\0",
    " ♡\0",
    " ♢\0",
    " ♧\0"};
int boards[MARK_MAX][NUMBER_MAX];
struct player
{
  char name[10];
  int number_card[MARK_MAX][NUMBER_MAX];
  int have_joker;
  int remain;
  int pass_count;
  int rank;
};
struct player p1, p2, p3, p4;
int remain_cards(struct player p);
int rank(struct player p);
int pass_rank(struct player p);
struct player pass_lose_process(struct player p);
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
int main(int argc, char const *argv[])
{
  system("clear");
  p1.remain = 1;
  p2.remain = 1;
  p3.remain = 1;
  p4.remain = 1;
  p1.pass_count = 0;
  p2.pass_count = 0;
  p3.pass_count = 0;
  p4.pass_count = 0;
  set_name();
  set_card();
  int start;
  if (p1.number_card[DIAMOND][SEVEN])
    start = PLAYER_1;
  if (p2.number_card[DIAMOND][SEVEN])
    start = PLAYER_2;
  if (p3.number_card[DIAMOND][SEVEN])
    start = PLAYER_3;
  if (p4.number_card[DIAMOND][SEVEN])
    start = PLAYER_4;
  while (1)
  { //手札の確認
    display();
    printf("スタート : s\n");
    char ch = getch();
    if (ch == '\n')
    {
      if (canview)
        turn = next_turn(turn);
      canview++;
      canview %= 2;
    }
    else if (ch == 's')
    {
      canview = 0;
      turn = start;
      game_started = 1;
      for (int y = 0; y < MARK_MAX; y++)
      {
        p1.number_card[y][SEVEN] = 0;
        p2.number_card[y][SEVEN] = 0;
        p3.number_card[y][SEVEN] = 0;
        p4.number_card[y][SEVEN] = 0;
        boards[y][SEVEN] = 1;
      }
      break;
    }
  }
  while (1)
  { //ゲーム開始
    if (rule)
    {
      method_display();
      if (getch() == 'r')
      {
        rule++;
        rule %= 2;
        continue;
      }
    }
    else
    {
      display();
      printf("仕様確認 : r\n");
      GoToNext(getch());
      if (p1.remain)
        remain_player_count++;
      if (p2.remain)
        remain_player_count++;
      if (p3.remain)
        remain_player_count++;
      if (p4.remain)
        remain_player_count++;
      if (remain_player_count == 1)
      {
        if (p1.remain)
          p1.rank = rank(p1);
        else if (p2.remain)
          p2.rank = rank(p2);
        else if (p3.remain)
          p3.rank = rank(p3);
        else if (p4.remain)
          p4.rank = rank(p4);
        break;
      }
    }
    remain_player_count = 0;
  }
  p1.remain = 0;
  p2.remain = 0;
  p3.remain = 0;
  p4.remain = 0;
  for (int y = 0; y < MARK_MAX; y++)
    for (int x = 0; x < NUMBER_MAX; x++)
      boards[y][x] = 1;
  display();
  printf("対戦終了\n");
  for (int r = 1; r <= 4; r++)
  {
    if (p1.rank == r)
      printf("%d位 %s\n", r, p1.name);
    else if (p2.rank == r)
      printf("%d位 %s\n", r, p2.name);
    else if (p3.rank == r)
      printf("%d位 %s\n", r, p3.name);
    else if (p4.rank == r)
      printf("%d位 %s\n", r, p4.name);
  }
  return 0;
}
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑

void set_name()
{
  for (int p = 0; p < PLAYER_MAX; p++)
  {
    printf("%d人目の名前は？(10文字以内)>", p + 1);
    if (p == 0)
    {
      fgets(p1.name, 11, stdin);
      p1.name[strlen(p1.name) - 1] = '\0';
    }
    if (p == 1)
    {
      fgets(p2.name, 11, stdin);
      p2.name[strlen(p2.name) - 1] = '\0';
    }
    if (p == 2)
    {
      fgets(p3.name, 11, stdin);
      p3.name[strlen(p3.name) - 1] = '\0';
    }
    if (p == 3)
    {
      fgets(p4.name, 11, stdin);
      p4.name[strlen(p4.name) - 1] = '\0';
    }
  }
}

void set_card()
{
  int cards[MARK_MAX][NUMBER_MAX];
  srand((unsigned int)time(NULL));
  for (int y = 0; y < MARK_MAX; y++)
  {
    for (int x = 0; x < NUMBER_MAX; x++)
    {
      cards[y][x] = 1;
    }
  }
  int joker = rand() % 4;
  switch (joker)
  {
  case PLAYER_1:
    p1.have_joker = 1;
    break;
  case PLAYER_2:
    p2.have_joker = 1;
    break;
  case PLAYER_3:
    p3.have_joker = 1;
    break;
  case PLAYER_4:
    p4.have_joker = 1;
    break;
  }
  for (int p = 0; p < PLAYER_MAX; p++)
  {
    int count = 0;
    while (1)
    {
      int Mark = rand() % MARK_MAX;
      int Number = rand() % NUMBER_MAX;
      if (cards[Mark][Number])
      {
        cards[Mark][Number] = 0;
        count++;
        if (p == PLAYER_1)
        {
          p1.number_card[Mark][Number] = 1;
        }
        if (p == PLAYER_2)
        {
          p2.number_card[Mark][Number] = 1;
        }
        if (p == PLAYER_3)
        {
          p3.number_card[Mark][Number] = 1;
        }
        if (p == PLAYER_4)
        {
          p4.number_card[Mark][Number] = 1;
        }
      }
      if (count == 13)
        break;
    }
  }
}

int next_turn(int turn)
{
  while (1)
  {
    turn++;
    turn %= 4;
    if (turn == PLAYER_1 && p1.remain)
      break;
    if (turn == PLAYER_2 && p2.remain)
      break;
    if (turn == PLAYER_3 && p3.remain)
      break;
    if (turn == PLAYER_4 && p4.remain)
      break;
  }
  return turn;
}

void display()
{
  system("clear");
  //盤面の描画
  for (int line = 0; line <= 13; line++)
    printf("////");
  printf("\n");
  for (int y = 0; y < MARK_MAX; y++)
  {
    for (int x = 0; x < NUMBER_MAX; x++)
    {
      if (x == 0)
        printf("%s:", mark[y]);
      if (canview && game_started && x == cursorX && y == cursorY)
      {
        printf("@@@|");
        continue;
      }
      else if (game_started && boards[y][x] == 1)
      {
        printf(" %-2d|", x + 1);
        continue;
      }
      else if (canview && game_started)
      {
        if (x == cursorX && y == cursorY)
        {
          printf("@@@|");
          continue;
        }
        else if (joker_selected)
        {
          if (joker_CanPut(x, y))
          {
            if (turn == PLAYER_1)
            {
              printf("ooo|");
              continue;
            }
            else if (turn == PLAYER_2)
            {
              printf("ooo|");
              continue;
            }
            else if (turn == PLAYER_3)
            {
              printf("ooo|");
              continue;
            }
            else if (turn == PLAYER_4)
            {
              printf("ooo|");
              continue;
            }
          }
          else
          {
            printf("   |");
            continue;
          }
        }
        else
        {
          if (turn == PLAYER_1 && p1.number_card[y][x] && CanPut(x, y))
          {
            printf("ooo|");
            continue;
          }
          else if (turn == PLAYER_2 && p2.number_card[y][x] && CanPut(x, y))
          {
            printf("ooo|");
            continue;
          }
          else if (turn == PLAYER_3 && p3.number_card[y][x] && CanPut(x, y))
          {
            printf("ooo|");
            continue;
          }
          else if (turn == PLAYER_4 && p4.number_card[y][x] && CanPut(x, y))
          {
            printf("ooo|");
            continue;
          }
          else
          {
            printf("   |");
            continue;
          }
        }
      }
      printf("   |");
    }
    printf("\n");
    for (int line = 0; line <= 13; line++)
      printf("----");
    printf("\n");
  }
  for (int line = 0; line <= 13; line++)
    printf("////");
  printf("\n");

  //手札の表示
  for (int p = 0; p < PLAYER_MAX; p++)
  {
    struct player bufp;
    switch (p)
    {
    case PLAYER_1:
      bufp = p1;
      break;
    case PLAYER_2:
      bufp = p2;
      break;
    case PLAYER_3:
      bufp = p3;
      break;
    case PLAYER_4:
      bufp = p4;
      break;
    }
    printf("%s ", bufp.name);
    if (game_started)
      printf(" パス %d回", bufp.pass_count);
    if (bufp.remain == 0)
      printf("%d 位", bufp.rank);
    printf("\n");
    for (int y = 0; y < MARK_MAX; y++)
    {
      for (int x = 0; x < NUMBER_MAX; x++)
      {
        if (x == 0)
          printf("%s:", mark[y]);
        if (bufp.remain && canview && turn == p)
        {
          if (x == cursorX && y == cursorY && game_started && joker_selected == 0)
            printf("@@@|");
          else if (bufp.number_card[y][x])
            printf(" %-2d|", x + 1);
          else
            printf("   |");
        }
        else if (y == 0 && x == 0 && bufp.remain == 0 && bufp.pass_count > 3)
          printf("脱落...");
        else if (x == 0 && p == turn)
          printf(" 手札を表示 : Enter");
      }
      printf("\n");
      for (int line = 0; line <= 13; line++)
        printf("----");
      printf("\n");
    }
    printf("ジョーカー");
    if (canview && turn == p)
    {
      if (joker_selected)
        printf("をどこに置きますか？↑↑");
      else if (bufp.have_joker == 1)
        printf(" ◯");
      if (game_started == 0)
        printf(" 手札を非表示 : Enter");
    }
    printf("\n\n");
  }
}

void method_display()
{
  system("clear");
  printf("カーソル : @@@\n");
  printf("上に移動 : w\n");
  printf("下に移動 : s\n");
  printf("右に移動 : d\n");
  printf("左に移動 : a\n");
  printf("パス : p\n");
  printf("置ける場所 : ooo\n");
  printf("ジョーカーを選択、解除 : j\n");
  printf("\n");
  printf("<ルール説明>\n");
  printf("パスは3回まででき、4回で負けとなります。\n");
  printf("隣り合った(隣接した)カードのみ場に置くことができます。\n");
  printf("1と13は隣接したものと見なします。\n");
  printf("ジョーカーは１枚です。\n");
  printf("1~13までのカードを全て使い切った人の勝ちです。\n");
  printf("その時、ジョーカーが手札に残ってしまった場合は負けとなります。\n");
  printf("\n");
  printf("ジョーカーなどの詳しいルールについてはこちらを参照↓↓\n");
  printf("https://playingcards.jp/game_rules/sevens_rules.html\n");
  printf("\n");
  printf("この画面を閉じる : r\n");
}

void GoToNext(char ch)
{
  if (ch == 'r')
  {
    rule++;
    rule %= 2;
  }
  if (rule == 0)
  {
    if (ch == '\n')
    {
      if (canview)
      {
        if (joker_selected)
        {
          if (joker_CanPut(cursorX, cursorY))
          {
            if (turn == PLAYER_1)
              p1.have_joker = 0;
            else if (turn == PLAYER_2)
              p2.have_joker = 0;
            else if (turn == PLAYER_3)
              p3.have_joker = 0;
            else if (turn == PLAYER_4)
              p4.have_joker = 0;
            joker_process(cursorX, cursorY);
          }
        }
        else
        {
          int put = 0;
          if (CanPut(cursorX, cursorY))
          {
            if (turn == PLAYER_1 && p1.number_card[cursorY][cursorX])
            {
              put = 1;
              p1.number_card[cursorY][cursorX] = 0;
            }
            else if (turn == PLAYER_2 && p2.number_card[cursorY][cursorX])
            {
              put = 1;
              p2.number_card[cursorY][cursorX] = 0;
            }
            else if (turn == PLAYER_3 && p3.number_card[cursorY][cursorX])
            {
              put = 1;
              p3.number_card[cursorY][cursorX] = 0;
            }
            else if (turn == PLAYER_4 && p4.number_card[cursorY][cursorX])
            {
              put = 1;
              p4.number_card[cursorY][cursorX] = 0;
            }
          }
          if (put)
          {
            boards[cursorY][cursorX] = 1;
            turn = next_turn(turn);
            cursorX = cursorY = 0;
            canview = 0;
          }
        }
        if (p1.remain && remain_cards(p1) == 0)
        {
          p1.rank = rank(p1);
          p1.remain = 0;
        }
        else if (p2.remain && remain_cards(p2) == 0)
        {
          p2.rank = rank(p2);
          p2.remain = 0;
        }
        else if (p3.remain && remain_cards(p3) == 0)
        {
          p3.rank = rank(p3);
          p3.remain = 0;
        }
        if (p4.remain && remain_cards(p4) == 0)
        {
          p4.rank = rank(p4);
          p4.remain = 0;
        }
      }
      else
        canview = 1;
    }
    if (canview)
    {
      if (ch == 'w')
      {
        cursorY--;
        if (cursorY < 0)
          cursorY = CLUB;
      }
      else if (ch == 's')
      {
        cursorY++;
        cursorY %= MARK_MAX;
      }
      else if (ch == 'd')
      {
        cursorX++;
        cursorX %= NUMBER_MAX;
      }
      else if (ch == 'a')
      {
        cursorX--;
        if (cursorX < 0)
          cursorX = KING;
      }
      else if (ch == 'p')
      {
        if (turn == PLAYER_1 && p1.remain)
        {
          p1.pass_count++;
          if (p1.pass_count > 3)
          {
            p1.remain = 0;
            p1.rank = pass_rank(p1);
            p1 = pass_lose_process(p1);
          }
        }
        else if (turn == PLAYER_2 && p2.remain)
        {
          p2.pass_count++;
          if (p2.pass_count > 3)
          {
            p2.remain = 0;
            p2.rank = pass_rank(p2);
            p2 = pass_lose_process(p2);
          }
        }
        else if (turn == PLAYER_3 && p3.remain)
        {
          p3.pass_count++;
          if (p3.pass_count > 3)
          {
            p3.remain = 0;
            p3.rank = pass_rank(p3);
            p3 = pass_lose_process(p3);
          }
        }
        else if (turn == PLAYER_4 && p4.remain)
        {
          p4.pass_count++;
          if (p4.pass_count > 3)
          {
            p4.remain = 0;
            p4.rank = pass_rank(p4);
            p4 = pass_lose_process(p4);
          }
        }
        turn = next_turn(turn);
        canview = 0;
        cursorX = cursorY = 0;
        joker_selected = 0;
      }
      else if (ch == 'j')
      {
        if (joker_selected)
        {
          joker_selected = 0;
          cursorX = cursorY = 0;
        }
        else
        {
          if (turn == PLAYER_1 && p1.have_joker)
          {
            joker_selected = 1;
            cursorX = cursorY = 0;
          }
          else if (turn == PLAYER_2 && p2.have_joker)
          {
            joker_selected = 1;
            cursorX = cursorY = 0;
          }
          else if (turn == PLAYER_3 && p3.have_joker)
          {
            joker_selected = 1;
            cursorX = cursorY = 0;
          }
          else if (turn == PLAYER_4 && p4.have_joker)
          {
            joker_selected = 1;
            cursorX = cursorY = 0;
          }
        }
      }
    }
  }
}

int CanPut(int _x, int _y)
{
  int flag1 = 1, flag2 = 1;
  int x = _x, y = _y;
  while (x != SEVEN)
  {
    x--;
    if (x < ACE)
      x = KING;
    if (boards[y][x] == 0)
    {
      flag1 = 0;
      break;
    }
  }
  x = _x;
  while (x != SEVEN)
  {
    x++;
    if (x > KING)
      x = ACE;
    if (boards[y][x] == 0)
    {
      flag2 = 0;
      break;
    }
  }
  if (flag1 == 1 || flag2 == 1)
    return 1;
  return 0;
}

int joker_CanPut(int _x, int _y)
{
  int x = _x, y = _y;
  struct player bufp;
  int judge = 0;
  int count = 0;
  switch (turn)
  {
  case PLAYER_1:
    bufp = p1;
    break;
  case PLAYER_2:
    bufp = p2;
    break;
  case PLAYER_3:
    bufp = p3;
    break;
  case PLAYER_4:
    bufp = p4;
    break;
  }
  if (bufp.number_card[y][x] == 0)
  {
    if (x == ACE)
    {
      if (bufp.number_card[y][TWO])
      {
        int x1 = KING;
        while (x1 != SEVEN)
        {
          if (boards[y][x1] == 0)
            return 0;
          x1--;
        }
        return 1;
      }
      if (bufp.number_card[y][KING])
      {
        int x1 = TWO;
        while (x1 != SEVEN)
        {
          if (boards[y][x1] == 0)
            return 0;
          x1++;
        }
        return 1;
      }
    }
    else if (x == KING)
    {
      if (bufp.number_card[y][ACE])
      {
        int x1 = QUEEN;
        while (x1 != SEVEN)
        {
          if (boards[y][x1] == 0)
            return 0;
          x1--;
        }
        return 1;
      }
      if (bufp.number_card[y][QUEEN])
      {
        int x1 = ACE;
        while (x != SEVEN)
        {
          if (boards[y][x1] == 0)
            return 0;
          x1++;
        }
        return 1;
      }
    }
    else
    {
      if (bufp.number_card[y][_x + 1])
      {
        int x1 = _x - 1;
        while (x1 != SEVEN)
        {
          if (x1 < ACE)
            x1 = KING;
          if (boards[y][x1] == 0)
            return 0;
          x1--;
        }
        return 1;
      }
      if (bufp.number_card[y][_x - 1])
      {
        int x1 = _x + 1;
        while (x1 != SEVEN)
        {
          if (x1 > KING)
            x1 = ACE;
          if (boards[y][x1] == 0)
            return 0;
          x1++;
        }
        return 1;
      }
    }
  }
  return 0;
}

void joker_process(int x, int y)
{
  switch (turn)
  {
  case PLAYER_1:
    if (x == ACE)
    {
      p1.number_card[y][KING] = 0;
      p1.number_card[y][TWO] = 0;
    }
    else if (x == KING)
    {
      p1.number_card[y][QUEEN] = 0;
      p1.number_card[y][ACE] = 0;
    }
    else
    {
      p1.number_card[y][x - 1] = 0;
      p1.number_card[y][x + 1] = 0;
    }
    break;
  case PLAYER_2:
    if (x == ACE)
    {
      p2.number_card[y][KING] = 0;
      p2.number_card[y][TWO] = 0;
    }
    else if (x == KING)
    {
      p2.number_card[y][QUEEN] = 0;
      p2.number_card[y][ACE] = 0;
    }
    else
    {
      p2.number_card[y][x - 1] = 0;
      p2.number_card[y][x + 1] = 0;
    }
    break;
  case PLAYER_3:
    if (x == ACE)
    {
      p3.number_card[y][KING] = 0;
      p3.number_card[y][TWO] = 0;
    }
    else if (x == KING)
    {
      p3.number_card[y][QUEEN] = 0;
      p3.number_card[y][ACE] = 0;
    }
    else
    {
      p3.number_card[y][x - 1] = 0;
      p3.number_card[y][x + 1] = 0;
    }
    break;
  case PLAYER_4:
    if (x == ACE)
    {
      p4.number_card[y][KING] = 0;
      p4.number_card[y][TWO] = 0;
    }
    else if (x == KING)
    {
      p4.number_card[y][QUEEN] = 0;
      p4.number_card[y][ACE] = 0;
    }
    else
    {
      p4.number_card[y][x - 1] = 0;
      p4.number_card[y][x + 1] = 0;
    }
    break;
  }
  if (x == ACE)
  {
    if (p1.number_card[y][ACE])
    {
      p1.number_card[y][ACE] = 0;
      p1.have_joker = 1;
    }
    else if (p2.number_card[y][ACE])
    {
      p2.number_card[y][ACE] = 0;
      p2.have_joker = 1;
    }
    else if (p3.number_card[y][ACE])
    {
      p3.number_card[y][ACE] = 0;
      p3.have_joker = 1;
    }
    else if (p4.number_card[y][ACE])
    {
      p4.number_card[y][ACE] = 0;
      p4.have_joker = 1;
    }
    boards[y][KING] = 1;
    boards[y][TWO] = 1;
  }
  else if (x == KING)
  {
    if (p1.number_card[y][KING])
    {
      p1.number_card[y][KING] = 0;
      p1.have_joker = 1;
    }
    else if (p2.number_card[y][KING])
    {
      p2.number_card[y][KING] = 0;
      p2.have_joker = 1;
    }
    else if (p3.number_card[y][KING])
    {
      p3.number_card[y][KING] = 0;
      p3.have_joker = 1;
    }
    else if (p4.number_card[y][KING])
    {
      p4.number_card[y][KING] = 0;
      p4.have_joker = 1;
    }
    boards[y][QUEEN] = 1;
    boards[y][ACE] = 1;
  }
  else
  {
    if (p1.number_card[y][x])
    {
      p1.number_card[y][x] = 0;
      p1.have_joker = 1;
    }
    else if (p2.number_card[y][x])
    {
      p2.number_card[y][x] = 0;
      p2.have_joker = 1;
    }
    else if (p3.number_card[y][x])
    {
      p3.number_card[y][x] = 0;
      p3.have_joker = 1;
    }
    else if (p4.number_card[y][x])
    {
      p4.number_card[y][x] = 0;
      p4.have_joker = 1;
    }
    boards[y][x - 1] = 1;
    boards[y][x + 1] = 1;
  }
  boards[y][x] = 1;
  joker_selected = 0;
  canview = 0;
  turn = next_turn(turn);
  cursorX = cursorY = 0;
}

int remain_cards(struct player p)
{
  for (int y = 0; y < MARK_MAX; y++)
  {
    for (int x = 0; x < NUMBER_MAX; x++)
    {
      if (p.number_card[y][x] == 1)
        return 1;
    }
  }
  return 0;
}

int rank(struct player p)
{
  if (p.have_joker)
  {
    return pass_rank(p);
  }
  else
  {
    for (int r = 0; r < PLAYER_MAX; r++)
    {
      if (ranks[r] == 1)
      {
        ranks[r] = 0;
        return r + 1;
      }
    }
  }
  return 0;
}

int pass_rank(struct player p)
{
  for (int r = 3; r >= 0; r--)
  {
    if (ranks[r] == 1)
    {
      ranks[r] = 0;
      return r + 1;
    }
  }
  return 0;
}

struct player pass_lose_process(struct player p)
{
  for (int y = 0; y < MARK_MAX; y++)
  {
    for (int x = 0; x < NUMBER_MAX; x++)
    {
      if (p.number_card[y][x])
      {
        p.number_card[y][x] = 0;
        boards[y][x] = 1;
      }
    }
  }
  return p;
}
