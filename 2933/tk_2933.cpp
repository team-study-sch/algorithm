#include <iostream>
#include <queue>
#include <vector>
#include <cstring>
using namespace std;

int R, C, N;
char cave[110][110];
int stick[110];
bool visited[110][110];

int dy[] = {1, -1, 0, 0};
int dx[] = {0, 0, -1, 1};

bool checkRange(int y, int x) {
    return visited[y][x] || cave[y][x] == '.' || y < 1 || y > R || x < 0 || x >= C;
}

void BFS(int sy, int sx) {
    queue<pair<int, int>> visit;
    visit.emplace(sy, sx);
    visited[sy][sx] = true;

    while(!visit.empty()) {
        auto pos = visit.front();
        visit.pop();

        for(int i = 0; i < 4; i++) {
            int y = pos.first + dy[i];
            int x = pos.second + dx[i];

            if(checkRange(y, x)) continue;
        
            visited[y][x] = true;
            visit.emplace(y, x);
        }
    }
}

vector<pair<int, int>> cluster;

void searchCluster(int sy, int sx) {
    for(int i = 0; i < 4; i++) {
        int y = sy + dy[i];
        int x = sx + dx[i];

        if(checkRange(y, x)) continue;
    
        cluster.emplace_back(y, x);
        visited[y][x] = true;
        cave[y][x] = '.';
        searchCluster(y, x);
    }
}

void cancleClusterMove(int a) {
    for(int i = a; i >= 0; i--) {
        cluster[i].first++;
    }
}

void downCluster(int a) { // a ~ b
    int size = cluster.size();
    
    for(int i = a; i < size; i++) {
        int & nowY = cluster[i].first;
        nowY--; // y축 감소
        
        if(cave[nowY][cluster[i].second] == 'x') { // 클러스터가 다른 클러스터와 겹쳤다면
            cancleClusterMove(i); // 앞전에 이동했던 크리스탈들 이동 취소
            break; // 그만 이동
        }
    }
}

void getDownCluster() {
    int size = cluster.size();

    bool comp = true;

    while(comp) {
        for(int i = 0; i < size; i++) {
            int & nowY = cluster[i].first;
            nowY--; // y축 감소
            
            if(cave[nowY][cluster[i].second] == 'x') { // 클러스터가 다른 클러스터와 겹쳤다면
                cancleClusterMove(i);
                comp = false;
                break;
            }
            if(nowY == 1) { // 클러스터가 바닥에 닿았다면
                downCluster(i + 1);
                comp = false;
                break;
            } 
        }
    }

    for(int i = 0; i < size; i++) {
        cave[cluster[i].first][cluster[i].second] = 'x';
        visited[cluster[i].first][cluster[i].second] = true;
    }
}

int main() {ios_base::sync_with_stdio(0); cin.tie(NULL);
    cin >> R >> C; 

    for(int i = R; i > 0; i--) { // R ~ 1 // 1이 바닥
        for(int j = 0; j < C; j++) { // 0 ~ C - 1
            cin >> cave[i][j];
        }
    }
    
    cin >> N;
    for(int i = 0; i < N; i++) cin >> stick[i];

    bool turn = true; // true = 1st;

    for(int i = 0; i < N; i++) {    
        if(turn) { // 왼쪽에서 던짐
            for(int j = 0; j < C; j++) {
                if(cave[stick[i]][j] == 'x') {
                    cave[stick[i]][j] = '.';
                    break;
                }
            }
            turn = !turn;
        }
        else { // 오른쪽에서 던짐
            for(int j = C - 1; j >= 0; j--) {
                if(cave[stick[i]][j] == 'x') {
                    cave[stick[i]][j] = '.';
                    break;
                }
            }
            turn = !turn;
        }

        memset(visited[0], 0, sizeof(visited));

        for(int j = 0; j < C; j++) { // 바닥만 조사
            if(cave[1][j] == 'x') { // 바닥과 연결된 클러스터들은 방문처리 해두기
                BFS(1, j);
            }
        }

        for(int j = 2; j <= R; j++) { // 방문처리 안된, 떠 있는 클러스터들 찾기
            for(int k = 0; k < C; k++) {
                if(!visited[j][k] && cave[j][k] == 'x') {
                    
                    cluster.clear();

                    cluster.emplace_back(j, k);
                    visited[j][k] = true;
                    cave[j][k] = '.';
                    searchCluster(j, k);
                    getDownCluster();
                }
            }   
        }
    }

    for(int i = R; i >= 1; i--) {
        for(int j = 0; j < C; j++) {
            cout << cave[i][j];
        }
        if(i > 1) cout << endl;
    }
}