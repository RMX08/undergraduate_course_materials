"""
快速测试图的可视化效果
"""

import numpy as np
from graph_analysis import GraphAnalyzer

# 测试案例1: 包含自环的图
print("测试案例1: 包含自环的图")
print("="*50)

# 创建一个包含自环的关联矩阵
# 4个顶点，5条边（包括1个自环）
incidence_with_loop = np.array([
    [1, 1, 0, 0, 0],  # v1: 连接e1, e2
    [1, 0, 1, 1, 0],  # v2: 连接e1, e3, e4
    [0, 1, 1, 0, 0],  # v3: 连接e2, e3
    [0, 0, 0, 1, 1]   # v4: 连接e4, e5(自环)
])

analyzer1 = GraphAnalyzer(incidence_with_loop)
print("关联矩阵:")
print(incidence_with_loop)
print("\n生成可视化图...")

analyzer1.print_analysis()
analyzer1.visualize_graph(
    title="测试图：包含自环和普通边", 
    save_path="test_graph_with_loop.png"
)

print("\n" + "="*50)
print("测试完成！图片已保存为: test_graph_with_loop.png")
