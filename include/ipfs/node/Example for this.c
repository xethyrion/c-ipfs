#include "repo/test_repo_config.h"
#include "repo/test_repo_identity.h"
#include "repo/test_repo_bootstrap_peers.h"
#include "repo/test_repo_fsrepo.h"
#include "cmd/ipfs/test_init.h"
#include "cid/test_cid.h"
#include "flatfs/test_flatfs.h"
#include "storage/test_ds_helper.h"
#include "storage/test_datastore.h"
#include "storage/test_blocks.h"
#include "ipfs/node/node.h"
/*TODOS!
 *Incoroparate protobuf funcs
 *Incoroparate multihash funcs
*/
int main(int argc, char** argv)
{
	printf("XETH TESTS\n");
	////Links//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Variables of link:
	char * name = "Alex";
	char * ahash = "QmYwAPJzv5CZsnA625s3Xf2nemtYgPpHdWEz79ojWnPbdG";
	struct Link * mylink;
	mylink = Create_Link(name,ahash);
	printf("===================================\n"	\
		   "Node Link:\n" 							\
		   " -Name: %s\n"   						\
		   " -Size: %lu\n"							\
		   "\n Cid Details:\n\n"					\
		   " -Version: %d\n"						\
		   " -Codec: %c\n"							\
		   " -Hash: %s\n"							\
		   " -Hash Length: %lu\n"  					\
		   "====================================\n" \
		   , mylink->name, mylink->size, mylink->Lcid->version,mylink->Lcid->codec,mylink->Lcid->hash,mylink->Lcid->hash_length);

	//Link Two for testing purposes
	char * name2 = "Simo";
	char * ahash2 = "QmYwAPJzv5CZsnA625s3Xf2nemtYgPpHdWEz79ojWnSimo";
	struct Link * mylink2;
	mylink2 = Create_Link(name2,ahash2);

	////Nodes/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//N_Create_From_Link
	struct Node * Mynode;
	Mynode = N_Create_From_Link(mylink);
	mylink->name = "HAHA";//Testing for valid node creation
	printf("Node Link[0] Name: %s\nHash: %s\n",Mynode->links[0]->name, Mynode->links[0]->Lcid->hash);

	//N_Add_Link
	Mynode = N_Add_Link(&Mynode, mylink2, sizeof(mylink2));
	mylink2->name = "HAHA";//Testing for valid node creation
	printf("Node Link[1] Name: %s\nHash: %s\n",Mynode->links[1]->name,Mynode->links[1]->Lcid->hash);

	//Node_Get_Link
	struct Link * ResultLink = Node_Get_Link(Mynode, "Simo");
	printf("\nResultLink: \nName: %s\nHash: %s\n\n", ResultLink->name, ResultLink->Lcid->hash);

	//Node_Resolve_Links
	struct Link_Proc * LPRX = Node_Resolve_Links(Mynode, "Alex/Simo");
	for(int i=0;i<LPRX->ammount;i++)
	{
		printf("RESOLVE LINKS-Result: %s\n", LPRX->links[i]->name);
	}
	Free_Link_Proc(LPRX);

	//Node_Remove Link

	int StatUs = Node_Remove_Link("Simo", Mynode);
	printf("Status: %d\n",StatUs);
	printf("Outlinkamt: %d\n", Mynode->link_ammount);

	//Node Copy
	struct Node * Node2;
	Node2 = Node_Copy(Mynode);
	printf("NODE COPY TEST: [0]: %s\n", Node2->links[0]->Lcid->hash);
	Node_Delete(Node2);

	//Node_Set_Data
	Node_Set_Data(Mynode, "DATA_HERE");

	//Node_Get_Data
	unsigned char * meh;
	meh = Node_Get_Data(Mynode);
	printf("NODE DATA: %s", meh);


	//Free Everything
	Free_Link(mylink);
	Free_Link(mylink2);
	Free_Link(ResultLink);

	Node_Delete(Mynode);
	return 0;
}
