#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ITEMS 9

typedef struct menu_t menu_t;

typedef struct {
  char* name;
  void (*f)();
} action_t;

typedef enum {
	action,
	sub_menu,
	empty,
} label_t;

typedef union {
	menu_t* sub_menu;
	action_t action;
} content_t;

typedef struct {
  label_t label;
  content_t content;
} item_t;

struct menu_t {
  char* name;
  item_t items[MAX_ITEMS];
  struct menu_t * super_menu;
};

menu_t* create_menu (const char * text) {
  menu_t* menu = malloc(sizeof(struct menu_t));
  menu->super_menu = NULL;
  char* menu_name = (char*) malloc(sizeof(char)*(strlen(text)+1));
  strcpy(menu_name, text);
  menu->name = menu_name;
  int i;
  for (i = 0; i < MAX_ITEMS; i++)
    menu->items[i].label = empty;
  return menu;
}

void add_sub_menu (menu_t* m, menu_t* sub_m) {
  if (sub_m->super_menu != NULL || m->items[MAX_ITEMS-1].label != empty)
  	return;
  sub_m->super_menu = m;
  int i = 0;
  while (i < MAX_ITEMS && m->items[i].label != empty)
    i++;
  m->items[i].label = sub_menu;
  m->items[i].content.sub_menu = sub_m;
}

void add_menu_action (menu_t* m, const char* text, void f()) {
  int i = 0;
  while (i < 9 && m->items[i].label != empty)
  	i++;
  char* name_menu = (char*) malloc(sizeof(char)*strlen(text)+1);
  strcpy(name_menu, text);
  m->items[i].content.action.name = name_menu;
  m->items[i].label = action;
  m->items[i].content.action.f = f;
}

void delete_menu (menu_t* m) {
  if (m->super_menu != NULL)
  	return;
  int i;
  for (i = 0; i < MAX_ITEMS; i++) {
    if (m->items[i].label == action)
      free(m->items[i].content.action.name);
    if (m->items[i].label == sub_menu) {
      m->items[i].content.sub_menu->super_menu = NULL;
      delete_menu(m->items[i].content.sub_menu);
    }
  }
  free(m->name);
  free(m);
}

void clear_buffer() {
  int c = 0;
  while (c != '\n' && c != EOF)
    c = getchar();
}

void launch_menu (menu_t* m) {
  printf("%s\n", m->name);
  for (int i = 0; i < MAX_ITEMS; i++) {
    if(m->items[i].label == action)
    	printf("%d - %s\n", (i+1), m->items[i].content.action.name);
    if(m->items[i].label == sub_menu)
    	printf("%d - %s\n", (i+1), m->items[i].content.sub_menu->name);
    if(m->items[i].label == 2)
    	i = 9;
  }
  printf("\n");
  char c[2];
  printf("Choice ? ");
  scanf("%1s", c);
  if (c[0] == 'p' && m->super_menu != NULL) {
    launch_menu(m->super_menu);
    return;
  }
  while (c[0] < '1' || c[0] > '9' || m->items[c[0]-'1'].label == empty) {
    printf("Wrong choice. New choice ? ");
    clear_buffer();
    scanf("%1s", c);
    if (c[0] == 'p' && m->super_menu != NULL) {
      launch_menu(m->super_menu);
      return;
    }
  }
  system("clear");
  if (m->items[c[0]-'1'].label == action) {
    m->items[c[0]-'1'].content.action.f("test");
    return;
  }
  if (m->items[c[0]-'1'].label == sub_menu) {
    launch_menu(m->items[c[0]-'1'].content.sub_menu);
    return;
  }
  printf("\n");
}

// TEST ZONE

void f1(char* a) {
  printf("Functionality 1 is called : %s\n\n", a);
}
 
void f2(char* b) {
  printf("Functionality 2 is called : %s\n\n", b);
}
 
bool cont = true;
 
void quit() {
  cont = false;
}
 
int main() {
  menu_t* m;
  menu_t* sm;
 
  m = create_menu("Main menu");
 
  add_menu_action(m,"Apply functionnality 1",f1);
  add_menu_action(m,"Apply functionnality 2",f2);
 
  sm = create_menu("First submenu");
  add_menu_action(sm,"Apply functionnality 2",f2);
  add_sub_menu(m,sm);
 
  sm = create_menu("Second submenu");
  add_menu_action(sm,"Apply functionnality 1",f1);
  add_sub_menu(m,sm);
 
  add_menu_action(m,"Quit",quit);
 
  while(cont) launch_menu(m);
 
  delete_menu(m);
 
  return EXIT_SUCCESS;
}