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

int main(int argc, char** argv)
{
	printf("XETH TESTS\n");
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
	//Nodes
	struct Node * Mynode;
	Mynode = N_Create_From_Link(mylink,sizeof(mylink));
	mylink->name = "HAHA";//Testing for valid node creation
	printf("Node Link[0] Name: %s\nHash: %s\n",Mynode->links[0]->name, Mynode->links[0]->Lcid->hash);
	Mynode = N_Add_Link(&Mynode, mylink2, sizeof(mylink2));
	mylink2->name = "HAHA";//Testing for valid node creation
	printf("Node Link[1] Name: %s\nHash: %s\n",Mynode->links[1]->name,Mynode->links[1]->Lcid->hash);
	struct Link * ResultLink = Node_Get_Link("Simo", Mynode);
	printf("\nResultLink: \nName: %s\nHash: %s\n", ResultLink->name, ResultLink->Lcid->hash);
	Node_Remove_Link("Simo", Mynode);
	printf("Outlinkamt: %d\n", Mynode->link_ammount);
	Free_Link(mylink);
	Free_Link(mylink2);
	Free_Link(ResultLink);
	Node_Delete(Mynode);
	return 0;
}
