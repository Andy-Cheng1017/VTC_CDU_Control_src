# 命名規則總結表

以下表格結合了 POSIX 標準和 Google Style Guide 的命名習慣，總結了對不同情境的命名建議：

| **情境（中文/英文）**     | **建議命名風格**    | **命名規則**           | **範例**                          | **範例擺放位置 / Example Usage** |
|--------------------------|---------------------|-------------------------|------------------------------------|----------------------------------|
| **型別名稱 / Type Name**             | `PascalCase` 或 `snake_case_t` | Google Style 使用 `PascalCase`，POSIX 標準使用 `snake_case_t` | `CardData`, `RsFunc_t`     | `typedef struct CardData {...};` 或 `typedef struct RsFunc_t {...};` |
| **結構體名稱 / Struct Name**           | `PascalCase` 或 `snake_case_t` | Google Style 使用 `PascalCase`，POSIX 標準使用 `snake_case_t` | `DeviceConfig`, `sensor_info_t` | `typedef struct DeviceConfig {...};` 或 `typedef struct sensor_info_t {...};` |
| **枚舉名稱 / Enum Name**             | `PascalCase` 或 `snake_case_t` | Google Style 使用 `PascalCase`，POSIX 標準使用 `snake_case_t` | `CardStatus`, `operation_mode_t` | `typedef enum CardStatus {...};` 或 `typedef enum operation_mode_t {...};` |
| **枚舉值 / Enum Value**               | `UPPER_SNAKE_CASE` | 全大寫，單詞用底線分隔            | `CARD_STATUS_READY`, `MODE_AUTO`  | `CardStatus status = CARD_STATUS_READY;` |
| **變數名稱（區域變數）/ Variable Name (Local)**  | `snake_case`       | 全小寫，單詞用底線分隔            | `card_rx_data`, `sensor_value`    | `uint8_t card_rx_data = 0;`      |
| **成員變數 / Member Variable**             | `snake_case`       | 全小寫，單詞用底線分隔            | `buffer_size`, `rx_length`        | `struct Card { uint8_t rx_length; };` |
| **靜態變數 / Static Variable**             | `snake_case`       | 前加 `s_` 表示靜態變數           | `s_card_rx_data`, `s_buffer_ptr`  | `static uint8_t s_card_rx_data = 0;` |
| **全域變數 / Global Variable**             | `snake_case`       | 前加 `g_` 表示全域變數           | `g_system_status`, `g_log_level`  | `uint8_t g_system_status = 0;`   |
| **常數（#define）/ Constant (Macro)**       | `UPPER_SNAKE_CASE` | 全大寫，單詞用底線分隔            | `MAX_BUFFER_SIZE`, `PI_VALUE`     | `#define MAX_BUFFER_SIZE 1024`    |
| **函式名稱 / Function Name**             | `PascalCase` 或 `snake_case` | Google Style 使用 `PascalCase`，POSIX 標準使用 `snake_case` | `InitializeSystem`, `process_data` | `void InitializeSystem(void) {...}` 或 `void process_data(void) {...}` |
| **回呼函式名稱 / Callback Function Name**         | `PascalCase` 或 `snake_case` | Google Style 使用 `PascalCase`，POSIX 標準使用 `snake_case` | `UartEventHandler`, `on_data_ready` | `void UartEventHandler(void) {...}` 或 `void on_data_ready(void) {...}` |
| **宏名稱 / Macro Name**               | `UPPER_SNAKE_CASE` | 全大寫，單詞用底線分隔            | `DEBUG_LEVEL`, `ENABLE_FEATURE`   | `#define DEBUG_LEVEL 1`           |
| **命名空間或模組名稱 / Namespace or Module Name**    | `PascalCase` 或 `snake_case` | Google Style 使用 `PascalCase`，POSIX 標準使用 `snake_case` | `CardProcessing`, `data_handler` | `namespace CardProcessing {...}` 或 `namespace data_handler {...}` |
| **文件名 / File Name**               | `snake_case`       | 全小寫，單詞用底線分隔            | `card_handler.c`, `system_init.h` | `// 文件名稱: card_handler.c`    |
| **測試名稱 / Test Name**             | `PascalCase` 或 `snake_case` | Google Style 使用 `PascalCase`，POSIX 標準使用 `snake_case` | `TestCardProcessing`, `test_card_processing` | `void TestCardProcessing(void) {...}` 或 `void test_card_processing(void) {...}` |

---

### 總結：
1. **PascalCase**：Google Style 的推薦風格，適用於型別名稱和函式名稱。
2. **snake_case_t**：POSIX 標準的推薦風格，適用於型別名稱，後加 `_t` 表示型別。
3. **snake_case**：適用於變數、函式、文件名等，簡潔清晰。
4. **UPPER_SNAKE_CASE**：常量與宏（定義或不可變項目），全大寫並用底線分隔。
5. **前綴標記**：靜態變數以 `s_` 開頭，全域變數以 `g_` 開頭。

此命名規範結合了 Google Style 和 POSIX 標準，適用於追求一致性與靈活性的 C 程式開發。

