/*
templated linked list class
I wrote this myself, there's probably a nicer way to do this, but I really don't know how
Anyone who wants to improve this may do so
*/

#include <iostream>

using namespace std;

//Basic node
template <typename T>
class node
{
public:
    //Pointer to the next item in the list
    node<T> *next;
    //Stored value
    T *variable;
    
    node()
    {
    	next = 0;
    	variable = 0;
    }
};

//Linked List class
template <typename T>
class LinkedList
{
private:
    //First node for keeping track of the whole list
    node<T> *FirstNode;
    //Keep track of the number of items
    int Count;
    
public:
    //Set count to 0 and FirstNode to NULL
    LinkedList()
    {
    	Count = 0;
    	FirstNode = 0;
    };
    
    //KILL IT
    ~LinkedList()
    {
    	delete FirstNode;
    }
    
    //For adding an item to the end of the list
    void AddNode( node<T> *n )
    {
		//Create a node that will traverse the list
    	node<T> *current;
    			
		//If the firstnode is null then it will get the value specified by the parameter
    	if( FirstNode == 0 )
    	{			
	    	FirstNode = n;
    	}
    	else
    	{
			//set current to the first value 			
			current = FirstNode;
			
			//Until the next value is null traverse the list
			while( current->next != 0 )
			current = current->next;
			
			//Set the next value to the value specified by the parameter
			current->next = n;
    	}
    	
		//Increase the counter
    	Count++;
    };
    
    //Does the same actions as the other AddNode but does not require a node
    //Creates the node to add from a variable instead
    void AddNode(T *var) 
    {
    	node<T> *newnode, *current;
    	newnode = new node<T>;
    	newnode->variable = var;
    	newnode->next = 0;
    	
    	if( FirstNode == 0 )
    	{			
			FirstNode = new node<T>;
			FirstNode->next = 0;
			FirstNode->variable = var;
    	}
    	else
    	{			
	    	current = FirstNode;
	    	while( current->next != 0 )
				current = current->next;
			current->next = newnode;
    	}
    	
    	Count++;
    };
    
    //Remove a node at the specified index
    //Reroute the pointers to keep the list intact with the exception of the removed node
    //Returns the removed node
    node<T>* RemoveNode( int index )
    {		
		//Check to see if the index is within the range of the list
		//Exit if it is not
    	if ( !(index >= 0 && index < Count) )
		    return 0;
    		
		//If the index is the firstnode
    	if ( index == 0 )
    	{
			//set a temporary value to the firstnode
			node<T> *temp = FirstNode;
    	
			//if there's nothing else in the list
			if ( FirstNode->next == 0 )
				//Set the first node to null
				FirstNode = 0;
			else
				//Set firstnode to its next value
				FirstNode = FirstNode->next;
    		
			//lower the counter
			Count--;
			
			//Return the temporary value
			return temp;
    	}
    	else
    	{
			//A counter for finding the node at the position specified
			//by the index parameter
    	    int counter = 0;
	    
			//create current node for traversal
			//previous node will reference the node before current
			//in the list
    	    node<T> *current = FirstNode;
    	    node<T> *previous = current;
    	
	   		//Loop until counter == index
    	    while ( counter < index )
    	    {
				//set previous to current
				previous = current;
				//advance current
				current = current->next;
				//increase the counter
				counter++;
    	    }
    	
		    //set the next of previous to the next of current thus removing current from the list
    	    previous->next = current->next;
    
    	    Count--;
    	
    	    return current;
    	}	
    };
    
    //Similar to above RemoveNode but compares the variable value
    //specified by the parameter instead
    node<T>* RemoveNode( T *var )
    {
    	node<T> *current = FirstNode;
    	node<T> *previous = current;
    	
    	if ( current == 0 )
    		return 0;
    	else
    	{
    		while ( current != 0 )
    		{
    			if ( current->variable == var )
    			{
    				if ( current == FirstNode )
    				{
    					node<T> *temp = FirstNode;
    					FirstNode = FirstNode->next;
    					return temp;
    				}
    				else
    				{					
    					previous->next = current->next;
    					return current;
    				}
    				
    				Count--;
    			}
    			
    			previous = current;
    			current = current->next;
    		}
    	}
    	
    	//Not found
    	return 0;
    }
    
    //Return node at given index
    node<T>* GetNode( int index )
    {
    	//Determine if index is valid
    	if ( !( index >= 0 && index < Count ) )
    	{
    		cout << FirstNode;
    		return 0;
    	}
    	
    	//Counter and slider for traversing list	
    	int counter = 0;
    	node<T> *current = FirstNode;
    	
    	//Slide to index position
    	while ( counter < index )
    	{
    		current = current->next;
    		counter++;
    	}
    	
    	//Return the node
    	return current;
    }
    
    int GetCount() { return Count; };
    
    //For testing purposes
    void PrintList()
    {
    	node<T> *current = FirstNode;
    				
    	if ( FirstNode != 0 )
    	{
    		cout << current->variable << " ";
    		
    		while ( current->next != 0 )
    		{
    			current = current->next;
    			cout << current->variable << " ";
    		}
    	}
    };
};
