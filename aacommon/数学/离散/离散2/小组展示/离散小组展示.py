import numpy as np  # 处理矩阵数组
import matplotlib
matplotlib.use('TkAgg') # 或者 'Qt5Agg'，强制指定后端
import matplotlib.pyplot as plt  # 画图展示
import networkx as nx  # 辅助画图
import itertools

# 解决matplotlib中文显示问题
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

# 直接内置距离矩阵 (顺序按：4, 1, 2, 3, 5, 6, 7, 8, 9)
node_names = ["4-工程楼", "1-校医院", "2-食品楼", "3-体育馆",
              "5-食堂", "6-宿舍", "7-信息北楼", "8-图书馆", "9-信息南楼"]

# 根据地图中的距离数据形成邻接矩阵
dist_matrix = np.array([
    [0,   992, 515, 114, 519, 1100,673, 974, 933], # 4-工程楼
    [992, 0,   864, 836, 1200,1600,1400,1800,1700],# 1-校医院
    [515, 864, 0,   400, 838, 1100,932, 1200,1200], # 2-食品楼
    [114, 836, 400, 0,   350, 550, 500, 750, 800], # 3-体育馆
    [519, 1200,838, 350, 0,   281, 224, 560, 738], # 5-食堂
    [1100,1600,1100,550, 281, 0,   572, 691, 861], # 6-宿舍
    [673, 1400,932, 500, 224, 572, 0,   300, 493], # 7-信息北
    [974, 1800,1200,750, 560, 691, 300, 0,   184], # 8-图书馆
    [933, 1700,1200,800, 738, 861, 493, 184, 0]    # 9-信息南
])

# 可视化布局，给每个点设定一个大概的相对位置（仅用于绘图，不参与数学计算）
layout_pos = {
    "1-校医院": (0, 8), "2-食品楼": (1, 4), "3-体育馆": (4, 6),
    "4-工程楼": (4, 4), "5-食堂": (7, 6), "6-宿舍": (9, 7),
    "7-信息北楼": (7, 3), "8-图书馆": (8, 1), "9-信息南楼": (6, 0)
}

num_nodes = len(node_names)
start_node_index = 0        # 假设调度中心为工程楼 在矩阵的第一行


# 2. 求解 TSP (动态规划 DP 算法)
def solve_tsp_dp(dist):
    n = len(dist)
    memo = {}

    def visit(mask, last):
        if (mask, last) in memo:
            return memo[(mask, last)]

        # 基准情况存入 memo
        if mask == (1 << start_node_index) | (1 << last):
            res = dist[start_node_index][last]
            memo[(mask, last)] = res  # 存入字典
            return res

        res = float('inf')
        prev_mask = mask & ~(1 << last)
        for prev in range(n):
            if prev != last and (prev_mask & (1 << prev)):
                res = min(res, visit(prev_mask, prev) + dist[prev][last])

        memo[(mask, last)] = res
        return res

    full_mask = (1 << n) - 1
    min_dist = float('inf')
    last_node = -1

    for i in range(n):
        if i != start_node_index:
            current_dist = visit(full_mask, i) + dist[i][start_node_index]
            if current_dist < min_dist:
                min_dist = current_dist
                last_node = i

    # 回溯最优路径
    path = [start_node_index]
    curr_mask = full_mask
    curr_node = last_node
    path_nodes = [curr_node]

    while True:
        prev_mask = curr_mask & ~(1 << curr_node)
        # 当只剩下起点时，说明回溯完成
        if prev_mask == (1 << start_node_index):
            break

        for prev in range(n):
            if prev != curr_node and (prev_mask & (1 << prev)):
                # 状态转移方程回溯：当前状态值 == 上一状态值 + 边权
                if (prev_mask, prev) in memo:  # 安全检查
                    if (memo[(curr_mask, curr_node)] == memo[(prev_mask, prev)] +
                                                        dist[prev][curr_node]):
                        path_nodes.append(prev)
                        curr_node = prev
                        curr_mask = prev_mask
                        break

    path.extend(path_nodes[::-1])
    path.append(start_node_index)
    return min_dist, path


min_total_dist, best_path = solve_tsp_dp(dist_matrix)


# 3. 修正后的可视化绘图函数
def draw_results(title, path=None):
    plt.figure(figsize=(12, 8))

    #创建一个以字符串命名的图
    G = nx.Graph()
    G.add_nodes_from(node_names)

    # 获取坐标字典
    pos = [layout_pos[name] for name in node_names]
    pos_dict = dict(zip(node_names, pos))

    # 绘制节点
    node_colors = ['#FF6B6B' if '工程楼' in n else '#4D96FF' for n in G.nodes()]
    nx.draw_networkx_nodes(G, pos_dict, node_color=node_colors, node_size=1200, edgecolors='white', linewidths=2)
    nx.draw_networkx_labels(G, pos_dict, font_family='SimHei', font_size=10, font_weight='bold')

    if path is None:
        # 初始图：手动添加所有边形成完全图（展示理论可能性）
        import itertools
        all_edges = list(itertools.combinations(node_names, 2))
        nx.draw_networkx_edges(G, pos_dict, edgelist=all_edges, edge_color='lightgray', style='dashed')
        plt.title("初始状态：基于距离矩阵的无向完全图模型 ($K_9$)", fontsize=15, pad=20)
    else:
        # 结果图：根据计算出的索引路径（path是索引列表）转换回节点名
        path_edges = [(node_names[path[i]], node_names[path[i + 1]]) for i in range(len(path) - 1)]

        # 绘制背景淡色的完全图边
        import itertools
        all_edges = list(itertools.combinations(node_names, 2))
        nx.draw_networkx_edges(G, pos_dict, edgelist=all_edges, alpha=0.05, edge_color='gray')

        # 高亮绘制最优路径（带箭头方向）
        nx.draw_networkx_edges(G, pos_dict, edgelist=path_edges, width=3,
                               edge_color='#1B9C85', arrows=True, arrowsize=20,
                               min_source_margin=15, min_target_margin=15)

        # 添加序号标注（第1站，第2站...）
        for i, idx in enumerate(path[:-1]):
            x, y = pos_dict[node_names[idx]]
            plt.text(x, y + 0.45, f"START" if i == 0 else f"第{i}站",
                     color='darkred', fontweight='bold', ha='center',
                     bbox=dict(facecolor='white', alpha=0.7, edgecolor='none'))

        plt.title(f"最终结果：最优巡检哈密顿回路\n(总里程: {min_total_dist}米)", fontsize=15, color='#1B9C85', pad=20)

    plt.axis('off')
    plt.tight_layout()


# 4. 执行与展示
print(f"最优巡检顺序: {' -> '.join([node_names[i] for i in best_path])}")
print(f"最短巡检总路程: {min_total_dist} 米")

draw_results("初始状态")
draw_results("最终结果", path=best_path)
plt.show()