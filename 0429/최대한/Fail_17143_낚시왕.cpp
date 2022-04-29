// move_shark() 작성 도중 종료.

#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;

int arr[101][101];
int tmp_arr[101][101];
int R, C, M;
// 1: 위, 2: 아래, 3: 오른쪽, 4: 왼쪽
int dx[5] = {0, -1, 1, 0, 0};
int dy[5] = {0, 0, 0, 1, -1};

struct Shark {
    int x;
    int y;
    int speed;
    int dir;
    int size;
};
// 낚시왕 이동, 걍 y축 이동하면 됨.
// y축에서 잡을 수 있는 상어 확인 후 잡음
// 상어 이동. 
// 이동할때 위치 확인
// 이동 후 잡아먹히는 상어 확인.
vector<Shark> v;
vector<Shark> tmp;

int sum = 0;
void solution(int x, int y) {
    put_shark();
    sum += get_shark(y);
    move_shark();
}

int change_dir(int dir) {
    if(dir == 1) return 2;
    else if (dir == 2) return 1;
    else if (dir == 3) return 4;
    else if (dir == 4) return 3;
}

void move_shark() {
    tmp.clear();
    for(int i = 0 ; i < v.size(); i++) {
        Shark shark;
        int x = v[i].x, y = v[i].y;
        int d = v[i].dir, s = v[i].speed, z = v[i].size;
        for(int j = 0; j < s; j++) {
            x = x + dx[d];
            y = y + dy[d];
            if(x < 1 || x > R || y < 1 || y > C) {
                // 벽을 마주치면 방향만 바꾸지않고 바로 움직이므로,
                d = change_dir(d);
                x = x + dx[d];
                y = y + dy[d];
            }
        }
        shark.x = x, shark.y = y, shark.dir = d, shark.size = z, shark.speed = s;
        tmp.push_back(shark);
    }
}

int get_shark(int idx) {
    // 같은 열에 있는 상어를 잡음
    int loc = 101;
    int min_idx = 0;
    for(int i = 0; i < v.size(); i++) {
        if(v[i].y == idx) {  // 같은 열에 존재하는지 확인
            if(v[i].x < loc) {  // 상어의 높이가 loc보다 작다면 (땅과 더 가깝다면,)
                loc = v[i].x;  // loc을 갱신
                min_idx = i;  // 잡힌 물고기를 삭제하기 위해 index를 기록.
            }
        }
    }
    if(min_idx != 0) {
        v.erase(v.begin()+min_idx);
        return v[min_idx].size;
    } else return 0;
}

void put_shark() {
    for(int i = 0; i < v.size(); i++) {
        int x = v[i].x, y = v[i].y;
        arr[x][y] = v[i].size;
    }
}

int main() {
    cin >> R >> C >> M;
    if(M == 0) {
        cout << 0 << endl;
        return 0;
    }
    for(int i = 0; i < M; i++) {
        Shark shark;
        int x, y, s, d, z;
        cin >> x >> y >> s >> d >> z;
        shark.x = x; shark.y = y; shark.speed = s; shark.dir = d; shark.size = z;
        v.push_back(shark);
    }
    for(int i = 1; i <= C; i++) {
        solution(0, i);
    }
}
