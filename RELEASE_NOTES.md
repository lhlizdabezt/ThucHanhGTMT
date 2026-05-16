# Release notes - v1.2.1

## Điểm chính

- Bổ sung GIF tự host `assets/gtmt-data-flow.gif` để mô phỏng đường dữ liệu AHT20 → BLE SoC → UART/VCOM → SQLite bằng tiêu đề tiếng Việt dạng raster.
- Nâng cấp README tiếng Việt với đường kiểm tra nhanh cho HR/kỹ sư, liên kết release và bằng chứng file cụ thể.
- Giữ SVG ASCII-safe để tránh lỗi hiển thị dấu tiếng Việt trên GitHub, còn phần mô tả và bảng biểu trong README dùng tiếng Việt có dấu.
- Đóng gói release như một mốc portfolio có thể review: source zip, báo cáo PDF, slide PPTX và tài liệu lab.

## Tài sản review

| Tài sản | Vai trò |
| --- | --- |
| `22207056_report_DoAn.pdf` | Báo cáo đồ án đo nhiệt độ/độ ẩm với AHT20 |
| `22207056_report_DoAn (2).pptx` | Slide thuyết trình đồ án |
| `22207056_report_lab6.pdf` | Báo cáo lab BLE Mesh |
| `ThucHanhGTMT-source-v1.2.1.zip` | Source snapshot từ commit phát hành |

## Phạm vi kỹ thuật

Repo này là bài lab/dự án học phần Giao tiếp máy tính và Thu nhận dữ liệu. Nội dung tập trung vào firmware Silicon Labs BLE SoC, cảm biến AHT20 qua I2C, LCD, BLE advertising, UART/VCOM và logger Windows C/SQLite.
