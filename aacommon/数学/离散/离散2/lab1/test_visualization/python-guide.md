## Python 知识点讲义与示例

### 1) 函数定义与使用

要点：
- 用 `def` 定义函数；参数可有默认值与关键字传参；`return` 返回结果；无 `return` 默认返回 `None`。
- 可以返回多个值（本质是打包成元组）。

示例：
```python
def greet(name, prefix="Hello"):
    message = f"{prefix}, {name}!"
    return message

# 位置参数
print(greet("Alice"))


# 关键字参数
print(greet(name="Bob", prefix="Hi"))

#返回多个值
def divide(a, b):
    if b == 0:
        return None, "division by zero"
    return a / b, None

result, err = divide(10, 2)
```

---

### 2) 输入与输出（input/print）

要点：
- `input()` 获取字符串；需要数字时用 `int()`/`float()` 转换。
- `print()` 可用 `sep` 指定分隔符、`end` 指定结尾；首选 f-string 格式化。

示例：
```python
raw = input("请输入一个整数: ")   # 用户输入以字符串的形式存入raw
try:
    number = int(raw)
    print(f"你输入的是 {number}")     # f"..." 表示格式化字符串
except ValueError:                  # raw无法被转换成整数，会抛出ValueError
    print("不是有效整数")

print("A", "B", "C", sep=", ", end=".\n")
```
#### if...else 和 try...except的区别：
if...else ：
- “事前检查”（预防机制），做事之前先确认条件是否满足
- 业务逻辑判断、变量一定存在/可控

try...except ：
- “事后补救”（兜底机制），直接执行代码，如果出了问题（抛出异常），由 except 捕获并处理。
- 类型转换、文件/网络操作

---

### 3) 条件判断（if / elif / else）与逻辑运算

要点：
- 比较运算：`== != > >= < <=`；逻辑运算：`and or not`；成员判断：`in`。
- 使用“卫语句”（及早返回）让分支更清晰。

示例：
```python
def classify(score):
    if score < 0 or score > 100:
        return "invalid"
    if score >= 90:
        return "A"
    elif score >= 75:
        return "B"
    elif score >= 60:
        return "C"
    else:
        return "D"
```

---

### 4) 循环（for / while）与控制（break, continue）

要点：
- `for` 常用于遍历序列或 `range()`；`while` 适合未知次数、条件驱动的循环。
- `break` 终止当前循环；`continue` 跳过本次迭代。

示例：
```python
# for + range
total = 0
for i in range(1, 6):  # 1..5
    total += i

# while 与输入校验
attempts = 0
while attempts < 3:
    pwd = input("密码: ")
    if pwd == "123":
        print("通过")
        break
    attempts += 1
else:
    print("失败")

# 简单的行输出模式（示例，不特定于作业题）
for row in range(1, 4):
    print("*" * row)
```

---

### 5) 字符串操作（拆分、清洗、计数、拼接）

要点：
- 常用方法：`strip()` 去空白、`lower()`/`upper()` 大小写、`split()` 分割、`join()` 拼接、`count()` 计数。
- 单词统计常见流程：清洗 → `split()` → 遍历统计或直接 `len()`。

示例：
```python
text = "  Hello world, hello Python  "
clean = text.strip().lower()
print(clean)
words = clean.split()          # 默认按任意空白分割 分割成字符串数组
print(len(words))              # 单词数 即数组长度

# 统计每个单词出现次数
counts = {}            # 相当于键值对
for w in words:
    counts[w] = counts.get(w, 0) + 1    # get 如果counts中没有w键，则返回0
    print(counts[w])

# 拼接
sentence = ", ".join(sorted(counts.keys()))   # 按照字母顺序输出键
# hello, python, world,
```

---

### 6) 列表/序列操作（索引、切片、增删改、排序）

要点：
- 索引从 0 开始；切片 `lst[start:end:step]`，不含 `end`；步长可为负实现反转 `lst[::-1]`。
- 修改：`append/extend/insert` 增；`remove/pop` 删；下标赋值改。
- 排序：原地 `list.sort()` 或返回新列表的 `sorted()`，支持 `key=` 与 `reverse=`。

示例：
```python
nums = [3, 1, 4, 1, 5, 9]

# 访问与切片
first = nums[0]
tail = nums[1:]
evens_by_index = nums[0::2]  # 偶数索引的元素
reversed_nums = nums[::-1]

# 增删改
nums.append(2)
nums.insert(1, 7)         # 在num[1]处插入7
removed = nums.pop()      # 删除最后一个

# 排序
nums.sort()               # 原地升序
desc = sorted(nums, reverse=True)

# max/min 与自定义key
longest = max(["a", "bbb", "cc"], key=len)

# 列表推导式
squares = [x * x for x in range(5)]
```

---

### 7) 类型与类型转换

要点：
- 字符串与数字：`int("12")`、`float("3.14")`、`str(123)`。
- 序列互转：`list(tuple_obj)`、`tuple(list_obj)`；布尔：`bool(x)` 遵循“空为假”。

示例：
```python
value = "42"
n = int(value)
s = str(3.5)
flag = bool("")   # False，因为空字符串
```

---

### 8) 实用内建与迭代技巧（len/sum/max/min/sorted/enumerate/zip）

要点：
- `len` 长度；`sum` 求和；`max/min` 最大最小；`sorted` 返回排序后的新列表。
- `enumerate(iterable, start=0)` 同时拿到索引和值；`zip(a, b)` 成对迭代。

示例：
```python
names = ["Tom", "Ada", "Li"]
scores = [88, 95, 90]

for idx, name in enumerate(names, start=1):
    print(idx, name)

for name, score in zip(names, scores):
    print(f"{name}: {score}")

avg = sum(scores) / len(scores)
top = max(scores)
sorted_pairs = sorted(zip(scores, names), reverse=True)
```
