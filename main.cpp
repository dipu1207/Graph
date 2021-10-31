#include <iostream>

#include <bits/stdc++.h>
#include<fstream>
using namespace std;
#define infinity 0x3f3f3f3f

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
class node{
   private:
		int source;
		int dest;
		int type;
	public:
	 	node(int s,int d,int t)
			{
	   			source=s;
	  		 	dest=d;
       			type=t;
			}
   		int  getsource()
    	{
    			return source;
		}

		int  getdest()
    	{
    		return dest;
		}
		int  gettype()
    	{
    		return type;
		}
};

vector<node> edgetype;
class graph{
	int V,E;

	public:
		vector<vector<int>>component;
		vector<int>headComponent; 
		vector<int>taken; // to store info about vertices which are disjoint
		vector<pair<int ,int>>*adj;
        vector<pair<int,int>>*adjreverse;
        vector<pair<int ,int>>*tempadj;
        
		void addEdge(int u,int v,int wt);
	    void addtempEdge(int u,int v, int wt);
		ofstream dfsPointer;
		 
		void set(int ver,int edge)
		{
			 V=ver;
	         E=edge;
	         adj=new vector<pair<int ,int>>[V];
	         adjreverse=new vector<pair<int,int>>[V];     
	         tempadj=new vector<pair<int ,int>>[V];
	         
			 dfsPointer.open("dfs_graph.gv");
	          
		    
	   	     }
	   	 
		    

	     
	void graphviz(int V);
    void TarjansSCC();
    void SCCfind(int i , int disc[],int low[],stack<int>*path,bool memberStack[],int &time,vector<vector<int>>&component);
    void DFS();
    void DFStraverse(int u,bool visited[],int startTime[],int endTime[],int dfsTraversal[],int finished[]);
    void dijkstra(int u,int v);
    void graphtype();
    void dfsOriginal(int i,bool dfsoriginal[],bool visited[]);
    void dfsReverse(int i,bool dfsreverse[],bool visited[]);
    void printSCC();
    void compressedGraph();
};
// to add edges in graph
void graph::addEdge(int u,int v, int w)
{

	adj[u].push_back(make_pair(v,w));
     adjreverse[v].push_back(make_pair(u,w));
}
//to add edges in temp list for compressed graph
void graph::addtempEdge(int u, int v, int wt)
{
	tempadj[u].push_back(make_pair(v,wt));
}
//to print the graph created 
void graph::graphviz(int V)
{
	//this array will take care of those vertices which has no edges
	// int taken[V]={0}; 
	vector<int>taken(V);
    taken.assign(V,0);
	ofstream Fp;
	Fp.open("graphviz.gv");
	Fp<<"digraph G {"<<"\n";
	for(int i=0;i<V;i++)
	{
	    for (auto it = adj[i].begin(); it!=adj[i].end(); it++)
	    {
	    	taken[i]=1;
	    	taken[it->first]=1;
	    	Fp<<i<<"->"<<it->first<<"[label=\""<<it->second<<"\"];\n";
		}
		Fp<<"\n";
	}
   this->taken=taken;
	//now run a loop to check if there are any nodes with no edge 
	for(int i=0;i<V;i++)
	   {
	   	if(taken[i]==0)
	   	 Fp<<i<<"\n";
	   	 
	   }
Fp<<"\n"<<"}";
Fp.close();

cout<<"\n a output image has been created for this directed graph using graphviz naming output_graph\n"<<endl;
string cmd="dot.exe -Tpng graphviz.gv -o Output_graph.png";
	 system((const char*)cmd.c_str());

	
}
void graph::printSCC()
{
	//this function will use  adj list , headComponent, and component vectors
	ofstream sccpointer;
	sccpointer.open("SCCgraphviz.gv");
	sccpointer<<"digraph G {"<<endl;
	int dest;

	for(int src=0;src<V;src++)
	{	// read the adjacency list 
	  for(auto itr=adj[src].begin();itr!=adj[src].end();itr++)
	  {
	    	dest=itr->first;
	    	if(headComponent[src]==headComponent[dest])
	            sccpointer<<src<<"->"<<dest<<"[label=\""<<itr->second<<"\" color=blue];\n";
	    	else
		             sccpointer<<src<<"->"<<dest<<"[label=\""<<itr->second<<"\" color=grey style=dashed];\n";
	    	  
	      }
	     if(taken[src]==0)
		   sccpointer<<src<<"[color=green]"<<"\n";   
      	    
		}
	sccpointer<<"}";
	sccpointer.close();
  string cmd="dot.exe -Tpng SCCgraphviz.gv -o Output_SCCgraph.png";
	 system((const char*)cmd.c_str());
	
}
//utility function for tarjens strongly connected component
void graph::SCCfind(int i , int disc[],int low[],stack<int>*path,bool memberStack[],int &time,vector<vector<int>> &component)
{
	  static int row=0;
   
	disc[i]=low[i]= ++time;
	//push the node in stack 
	path->push(i);
	memberStack[i]=true;
	//now we take each node adjacent to current node
	for(auto iterator=adj[i].begin();iterator!=adj[i].end();iterator++)
	{
		int currAdj=iterator->first;  //currAdj is current adjacent of node 
		//if the node is not visited then call SCCfind for that
		if(disc[currAdj]==-1)
          {
          	      SCCfind(currAdj,disc,low,path,memberStack,time,component);
			 	
		          low[i]=min(low[i],low[currAdj]);
			 }   
			 
		else if(memberStack[currAdj]==true)
		      low[i]=min(low[i],disc[currAdj]);	 	 	
	}
	
	int elementStack=0;//to store element of stack
	if(low[i]==disc[i])
	{
		while(path->top()!=i)
		{
			 elementStack=(int)path->top();
			 //print the element of component
			 //cout<<elementStack<<" \t";
			 memberStack[elementStack]=false;
			
		     component[row].push_back(elementStack);
	  
			 path->pop();
		}
		elementStack=(int)path->top();
	  // cout<<elementStack<<"\n";
	   component[row].push_back(elementStack);
	 	memberStack[elementStack]=false;
	    path->pop();
        row++;
	}
  
}//end of SCCfind

