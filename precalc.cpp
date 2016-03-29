#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <memory.h>
#include <cmath>
#include <iomanip>
#include <pthread.h>
#include <semaphore.h>

#include <vector>
#include <queue>
#include <deque>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <bitset>
#include <algorithm>
#include <functional>

#define ABS(a) ((a)<0?(-(a)):(a))
#define SIGN(a) (((a)>0)-((a)<0))
#define SQR(a) ((a)*(a))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define PI (3.1415926535897932384626433832795)
#define INF (2147483647)
#define LLINF (9223372036854775807LL)
#define INF2 (1073741823)
#define EPS (0.00000001)

#define MOD (1000000007)

#define y1 stupid_cmath
#define y0 stupid_cmath_too

using namespace std;

typedef long long LL;
template<typename T1,typename T2> ostream& operator<<(ostream &O,pair<T1,T2> &t) {return O<<"("<<t.first<<","<<t.second<<")";}
template<typename T> ostream& operator<<(ostream &O,vector<T> &t){for(int _=0;_<(int)t.size();++_)O<<t[_]<<" ";return O; }
void dout(){cout<<endl;} template<typename Head, typename... Tail> void dout(Head H, Tail... T){cout<<H<<" "; dout(T...);}

int mm[20][80][20][80], m[20][80];

int main()
{
	//ios_base::sync_with_stdio(0);

	freopen("level1.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);

	char ss[20][81];
	int N = 20, M = 80;
	for(int i=0; i<N; ++i){
		cin >> ss[i];
	}

	for(int i1=0; i1<N; ++i1) for(int j1=0; j1<M; ++j1)	if(ss[i1][j1] != '#'){
		for(int i=0; i<N; ++i){
			for(int j=0; j<M; ++j){
				m[i][j] = INF2;
			}
		}
		m[i1][j1] = 1;
		for(int c=1; c<=1600; ++c)
			for(int i=0; i<N; ++i) for(int j=0; j<M; ++j) if(m[i][j] > c+1 && ss[i][j] != '#')
				if(m[i-1][j]==c || m[i+1][j]==c || m[i][j-1]==c || m[i][j+1]==c)
					m[i][j] = c+1;

		for(int i2=0; i2<N; ++i2) for(int j2=0; j2<M; ++j2)
			mm[i1][j1][i2][j2] = MAX(0, m[i2][j2] - 1);
	}

	ofstream out("dist.txt");
	for(int i1=0; i1<N; ++i1){
		for(int j1=0; j1<M; ++j1){
			for(int i2=0; i2<N; ++i2){
				for(int j2=0; j2<M; ++j2){
					out << mm[i1][j1][i2][j2] << " ";
				}
				out << endl;
			}
			out << endl;
		}
		out << endl;
	}

	return 0;
}
