#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

void print_vect_vect( vector<vector<double>> v){
    int len = v.size();
    for (int i=0; i<len; i++){
        int len_sub = v[i].size();
        for (int j=0; j<len_sub; j++){
            cout<<v[i][j]<<',';
        }
        cout<<endl;
    }
}

void print_vect( vector<double> v){
    int len = v.size();
    for (int i=0; i<len; i++){
        cout<<v[i]<<endl;
    }
}

bool comp_sort_edge(const vector<double> &a, const vector<double> &b){
    return(a[2]<b[2]);
}

class MST{
    public:
        string file_address;
        double number_of_nodes=0;
        double number_of_edges=0;
        vector<vector<double>> graph_raw;
        vector<vector<double>> graph_adj;
        vector<double> tree_edges;
        vector<double> tree_nodes;
        vector<double> candidate_edges;
        double Ans;

        void read_graph(string file_address){
            ifstream infile;
            string row;
            double node1;
            double node2;
            double edge_weight;

            infile.open(file_address);

            infile >> number_of_nodes;
            infile >> number_of_edges;
            getline(infile,row);

            while(infile.good()){
                getline(infile,row);
                stringstream s(row);
                s>>node1;
                s>>node2;
                s>>edge_weight;
                
                vector<double> edge_data = {node1-1, node2-1, edge_weight,0};
                graph_raw.push_back(edge_data);
            }
            
        }

        void set_graph_adj(vector<vector<double>> &graph_raw, vector<vector<double>> &graph_adj){
            int num_edges = graph_raw.size();
            for (int i=0; i<num_edges; i++){
                int node1 = graph_raw[i][0];
                int node2 = graph_raw[i][1];
                graph_adj[node1].push_back(i);
                graph_adj[node2].push_back(i);
            }

        }

        void update_candidate_edges(vector<vector<double>> &graph_raw, vector<vector<double>> &graph_adj, vector<double> &candidate_edges, int &new_node){
            vector<double> edges = graph_adj[new_node];
            int len = edges.size();
            for (int i=0; i<len; i++){
                int edge = edges[i];
                if (graph_raw[edge][3]==0){
                    candidate_edges.push_back(edge);
                    graph_raw[edge][3]=1;
                }
            }
            sort(candidate_edges.begin(),candidate_edges.end());
        }

        void set_first_edge(vector<double> &tree_edges, vector<vector<double>> &graph_raw, vector<vector<double>> &graph_adj, vector<double> &candidate_edges, vector<double> &tree_nodes){
            tree_edges.push_back(0);
            graph_raw[0][3]=1;
            int node1 = graph_raw[0][0];
            int node2 = graph_raw[0][1];
            tree_nodes[node1]=1;
            tree_nodes[node2]=1;
            update_candidate_edges(graph_raw,graph_adj,candidate_edges,node1);
            update_candidate_edges(graph_raw,graph_adj,candidate_edges,node2);

        }

        void find_next_edge(vector<double> &candidate_edges, vector<double> &tree_edges, vector<double> &tree_nodes,vector<vector<double>> &graph_raw, vector<vector<double>> &graph_adj){
            int len = candidate_edges.size();

            for(int i=0; i<len; i++){
                int edge = candidate_edges[i];
                int node1 = graph_raw[edge][0];
                int node2 = graph_raw[edge][1];

                if(tree_nodes[node1]==0){
                    tree_edges.push_back(edge);
                    tree_nodes[node1] = 1;
                    update_candidate_edges(graph_raw,graph_adj,candidate_edges,node1);
                    return;
                }else if(tree_nodes[node2]==0){
                    tree_edges.push_back(edge);
                    tree_nodes[node2] = 1;
                    update_candidate_edges(graph_raw,graph_adj,candidate_edges,node2);
                    return; 
                }
            }
            
        }

        void find_mst(vector<double> &candidate_edges, vector<double> &tree_edges, vector<double> &tree_nodes,vector<vector<double>> &graph_raw, vector<vector<double>> &graph_adj,int number_of_nodes){
            
            for (int i=0; i<number_of_nodes; i++){
                find_next_edge(candidate_edges, tree_edges, tree_nodes,graph_raw,graph_adj);
            }

        }
        
        double calc_tree_weight(vector<double> &tree_edges,vector<vector<double>> &graph_raw){
            int len = tree_edges.size();
            double ans = 0;

            for (int i=0; i<len; i++){
                int edge = tree_edges[i];
                ans = ans + graph_raw[edge][2];
            }

            return ans;
        }

        MST(string file_add){
            
            file_address = file_add;
            read_graph(file_address);
            sort(graph_raw.begin(),graph_raw.end(),comp_sort_edge);
            vector<double> init = {};
            graph_adj.resize(number_of_nodes,init);
            set_graph_adj(graph_raw,graph_adj);
            tree_nodes.resize(number_of_nodes,0);
            set_first_edge(tree_edges, graph_raw, graph_adj, candidate_edges, tree_nodes);
            find_mst(candidate_edges, tree_edges, tree_nodes,graph_raw, graph_adj,number_of_nodes);
            Ans = calc_tree_weight(tree_edges,graph_raw);
        }


};




int main(){
    string file_address = "G:\\My Drive\\course\\Algorithm\\Minimum Spanning Tree\\edges.txt";
    MST mst(file_address);
    cout.precision(100);
    cout<<mst.tree_edges.size()<<endl<<mst.Ans<<endl;

    return 0;
}