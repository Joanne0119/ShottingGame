# OpenGL Xcode 建置教學（macOS）

## 安裝必要函式庫

使用 Homebrew 安裝以下函式庫：

```bash
brew install glew glfw glm
```

---

## Xcode 設定步驟

### 1. Code Signing

* 在 `Signing & Capabilities` 中：

  * 將 **Code Signing Identity** 設為 **Sign to Local**
  * 在 **Hardened Runtime** 區段中，**勾選** `Disable Library Validation`

---

### 2. 設定 Build Settings

* 開啟 `Build Settings`，搜尋 `Header Search Paths` 並加入以下路徑（請設為 **recursive**）：

```
/opt/homebrew/Cellar/glfw/3.4/include
/opt/homebrew/Cellar/glew/2.2.0_1/include
/opt/homebrew/Cellar/glm/1.0.1/include
```

---

### 3. 加入 Link Libraries

* 到 `Build Phases` → `Link Binary with Libraries` → 點擊 ➕ 號
* 點選「Go to Folder」（Shift + Command + . 可顯示隱藏資料夾）
* 加入以下 `.dylib` 檔案（通常在 `/opt/homebrew/Cellar/.../lib` 中）：

例如：

* `libglfw.3.dylib`
* `libGLEW.2.2.dylib`

---

### 4. 設定工作目錄

若有找不到檔案的錯誤：

* 前往：**Product > Scheme > Edit Scheme**

  * 選擇左側的 **Run**
  * 上方點選 **Options**
  * 勾選 `Use custom working directory`，並設為你的 **專案根目錄**

---

## 檔案使用與修改

1. **複製老師的檔案到專案中**（不要拖拽進去，請手動複製貼上）
2. **調整 `#include` 路徑：**

```cpp
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
```

---

### 常見問題排除

#### Shader 編碼錯誤

若貼上 shader 後報錯，可能是因為 BOM 編碼問題：

1. 用 VSCode 打開 `.glsl` 檔
2. 點右下角的編碼格式（例如 `UTF-8 with BOM`）
3. 點選 `Save with Encoding` → 選擇 `UTF-8`

---

#### Thread 1: EXC\_BAD\_ACCESS 錯誤

請確認是否加上下列程式碼（可能在老師檔案中被註解）：

```cpp
// 設定 OpenGL 版本與 Core Profile
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
```
---
                        
# 專案內容說明
## 遊戲模式简介

使用 OpenGL 實作的 2D 射擊遊戲，玩家擁有 5 條生命。持續擊落小敵人後，會觸發 boss 出場，遊戲會持續循環直到玩家死亡。
---
## 操作說明

    * 按下空白鍵：開始 / 暫停 遊戲

    * 使用滑鼠左右移動：控制玩家方向並發射子彈

    * 按 "S" 或 "s"：切換玩家形狀，共有 3 種模型
---
## 敵人種類

    * 小敵人：

    * 向下滑行者

    * 水平彈跳者

    * 水平平移者

    只有 1 條生命，被打中會爆炸消失

    * Boss：

    出場條件：打掉 10 個小敵人後觸發

    狀態一 (HP > 70%)：上下小度漂動，射出子彈

    狀態二 (< 50% HP < 70%)：轉為散彈攻擊，並上左右移動

    狀態三 (HP < 50%)：射出追蹤彈，移動速度提升
---
## 遊戲結束條件

    玩家有 5 條生命，生命用完後遊戲立即結束

    打倒 boss 後遊戲繼續循環直到死亡
