/**
 * An implementation of an IPFS node
 * Copying the go-ipfs-node project
 */
#ifndef IPFS_NODE_H
#define IPFS_NODE_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inttypes.h"
#include "ipfs/cid/cid.h"

int debug = 0; // Set to 1 to enable debug mode of this program

/*====================================================================================
 *
 * Structures
 *
 *===================================================================================*/

struct Link
{
	char * name;
	size_t size;
	struct Cid * Lcid;
};
struct Node
{
	unsigned char * data;
	unsigned char * encoded;
	struct Cid * cached;
	int link_ammount;
	struct Link * links[];
};
/*====================================================================================
 *
 * Functions
 *
 *===================================================================================*/

/*====================================================================================
 * Link Functions
 *===================================================================================*/
/* Create_Link
 * @Param name: The name of the link (char *)
 * @Param size: Size of the link (size_t)
 * @Param ahash: An Qmhash
 */
struct Link * Create_Link(char * name, unsigned char * ahash)
{
	struct Link * mylink;
	mylink = malloc(sizeof(struct Link));
	mylink->name = name;
	int ver = 0;
	size_t lenhash = strlen(ahash)-1;
	ipfs_cid_new(ver, ahash, lenhash*2, CID_PROTOBUF, &mylink->Lcid);
	mylink->size = sizeof(mylink) + mylink->Lcid->hash_length; //Unsure of this
	return mylink;
}
/* Free_Link
 * @param L: Free the link you have allocated.
 */
void Free_Link(struct Link * L)
{
	ipfs_cid_free(L->Lcid);
	free(L);
}
/*====================================================================================
 * Node Functions
 *===================================================================================*/
/*Create_Empty_Node
 * Creates an empty node, allocates the required memory
 * Returns a fresh new node with no data set in it.
 */
struct Node * Create_Empty_Node()
{
	struct Node * N;
	N = (struct Node *)malloc(sizeof(struct Node));
	return N;
}
/*Node_Set_Data
 * Sets the data of a node
 * @param Node: The node which you want to set data in.
 * @param Data, the data you want to assign to the node
 * returns 1 on success 0 on failure
 */
int Node_Set_Data(struct Node * N, unsigned char * Data)
{
	if(!N || !Data)
	{
		return 0;
	}
	N->encoded = NULL;
	N->cached = NULL;
	N->data = Data;
	return 1;
}
/*Node_Get_Data
 * Gets data from a node
 * @param Node: = The node you want to get data from. (unsigned char *)
 * Returns data of node.
 */
unsigned char * Node_Get_Data(struct Node * N)
{
	unsigned char * DATA;
	DATA = N->data;
	return DATA;
}

/*Node_Copy: Returns a copy of the node you input
 * @param Node: The node you want to copy (struct CP_Node *)
 * Returns a copy of the node you wanted to copy.
 */
struct Node * Node_Copy(struct Node * CP_Node)
{
	struct Node * CN;
	CN = (struct Node*) malloc(sizeof(struct Node) + sizeof(struct Link) * 2);
	if(CP_Node->link_ammount != 0)
	{
		for(int i=0; i<CP_Node->link_ammount; i++)
		{
			CN->links[i] = malloc(sizeof(struct Link));
		}
	}
	memcpy(CN, CP_Node, sizeof(struct Node));
	memcpy(CN->links[0],CP_Node->links[0], sizeof(struct Link));
	return CN;
}
/*Node_Delete
 * Once you are finished using a node, always delete it using this.
 * It will take care of the links inside it.
 * @param N: the node you want to free. (struct Node *)
 */
void Node_Delete(struct Node * N)
{
	if(N->link_ammount > 0)
	{
		for(int i=0; i<N->link_ammount; i++)
		{
			free(N->links[i]);
		}
	}
	free(N);
}
/*Node_Get_Link
 * Returns a copy of the link with given name
 * @param Name: (char * name) searches for link with this name
 * Returns the link struct if it's found otherwise returns NULL
 */
