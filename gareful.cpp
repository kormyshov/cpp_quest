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

const int UP = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int LEFT = 3;
const int mdelta[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

const int N = 20;
const int M = 80;
int dist[20][80][20][80];
char SS[20][81];
vector<pair<int,int> > golds;

int stat_table[11][11], stat_sum;

const int CNT_SHUFFLE = 5;

int glob_cnt;

class Field{
public:
	//char ss[20][81];
	int x, y;
	int health;
	int fgolds[64];
	int sum;
	int fmonsters[106];
	vector<pair<int,int> > monsters;

	vector<int> path;

	Field(char _ss[20][81]){
		//memcpy(ss, _ss, sizeof(ss));

		for(int i=0; i<N; ++i)
			for(int j=0; j<M; ++j){
				if(_ss[i][j] == '@') x=i, y=j;
				if(_ss[i][j] == 'M') monsters.push_back({i, j});
			}
		health = 100;
		sum = 0;

		memset(fgolds, 0, sizeof(fgolds));
		fgolds[45] = 1;
		memset(fmonsters, 0, sizeof(fmonsters));
	}
	Field(const Field &a){
		//memcpy(ss, a.ss, sizeof(ss));
		x = a.x;
		y = a.y;
		health = a.health;
		sum = a.sum;
		memcpy(fgolds, a.fgolds, sizeof(fgolds));
		memcpy(fmonsters, a.fmonsters, sizeof(fmonsters));
		monsters.assign(a.monsters.begin(), a.monsters.end());
		path.assign(a.path.begin(), a.path.end());
	}

	void Set(const Field &a){
		x = a.x;
		y = a.y;
		health = a.health;
		sum = a.sum;
		memcpy(fgolds, a.fgolds, sizeof(fgolds));
		memcpy(fmonsters, a.fmonsters, sizeof(fmonsters));
		monsters.assign(a.monsters.begin(), a.monsters.end());
		path.assign(a.path.begin(), a.path.end());
	}

	int Move(int x1, int y1){
		for(int i=0; i<4; ++i)
			if(x+mdelta[i][0] == x1 && y+mdelta[i][1] == y1)
				return Move(i);
		cout << "Error on Move\n";
		return -2;
	}

	int Move(int direction){
		int x1 = x + mdelta[direction][0];
		int y1 = y + mdelta[direction][1];
		if(SS[x1][y1] == '#') return 0;

		for(int i=0; i<(int)golds.size(); ++i)
			if(golds[i] == make_pair(x1, y1) && !fgolds[i])
				fgolds[i] = 1, sum++;

		int step_to_monster = 0;
		for(auto p : monsters)
			if(p == make_pair(x1, y1)) step_to_monster++;

		if(!step_to_monster)
			health = MIN(100, health+1);
		else{
			health -= 20 * step_to_monster;
			if(health <= 0) return -1;

			for(int i=0; i<(int)monsters.size(); ++i)
				if(monsters[i] == make_pair(x1, y1))
					fmonsters[i] = -1, monsters[i] = {-10, -10};
		}

		for(int i=0; i<(int)monsters.size(); ++i)
			if(fmonsters[i] == 1){
				int x2 = monsters[i].first;
				int y2 = monsters[i].second;
				int cur = dist[x1][y1][x2][y2];
				for(int j=0; j<4; ++j)
					if(dist[x1][y1][x2+mdelta[j][0]][y2+mdelta[j][1]] == cur-1){
						x2 += mdelta[j][0];
						y2 += mdelta[j][1];
						break;
					}
				monsters[i].first = x2;
				monsters[i].second = y2;
			}else
			if(fmonsters[i] == 0)
				if(dist[x1][y1][monsters[i].first][monsters[i].second] <= 5)
					fmonsters[i] = 1;

		x = x1;
		y = y1;
		path.push_back(direction);

		return 1;
	}

	int StepToGoal(int gx, int gy){
		int cur = dist[gx][gy][x][y];
		if(!cur) return 2;
		vector<int> v;
		for(int i=0; i<4; ++i){
			int tox = x + mdelta[i][0];
			int toy = y + mdelta[i][1];
			if(dist[gx][gy][tox][toy] == cur-1 && SS[tox][toy] != '#')
				v.push_back(i);
		}
		int ind = v[rand()%v.size()];
		int x1 = x + mdelta[ind][0];
		int y1 = y + mdelta[ind][1];
		return Move(x1, y1);
	}

	int MoveToGoal(int gx, int gy){
		do{
			int ret = StepToGoal(gx, gy);
			if(!ret) cout << "Error on MoveToGoal\n";
			if(ret != 1) return ret;
		}while(x!=gx || y!=gy);
		return 2;
	}

	int BruteToGoal(int gx, int gy, int c1, int c2, int step){

		int hpm = 0;
		for(int i=0; i<(int)monsters.size(); ++i)
			if(fmonsters[i] == 1) hpm += 20;

		if(x == gx && y == gy){
			if(hpm > health) return -1;
			return 2;
		}

		glob_cnt++;
		if(glob_cnt > 10000000) return -3;

		if(dist[x][y][gx][gy] > step) return -2;
		if(health+step < hpm) return -2;

		if(hpm == 0){
			Field *single = new Field(*this);
			single->StepToGoal(gx, gy);
			int shp = 0;
			for(int i=0; i<(int)monsters.size(); ++i)
				if(single->fmonsters[i] == 1) shp++;

			if(!shp){
				int ret = single->BruteToGoal(gx, gy, c1, c2, step-1);
				if(ret == 2){
					Set(*single);
					delete single;
					return 2;
				}
			}
			delete single;
		}

		Field *next[4];
		int *grade = new int[4];
		for(int i=0; i<4; ++i){
			next[i] = new Field(*this);
			int ret = next[i]->Move(i);
			grade[i] = (ret == 1 ? next[i]->GetGrade(gx, gy, c1, c2) : -INF);
		}
		for(int i=0; i<3; ++i)
			if(grade[i] < grade[i+1]){
				swap(grade[i], grade[i+1]);
				swap(next[i], next[i+1]);
				if(i) i-= 2;
			}

		int ind = -1, mstep = INF;
		for(int i=0; i<4; ++i) if(grade[i] > -INF){
			int ret = next[i]->BruteToGoal(gx, gy, c1, c2, MIN(step-1, mstep-path.size()));
			if(ret == 2)
				if(ind < 0 || mstep > next[i]->path.size())
					ind = i, mstep = next[i]->path.size();
		}

		if(ind >= 0) Set(*next[ind]);

		for(int i=0; i<4; ++i) delete next[i];
		delete []grade;

		if(ind < 0) return -1;
		return 2;
	}

	pair<int,int> GetFreeGoal(){
		int mm[N][M];
		memset(mm, -1, sizeof(mm));
		for(int i=0; i<N; ++i)
			for(int j=0; j<M; ++j)
				if(SS[i][j]!='#'){
					mm[i][j] = 0;
					for(int k=0; k<(int)monsters.size(); ++k)
						if(fmonsters[k] >= 0 &&
							((fmonsters[k] == 1 && dist[i][j][monsters[k].first][monsters[k].second] <= dist[i][j][x][y]) ||
							  (fmonsters[k] == 0 && dist[i][j][monsters[k].first][monsters[k].second] <= 5)))
							mm[i][j] = -1;
				}
		queue<pair<int,int>> q;
		q.push({x, y});
		while(!q.empty()){
			auto p = q.front(); q.pop();
			for(int i=0; i<4; ++i)
				if(!mm[p.first+mdelta[i][0]][p.second+mdelta[i][1]])
					mm[p.first+mdelta[i][0]][p.second+mdelta[i][1]] = mm[p.first][p.second] + 1,
					q.push({p.first+mdelta[i][0], p.second+mdelta[i][1]});
		}
		int mx = -1, mi, mj;
		for(int i=0; i<N; ++i)
			for(int j=0; j<M; ++j)
				if(mx < mm[i][j]) mx = mm[i][j], mi = i, mj = j;

		return make_pair(mi, mj);
	}

	pair<int,int> GetGoal(){
		if(sum == 64) return make_pair(1, 1);
		if(sum == 63){
			fgolds[45] = 0;
			return golds[45];
		}
		vector<pair<int,int> > v;
		for(int i=0; i<(int)golds.size(); ++i)
			if(fgolds[i] == 0)
				v.push_back({dist[x][y][golds[i].first][golds[i].second], i});
		sort(v.begin(), v.end());
		if(v[0].first <= 5) return golds[v[0].second];
		random_shuffle(v.begin(), v.begin()+MIN(CNT_SHUFFLE, v.size()));
		return golds[v[0].second];
	}

	void Print(){
		if(health <= 0){
			cout << "You died :(\n";
			return ;
		}
		cout << "Gold: " << sum << "\t\tHP:" << health << endl;

		char s[20][81];
		memcpy(s, SS, sizeof(s));
		for(int i=0; i<N; ++i)
			for(int j=0; j<M; ++j)
				if(SS[i][j] == '#') s[i][j] = '#';
				else s[i][j] = '.';

		for(int i=0; i<(int)golds.size(); ++i)
			if(!fgolds[i]) s[golds[i].first][golds[i].second] = '$';

		for(int i=0; i<(int)monsters.size(); ++i){
			int x1 = monsters[i].first;
			int y1 = monsters[i].second;
			if(!fmonsters[i]) s[x1][y1] = 'M';
			else if(fmonsters[i] == 1){
				if(s[x1][y1]>='1' && s[x1][y1]<='9') s[x1][y1]++;
				else s[x1][y1] = '1';
			}
		}

		s[x][y] = '@';

		for(int i=0; i<N; ++i)
			cout << s[i] << endl;
		cout << endl;
	}

	int GetGrade(int gx, int gy, int c1, int c2){
		int d = dist[x][y][gx][gy];
		int h = health;
		int w = 0;
		for(int i=0; i<106; ++i) h += 20 * (fmonsters[i] == -1), w += fmonsters[i] == 1;
		int c3 = 10 - c1 - c2;
		return -c1 * d + c2 * h - c3 * w;
	}

	pair<int, vector<int>> GetBrute(){
		int limit[101] = {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
		do{
			pair<int,int> p = GetGoal();

			cout << "Go to the point " << p << endl;

			int depth;
			int mc1, mc2, mret;
			for(depth = 10; depth < 100; depth+=5){

				cout << "Depth : " << depth << endl << "i : ";

				mc1 = mc2 = mret = -1;
				for(int i=0; i<limit[depth]; ++i){
					cout << i << " "; cout.flush();
					int rnd = rand() % stat_sum;
					int c1, c2, csum=0;
					for(c1=0; c1<11 && csum<=rnd; ++c1) for(c2=0; c2<11; ++c2){
						csum += stat_table[c1][c2];
						if(csum > rnd) break;
					}

					Field field(*this);
					glob_cnt = 0;
					int ret = field.BruteToGoal(p.first, p.second, c1, c2, depth);
					if(ret == 2 && (mret < 0 || field.path.size() < mret))
						mc1 = c1, mc2 = c2, mret = field.path.size();
				}
				cout << endl;
				if(mret >= 0) break;
			}
			cout << endl << "Mret : " << mret << endl;

			if(mret < 0) return make_pair(-3, path);

			glob_cnt = 0;
			int ret = BruteToGoal(p.first, p.second, mc1, mc2, depth);

			stat_table[mc1][mc2]++;
			stat_sum++;

			for(int i=0; i<11; ++i){
				for(int j=0; j<11; ++j) cout << stat_table[i][j] << " ";
				cout << endl;
			}
			cout << endl;

			Print();

		}while(sum != 64 || x!=1 || y!=1);

		return make_pair(2, path);
	}

	int StepToFreeGoal(int gx, int gy){
		int mm[N][M];
		memset(mm, -1, sizeof(mm));
		for(int i=0; i<N; ++i)
			for(int j=0; j<M; ++j)
				if(SS[i][j]!='#'){
					mm[i][j] = 0;
					for(int k=0; k<(int)monsters.size(); ++k)
						if(fmonsters[k] >= 0 &&
							((fmonsters[k] == 1 && dist[i][j][monsters[k].first][monsters[k].second] <= dist[i][j][x][y]) ||
							  (fmonsters[k] == 0 && dist[i][j][monsters[k].first][monsters[k].second] <= 5)))
							mm[i][j] = -1;
				}
		queue<pair<int,int>> q;
		q.push({gx, gy});
		while(!q.empty()){
			auto p = q.front(); q.pop();
			for(int i=0; i<4; ++i)
				if(!mm[p.first+mdelta[i][0]][p.second+mdelta[i][1]])
					mm[p.first+mdelta[i][0]][p.second+mdelta[i][1]] = mm[p.first][p.second] + 1,
					q.push({p.first+mdelta[i][0], p.second+mdelta[i][1]});
		}
		int cur = mm[x][y];
		vector<int> v;
		for(int i=0; i<4; ++i){
			int tox = x + mdelta[i][0];
			int toy = y + mdelta[i][1];
			if(mm[tox][toy] == cur-1)
				v.push_back(i);
		}
		int ind = v[rand()%v.size()];
		int x1 = x + mdelta[ind][0];
		int y1 = y + mdelta[ind][1];
		return Move(x1, y1);
	}

	int CarefulMoveToGoal(int gx, int gy){
		int hp = health;
		while((x!=gx || y!=gy) && hp == health && path.size()<1500){

			int cnt = 0;
			for(int i=0; i<(int)monsters.size(); ++i)
				if(fmonsters[i] == 1) cnt++;

			if(!cnt){
				int ret = StepToGoal(gx, gy);
				if(ret != 1){
					cout << "Error on Careful from Goal" << endl;
					return -1;
				}
			}else{
				pair<int,int> cur = {x, y};
				auto p = GetFreeGoal();
				int ret = StepToFreeGoal(p.first, p.second);
				if(ret != 1){
					cout << "Error on Careful from FreeGoal " << p << endl;
					return -1;
				}
				if(x!=gx || y!=gy){
					ret = StepToGoal(cur.first, cur.second);
					if(ret != 1){
						cout << "Error on Careful from Goal\n" << endl;
						return -1;
					}
				}
			}

		}
		return 1;
	}

	pair<int, vector<int>> GetCareful(){
		pair<int,int> goal = {1, 1};
		int rnd = 20 + rand()%61 + rand()%21;
		cout << "Health limit : " << rnd << endl;
		do{
			if(path.size() > 1016) return make_pair(-3, path);
			if(goal == make_pair(x, y)){
				goal = GetGoal();
			}
			if(x==10 && y==51){
				Move(DOWN);
				Move(DOWN);
				Move(UP);
				Move(UP);
				Move(UP);
				Move(LEFT);
				Move(LEFT);
				Move(UP);
				Move(UP);
				Move(UP);
				Move(RIGHT);
				Move(RIGHT);
				Move(RIGHT);
				Move(RIGHT);
				Move(UP);
				Move(UP);
				Move(UP);
				Move(UP);
				Move(UP);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				Move(LEFT);
				break;
			}

			if(health < rnd){
				pair<int,int> cur = {x, y};
				auto p = GetFreeGoal();
				int ret = StepToFreeGoal(p.first, p.second);
				if(ret != 1){
					cout << "Error on Careful from FreeGoal " << p << endl;
					return make_pair(0, path);
				}
				if(goal != make_pair(x, y)){
					ret = StepToGoal(cur.first, cur.second);
					if(ret != 1){
						cout << "Error on Careful from Goal " << goal << endl;
						return make_pair(0, path);
					}
				}
			}else{
				//CarefulMoveToGoal(goal.first, goal.second);
				int cnt = 0;
				for(int i=0; i<(int)monsters.size(); ++i)
					if(fmonsters[i] == 1) cnt++;

				if(!cnt){
					int ret = StepToGoal(goal.first, goal.second);
					if(ret != 1){
						cout << "Error on Careful from Goal " << goal << endl;
						return make_pair(0, path);
					}
				}else{
					pair<int,int> cur = {x, y};
					auto p = GetFreeGoal();
					int ret = StepToFreeGoal(p.first, p.second);
					if(ret != 1){
						cout << "Error on Careful from FreeGoal " << p << endl;
						return make_pair(0, path);
					}
					if(goal != make_pair(x, y)){
						ret = StepToGoal(cur.first, cur.second);
						if(ret != 1){
							cout << "Error on Careful from Goal " << goal << endl;
							return make_pair(0, path);
						}
					}
				}
			}
			//cout << "Sum: " << sum << "\t\tx: " << x << "\t\ty: " << y << "\t\tHealth: " << health << "\t\tcnt: " << cnt << endl;
			//Print();

		}while(sum != 64 || x != 1 || y != 1);

		return make_pair(2, path);
	}

	pair<int, vector<int>> GetStupid(){
		int rnd = 10 + rand()%80;
		int tm = 0;
		do{
			tm++;
			if(tm > 2000) return make_pair(-2, path);
			pair<int,int> p = GetGoal();

			int cnt = 0;
			for(int i=0; i<(int)monsters.size(); ++i)
				if(fmonsters[i]==1 && dist[x][y][monsters[i].first][monsters[i].second]==1) cnt++;
			if(cnt * 20 < health){
				for(int i=0; i<(int)monsters.size(); ++i)
					if(fmonsters[i]==1 && dist[x][y][monsters[i].first][monsters[i].second]==1)
						p = monsters[i];
			}else
				p = GetFreeGoal();

			if(health < rnd)
				p = GetFreeGoal();

			if(p == make_pair(x, y)) p = GetGoal();

			int ret = StepToGoal(p.first, p.second);
			if(!ret){
				cout << "Error on GetStupid\n";
				return make_pair(0, path);
			}
			if(ret < 0) return make_pair(-1, path);

			cout << "Sum: " << sum << "\t\tx: " << x << "\t\ty: " << y << endl;
			//Print();

		}while(sum!=64 || x!=1 || y!=1);

		return make_pair(2, path);
	}

};



Field load(){
	ifstream in("dist.txt");
	for(int i1=0; i1<N; ++i1) for(int j1=0; j1<M; ++j1)
		for(int i2=0; i2<N; ++i2) for(int j2=0; j2<M; ++j2)
			in >> dist[i1][j1][i2][j2];

	ifstream I1("level1.txt");
	for(int i=0; i<N; ++i)
		I1 >> SS[i];

	for(int i=0; i<N; ++i) for(int j=0; j<M; ++j)
			if(SS[i][j] == '$')
				golds.push_back({i, j});

	Field f(SS);
	SS[1][1] = '.';

	for(int i=0; i<11; ++i)
		for(int j=0; j<11; ++j)
			if(i+j <= 10)
				stat_table[i][j] = 1, stat_sum++;

	return f;
}

int main()
{
	//ios_base::sync_with_stdio(0);

	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);

	auto tm = time(0);
	srand(tm);

	Field field = load();
	//field.Print();

	auto res = field.GetCareful();
	cout << "Ans: " << res.first << " Length: " << res.second.size() << endl;
	if(res.first == 2){
		char str[99];
		sprintf(str, "sol/%d.txt", tm);
		ofstream out(str);
		out << res.second;
	}
	field.Print();
	cout << "Path: ";
	for(int i=field.path.size()-40; i<field.path.size(); ++i)
		cout << field.path[i] << " ";
	cout << endl;

	return 0;
}
