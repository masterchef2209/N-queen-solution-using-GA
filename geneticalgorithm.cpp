#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define pb push_back
#define eb emplace_back
#define mp(x,y) make_pair(x,y)
#define mod 1000000007

double PI=3.1415926535897932384626;

//template<typename T> T power(T x,T y,ll m=mod){T ans=1;while(y>0){if(y&1LL) ans=(ans*x)%m;y>>=1LL;x=(x*x)%m;}return ans%m;}

#define fi first
#define se second

typedef struct
{
    string pattern;
    int cost;
} individual;

typedef vector<individual*> chess_population;
int ind=0;
chess_population population;
int chessBoardSize=8;
int initialPopulationCount = 4;

int fitnessValue(string pattern)
{
    int fit=(chessBoardSize*(chessBoardSize-1))/2;         
    for(int i=0; i<chessBoardSize; i++)
        for(int j=i+1; j<chessBoardSize; j++)
            if((pattern[i] == pattern[j]) ||  (i-pattern[i] == j-pattern[j]) || (i+pattern[i] == j+pattern[j]))
                fit--;
    return fit;
}

int findCeil(int arr[], int r, int l, int h)  
{  
    int mid;  
    while (l < h)  
    {  
        mid = l + ((h - l) >> 1); // Same as mid = (l+h)/2  
        (r > arr[mid]) ? (l = mid + 1) : (h = mid);  
    }  
    return (arr[l] >= r) ? l : -1;  
}  

int myRand(int freq[], int n)  
{  
    int prefix[n], i;  
    prefix[0] = freq[0];  
    for (i = 1; i < n; ++i)  
        prefix[i] = prefix[i - 1] + freq[i];  
    int r = (rand() % prefix[n - 1]) + 1;  
  
    int indexc = findCeil(prefix, r, 0, n - 1);  
    return indexc;  
}  

void generatePopulation()
{
    vector< string >pop;
    for(int i=0;i<4;i++)
    {
        string vv("");
        for(int j=0;j<8;j++)
        {
            char xx;
            cin>>xx;
            vv+=xx;
        }
        pop.emplace_back(vv);
    }
    for(int i=0;i<4;i++)
    {
        individual *temp = new individual;
        temp->pattern=pop[i];
        temp->cost=fitnessValue(pop[i]);
        population.push_back(temp);
    }
}

individual* reproduce(individual *x, individual *y)
{
    individual *child = new individual;
    int n = chessBoardSize;
    int c = rand()%n;
    child->pattern = (x->pattern).substr(0,c) + (y->pattern).substr(c,n-c+1);
    child->cost = fitnessValue(child->pattern);
    return child;
}

individual* mutate(individual *child)
{
    int randomQueen = rand()%(chessBoardSize)+1;
    int randomPosition= rand()%(chessBoardSize)+1;
    child->pattern[randomQueen] = randomPosition+48;
    return child;
}

int randomSelection()
{
    int fitfreq[4];
    for(int i=0;i<4;i++)
    {
        fitfreq[i]=(population[i]->cost);
    }
    int randomPos=myRand(fitfreq,4);
    return randomPos;
}

bool isFit(individual *test)
{
    if(fitnessValue(test->pattern)==((chessBoardSize*(chessBoardSize-1))/2))
        return true;
    return false;
}

bool comp(individual *a, individual*b)
{
    return(a->cost > b->cost);
}

void pprint(string &cc)
{
    int QueenMatrix[8][8];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            QueenMatrix[i][j] = 0;
        }
        int x = cc[i] - '0';
        QueenMatrix[i][x - 1] = 1;
    }
    cout << "\n---------------------------------\n";
    for (int i = 0; i < 8; i++) {
        cout << "| ";
        for (int j = 0; j < 8; j++) {
            if(QueenMatrix[i][j])
            {
                cout<<"Q";
            }
            else
            {
                cout<<".";
            }
                cout <<" | ";
        }
        cout << "\n---------------------------------\n";
    }
}

individual* GA()
{
    int randomNum1,randomNum2;
    individual *individualX,*individualY,*child;
    bool found =0;
    while(!found)
    {
        cout<<"\n_________________________________________________________________\n";
        cout<<"Generation "<<ind++<<"\n";
        chess_population new_population;
        cout<<".....................................................\n";
        cout<<"Current Population\n";
        for(int i=0;i<population.size();i++)
        {
            cout<<(i+1)<<")\n";
            pprint(population[i]->pattern);
            cout<<"\n";
        }
        cout<<".....................................................\n";
        cout<<"Selection and CrossOver\n";
        for(unsigned int i=0; i<population.size(); i++)
        {
            sort(population.begin(),population.end(),comp);
            randomNum1 = randomSelection();
            individualX = population[randomNum1];
            randomNum2 =randomSelection();
            individualY = population[randomNum2];
            child = reproduce(individualX,individualY);
            cout<<(randomNum1+1)<<")\n";
            pprint(individualX->pattern);
            cout<<" + \n";
            cout<<(randomNum2+1)<<")\n";
            pprint(individualY->pattern);
            cout<<" = ";
            pprint(child->pattern);
            cout<<"\n";
            if(rand()%2==0)   
            {
                child = mutate(child);
                cout<<"After Mutation\n";
                pprint(child->pattern);
            }
            else
            {
                cout<<"No Mutation\n";
            }
            cout<<"\n";
            if(isFit(child))
            {
                found=1;
                return child;
            }
            new_population.push_back(child);
            if(i<(population.size()-1))
            {
                cout<<".....................................................\n";
            }
        }
        cout<<"_________________________________________________________________\n";
        population = new_population;
    }
    return child;
}

int main()
{
    srand(time(0));  
    clock_t start_time, end_time;       
    start_time = clock();
    cout<<"Please Enter positions of queen for first 4 boards of population\n"<<endl<<endl;
    generatePopulation();
    individual *solution = GA();
    cout<<"Solution found after "<<ind<<" generations"<<endl;
    pprint(solution->pattern);
    end_time = clock();

    cout << "Time required for execution: \t"  << 1000*((double)(end_time-start_time)/CLOCKS_PER_SEC) << " milliseconds." << "\n\n";
    return 0;
}