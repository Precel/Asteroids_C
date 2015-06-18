typedef struct node{
	int data;
	struct node *ptr;
} node;

// Insert element to sorted linked list 
node* insert(int num);

//Print whole list
void print_list();

//Free list memory
void free_list(node *head);

//Return head of linked list
node* get_head();