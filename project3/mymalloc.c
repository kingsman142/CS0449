#include <stdio.h>

struct node{
	int size_of_chunk;
	int free_space; //1 = free space; 0 = used space
	struct node* previous_node;
	struct node* next_node;
};

struct node* head;
struct node* curr_node;
struct node* tail;

void  try_coalesce(void*);
void* find_largest_free_space(int);

//Allocate memory of the desired size
void* my_worstfit_malloc(int size){
	void* old_brk = sbrk(0); //Get start address where memory will be placed

	if(head == 0){ //Start the linked list of allocations
		sbrk(size + sizeof(struct node)); //Move the _brk pointer to insert our new head
		head = old_brk;
		head->size_of_chunk = size;
		head->free_space = 0;
		head->next_node = 0;
		head->previous_node = 0;
		tail = head;
		curr_node = head;
	} else{ //Use the worst-fit algorithm to decide the next chunk of space to place the new data
		struct node* free_chunk = find_largest_free_space(size);
		if(free_chunk == 0){ //No free chunk of the desired size was found
			sbrk(size + sizeof(struct node));
			tail = (struct node*) old_brk;
			tail->free_space = 0;
			tail->size_of_chunk = size;
			tail->previous_node = curr_node;
			tail->next_node = 0;

			curr_node->next_node = tail;
			curr_node = tail;
		} else{
			free_chunk->free_space = 0;
			
			//If there is extra space between this new allocation and the next allocation, create a new node
			int new_free_space = free_chunk->size_of_chunk - sizeof(struct node);
			if(new_free_space > (size + sizeof(struct node))){ //Is there enough space to allocate the memory plus a new node?
				struct node* new_node = ((void*) free_chunk) + sizeof(struct node) + size;
				new_node->free_space = 1;
				new_node->size_of_chunk = new_free_space - size;
				new_node->previous_node = free_chunk;
				new_node->next_node = free_chunk->next_node;
				free_chunk->next_node = new_node;
				free_chunk->size_of_chunk = size;
			}
			
			//Return pointer to this free chunk that was found
			return ((void*) free_chunk) + sizeof(struct node);
		}			
	}

	//Return a pointer to the new memory allocated on the heap
	return old_brk + sizeof(struct node);
}

//Using the worst-fit algorithm, find the next free space
void* find_largest_free_space(int space){
	struct node* curr = head;
	struct node* largest_block = 0;

	//Traverse the entire linked list to find the largest block for worst fit	
	while(curr != 0 && curr != tail){
		if(largest_block == 0){
			if(curr->size_of_chunk >= space && curr->free_space == 1){
				largest_block = curr;
			}
		} else{
			if(curr->size_of_chunk > largest_block->size_of_chunk && curr->free_space == 1){
				largest_block = curr;
			}		
		}

		curr = curr->next_node;
	}

	//Check the last node to see if it's the largest block for worst fit
	if(curr != 0 && curr->size_of_chunk >= space && curr->free_space == 1){
		if(largest_block == 0){
			largest_block = curr;
		} else if(curr->size_of_chunk > largest_block->size_of_chunk){
			largest_block = curr;
		}
	}
	
	return largest_block;
}

//Frees a given place in memory
void my_free(void* ptr){
	struct node* curr = ptr - sizeof(struct node);
	curr->free_space = 1; //Mark the chunk as free

	//If the last chunk is free'd, make sure to reduce
	//the size of the heap and free all of the new
	//"last" nodes.
	while(curr != 0 && curr == tail){
		if(curr->free_space != 1) break; //Don't free the memory if it's in use

		tail->next_node = 0;
		tail = tail->previous_node;
		sbrk( -( ((void*) sbrk(0)) - ((void*) curr) ) ); //Move the _brk pointer back to reduce the size of the heap
		curr = tail;
		curr_node = tail; //Make sure curr node is up-to-date
		if(curr == 0){ //If we've cleared all of the memory, factory reset the entire linked list
			head = 0;
			tail = 0;
			break;
		}
	}

	try_coalesce(curr);
}

//When freeing memory, if there are two neighboring free chunks, combine them into one
void try_coalesce(void* addr){
	if(addr == 0) return;

	struct node* curr = (struct node*) addr;
	struct node* prev = curr->previous_node;
	struct node* next = curr->next_node;
	
	//Combine all previous free adjacent chunks relative to the current address
	while(prev != 0 && prev->free_space == 1){
		if(prev->previous_node != 0 && prev->previous_node->free_space == 1){
			prev = prev->previous_node;
		} else{
			break;
		}
	}
	
	//Combine all next free adjacent chunks relative to the current address
	while(next != 0 && next->next_node == 1){
		if(next->next_node != 0 && next->next_node->free_space == 1){
			next = next->next_node;
		} else{
			break;
		}
	}

	//Previous chunk can be coalesced with the current chunk
	if(prev != 0 && prev->free_space == 1){
		if(next != 0 && next->free_space == 1){ //Next chunk can be coalesced with previous and current chunk
			prev->next_node = next->next_node;
			prev->size_of_chunk += curr->size_of_chunk + next->size_of_chunk + 2*sizeof(struct node);
		} else{ //Only coalesce the previous chunk
			prev->next_node = next;
			prev->size_of_chunk += curr->size_of_chunk + sizeof(struct node);
		}
	} else if(next != 0 && next->free_space == 1){ //Only coalesce the next chunk
		curr->next_node = next->next_node;
		curr->size_of_chunk += next->size_of_chunk + sizeof(struct node);
	}

	//Make sure the global curr node is up-to-date
	if(curr_node > tail){
		curr_node = tail;
	}
}
