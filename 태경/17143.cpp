#include <iostream>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;

#define F first
#define S second

struct Shark
{
    pair<int, int> pos;
    int speed;
    int dir;
    int size;
    Shark(int r, int c, int s, int d, int z) : pos(r, c), speed(s), dir(d), size(z) {}
    Shark(int s, int d, int z) : Shark(0, 0, s, d, z) {}
    Shark() : Shark(-1, -1, -1) {}
};

queue<Shark> map[101][102];

int R, C, M; 
int r, c, s, d, z;
int ans = 0;

// 위 아래 오른쪽 왼쪽
int dy[] = {0, -1, 1, 0, 0};
int dx[] = {0, 0, 0, 1, -1}; 

struct Data {
    int r;
    int c;
    int d;
};

Data sharkMove(int r, int c, int s, int d) {
    while(s) {
        r += dy[d];
        c += dx[d];

        if(r < 1 || r > R || c < 1 || c > C) { // 상어가 범위를 벗어나면
            // 이동을 취소하고 방향을 반대로 튼다
            r -= dy[d];
            c -= dx[d];

            if(d == 1) d = 2;
            else if(d == 2) d = 1;
            else if(d == 3) d = 4;
            else if(d == 4) d = 3;

            r += dy[d];
            c += dx[d];
        }
        s--;
    }
    return {r, c, d};
}

void solve() {
    int king = 0;

    while(king < C + 1) { // 왕이 C + 1에 도달하면 멈춤
        
        king++; // 낚시왕 이동

        for(int i = 1; i <= R; i++) { // 낚시왕이 있는 열만 뒤진다
            if(map[i][king].size()) { // 상어가 있어?
                ans += map[i][king].front().size; // 답 증가
                map[i][king].pop(); // 상어 없앰
                break;
            }
        }

        queue<Shark> temp;

        for(int i = 1; i <= R; i++) { // 상어들의 이동
            for(int j = 1; j <= C; j++) {
                
                if(!map[i][j].empty()) { // 상어가 있다면
                    auto shark = map[i][j].front();
                    map[i][j].pop();

                    auto newData = sharkMove(i, j, shark.speed, shark.dir);
                    temp.emplace(newData.r, newData.c, shark.speed, newData.d, shark.size); // 임시 큐에 넣음
                }
            }
        }

        int size = temp.size();
        for(int i = 0; i < size; i++) {
            auto shark = temp.front();
            temp.pop();

            if(map[shark.pos.F][shark.pos.S].empty()) map[shark.pos.F][shark.pos.S].push(shark);
            else {
                if(map[shark.pos.F][shark.pos.S].front().size > shark.size) continue;

                map[shark.pos.F][shark.pos.S].pop();
                map[shark.pos.F][shark.pos.S].push(shark);
            }
        }
    }
}

int main() {ios_base::sync_with_stdio(0); cin.tie(NULL);
    cin >> R >> C >> M;

    for(int i = 0; i < M; i++) {
        cin >> r >> c >> s >> d >> z;
        
        if (d <= 2) s %= (R - 1) * 2;
        else s %= (C - 1) * 2;

        map[r][c].emplace(s, d, z);
    }   

    solve();
    cout << ans;
}