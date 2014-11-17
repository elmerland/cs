
typedef struct node node_t;
struct node {
  int     child_pid;
  node_t* next;
};

void add_child(int pid);
void remove_child(int pid);