//to find strongly connected components
void graph::TarjansSCC()
{
   
       component.clear();
       headComponent.clear();
     
	//first create arrays to store the low and disc of each node
	int time=0;
	int * disc=new int[V];
	int * low=new int[V];
	// create a array to find if the node is in stack or not
	bool * memberStack=new bool[V];
	vector<vector<int>>component(V);
	//initialise the arrays
	for(int i=0;i<V;i++)
	{
		memberStack[i]=false;
		disc[i]=-1; //-1 means the values are not calculated
		low[i]=-1;
	}
	//to store all cooonected ancestor 
	stack<int> *path = new stack<int>();
	//now we call the function to find strongly connected component with vertex i 
	cout<<"the strongly connected component graph is printed using graphviz naming output_SCCgraph"<<endl;
	for(int i=0;i<V;i++)
	 {
	 	if(disc[i]==-1)
	 	{
	 		SCCfind(i,disc,low,path,memberStack,time,component);
		 }
	 }
	 
	 

this->component=component;
//create a array to store component no of each node 
//nodes in same component have same no
vector<int>headComponent(V,0);
   int comp=0;
for(int i=0;i<component.size();i++)
{
	
	for(int j=0;j<component[i].size();j++)
	{
           	headComponent[component[i][j]]=comp;	
	}
	comp++;
}
this->headComponent=headComponent;

//Call print graph with coloring edges of component
   printSCC();

}//end of tarjans algo for SCC

