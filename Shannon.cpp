/************************************************************************/  
/*  File Name: Shanno-Fano.cpp 
*       @Function: Lossless Compression 
@Author: Sophia Zhang 
@Create Time: 2012-9-26 20:20 
@Last Modify: 2012-9-26 20:57 
*/  
/************************************************************************/  
  
#include"iostream"  
#include "queue"  
#include "map"  
#include "string"  
#include "iterator"  
#include "vector"  
#include "algorithm"  
#include "math.h"  
using namespace std;  
  
#define NChar 8 //suppose use 8 bits to describe all symbols  
#define Nsymbols 1<<NChar //can describe 256 symbols totally (include a-z, A-Z)  
#define INF 1<<31-1  
  
typedef vector<bool> SF_Code;//8 bit code of one char  
map<char,SF_Code> SF_Dic; //huffman coding dictionary  
int Sumvec[Nsymbols];   //record the sum of symbol count after sorting  
  
class HTree  
{  
public :  
    HTree* left;  
    HTree* right;  
    char ch;  
    int weight;  
  
    HTree(){left = right = NULL; weight=0;ch ='\0';}  
    HTree(HTree* l,HTree* r,int w,char c){left = l; right = r;  weight=w;   ch=c;}  
    ~HTree(){delete left; delete right;}  
    bool Isleaf(){return !left && !right; }  
};  
  
bool comp(const HTree* t1, const HTree* t2)//function for sorting  
{   return (*t1).weight>(*t2).weight;    }  
  
typedef vector<HTree*> TreeVector;  
TreeVector TreeArr;//record the symbol count array after sorting  
  
void Optimize_Tree(int a,int b,HTree& root)//find optimal separate point and optimize tree recursively  
{  
    if(a==b)//build one leaf node  
    {  
        root = *TreeArr[a-1];  
        return;  
    }  
    else if(b-a==1)//build 2 leaf node  
    {  
        root.left = TreeArr[a-1];  
        root.right=TreeArr[b-1];  
        return;  
    }  
    //find optimizing point x  
    int x,minn=INF,curdiff;  
    for(int i=a;i<b;i++)//find the point that minimize the difference between left and right; this can also be implemented by dichotomy  
    {  
        curdiff = Sumvec[i]*2-Sumvec[a-1]-Sumvec[b];  
        if(abs(curdiff)<minn){  
            x=i;  
            minn = abs(curdiff);  
        }  
        else break;//because this algorithm has monotonicity  
    }  
    HTree*lc = new HTree;   HTree *rc = new HTree;  
    root.left = lc;     root.right = rc;  
    Optimize_Tree(a,x,*lc);  
    Optimize_Tree(x+1,b,*rc);  
}  
  
HTree* BuildTree(int* freqency)//create the tree use Optimize_Tree  
{  
    int i;  
    for(i=0;i<Nsymbols;i++)//statistic  
    {  
        if(freqency[i])  
            TreeArr.push_back(new HTree (NULL,NULL,freqency[i], (char)i));  
    }  
    sort(TreeArr.begin(), TreeArr.end(), comp);  
    memset(Sumvec,0,sizeof(Sumvec));  
    for(i=1;i<=TreeArr.size();i++)  
        Sumvec[i] = Sumvec[i-1]+TreeArr[i-1]->weight;  
    HTree* root = new HTree;  
    Optimize_Tree(1,TreeArr.size(),*root);  
    return root;  
}  
  
/************************************************************************/  
/* Give Shanno Coding to the Shanno Tree 
/*PS: actually, this generative process is same as Huffman coding 
/************************************************************************/  
void Generate_Coding(HTree* root, SF_Code& curcode)  
{  
    if(root->Isleaf())  
    {  
        SF_Dic[root->ch] = curcode;  
        return;  
    }  
    SF_Code lcode = curcode;  
    SF_Code rcode = curcode;  
    lcode.push_back(false);  
    rcode.push_back(true);  
    Generate_Coding(root->left,lcode);  
    Generate_Coding(root->right,rcode);  
}  
  
int main()  
{  
    int freq[Nsymbols] = {0};  
    char *str = "bbbbbbbccccccaaaaaaaaaaaaaaaeeeeedddddd";//15a,7b,6c,6d,5e  
  
    //statistic character frequency  
    while (*str!='\0')      freq[*str++]++;  
  
    //build tree  
    HTree* r = BuildTree(freq);  
    SF_Code nullcode;  
    Generate_Coding(r,nullcode);  
  
    for(map<char,SF_Code>::iterator it = SF_Dic.begin(); it != SF_Dic.end(); it++) {    
        cout<<(*it).first<<'\t';    
        std::copy(it->second.begin(),it->second.end(),std::ostream_iterator<bool>(cout));    
        cout<<endl;    
    }    
}  