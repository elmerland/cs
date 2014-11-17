#include "list.h"
#include <stdlib.h>
#include "print-pid.h"

node_t* head = (node_t*)0;

void add_child(int pid){
   node_t* nd = (node_t*)malloc(sizeof(node_t));
   nd->child_pid = pid;
   node_t* old_head = head;
   head = nd;
   print_pid("adding child: %d \n", pid);
   nd->next = old_head;
   
}

void remove_child(int pid) {

    node_t* prev = (node_t*)0;
    node_t* current = head;

    while(current && current->child_pid != pid) {
      prev = current;
      current = current->next;
    }

    if(current == (node_t*)0) { /* child not in list */
      print_pid("Child not in list %d \n", pid);
    }
    else {
      if(prev == (node_t*)0) { /* remove head element */
        head = current->next;
        free(current);
        print_pid("Removed child from list %d \n", pid);
      }
      else { /* remove in body of list */
        prev->next = current->next;
        free(current);
        print_pid("Removed child from list %d \n", pid);
      }  
   }

}

