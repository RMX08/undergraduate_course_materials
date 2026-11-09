import numpy as np  # 处理矩阵数组
import matplotlib
matplotlib.use('TkAgg') # 或者 'Qt5Agg'，强制指定后端
import matplotlib.pyplot as plt  # 画图展示
import networkx as nx  # 辅助画图


# 根据顶点数，随机生成邻接矩阵。
def random_A(n):
    A = np.zeros((n, n), dtype=int)  #初始化一个 nxn 的全0矩阵，值为int类型

    # 遍历矩阵，填充矩阵的右上角（无向图，左下角和右上角是对称的）。
    for i in range(n):
        # 对角线(i==j)为0，j 从 i+1 开始
        for j in range(i + 1, n):
            # 随机生成一个 5 到 50 之间的整数作为距离
            weight = np.random.randint(5, 50)

            # A→B = B→A
            A[i][j] = weight
            A[j][i] = weight

    return A


# 计算近似解算法 (最近邻算法)

# 在没去过的邻居中，每次都选最近的一个走。
def approx_solution(n, A):
    visited = [False] * n   # 访问数组
    path = []   # 记录走过的路径
    cost = 0      # 记录总权重

    # 从 0 号节点出发
    current = 0
    visited[0] = True
    path.append(0)

    # 循环找下一个点
    for _ in range(n - 1):
        # 初始化
        minn = float('inf')  # 初始化无穷大
        next_node = -1  # 还没找到下一个点

        # 遍历邻居
        for j in range(n):
            # 更新找到最小距离邻居
            if not visited[j] and A[current][j] < minn:
                minn = A[current][j]  # 更新最小距离
                next_node = j  # 锁定这个节点

        # 加入路径
        if next_node != -1:
            visited[next_node] = True
            path.append(next_node)
            cost += minn
            current = next_node

    # 从终点回到起点 0，形成闭环
    cost += A[current][0]
    path.append(0)

    return path, cost



# 精确解算法 (暴力搜索 DFS )

# 定义全局变量
best_path = []
min_cost = float('inf')


# 暴力穷举：递归函数，尝试走完所有的路，走到头了就算算总距离，如果比记录的最好成绩还好，就更新记录。
def exact_solution(n, A):
    global min_cost, best_path

    # 重置变量
    min_cost = float('inf')
    best_path = []

    # 访问数组
    visited = [False] * n
    visited[0] = True

    # 内部递归函数
    # u: 当前顶点, count: 总访问过顶点数, current_cost: 当前总权重, current_path: 当前路径
    def dfs(u, count, current_cost, current_path):
        global min_cost, best_path

        # 如果当前权重已经大于之前的最优解，没有必要再递归
        if current_cost >= min_cost:
            return

        # 所有点都去过了
        if count == n:
            # 终点 → 顶点
            total = current_cost + A[u][0]
            # 更新最优解
            if total < min_cost:
                min_cost = total
                best_path = current_path[:] + [0]  # 记下来
            return

        # 循环遍历所有没去过的邻居
        for v in range(n):
            if not visited[v]:
                visited[v] = True
                current_path.append(v)

                # 下一层
                dfs(v, count + 1, current_cost + A[u][v], current_path)

                # 回溯，清空当前临时路径
                current_path.pop()
                visited[v] = False

    # 启动递归
    dfs(0, 1, 0, [0])

    return best_path, min_cost



# 可视化模块

# 因为没有坐标(x,y)，只有距离,为了画图,把点均匀地排成圆形布局）。
def get_layout(n):
    G_temp = nx.complete_graph(n)
    return nx.circular_layout(G_temp)


# 可视化初始邻接矩阵，显示所有的边和权值。
def visualize_initial(n, A):
    G = nx.Graph()
    G.add_nodes_from(range(n))

    # 把矩阵里的数据变成图里的边
    for i in range(n):
        for j in range(i + 1, n):
            # 将边的权重加进去
            G.add_edge(i, j, weight=A[i][j])

    pos = get_layout(n)

    plt.figure(figsize=(6, 6))  # 创建一个正方形的画板

    # 画点
    nx.draw_networkx_nodes(G, pos, node_color='lightgray', node_size=600)
    nx.draw_networkx_labels(G, pos, font_weight='bold')

    # 画边（虚线表示潜在的路径）
    nx.draw_networkx_edges(G, pos, edge_color='gray', style='dashed', alpha=0.5)

    # 画边上的数字（权值）
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_size=8)

    plt.title(f"Initial Random Graph (N={n})")
    plt.axis('off')  # 关掉坐标轴刻度
    plt.show()      # 弹窗显示


# 可视化计算结果，用红实线标出路径。
def visualize_solution(n, A, path, title):
    G = nx.Graph()
    G.add_nodes_from(range(n))
    pos = get_layout(n)  # 使用和初始时一样的布局，保证点的位置不动

    plt.figure(figsize=(6, 6))

    # 画背景点
    nx.draw_networkx_nodes(G, pos, node_color='lightblue', node_size=600)
    nx.draw_networkx_labels(G, pos)

    # 根据结果 path 列表生成边
    path_edges = []
    path_weights = {}
    for i in range(len(path) - 1):
        u = path[i]
        v = path[i + 1]
        path_edges.append((u, v))
        path_weights[(u, v)] = A[u][v]

    # 画红线
    nx.draw_networkx_edges(G, pos, edgelist=path_edges, edge_color='red', width=2)

    # 在红线上标出距离
    nx.draw_networkx_edge_labels(G, pos, edge_labels=path_weights, font_color='red')

    plt.title(title)
    plt.axis('off')
    plt.show()


# 主函数
def main():
    print("========= 无人机巡检路径规划 =========")

    # 获取顶点数
    try:
        nn = input("请输入监测点数量 N : ")
        n = int(nn)
        if n < 2:
            print("至少需要 2 个点，请重新输入！！")
            exit()
    except ValueError:
        print("输入错误，请输入整数!")
        exit()

    # 随机生成距离矩阵
    A = random_A(n)

    print(f"\n========== 随机生成 {n} 个顶点的距离矩阵... ==========")
    print("生成的邻接矩阵如下:")
    print(A)

    # 可视化初始距离矩阵
    visualize_initial(n, A)

    # 近似算法
    print(f"\n========== 计算 近似解 (最近邻) ==========")
    path1, cost1 = approx_solution(n, A)
    print(f"近似路径: {path1}")
    print(f"近似总距离: {cost1}")

    visualize_solution(n, A, path1, f"Approximate Result (Cost: {cost1})")

    # 精确算法 (仅当 N <= 13 时)
    if n <= 13:
        print(f"\n========== 计算 精确解  (暴力搜索) ==========")
        path2, cost2 = exact_solution(n, A)
        print(f"精确路径: {path2}")
        print(f"精确总距离: {cost2}")

        # 计算误差率
        error_rate = ((cost1 - cost2) / cost2) * 100
        print(f"近似算法误差率: {error_rate:.2f}%")

        visualize_solution(n, A, path2, f"Exact Result (Cost: {cost2})")
    else:
        print("\n========== 当前顶点数大于 13，将跳过精确解计算（防止电脑卡死）==========")

    print("\n========== 实验结束 ==========")

if __name__ == "__main__":
    main()