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
//In progress
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
struct Link * Node_Get_Link(char * Name, struct Node * N)
{
	struct Link * L;
	for(int i=0;i<N->link_ammount;i++)
	{
		if(N->links[i]->name==Name)
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
			printf("FOUND THIS GUY: %s\n", mynode->links[i]->name);
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
struct Node * N_Create_From_Link(struct Link * mylink, size_t linksize)
{
	struct Node * mynode;
	mynode = (struct Node *) malloc(sizeof(struct Node) + linksize);
	mynode->link_ammount = 0;
	mynode->link_ammount++;
	mynode->links[0] = malloc(sizeof(struct Link));
	memcpy(mynode->links[0], mylink, sizeof(struct Link));
	return mynode;
}
//Finished
/*Node_Resolve_Max_Size
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
/*Node_Tree() Basically a unix-like ls
 *@Param1: Result char * foo[strlen(sentence)]
 *@Param2: char sentence[] = foo/bar/bin/whatever
 *Return: 0 if failure, 1 if success
*/
int Node_Tree(char * result, char * input1)
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