struct Link * Node_Get_Link(struct Node * N, char * Name)
{
	struct Link * L;
	for(int i=0;i<N->link_ammount;i++)
	{
		if(strcmp(N->links[i]->name,Name) == 0)
		{
			L = (struct Link *)malloc(sizeof(struct Link));
			memcpy(L,N->links[i],sizeof(struct Link));
			int ver = L->Lcid->version;
			char * ahash = L->Lcid->hash;
			size_t lenhash = L->Lcid->hash_length;
			ipfs_cid_new(ver, ahash, lenhash, CID_PROTOBUF, &L->Lcid);
			return L;
		}
	}
	return NULL;
}
/*Node_Remove_Link
 * Removes a link from node if found by name.
 * @param name: Name of link (char * name)
 * returns 1 on success, 0 on failure.
 */
int Node_Remove_Link(char * Name, struct Node * mynode)
{
	for(int i=0; i<mynode->link_ammount; i++)
	{
		if(mynode->links[i]->name == Name)
		{
			for(int x=i;x<mynode->link_ammount && x+1 != mynode->link_ammount;i++)
			{
				memcpy(mynode->links[x],mynode->links[x+1],sizeof(struct Link));
			}
			free(mynode->links[mynode->link_ammount-1]);
			mynode->link_ammount--;
			return 1;
		}
	}
	return 0;
}
/* N_Add_Link
 * Adds a link to your node
 * @param mynode: &yournode
 * @param mylink: the CID you want to create a node from
 * @param linksz: sizeof(your cid here)
 * Returns your node with the newly added link
 */
struct Node * N_Add_Link(struct Node ** mynode, struct Link * mylink, size_t linksz)
{
	struct Node * Nl = *mynode;
	Nl->link_ammount++;
	size_t calculatesize = 0;
	if(Nl->link_ammount != 0)
	{
		for(int i=0; i<Nl->link_ammount-1;i++)
		{
			calculatesize = calculatesize + sizeof(Nl->links[i]);
		}
		calculatesize = calculatesize + linksz;
		Nl = (struct Node *) realloc(Nl, sizeof(struct Node) + calculatesize);
	}
	else
	{
		Nl = (struct Node *) malloc(sizeof(struct Node) + linksz);
	}
	Nl->links[Nl->link_ammount-1] = malloc(sizeof(struct Link));
	memcpy(Nl->links[Nl->link_ammount-1],mylink,sizeof(struct Link));
	return Nl;
}

/*N_Create_From_Link
 * Create a node from a link
 * @param mylink: the link you want to create it from. (struct Cid *)
 * @param linksize: sizeof(the link in mylink) (size_T)
 * Returns a fresh new node with the link you specified. Has to be freed with Node_Free preferably.
 */
struct Node * N_Create_From_Link(struct Link * mylink)
{
	struct Node * mynode;
	mynode = (struct Node *) malloc(sizeof(struct Node) + sizeof(struct Link));
	mynode->link_ammount = 0;
	mynode->link_ammount++;
	mynode->links[0] = malloc(sizeof(struct Link));
	memcpy(mynode->links[0], mylink, sizeof(struct Link));
	return mynode;
}
/*N_Create_From_Data
 * @param data: bytes buffer you want to create the node from
 * returns a node with the data you inputted.
 */
struct Node * N_Create_From_Data(unsigned char * data)
{
	struct Node * mynode;
	mynode = (struct Node *) malloc(sizeof(struct Node));
	mynode->data = data;
	return mynode;
}
/*Node_Resolve_Max_Size
 * !!!This shouldn't concern you!
 *Gets the ammount of words that will be returned by Node_Resolve
 *@Param1: The string that will be processed (eg: char * sentence = "foo/bar/bin")
 *Returns either -1 if something went wrong or the ammount of words that would be processed.
*/
int Node_Resolve_Max_Size(char * input1)
{
	if(!input1)
	{
		return -1; // Input is null, therefor nothing can be processed.
	}
	char input[strlen(input1)];
	bzero(input, strlen(input1));
	strcpy(input, input1);
	int num = 0;
	char * tr;
	char * end;
	tr=strtok_r(input,"/",&end);
	for(int i = 0;tr;i++)
	{
		tr=strtok_r(NULL,"/",&end);
		num++;
	}
	return num;
}

/*Node_Resolve Basically stores everything in a pointer array eg: char * bla[Max_Words_]
 * !!!This shouldn't concern you!!!
 *@param1: Pointer array(char * foo[x], X=Whatever ammount there is. should be used with the helper function Node_Resolve_Max_Size)
 *@param2: Sentence to gather words/paths from (Eg: char * meh = "foo/bar/bin")
 *@Returns 1 or 0, 0 if something went wrong, 1 if everything went smoothly.
*/

