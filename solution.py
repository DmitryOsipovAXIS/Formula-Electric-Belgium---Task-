import math
import heapq

# Движения: (dx, dy, символ, стоимость)
MOVES = [
    (-1, 0, '^', 1),
    (1, 0, 'v', 1),
    (0, -1, '<', 1),
    (0, 1, '>', 1),
    (-1, -1, '\\', math.sqrt(2)),
    (-1, 1, '/', math.sqrt(2)),
    (1, -1, '/', math.sqrt(2)),
    (1, 1, '\\', math.sqrt(2)),
]

def read_grid(filename):
    with open(filename, 'r') as f:
        return [list(line.strip()) for line in f]

def dijkstra(grid):
    n, m = len(grid), len(grid[0])
    dist = [[float('inf')] * m for _ in range(n)]
    parent = [[None] * m for _ in range(n)]

    pq = []
    dist[0][0] = 0
    heapq.heappush(pq, (0, 0, 0))  # (cost, x, y)

    while pq:
        cost, x, y = heapq.heappop(pq)

        if cost > dist[x][y]:
            continue

        # если достигли нижней строки — можно остановиться
        if x == n - 1:
            break

        for dx, dy, symbol, w in MOVES:
            nx, ny = x + dx, y + dy

            if 0 <= nx < n and 0 <= ny < m and grid[nx][ny] == '0':
                new_cost = cost + w
                if new_cost < dist[nx][ny]:
                    dist[nx][ny] = new_cost
                    parent[nx][ny] = (x, y, symbol)
                    heapq.heappush(pq, (new_cost, nx, ny))

    # найти лучшую клетку в нижней строке
    end_y = min(range(m), key=lambda y: dist[n-1][y])
    if dist[n-1][end_y] == float('inf'):
        return None, None

    return (n-1, end_y), parent, dist[n-1][end_y]

def reconstruct_path(grid, parent, end):
    x, y = end
    result = [row[:] for row in grid]

    while parent[x][y]:
        px, py, symbol = parent[x][y]
        result[px][py] = symbol
        x, y = px, py

    return result

def main():
    grid = read_grid("input.txt")

    end, parent, cost = dijkstra(grid)
    if end is None:
        print("No path found")
        return

    result = reconstruct_path(grid, parent, end)

    for row in result:
        print("".join(row))

    print(f"Total cost: {cost:.3f}")

if __name__ == "__main__":
    main()