/* $ @  list.h  
 * Copyright (C) 2017 Hsiang Chen 
 * 
 * This software is free software, you may redistribute it under the terms of GNU General Public License. 
 * For detail see <http://www.gnu.org/licenses>
 * 
 * This file is generated from Makefile, DO NOT EDIT it.
 * */
#ifndef  _LIST_H_
#define  _LIST_H_
/* start of  list.h.in  */
#define SUB_DATA 0x0001
#define SUB_LIST 0x0002
#define SUB_PTR  0x0004
#define SUB_LEN  0x0008
#define SUB_FREE 0x0200
struct _ListNode;

typedef struct _List {
  struct _ListNode* head;
  struct _ListNode* tail;
  size_t length;
} List;

typedef struct _ListNode {
  union {
    struct {
      void* ptr;
      size_t len;
    } data;
    struct _List list;
  };
  struct {
    int sub;
  } tags;
  struct _ListNode* next;
  struct _ListNode* previous;
} ListNode;
/* end of  list.h.in  */
void list_free(List* lo);
ListNode* list_get(List* lo, size_t ix);
int list_insert(List* lo, size_t ix, ListNode* no);
int list_remove(List* lo, size_t ix);
#endif  // _LIST_H_
