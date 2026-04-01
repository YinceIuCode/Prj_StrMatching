## 📢 Hướng dẫn code thuật toán cho các thành viên
Mọi người lưu ý, để code ghép vào file main.cpp chạy được tự động và không bị lỗi, tất cả các hàm thuật toán (Naive, KMP, BM, RK, Aho) đều phải tuân theo đúng 1 cấu trúc hàm chuẩn như sau:

```C++
void searchTenThuatToan(const ProjectData& data, std::vector<std::vector<Occurrence>>& all_results, long long& comparisons)
```

## Giải thích chi tiết các biến truyền vào:

1. `const ProjectData& data` **(Dữ liệu đầu vào - Chỉ đọc):**
Đây là một `struct` chứa toàn bộ dữ liệu đã được file `main` đọc sẵn từ `puzzle.txt`. Mọi người lấy thông từ đây để làm:
* `data.R` và `data.C`: Số hàng và số cột của ma trận.
* `data.K`: Số lượng từ khóa cần tìm.
* `data.grid`: Ma trận ký tự 2 chiều (`vector<vector<char>>`). Ví dụ lấy ký tự ở hàng `r` cột `c` thì gọi `data.grid[r][c]`.
* `data.keywords`: Danh sách các từ khóa (`vector<string>`).

2. `std::vector<std::vector<Occurence>>& all_results` **(Nơi lưu kết quả đầu ra):**
Đây là mảng 2 chiều dùng để trả về kết quả cho hàm `main` in ra file. File `main` đã khởi tạo sẵn cho nó có `K` ngăn kéo (tương ứng với `K` từ khóa).
* **Cách dùng:** Nếu bạn tìm thấy từ khóa thứ `i` (tức là `data.keywords[i]`) trên ma trận, bạn đóng gói tọa độ vào `Occurrence` và nhét nó vào ngăn kéo thứ `i`.
* **Code mẫu:** `all_results[i].push_back({start_r, start_c, end_r, end_c});`

3. `long long& comparisons` **(Biến đếm số phép so sánh)**:
Đây là biến do `main` truyền vào để chấm điểm hiệu năng.
* **Cách dùng:** Cứ mỗi lần lệnh `if` của bạn lấy một ký tự trong `grid` ra so sánh với một ký tự trong keyword, bạn **BẮT BUỘC** phải thêm dòng `comparisons++`; ngay trước hoặc trong lúc đó.

## 🛠️ KHUNG CODE MẪU (TEMPLATE) DÀNH CHO MỌI NGƯỜI:
Mọi người cứ copy bộ khung này về, đổi tên hàm thành thuật toán của mình và code phần ruột bên trong là xong!
```C++
#include "Algorithms.h" // Nhớ include file header chung của nhóm

void searchThuatToanCuaBan(const ProjectData& data, std::vector<std::vector<Occurrence>>& all_results, long long& comparisons) {
    
    // Thuật toán của bạn (KMP/BM/RK thì lặp từng từ như dưới đây)
    
    for (int i = 0; i < data.K; ++i) {
        std::string current_word = data.keywords[i];
        
        // 1. Tiền xử lý từ khóa (Build bảng LPS của KMP, bảng Bad Match của BM,...)
        // ...
        
        // 2. Quét ma trận theo 2 hướng
        // ...
        // if (data.grid[r][c] == current_word[idx]) {
        //     comparisons++; // <--- NHỚ CỘNG BIẾN NÀY LÊN
        //     ...
        //     Nếu tìm thấy nguyên chữ thì lưu kết quả:
        //     all_results[i].push_back({r_start, c_start, r_end, c_end});
        // }
    }
}
```
***Lưu ý: Mọi người code có thể theo template hoặc không, miễn xuất ra đúng kết quả là được***