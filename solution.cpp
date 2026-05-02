#include <bits/stdc++.h>
using namespace std;

struct Move {
    int dx, dy;
    char symbol;
    double cost;
};

vector<Move> moves = {
    {-1, 0, '^', 1},
    {1, 0, 'v', 1},
    {0, -1, '<', 1},
    {0, 1, '>', 1},
    {-1, -1, '\\', sqrt(2)},
    {-1, 1, '/', sqrt(2)},
    {1, -1, '/', sqrt(2)},
    {1, 1, '\\', sqrt(2)}
};

int main() {
    ifstream file("input.txt");
    vector<string> grid;
    string line;

    while (getline(file, line)) {
        grid.push_back(line);
    }

    int n = grid.size();
    int m = grid[0].size();

    vector<vector<double>> dist(n, vector<double>(m, 1e18));
    vector<vector<tuple<int,int,char>>> parent(n, vector<tuple<int,int,char>>(m));

    priority_queue<
        tuple<double,int,int>,
        vector<tuple<double,int,int>>,
        greater<>
    > pq;

    dist[0][0] = 0;
    pq.push({0, 0, 0});

    while (!pq.empty()) {
        auto [cost, x, y] = pq.top();
        pq.pop();

        if (cost > dist[x][y]) continue;

        if (x == n - 1) break;

        for (auto &mv : moves) {
            int nx = x + mv.dx;
            int ny = y + mv.dy;

            if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '0') {
                double new_cost = cost + mv.cost;

                if (new_cost < dist[nx][ny]) {
                    dist[nx][ny] = new_cost;
                    parent[nx][ny] = {x, y, mv.symbol};
                    pq.push({new_cost, nx, ny});
                }
            }
        }
    }

    // найти лучший выход
    int end_y = -1;
    double best = 1e18;

    for (int y = 0; y < m; y++) {
        if (dist[n-1][y] < best) {
            best = dist[n-1][y];
            end_y = y;
        }
    }

    if (end_y == -1) {
        cout << "No path found\n";
        return 0;
    }

    // восстановление пути
    int x = n - 1, y = end_y;
    vector<string> result = grid;

    while (x != 0 || y != 0) {
        auto [px, py, sym] = parent[x][y];
        result[px][py] = sym;
        tie(x, y) = make_tuple(px, py);
    }

    // вывод
    for (auto &row : result) {
        cout << row << endl;
    }

    cout << "Total cost: " << fixed << setprecision(3) << best << endl;

    return 0;
}