int Node_Resolve(char ** result, char * input1)
{
	if(!input1)
	{
		return 0; // Input is null, therefor nothing can be processed.
	}
	char input[strlen(input1)];
	bzero(input, strlen(input1));
	strcpy(input, input1);
	char * tr;
	char * end;
	tr=strtok_r(input,"/",&end);
	for(int i = 0;tr;i++)
	{
		if(debug == 1)
		{
			printf("TR: %s\n", tr);
		}
		result[i] = (char *) malloc(strlen(tr)+1);
		strcpy(result[i], tr);
		tr=strtok_r(NULL,"/",&end);
	}
	return 1;
}
/**************************************************************************************************************************************
 *|||||||||||||||||||||||||||||||||||||||| !!!! IMPORTANT !!! ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*
 **************************************************************************************************************************************
 * Not sure where this is used, I'm making something to easen it up for all of you.
 * This in itself will get all the links for you in a link[] array inside Link_Proc
 * the memory allocation for storage will be noted in the ammount of links.
 * After being done with this, you have to free the array for which you will have a function specially made for you.
 *
 * What this does:
 * It searches for links using a path like /foo/bar/bin/, if links with those names are found in the node you specify, it stores them
 * in a custom struct, Link_Proc; which is what Node_Resolve_Link returns.
 * Notes:
 * Use it, free it, it's all already laid out for you.
 * There will also be a tutorial demonstrating it in the same folder here so everyone can understand this.
*/
struct Link_Proc
{
	char * remaining_links; // Not your concern.
	int ammount; //This will store the ammount of links, so you know what to process.
	struct Link * links[]; // Link array
};
/*Node_Resolve_Links
 * Processes a path returning all links.
 * @param N: The node you want to get links from
 * @param path: The "foo/bar/bin" path
 */
struct Link_Proc * Node_Resolve_Links(struct Node * N, char * path)
{
	if(!N || !path)
	{
		return NULL;
	}
	int expected_link_ammount = Node_Resolve_Max_Size(path);
	struct Link_Proc * LProc = (struct Link_Proc *) malloc(sizeof(struct Link_Proc) + sizeof(struct Link) * expected_link_ammount);
	LProc->ammount = 0;
	char * linknames[expected_link_ammount];
	Node_Resolve(linknames, path);
	for(int i=0;i<expected_link_ammount; i++)
	{
		struct Link * proclink;
		proclink = Node_Get_Link(N, linknames[i]);
		if(proclink)
		{
			LProc->links[i] = (struct Link *)malloc(sizeof(struct Link));
			memcpy(LProc->links[i], proclink, sizeof(struct Link));
			LProc->ammount++;
			free(proclink);
		}
	}
	//Freeing pointer array
	for(int i=0;i<expected_link_ammount; i++)
	{
		free(linknames[i]);
	}
	return LProc;
}
/*Free_link_Proc
 * frees the Link_Proc struct you created.
 * @param1: Link_Proc struct (struct Link_Proc *)
 */
void Free_Link_Proc(struct Link_Proc * LPRC)
{
	if(LPRC->ammount != 0)
	{
		for(int i=0;i<LPRC->ammount;i++)
		{
			Free_Link(LPRC->links[i]);
		}
	}
	free(LPRC);
}

/*Node_Tree() Basically a unix-like ls
 *@Param1: Result char * foo[strlen(sentence)]
 *@Param2: char sentence[] = foo/bar/bin/whatever
 *Return: 0 if failure, 1 if success
*/
int Node_Tree(char * result, char * input1) //I don't know where you use this but here it is.
{
	if(!input1)
	{
		return 0;
	}
	char input[strlen(input1)];
	bzero(input, strlen(input1));
	strcpy(input, input1);
	for(int i=0; i<strlen(input); i++)
	{
		if(input[i] == '/')
		{
			input[i] = ' ';
		}
	}
	strcpy(result, input);
	if(debug == 1)
	{
		printf("Node_Tree Internal Result: %s\n",result);
	}
	return 1;
}
#endif
