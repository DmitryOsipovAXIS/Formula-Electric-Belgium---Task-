# Formula Electric Belgium — Grid shortest path

Find a **minimum-cost path** from the top-left corner of a map to **any cell on the bottom row**. Only cells marked `0` are walkable; `x` is an obstacle.

## Problem model

- **Graph:** each walkable cell is a node.
- **Edges:** 8 directions from a cell — up, down, left, right (cost **1**), and the four diagonals (cost **√2**), same as Euclidean step length on a grid.
- **Weights:** all non-negative → **Dijkstra’s algorithm** gives the shortest path.
- **Goal:** minimize total distance from `(0, 0)` to some `(n - 1, y)` for any column `y` where the cell is `0`.

The program prints the grid with the chosen route drawn using arrows (`^`, `v`, `<`, `>`, `/`, `\`) on cells along the path (except the start), then prints `Total cost` with three decimal places.

For the bundled `input.txt`, the optimal cost is about **213.083**.

## Input format (`input.txt`)

- Several lines of equal length (a rectangular grid).
- Characters: `0` (free) or `x` (blocked).
- The solution assumes `(0, 0)` is `0` and that at least one path to the last row exists.

Place `input.txt` in the **current working directory** when you run either program.

---

## Python solution (`solution.py`)

### Steps in the code

1. **`read_grid`** — Read all lines and build a list of lists of characters.
2. **`dijkstra`** — Run Dijkstra from `(0, 0)`:
   - `dist[i][j]` = best known cost to `(i, j)`; start with infinity except `dist[0][0] = 0`.
   - Priority queue stores `(cost, row, col)`; always expand the smallest cost.
   - Skip stale entries when `cost > dist[x][y]`.
   - Relax each allowed move into a neighboring `0` cell; update `dist` and `parent` if the new cost is better.
   - **Early stop:** when a state on the **last row** is popped, its cost is already minimal among all exits (standard Dijkstra property), so the main loop can stop.
   - After the loop, pick the column on the last row with minimum `dist` (handles the case where the loop ended without the early break).
3. **`reconstruct_path`** — Walk `parent` from the chosen bottom cell back to the start and write the move symbol on each predecessor cell.
4. **`main`** — Load `input.txt`, run Dijkstra, print the annotated grid and total cost.

### Run

```bash
python solution.py
```

Requires Python 3 and only the standard library (`heapq`, `math`).

---

## C++ solution (`solution.cpp`)

### Steps in the code

1. **Read** `input.txt` into `vector<string> grid`.
2. **Dijkstra** — Same logic as Python:
   - `dist` as `double`, initialized to a large sentinel (`1e18`).
   - `parent[row][col]` stores `(prev_row, prev_col, arrow_char)`.
   - `priority_queue` with `greater<>` so the smallest cost is on top (min-heap).
   - Same moves, costs, obstacle check, and early exit when popping a cell on the last row.
3. **Best exit** — Scan the last row for the minimum `dist[n - 1][y]` and column `end_y`.
4. **Reconstruct** — From `(n - 1, end_y)`, follow parents until `(0, 0)`, overwriting `result[px][py]` with the stored arrow.
5. **Print** the grid and `Total cost` with fixed three decimals (`iomanip`).

### Build and run

With g++ (MinGW or similar on Windows):

```bash
g++ -std=c++17 -O2 -o solution solution.cpp
./solution
```

On Windows you may run `solution.exe` instead of `./solution`. Ensure `input.txt` is in the same directory as the executable (or run from the project folder).

---

## Algorithm choice

**Dijkstra** is appropriate here because every step has a **non-negative** cost. A single-source shortest path to “any of several targets” is handled by connecting the targets only implicitly: it is enough to minimize distance to each cell on the last row (or stop as soon as the best exit is finalized).

Breadth-first search would be wrong for this cost model because diagonal edges are more expensive than axis-aligned ones.
