/* ***
 * @ $list.c
 * 
 * Copyright (C) 2017 Hsiang Chen
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * ***/

/* ***
 * A linear list / hashed table for storing data.
 * ***/
#include "unistd.h"
#include "sys.h"
#include "list.h"

/*release resource of list*/
void list_free(List* lo)
{
  if (lo != NULL) {
    if (lo->length > 0) {
      size_t i;
      for (i = 0; i < lo->length; i++) {
        list_remove(lo, i);
      }
    }
  }
}

ListNode* list_get(List* lo, size_t ix)
{
  if (lo != NULL) {
    size_t i;
    ListNode* no;

    for (i = 0, no = lo->head; i < lo->length && no != NULL; i++, no = no->next) {
      if (i == ix) {
        return no;
      }
    }
  }

  return NULL;
}

/*pass `0` on `ix` to insert before head, `-1` to append after tail*/
int list_insert(List* lo, size_t ix, ListNode* no)
{
  ListNode* ln = list_get(lo, ix),* in = NULL,* sn;

  if (lo == NULL) return -1;
  if (no == NULL) return -1;

  if ((sn = (ListNode*) new_chunk()) == NULL) return -1;

  FUN(memcpy)(sn, no, sizeof(ListNode));

  if (ln == NULL) {
    if (ix == -1) {
      ln = lo->tail;
    } else {
      ln = lo->head;
    }
  }

  if (ln != NULL) {
    if (ix == -1) {
      in = ln;
      ln = NULL;
    } else {
      in = ln->previous;
    }
  }

  sn->next = ln;
  sn->previous = in;

  if (ln != NULL) {
    ln->previous = sn;
  } else {
    /*reach the tail of list, `sn` is the last node*/
    lo->tail = sn;
  }

  if (in != NULL) {
    in->next = sn;
  } else {
    /*reach the head of list, `sn` is the first node in list*/
    lo->head = sn;
  }

  lo->length++;

  return 0;
}

int list_remove(List* lo, size_t ix)
{
  ListNode* ln = list_get(lo, ix);
  ListNode* l1 = NULL,* l2 = NULL;

  if (lo == NULL) return -1;
  if (ln == NULL) return -1;

  l1 = ln->previous;
  l2 = ln->next;

  if (l1 != NULL) {
    l1->next = l2;
  } else {
    /*`ln` used to be head*/
    lo->head = l2;
  }

  if (l2 != NULL) {
    l2->previous = l1;
  } else {
    /*`ln` used to be tail*/
    lo->tail = l1;
  }

  if (ln->tags.sub & SUB_LIST) {
    list_free(&ln->list);
  }

  if (ln->tags.sub & SUB_PTR && ln->tags.sub & SUB_FREE) {
    free_memory(NULL, ln->data.ptr);
  }

  free_chunk(ln);
  lo->length--;

  return 0;
}

/*end*/
