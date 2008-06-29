#include <iostream>

using namespace std;

template <typename T>
class node
{
public:
	node<T> *next;
	T *variable;
	
	node()
	{
		next = 0;
		variable = 0;
	}
};

template <typename T>
class LinkedList
{
private:
	node<T> *FirstNode;
	int Count;
	
public:
	LinkedList()
	{
		Count = 0;
		FirstNode = 0;
	};
	
	~LinkedList()
	{
		delete FirstNode;
	}
	
	void AddNode( node<T> *n )
	{
		node<T> *current;
				
		if( FirstNode == 0 )
		{			
			FirstNode = n;
		}
		else
		{			
			current = FirstNode;
			
			while( current->next != 0 )
				current = current->next;
			current->next = n;
		}
		
		Count++;
	};
	
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
	
	node<T>* RemoveNode( int index )
	{		
		if ( !(index >= 0 && index < Count) )
			return 0;
			
		if ( index == 0 )
		{
			node<T> *temp = FirstNode;
			
			if ( FirstNode->next == 0 )
				FirstNode = 0;
			else
				FirstNode = FirstNode->next;
				
			Count--;
			
			return temp;
		}
		else
		{
			int counter = 0;
			node<T> *current = FirstNode;
			node<T> *previous = current;
			
			while ( counter < index )
			{
				previous = current;
				current = current->next;
				counter++;
			}
			
			previous->next = current->next;
		
			Count--;
			
			return current;
		}	
	};
	
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
	
	node<T>* GetNode( int index )
	{
		if ( !( index >= 0 && index < Count ) )
		{
			cout << FirstNode;
			return 0;
		}
			
		int counter = 0;
		node<T> *current = FirstNode;
		
		while ( counter < index )
		{
			current = current->next;
			counter++;
		}
		
		return current;
	}
	
	int GetCount() { return Count; };
	
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