//dijkstra algo using priority queue
void graph::dijkstra(int u,int v)
{
	//check if source and destination are valid
	if(u<0&&u>=V&&v<0&&v>=V)
	  {
	  	cout<<"source and destination are not valid \n";
	  	return;
	  }
	//create a array to store the value of distances 
	vector<int>distance(V,infinity); // initially all the distances is infinity 
	// now create a priority queue using stl 
	priority_queue<pair<int,int>,vector<pair<int,int>>, greater<pair<int,int>>>pqueue;
	//insert source into priority queue 
	pqueue.push(make_pair(0,u));
	//set the distance of source as 0
	distance[u]=0;
	while(!pqueue.empty())
	{
		int curr=pqueue.top().second;//here the distance of the node is taken 
		pqueue.pop();
	
	for(auto itr=adj[curr].begin();itr!=adj[curr].end();itr++)
	{
		int vert=itr->first;
		int wei=itr->second;
		if(distance[vert]>distance[curr]+wei)
		{
			distance[vert]=distance[curr]+wei;
			pqueue.push(make_pair(distance[vert],vert));
		}
	  }
     }
	 //print the distances
	    for(int i=0;i<V;i++)
	       {
	       	if(distance[i]==infinity)
	          	cout<<i<<"\t"<<"infinity"<<endl;
	       	else
			 cout<<i<<"\t"<<distance[i]<<endl;
		   }
	       if(distance[v]!=infinity)
	    cout<< "the distance between\t"<<u <<"\tand\t"<<v<<"\t=\t"<<distance[v]<<endl;  
        else
		 cout<< "the distance between\t"<<u <<"\tand\t"<<v<<"\t=\t"<<"infinity"<<endl;  
         
}
//function to find dfs of nodes
void graph::DFStraverse(int u,bool visited[],int startTime[],int endTime[],int dfsTraversal[],int finished[])
{
	
    static int index=0;
	static int time=1;
    
   //mark the node visited
   visited[u]=true;
   // add this node to dfs traverse array
    dfsTraversal[index++]=u;
   //set the start time of node
    startTime[u]= time++;
    // now traverse for each neighbour of the curr node
   
    for(auto iterator=adj[u].begin();iterator!=adj[u].end();iterator++)
    {
    	//neigh is the current neighbour of the node whose dfs tree is being created
        int    neigh=iterator->first;
      // if the neigh node is not visited  then it must be the forward edge
	    if(visited[neigh]==false)
         {
         	//first print the node as front edge
         	cout<<"tree edge   "<< u<<"  -->  "<<neigh<<"\n";
         	//also write it in a file
         	//dfsPointer<<u<<" -> "<<neigh<<"[label = Tree_Edge color= blue]"<<endl;
         	//call dfs traversal on neigh node
         	node obj= node(u,neigh,0);
         	edgetype.push_back(obj);
         	DFStraverse(neigh,visited,startTime,endTime,dfsTraversal,finished);
		 }
		 //here neigh node has already been traversed
		 else{
		 	   //now we need to find out of three different edge types
		 	   if((startTime[u]>startTime[neigh])&& finished[neigh]==0)
		 	      {
		 	      	//print back edge and also write in the file
		 	      	cout<<"back edge  "<< u<<"  -->  "<<neigh<<"\n";
				//     dfsPointer<<u<<" -> "<<neigh<<"[label = Back_Edge color= orange]"<<endl;
				   	node obj= node(u,neigh,2);
         	        edgetype.push_back(obj);
				   }
				   
			
				   
			else if((startTime[u]>startTime[neigh] )&& ((endTime[u]>endTime[neigh] && endTime[neigh]!=0)||(endTime[u]==0 && endTime[neigh]>0)))
			{
			    cout<<"cross edge   "<< u<<"  -->  "<<neigh<<"\n";	
			   //  dfsPointer<<u<<" -> "<<neigh<<"[label = Cross_Edge color= grey]"<<endl;
			    	node obj= node(u,neigh,3);
                 	edgetype.push_back(obj);
			}
				 else 
				 {
				 	cout<<"forward edge  "<< u<<"  -->  "<<neigh<<"\n";
				    //dfsPointer<<u<<" -> "<<neigh<<"[label = Forward_Edge color= green]"<<endl;
				 	node obj= node(u,neigh,1);
         	        edgetype.push_back(obj);
				 }  
			
		 }
		 
		 
	 }
		 endTime[u]=time++;
		 finished[u]=1;
	
}//end of DFStraverse

//function to calculate dfs of graph
void graph::DFS()
{
	
	//create array to store start time , end time, and visited or not 
	bool *visited=new bool[V];
	int * startTime=new int[V];
	int * dfsTraversal=new int[V];
	int * endTime=new int[V];
	// create an array to store finished info of a vertex
	int * finished=new int[V];
	//initialize each array
	for(int i=0;i<V;i++)
      {
      	visited[i]=false;
      	startTime[i]=0;
      	endTime[i]=0;
      	dfsTraversal[i]=-1;
      	finished[i]=0;
    }
    //write info in a file 
    
    //call dfstraversal for each vertex so that if there are more that one component then they are also included
	for(int i=0;i<V;i++)
	{
		if(!visited[i])
		   DFStraverse(i,visited,startTime,endTime,dfsTraversal,finished);
	}
	//print dfs traversal
	cout<<"\n dfs traversal of the directed graph is"<<endl;
	for(int i=0;i<V;i++)
	  cout<<dfsTraversal[i]<<"\t";
    cout<<"\n\n\n start time of the graph from vertex 0 to V is "<<endl;
 	for(int i=0;i<V;i++)
	  cout<<startTime[i]<<"\t";
    cout<<"\n\n finish time of the graph form vertex 0 to V is"<<endl;
	for(int i=0;i<V;i++)
	  cout<<endTime[i]<<"\t";

 dfsPointer<<"digraph G {\n";
     for(auto itr=edgetype.begin();itr!=edgetype.end(); itr++)
	 {
	 	if(itr->gettype() ==0)
	 	 dfsPointer<<"\""<<itr->getsource()<<"("<<startTime[itr->getsource()]<<","<<endTime[itr->getsource()]<<")\""<<"->"<<"\""<<itr->getdest()<<"("<<startTime[itr->getdest()]<<","<<endTime[itr->getdest()]<<")\""<<"[label = Tree_Edge color= blue]"<<endl; 
		
		else if(itr->gettype() ==1)
	 	 dfsPointer<<"\""<<itr->getsource()<<"("<<startTime[itr->getsource()]<<","<<endTime[itr->getsource()]<<")\""<<"->"<<"\""<<itr->getdest()<<"("<<startTime[itr->getdest()]<<","<<endTime[itr->getdest()]<<")\""<<"[label = forward_edge color= green ]"<<endl; 
		else if(itr->gettype() ==2)
	 	 dfsPointer<<"\""<<itr->getsource()<<"("<<startTime[itr->getsource()]<<","<<endTime[itr->getsource()]<<")\""<<"->"<<"\""<<itr->getdest()<<"("<<startTime[itr->getdest()]<<","<<endTime[itr->getdest()]<<")\""<<"[label = back_edge color= black style=dashed]"<<endl; 
	     else if(itr->gettype() ==3)
	 	 dfsPointer<<"\""<<itr->getsource()<<"("<<startTime[itr->getsource()]<<","<<endTime[itr->getsource()]<<")\""<<"->"<<"\""<<itr->getdest()<<"("<<startTime[itr->getdest()]<<","<<endTime[itr->getdest()]<<")\""<<"[label = cross_edge color= grey]"<<endl; 
			  		  	  	  
			   }
			             
  //now we will check the taken array to print those vertices which has in and out degree 0
  for(int i=0;i<V;i++)
  {
  	if(taken[i]==0)
  	  dfsPointer<<i<<"[color= purple]\n";
  }
dfsPointer<<"\n}";


dfsPointer.close();
cout<<"\n a graph is also created naimg output_DFSgraph\n";
string cmd="dot.exe -Tpng dfs_graph.gv -o Output_dfs_graph.png";
	 system((const char*)cmd.c_str());

}
void graph::dfsOriginal(int i ,bool dfsoriginal[],bool visited[])
{
      visited[i]=true;
      dfsoriginal[i]=true;
	  //check for adjacent of current node
	  for(auto itr =adj[i].begin();itr!=adj[i].end();itr++)
	  {
	  	   int curr=itr->first;
	  	   if(visited[curr]==false)
	  	      dfsOriginal(curr,dfsoriginal,visited);
			  
		  }	
   
}
void graph::dfsReverse(int i ,bool dfsreverse[],bool visited[])
{
	 visited[i]=true;
      dfsreverse[i]=true;
	  //check for adjacent of current node
	  for(auto itr =adjreverse[i].begin();itr!=adjreverse[i].end();itr++)
	  {
	  	   int curr=itr->first;
	  	   if(visited[curr]==false)
	  	      dfsReverse(curr,dfsreverse,visited);
			  
		  }	
}
//function to determine whether graph is semiconnected or not

void graph::graphtype()
{
	//here we will create two arrays to store the dfs of a vertex
	bool *dfsoriginal=new bool[V];
	bool *dfsreverse=new bool[V];
	
	//create a array visited to store which vertex is visited
    bool *visited=new bool[V];
	//create array to store union of dfs 
	bool * unionarray = new bool[V];
	for(int p=0;p<V;p++)
    {
    	dfsoriginal[p]=false;
    	dfsreverse[p]=false;
	     visited[p]=false;
	}
   
    //run the loop for each vertex 
	for(int i=0;i<V;i++)
	{
	    // call dfs function with i as start node in both graph and reverse graph	
	    dfsOriginal(i,dfsoriginal,visited);
	   
	    //clear visited array
	    for(int i=0;i<V;i++)
	    { visited[i]=false;}
	    dfsReverse(i,dfsreverse,visited);
	    
		//after the function call we find union of these to arrays and store it in new array 
	    //if there is any element which has not been covered then its not semiconnected and we exit
	    //else we repeat
         for(int itr=0;itr<V;itr++)
            {
            	if(dfsoriginal[itr]==true||dfsreverse[itr]==true)
            	   unionarray[itr]=true;
			}
		//now check if union array is equal to size of graph means it carry all elements
		for(int itr=0;itr<V;itr++)
		   {
		    	if(unionarray[itr]==false)
		   	      {
		   	      	cout<<"\n This graph is not semiConnected\n";
		   	      	return;}
			}	
	     //here we clear the arrays to start for next vertex
         for(int p=0;p<V;p++)
       {
    	dfsoriginal[p]=false;
    	dfsreverse[p]=false;
    	unionarray[p]=false;
        visited[p]=false;
	 	}
	
	}
	//the graph is semiconnected
	cout<<"\n\t\t\tthe graph is semi connected\n";
	
}//end of graphtype


	
//compressed graph function
  void graph::compressedGraph()
  {
  	//clear the global vectore component and headComponent
  	    tempadj->clear();
       component.clear();
       headComponent.clear();
       
  	//first i will call tarjens SCC to calculate the components
  	TarjansSCC();
    //here i will create a 2d array of size V*V to store info about edges between components  	
    bool isEdge[V][V];
    // set all values in this array as false
    for(int i=0;i<V;i++)
    {
    	for(int j=0;j<V;j++)
    	  isEdge[i][j]=false;
	}
	//now we iterate all vertices and if they are in same component we just add that edge in the tempadj list
	for(int u=0;u<V;u++)
	{
		int v,wt;
		for(auto itr=adj[u].begin();itr!=adj[u].end();itr++)
		{
			v=itr->first;
			wt=itr->second;
			//get the head component info of u and v
			if(headComponent[u]==headComponent[v])
            {
			   // both vertices are in same component
			   //we just call addtempEdge function
			  
			      addtempEdge(u,v,wt);
		      }
	        else
			{
	           // here we have u and v belonging to different components 
			  
			  // first thind we do find head component of both u and v
			  int uHead=headComponent[u];
			  int vHead=headComponent[v];
			  //now check if isEdge is false or not
			  if(isEdge[uHead][vHead]==false)
			  {
			  	//call addtempEdge function 
			  	addtempEdge(u,v,wt);
			  	// now set the isEdge[uHead][vHead]=true;
			     
				 isEdge[uHead][vHead]=true;	  
			    }
		    }
	
	
		}//one vertex examined 
	}//all nodes traversed
	// print the graph using tempadj list
  ofstream fp;
  fp.open("compressedGraph.gv");
  fp<<"digraph G{"<<"\n";
  for(int i=0;i<V;i++)
  {
  	for(auto itr=tempadj[i].begin();itr!=tempadj[i].end();itr++)
  	{
  		fp<<i<<"->"<<itr->first<<"[label=\""<<itr->second<<"\"];\n";
  		}
  		
    if(taken[i]==0)
	  fp<<i<<"[color=green]"<<"\n"	;	
  }
  
  fp<<"}\n";
  fp.close();
   component.clear();
       headComponent.clear();
  string cmd="dot.exe -Tpng compressedGraph.gv -o Output_CompressedGraph.png";
	 system((const char*)cmd.c_str());
  }
//main function
int main(int argc, char** argv) {
	//read files from test case 
	graph obj;
	char filename[80],name[60];
	cout<<"enter test case file name"<<endl;
	cin>>name;
	sprintf(filename,"%s.txt",name);
	ifstream fp;
	fp.open(filename);
	if(!fp)
	cout<< "test file cant be opened"<<endl;
	
	 else{
	 	  
	 	  int source,dest,weight;
	 	  int ver,edge;
	 	  fp>>ver>>edge;
	 	  obj.set(ver,edge);
	     for(int i=0;i<edge;i++)
		 {
		 	fp>>source>>dest>>weight;
		 	obj.addEdge(source,dest,weight);
				  
     	  }	 	
	    
		//creating a graph
		 	 obj.graphviz(ver);
	 	
		int choice;
		char cont;
		while(1)
		{
		cout<<"\n\nenter operation to perform on the directed graph\n"<<endl;
		
		cout<<"1.find all strongly connected components "<<endl;
		cout<<"2.find dfs of the graph \t";
		cout<<"3.find the distance between source and destination"<<endl;
		cout<<"4.check if the graph is semi connected or not \t";
		cout<<"5.to print the minimal strongly connected graph"<<endl;
		cout<<"6.exit"<<endl;
		cin>>choice;
	
		switch(choice)
		{
		
	     case 1: 
			    obj.TarjansSCC();
	          break;
	          
		case 2:	obj.DFS();
	          break;
	    case 3:cout<<"enter source and destination"<<endl;
		         cin>>source>>dest;
		         obj.dijkstra(source,dest);
		         break;
	    case 4: obj.graphtype();
		        break;
		case 5: obj.compressedGraph();
		   break;      
	    case 6:exit(0);
		  break;      
	    default: exit(0);
		   }
		 cout<<"\n\ncontinue"<<"\n"<<"Y\t or N"<<endl;
		 cin>>cont;
		 if(cont=='N'||cont=='n')
		    exit(0);
		 }
	 }
	return 0;
}
